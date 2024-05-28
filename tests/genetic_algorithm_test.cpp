#include "../src/genetic_algorithm/Chromosome.h"
#include <gtest/gtest.h>
#include <vector>

constexpr int NumberOfMachines = 12;

TEST(Chromosome, CreateRandomChromosome) {
  Chromosome RandomChromosome(NumberOfMachines);
  ASSERT_EQ(RandomChromosome.getNumberOfMachines(), NumberOfMachines);
  ASSERT_NE(RandomChromosome.getNumberOfCells(), 0);
  ASSERT_EQ(RandomChromosome.getGen().size(), NumberOfMachines + 1);
  std::vector<std::vector<int>> MachinesByCells =
      RandomChromosome.getMachinesByCells();
  ASSERT_EQ(MachinesByCells.size(), RandomChromosome.getNumberOfCells());
}

TEST(Chromosome, CreateChromosome) {
  std::vector<double> Gen = {0.7,  0.89, 0.12, 0.54, 0.37, 0.78, 0.41,
                             0.19, 0.94, 0.64, 0.68, 0.31, 0.29};
  Chromosome C(NumberOfMachines, Gen);
  ASSERT_NE(C.getNumberOfCells(), 0);
  ASSERT_EQ(C.getGen().size(), NumberOfMachines + 1);
  std::vector<std::vector<int>> MachinesByCells = C.getMachinesByCells();
  ASSERT_EQ(MachinesByCells.size(), C.getNumberOfCells());

  std::vector<std::vector<int>> ExpectedMachinesByCells = {
      {3, 8}, {5, 7, 12}, {1, 4, 10, 11}, {2, 6, 9}};
  for (size_t I = 0; I < C.getNumberOfCells(); ++I) {
    for (size_t J = 0; J < ExpectedMachinesByCells[I].size(); ++J) {
      ASSERT_EQ(ExpectedMachinesByCells[I][J], MachinesByCells[I][J]);
    }
  }
}
