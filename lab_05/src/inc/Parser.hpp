#pragma once

#include <gumbo.h>
#include <string>
#include <vector>

class Parser {
private:
  GumboOutput *output;

  std::vector<const GumboNode *> filterNodes(GumboTag tag);
  std::vector<const GumboNode *> filterNodes(const GumboNode *node,
                                             GumboTag tag);

public:
  Parser(const std::string &data);
  ~Parser();

  std::string getTitle();
  std::string getIngredients();
  std::vector<std::string> getSteps();
  std::string getImage();
};
