#include "AntPF.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <ranges>
#include <vector>

const double MIN_PHEROMONE = 1e-10;

std::pair<double, std::vector<int>> AntPathFinder::find(const Matrix &mat) {
  double q = calc_q(mat);
  std::vector<int> best_way;
  double min_length = std::numeric_limits<double>::infinity();

  Matrix pheromones = get_pheromones(mat);
  Matrix visibility = get_visibility(mat);
  int ants = mat.size();

  for (const auto &_ : std::views::iota(0, m_days)) {
    std::vector<int> route(mat.size());
    std::iota(route.begin(), route.end(), 0);

    std::vector<std::vector<int>> visited = get_visited_places(route, ants);

    for (const auto &ant : std::views::iota(0, ants)) {
      while (visited[ant].size() != mat.size()) {
        std::vector<double> pk =
            find_possibilities(pheromones, visibility, visited, ant, mat);
        int chosen_place = choose_next(pk);
        visited[ant].push_back(chosen_place);
      }

      visited[ant].push_back(visited[ant][0]);

      double cur_length = calc_length(visited[ant], mat);
      if (cur_length < min_length) {
        min_length = cur_length;
        best_way = visited[ant];
      }
    }

    pheromones = update_pheromones(visited, pheromones, q, mat);
    pheromones = update_pheromones_elite(pheromones, q, best_way, mat);
  }

  return {min_length, best_way};
}

double AntPathFinder::calc_q(const Matrix &mat) {
  double q = 0;
  int count = 0;

  for (const auto &i : std::views::iota(0ull, mat.size()))
    for (const auto &j :
         std::views::iota(0ull, mat.size()) |
             std::views::filter([&i](int j) { return i != j; })) {
      q += mat[i][j];
      count++;
    }

  return q / count;
}

AntPathFinder::Matrix AntPathFinder::get_pheromones(const Matrix &mat) {
  double min_phero = 1.0;
  return Matrix(mat.size(), std::vector<double>(mat.size(), min_phero));
}

AntPathFinder::Matrix AntPathFinder::get_visibility(const Matrix &mat) {
  Matrix visibility(mat.size(), std::vector<double>(mat.size(), 0.));

  for (const auto &i : std::views::iota(0ull, mat.size()))
    for (const auto &j : std::views::iota(0ull, mat.size()) |
                             std::views::filter([&i](int j) { return i != j; }))
      visibility[i][j] = 1.0 / mat[i][j];

  return visibility;
}

std::vector<std::vector<int>>
AntPathFinder::get_visited_places(const std::vector<int> &route, int ants) {
  std::vector<std::vector<int>> visited(ants);
  for (const auto &ant : std::views::iota(0, ants))
    visited[ant].push_back(route[ant]);

  return visited;
}

double AntPathFinder::calc_length(const std::vector<int> &route,
                                  const Matrix &mat) {
  double length = 0;
  for (size_t way_len = 1; way_len < route.size(); ++way_len) {
    length += mat[route[way_len - 1]][route[way_len]];
  }
  return length;
}

AntPathFinder::Matrix
AntPathFinder::update_pheromones(const std::vector<std::vector<int>> &visited,
                                 Matrix &pheromones, double q,
                                 const Matrix &mat) {
  for (const auto &i : std::views::iota(0ull, mat.size()))
    for (const auto &j : std::views::iota(0ull, mat.size())) {
      double delta_pheromones = 0;
      for (int ant = 0; ant < mat.size(); ++ant) {
        double length = calc_length(visited[ant], mat);
        delta_pheromones += q / length;
      }
      pheromones[i][j] *= (1 - m_k_evaporation);
      pheromones[i][j] += delta_pheromones;

      pheromones[i][j] = std::max(pheromones[i][j], MIN_PHEROMONE);
    }

  return pheromones;
}

AntPathFinder::Matrix
AntPathFinder::update_pheromones_elite(Matrix &pheromones, double q,
                                       const std::vector<int> &best_way,
                                       const Matrix &mat) {
  double length = calc_length(best_way, mat);
  for (const auto &i : std::views::iota(0ull, mat.size()))
    for (const auto &j : std::views::iota(0ull, mat.size())) {
      double delta_pheromones = m_e * q / length;
      pheromones[i][j] += delta_pheromones;
    }

  return pheromones;
}

std::vector<double> AntPathFinder::find_possibilities(
    const Matrix &pheromones, const Matrix &visibility,
    const std::vector<std::vector<int>> &visited, int ant, const Matrix &mat) {
  std::vector<double> pk(mat.size(), 0.);
  int ant_place = visited[ant].back();

  for (const auto &place : std::views::iota(0ull, mat.size())) {
    if (std::find(visited[ant].begin(), visited[ant].end(), place) ==
        visited[ant].end()) {
      pk[place] = std::pow(pheromones[ant_place][place], m_alpha) *
                  std::pow(visibility[ant_place][place], m_beta);
    }
  }

  if (visited[ant].size() == mat.size()) {
    int start_place = visited[ant][0];
    pk[start_place] = std::pow(pheromones[ant_place][start_place], m_alpha) *
                      std::pow(visibility[ant_place][start_place], m_beta);
  }

  double sum_pk = std::accumulate(pk.begin(), pk.end(), 0.0);
  for (const auto &place : std::views::iota(0ull, mat.size()))
    pk[place] /= sum_pk;

  return pk;
}

int AntPathFinder::choose_next(const std::vector<double> &pk) {
  double possibility = static_cast<double>(rand()) / RAND_MAX;
  double choice = 0;
  int chosen_place = 0;

  while ((choice < possibility) && (chosen_place < pk.size())) {
    choice += pk[chosen_place];
    chosen_place++;
  }

  return chosen_place - 1;
}
