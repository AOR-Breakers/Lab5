#pragma once
#include <vector>

class LocalSearch {
  std::vector<std::vector<int>> ProductToMachines;
  std::vector<std::vector<int>> MachineToProducts;
  int ProductsNumber;
  int MachinesNumber;
  int TotalMachinesUses;

  double calculateCoef(const std::vector<int> &Machines,
                       const std::vector<int> &CellMachines);

  double calculatePairCoef(const std::vector<std::vector<int>> &MachinesByCells,
                           const std::vector<std::vector<int>> &ProductByCells);

  std::pair<int, int>
  calculateMachinesOutAndIn(const std::vector<int> &Machines,
                            const std::vector<int> &CellMachines);

public:
  LocalSearch() = delete;

  LocalSearch(const std::vector<std::vector<int>> MachineToProducts,
              int ProductsNumber, int MachinesNumber)
      : ProductToMachines(ProductsNumber), MachineToProducts(MachineToProducts),
        ProductsNumber(ProductsNumber), MachinesNumber(MachinesNumber) {
    TotalMachinesUses = 0;
    for (std::size_t I = 0; I < MachinesNumber; ++I) {
      std::vector<int> Products = MachineToProducts[I];
      for (int &Product : Products) {
        ProductToMachines[Product - 1].push_back(I + 1);
      }
      TotalMachinesUses += Products.size();
    }
  }

  std::vector<std::vector<int>>
  getProducts(const std::vector<std::vector<int>> &MachinesByCells);

  std::vector<std::vector<int>>
  getMachines(const std::vector<std::vector<int>> &ProductByCells);

  std::pair<std::vector<std::vector<int>>, double>
  start(const std::vector<std::vector<int>> &MachinesByCells);

  int getMachinesNumber() { return MachinesNumber; }
};
