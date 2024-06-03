#include "genetic_algorithm/Crossover/CoinTossCrossover.h"
#include "genetic_algorithm/GeneticAlgo.h"
#include "genetic_algorithm/Mutation/GenMutation.h"
#include "genetic_algorithm/Mutation/Mutation.h"
#include "genetic_algorithm/Selection/RandomSelection.h"
#include "genetic_algorithm/Selection/Selection.h"
#include "local_search/LocalSearch.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void printVector(std::ofstream &Out, std::vector<int> &V) {
  for (auto &El : V) {
    Out << El << ' ';
  }
  Out << '\n';
}

void solve(std::string Filename) {
  std::ifstream InFile("data/" + Filename + ".txt");
  int NumberOfMachines, NumberOfProducts;
  InFile >> NumberOfMachines >> NumberOfProducts;
  std::vector<std::vector<int>> MachineToProducts(NumberOfMachines);
  for (size_t I = 0; I < NumberOfMachines; ++I) {
    std::string Line;
    std::getline(InFile, Line);
    std::stringstream Iss(Line);
    int X;
    while (Iss >> X) {
      MachineToProducts[I].push_back(X);
    }
  }

  Crossover *CrossoverAlg = new CoinTossCrossover();
  Mutation *MutationAlg = new GenMutation(0.7);
  Selection *SelectionAlg = new RandomSelection();

  LocalSearch LS(MachineToProducts, NumberOfProducts, NumberOfMachines);
  GeneticAlgorithm GA(LS, CrossoverAlg, MutationAlg, SelectionAlg);
  int PopulationSize = 100;
  int TopSize = 20;
  int RandomGeneratedSize = 20;

  double SimilarityTolerance = 0.05;
  double TargetSimilarity = 0.95;
  double MutationProbabilty = 0.5;
  int64_t MaximalCyclesNumber = 500000;
  int64_t MaximalCyclesWithoutNewBest = 10000;

  std::chrono::steady_clock::time_point BeginTime =
      std::chrono::steady_clock::now();
  Chromosome Best =
      GA.start(PopulationSize, TopSize, RandomGeneratedSize,
               SimilarityTolerance, TargetSimilarity, MutationProbabilty,
               MaximalCyclesNumber, MaximalCyclesWithoutNewBest

      );

  std::chrono::steady_clock::time_point EndTime =
      std::chrono::steady_clock::now();

  std::cout << Filename << '\n';
  std::cout << "Overall best: " << Best.getFitness() << '\n';
  auto AlgoTime = EndTime - BeginTime;
  std::cout
      << "Algo time: "
      << std::chrono::duration_cast<std::chrono::minutes>(AlgoTime).count()
      << ':'
      << std::chrono::duration_cast<std::chrono::seconds>(AlgoTime).count() % 60
      << '.'
      << std::chrono::duration_cast<std::chrono::milliseconds>(AlgoTime)
                 .count() %
             1000
      << "\n\n";

  std::vector<std::vector<int>> MachinesByCells =
      LS.start(Best.getMachinesByCells()).first;
  std::vector<std::vector<int>> ProductsBYCells =
      LS.getProducts(MachinesByCells);

  std::vector<int> MachineToCluster(NumberOfMachines);
  std::vector<int> ProductsToCluster(NumberOfProducts);

  int Index = 1;
  for (auto &Row : MachinesByCells) {
    if (Row.size() == 0) {
      continue;
    }
    for (auto &El : Row) {
      MachineToCluster[El - 1] = Index;
    }
    Index++;
  }

  std::cout << '\n';

  Index = 1;
  for (auto &Row : ProductsBYCells) {
    if (Row.size() == 0) {
      continue;
    }
    for (auto &El : Row) {
      ProductsToCluster[El - 1] = Index;
    }
    Index++;
  }

  std::ofstream OutFile("answers/" + Filename + ".sol");
  printVector(OutFile, MachineToCluster);
  printVector(OutFile, ProductsToCluster);
}

int main() {
  std::vector<std::string> Files = {"20x20", "24x40", "30x50", "30x90",
                                    "37x53"};
  for (auto &Filename : Files) {

    std::cout << Filename << '\n';
    solve(Filename);
  }
}
