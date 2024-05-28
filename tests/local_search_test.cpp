#include "../src/local_search/LocalSearch.h"
#include <gtest/gtest.h>
#include <vector>

std::vector<std::vector<int>> ProductMachines = {
    {1, 4},     {2, 9},  {3, 6, 8}, {1, 4, 11}, {3, 6, 8},
    {1, 4, 11}, {3, 8},  {2, 9},    {3, 6, 8},  {5, 10, 12},
    {2, 9},     {4, 11}, {1, 11},   {5, 7, 12}, {5, 7, 10, 12}};

std::vector<std::vector<int>> MachineProducts = {
    {1, 4, 6, 13}, {2, 8, 11}, {3, 5, 7, 9},   {1, 4, 6, 12},
    {10, 14, 15},  {3, 5, 9},  {14, 15},       {3, 5, 7, 9},
    {2, 8, 11},    {10, 15},   {4, 6, 12, 13}, {10, 14, 15}};

TEST(LocalSearch, GettingProductsByCells) {
  std::vector<std::vector<int>> MachinesByCells = {
      {3, 8}, {5, 7, 12}, {1, 4, 10, 11}, {2, 6, 9}};

  LocalSearch LS(MachineProducts, 15, 12);

  std::vector<std::vector<int>> Products = LS.getProducts(MachinesByCells);

  std::vector<std::vector<int>> ExpectedProducts = {
      {3, 5, 7, 9}, {10, 14, 15}, {1, 4, 6, 12, 13}, {2, 8, 11}};

  ASSERT_EQ(ExpectedProducts.size(), Products.size());

  for (size_t I = 0; I < Products.size(); ++I) {
    ASSERT_EQ(ExpectedProducts[I], Products[I]);
  }
}

TEST(LocalSearch, GettingMachinesByCells) {
  std::vector<std::vector<int>> ProductByCells = {
      {3, 5, 7, 9}, {10, 14, 15}, {1, 4, 6, 12, 13}, {2, 8, 11}};

  LocalSearch LS(MachineProducts, 15, 12);

  std::vector<std::vector<int>> Machines = LS.getMachines(ProductByCells);

  std::vector<std::vector<int>> ExpectedMachines = {
      {3, 6, 8}, {5, 7, 10, 12}, {1, 4, 11}, {2, 9}};

  ASSERT_EQ(ExpectedMachines.size(), Machines.size());
  ASSERT_EQ(ExpectedMachines.size(), Machines.size());

  for (size_t I = 0; I < Machines.size(); ++I) {
    ASSERT_EQ(ExpectedMachines[I], Machines[I]);
  }
}

TEST(LocalSearch, Iteration) {
  std::vector<std::vector<int>> MachinesByCells = {
      {3, 8}, {5, 7, 12}, {1, 4, 10, 11}, {2, 6, 9}};

  LocalSearch LS(MachineProducts, 15, 12);
  std::pair<std::vector<std::vector<int>>, double> Res =
      LS.start(MachinesByCells);

  std::vector<std::vector<int>> Machines = Res.first;
  std::vector<std::vector<int>> ExpectedMachines = {
      {3, 6, 8}, {5, 7, 10, 12}, {1, 4, 11}, {2, 9}};

  ASSERT_EQ(ExpectedMachines.size(), Machines.size());
  ASSERT_EQ(std::abs(Res.second - 0.8667) < 1e-4, true);

  for (size_t I = 0; I < Machines.size(); ++I) {
    ASSERT_EQ(ExpectedMachines[I], Machines[I]);
  }
}
