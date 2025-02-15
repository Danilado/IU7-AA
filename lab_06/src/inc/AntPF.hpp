#pragma once

#include "BasePF.hpp"

class AntPathFinder : public BasePathFinder {
public:
  virtual ~AntPathFinder() = default;

  virtual std::pair<double, std::vector<int>> find(const Matrix &mat) override;

  void setKEvaporation(double k_evaporation) {
    m_k_evaporation = k_evaporation;
  }
  void setE(double e) { m_e = e; }
  void setAlpha(double alpha) {
    m_alpha = alpha;
    m_beta = 1 - m_alpha;
  }
  void setDays(double days) { m_days = days; }

private:
  double calc_q(const Matrix &mat);
  Matrix get_pheromones(const Matrix &mat);
  Matrix get_visibility(const Matrix &mat);
  std::vector<std::vector<int>>
  get_visited_places(const std::vector<int> &route, int ants);
  double calc_length(const std::vector<int> &route, const Matrix &mat);
  Matrix update_pheromones(const std::vector<std::vector<int>> &visited,
                           Matrix &pheromones, double q, const Matrix &mat);
  Matrix update_pheromones_elite(Matrix &pheromones, double q,
                                 const std::vector<int> &best_way,
                                 const Matrix &mat);
  std::vector<double>
  find_possibilities(const Matrix &pheromones, const Matrix &visibility,
                     const std::vector<std::vector<int>> &visited, int ant,
                     const Matrix &mat);
  int choose_next(const std::vector<double> &pk);

private:
  double m_k_evaporation;
  double m_e;
  double m_alpha;
  double m_beta;
  int m_days = 10;
};