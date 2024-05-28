#include "../src/genetic_algorithm/Chromosome.h"
#include <gtest/gtest.h>
#include <vector>

constexpr int NumberOfMachines = 12;

TEST(Chromosome, CreateChromosome) {
  Chromosome RandomChromosome(NumberOfMachines);
  ASSERT_EQ(RandomChromosome.getNumberOfMachines(), NumberOfMachines);
  ASSERT_NE(RandomChromosome.getNumberOfCells(), 0);
  ASSERT_EQ(RandomChromosome.getGen().size(), NumberOfMachines + 1);
  std::vector<std::vector<int>> MachinesByCells =
      RandomChromosome.getMachinesByCells();
  ASSERT_EQ(MachinesByCells.size(), RandomChromosome.getNumberOfCells());
}
