#include "Parser.hpp"

#include <cstring>
#include <format>
#include <gumbo.h>
#include <iostream>
#include <ranges>

constexpr std::string_view website = "chiefs.kz";

Parser::Parser(const std::string &data) { output = gumbo_parse(data.c_str()); }

Parser::~Parser() { gumbo_destroy_output(&kGumboDefaultOptions, output); }

namespace {
void filterNodesRec(const GumboNode *node, GumboTag tag,
                    std::vector<const GumboNode *> &result) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }

  const GumboElement *element = &node->v.element;
  if (element->tag == tag) {
    result.push_back(node);
  }

  for (int i = 0; i < element->children.length; ++i) {
    filterNodesRec((GumboNode *)element->children.data[i], tag, result);
  }
}

std::string extractAttribute(const GumboNode *node,
                             const std::string &attribute) {
  if (node->type != GUMBO_NODE_ELEMENT)
    return {};

  const GumboElement *element = &node->v.element;

  for (int i = 0; i < element->attributes.length; ++i) {
    GumboAttribute *attr = (GumboAttribute *)element->attributes.data[i];

    if (attr && attr->name == attribute) {
      return attr->value;
    }
  }

  return {};
}

bool hasClass(const GumboNode *node, const std::string &className) {
  if (node->type != GUMBO_NODE_ELEMENT)
    return false;

  return extractAttribute(node, "class") == className;
}

std::string extractText(const GumboNode *node) {
  GumboNode *title_text =
      static_cast<GumboNode *>(node->v.element.children.data[0]);
  if (title_text->type == GUMBO_NODE_TEXT) {
    return title_text->v.text.text;
  }

  return {};
}
} // namespace

std::vector<const GumboNode *> Parser::filterNodes(GumboTag tag) {
  std::vector<const GumboNode *> result;
  filterNodesRec(output->root, tag, result);
  return result;
}

std::vector<const GumboNode *> Parser::filterNodes(const GumboNode *node,
                                                   GumboTag tag) {
  std::vector<const GumboNode *> result;
  filterNodesRec(node, tag, result);
  return result;
}

std::string Parser::getTitle() {
  /*
  <header class="s-title">
    <h1>Блюда из кабачков — вкусные и простые рецепты с фото</h1>
  </header>
  */

  auto candidates = filterNodes(GUMBO_TAG_HEADER);

  for (const GumboNode *node :
       candidates | std::views::filter([](const GumboNode *node) {
         return hasClass(node, "s-title");
       })) {                                              // header.s-title
    auto subcandidates = filterNodes(node, GUMBO_TAG_H1); // h1
    if (subcandidates.empty())
      continue;

    return extractText(subcandidates.front());
  }

  return "[NO TITLE FOUND]";
}

std::string Parser::getIngredients() {
  /*
  <dl class="ingredients">
      <dt>50 гр</dt><dd>Цельнозерновая</dd>
      <dt>8 гр</dt><dd>Соль</dd>
      <dt>30 г</dt><dd>Овсяные хлопья</dd>
      <dt>350 г</dt><dd>Мука </dd>
      <dt>20 мг</dt><dd>Мёд</dd>
      <dt>1 шт</dt><dd>Яичный белок</dd>
      <dt>250 мг</dt><dd>Вода</dd>
  </dl>
  */

  auto candidates = filterNodes(GUMBO_TAG_DL);

  for (const GumboNode *node :
       candidates | std::views::filter([](const GumboNode *node) {
         return hasClass(node, "ingredients");
       })) {                                         // dl.ingredients
    auto weights = filterNodes(node, GUMBO_TAG_DT);  // dt
    auto products = filterNodes(node, GUMBO_TAG_DD); // dd

    size_t minsize = std::min(weights.size(), products.size());
    std::string res = "{[";

    for (size_t i = 0; i < minsize; ++i) {
      std::string tmp = "{";
      tmp.append(std::format("\"name\":\"{}\",\"quantity\":\"{}\"",
                             extractText(products[i]),
                             extractText(weights[i])));
      tmp.append("},");

      res.append(tmp);
    }

    res.append("]}");

    return res;
  }

  return {};
}

std::vector<std::string> Parser::getSteps() {
  /*
  <div class="instructions">
    <ol>
        <li><span>Затем руками вымешиваем тесто не менее 20 минут...</span></li>
        <li><span>В одной порции пшеничной ...</span></li>
        <li><span>Из полученной порции опары и будем ...</span></li>
        <li><span>Для этого все ингредиенты ...</span></li>
    </ol>
  </div>
  */

  auto candidates = filterNodes(GUMBO_TAG_DIV);
  for (const GumboNode *node :
       candidates | std::views::filter([](const GumboNode *node) {
         return hasClass(node, "instructions");
       })) {                                        // div.instructions
    auto spans = filterNodes(node, GUMBO_TAG_SPAN); // span

    if (spans.empty())
      continue;

    std::vector<std::string> res;
    res.resize(spans.size());

    for (const GumboNode *span : spans) {
      std::string tmptext = extractText(span);
      if (tmptext.size())
        res.emplace_back(tmptext);
    }

    return res;
  }

  return {};
}

std::string Parser::getImage() {
  /*
  <div class = "image">
      <a href = "#">
          <img src = "img/0/2061.jpg" alt = "" />
      </ a>
  </ div>
  */

  auto candidates = filterNodes(GUMBO_TAG_DIV);
  for (const GumboNode *node :
       candidates | std::views::filter([](const GumboNode *node) {
         return hasClass(node, "image");
       })) {                                          // div.image
    auto links = filterNodes(node, GUMBO_TAG_A);      //
    for (const GumboNode *link : links) {             // a
      auto images = filterNodes(link, GUMBO_TAG_IMG); // img
      if (images.empty())
        continue;

      return std::format("{}/{}", website,
                         extractAttribute(images.front(), "src").c_str());
    }
  }

  return "[NULL]";
}
