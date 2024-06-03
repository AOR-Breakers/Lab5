#pragma once

#include "Crossover.h"
#include <cstdlib>
#include <iostream>
#include <utility>

class CoinTossCrossover : public Crossover {
private:
  double HeadProbabilty;
  bool coinToss() {
    return static_cast<double>(rand()) / RAND_MAX < HeadProbabilty;
  }

public:
  CoinTossCrossover() : HeadProbabilty(0.5){};
  CoinTossCrossover(double HeadProbabilty) : HeadProbabilty(HeadProbabilty){};

  std::pair<Chromosome, Chromosome> crossover(const Chromosome &P1,
                                              const Chromosome &P2) {
    int NumberOfMachines = P1.getNumberOfMachines();
    std::vector<double> P1Gen = P1.getGen();
    std::vector<double> P2Gen = P2.getGen();
    std::vector<double> C1Gen(NumberOfMachines + 1);
    std::vector<double> C2Gen(NumberOfMachines + 1);

    for (size_t I = 0; I < NumberOfMachines + 1; ++I) {
      if (coinToss()) {
        C1Gen[I] = P2Gen[I];
        C2Gen[I] = P1Gen[I];
      } else {
        C1Gen[I] = P1Gen[I];
        C2Gen[I] = P2Gen[I];
      }
    }

    return std::make_pair(Chromosome(NumberOfMachines, C1Gen),
                          Chromosome(NumberOfMachines, C2Gen));
  }
};
