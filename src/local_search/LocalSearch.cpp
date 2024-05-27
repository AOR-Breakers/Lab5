#include "LocalSearch.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

std::vector<std::vector<int>>
LocalSearch::getProducts(const std::vector<std::vector<int>> &MachinesByCells) {
  std::vector<std::vector<int>> ProductsByCells(MachinesByCells.size());

  for (size_t I = 0; I < ProductsNumber; ++I) {
    std::vector<int> Machines = ProductToMachines[I];

    double BestCell = 0;
    double BestCoef = 0;
    for (size_t J = 0; J < MachinesByCells.size(); ++J) {
      std::vector<int> CellMachines = MachinesByCells[J];
      double Coef = calculateCoef(Machines, CellMachines);

      if (Coef > BestCoef) {
        BestCell = J;
        BestCoef = Coef;
      }
    }

    ProductsByCells[BestCell].push_back(I + 1);
  }

  return ProductsByCells;
}

std::vector<std::vector<int>>
LocalSearch::getMachines(const std::vector<std::vector<int>> &ProductByCells) {
  std::vector<std::vector<int>> MachinesByCells(ProductByCells.size());

  for (size_t I = 0; I < MachinesNumber; ++I) {
    std::vector<int> Products = MachineToProducts[I];

    double BestCell = 0;
    double BestCoef = 0;
    for (size_t J = 0; J < ProductByCells.size(); ++J) {
      std::vector<int> CellProducts = ProductByCells[J];
      double Coef = calculateCoef(Products, CellProducts);

      if (Coef > BestCoef) {
        BestCell = J;
        BestCoef = Coef;
      }
    }

    MachinesByCells[BestCell].push_back(I + 1);
  }

  return MachinesByCells;
};

double LocalSearch::calculateCoef(const std::vector<int> &Machines,
                                  const std::vector<int> &CellMachines) {

  std::pair<int, int> OutAndIn =
      calculateMachinesOutAndIn(Machines, CellMachines);
  double Coef = static_cast<double>(TotalMachinesUses - OutAndIn.first) /
                static_cast<double>(TotalMachinesUses + OutAndIn.second);
  return Coef;
}

std::pair<int, int>
LocalSearch::calculateMachinesOutAndIn(const std::vector<int> &Machines,
                                       const std::vector<int> &CellMachines) {
  std::vector<int> MachinesIntersection;
  std::set_intersection(Machines.begin(), Machines.end(), CellMachines.begin(),
                        CellMachines.end(),
                        std::back_inserter(MachinesIntersection));

  int TotalMachinesUsesOut = Machines.size() - MachinesIntersection.size();
  int TotalMachinesUnusedIn = CellMachines.size() - MachinesIntersection.size();

  return std::make_pair(TotalMachinesUsesOut, TotalMachinesUnusedIn);
}

std::pair<std::vector<std::vector<int>>, double>
LocalSearch::start(const std::vector<std::vector<int>> &MachinesByCells) {
  double Coef = 0;

  std::vector<std::vector<int>> MachinesInitial = MachinesByCells;
  std::vector<std::vector<int>> MachinesFinal = MachinesByCells;
  double LastCoef = 0;
  do {
    LastCoef = Coef;
    MachinesInitial = MachinesFinal;

    std::vector<std::vector<int>> ProductsByCells =
        getProducts(MachinesByCells);

    MachinesFinal = getMachines(ProductsByCells);

    if (std::equal(MachinesInitial.begin(), MachinesInitial.end(),
                   MachinesFinal.begin(), MachinesFinal.end())) {
      return std::make_pair(MachinesInitial, Coef);
    }

    double Coef1 = calculatePairCoef(MachinesInitial, ProductsByCells);
    double Coef2 = calculatePairCoef(MachinesFinal, ProductsByCells);

    Coef = std::max(Coef1, Coef2);
  } while (Coef > LastCoef);
  return std::make_pair(MachinesInitial, LastCoef);
}

double LocalSearch::calculatePairCoef(
    const std::vector<std::vector<int>> &MachinesByCells,
    const std::vector<std::vector<int>> &ProductByCells) {

  int N1Out = 0;
  int N0In = 0;
  for (size_t I = 0; I < MachinesByCells.size(); ++I) {
    std::vector<int> CellMachines = MachinesByCells[I];
    for (size_t J = 0; J < ProductByCells[I].size(); ++J) {

      std::vector<int> ProductMachines =
          ProductToMachines[ProductByCells[I][J] - 1];

      std::pair<int, int> OutAndIn =
          calculateMachinesOutAndIn(ProductMachines, CellMachines);

      N1Out += OutAndIn.first;
      N0In += OutAndIn.second;
    }
  }

  double Coef = static_cast<double>(TotalMachinesUses - N1Out) /
                static_cast<double>(TotalMachinesUses + N0In);
  return Coef;
}
