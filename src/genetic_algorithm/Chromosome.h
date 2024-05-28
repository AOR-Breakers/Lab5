#include <cmath>
#include <vector>
class Chromosome {
private:
  int NumberOfMachines;
  std::vector<double> Gen;

  double Fitness;

public:
  Chromosome() = delete;
  Chromosome(const int NumberOfMachines)
      : NumberOfMachines(NumberOfMachines), Gen(NumberOfMachines + 1) {
    for (size_t I = 0; I < NumberOfMachines + 1; ++I) {
      Gen[I] = static_cast<double>(rand()) / RAND_MAX;
    }
  };

  Chromosome(const int NumberOfMachines, const std::vector<double> &Gen)
      : Gen(Gen), NumberOfMachines(NumberOfMachines){};

  void setFitness(double Fitness) { Fitness = Fitness; }
  double getFitness() const { return Fitness; }

  int getNumberOfMachines() const { return NumberOfMachines; }
  int getNumberOfCells() const {
    return std::ceil(Gen[NumberOfMachines] * NumberOfMachines);
  }
  std::vector<double> getGen() const { return Gen; }

  std::vector<std::vector<int>> getMachinesByCells() const {
    int NumberOfCells = getNumberOfCells();
    std::vector<std::vector<int>> MachinesByCells(NumberOfCells);

    for (std::size_t I = 0; I < NumberOfMachines; ++I) {
      int Cell = std::ceil(Gen[I] * NumberOfCells);
      MachinesByCells[Cell - 1].push_back(I + 1);
    }
    return MachinesByCells;
  }
};
