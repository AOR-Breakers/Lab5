#pragma once

#include "../local_search/LocalSearch.h"
#include "Chromosome.h"
#include "Crossover/Crossover.h"
#include "Mutation/Mutation.h"
#include "Selection/Selection.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

class GeneticAlgorithm {
private:
  Crossover *CrossoverAlg;
  Mutation *MutationAlg;
  Selection *SelectionAlg;

  LocalSearch LS;

  int64_t Cycle = 0;

  std::random_device RandomDevice;
  std::default_random_engine Engine;
  std::uniform_real_distribution<double> DistProb;

  std::vector<Chromosome> generateFirstPopulation(int PopulationSize) {
    std::vector<Chromosome> Chromosomes;
    for (int Iter = 0; Iter < PopulationSize; ++Iter) {
      Chromosome C = Chromosome(LS.getMachinesNumber());
      Chromosomes.push_back(C);
    }
    Chromosomes = calculateFitness(Chromosomes);

    std::sort(std::begin(Chromosomes), std::end(Chromosomes),
              [](const Chromosome &A, const Chromosome &B) {
                return A.getFitness() > B.getFitness();
              });

    return Chromosomes;
  }

  std::vector<Chromosome>
  calculateFitness(std::vector<Chromosome> &Chromosomes) {
    for (size_t I = 0; I < Chromosomes.size(); ++I) {
      Chromosome C = Chromosomes[I];
      if (C.getFitness() != -1) {
        continue;
      }
      double Fitness = LS.start(C.getMachinesByCells()).second;
      Chromosomes[I].setFitness(Fitness);
    }
    return Chromosomes;
  }

public:
  GeneticAlgorithm() = delete;

  explicit GeneticAlgorithm(LocalSearch &LS, Crossover *CrossoverAlg,
                            Mutation *MutationAlg, Selection *SelectionAlg)
      : LS(LS), CrossoverAlg(CrossoverAlg), MutationAlg(MutationAlg),
        SelectionAlg(SelectionAlg) {
    Engine = std::default_random_engine(RandomDevice());
    DistProb = std::uniform_real_distribution<double>(0, 1);
  };

  Chromosome start(const int PopulationSize, const int TopSize,
                   const int RandomGeneratedSize,
                   const double SimilarityTolerance = 0.05,
                   const double TargetSimilarity = 0.95,
                   const double MutationProbabilty = 0.5,
                   const int64_t MaximalCyclesNumber = 1e6,
                   const int64_t MaximalCyclesWithoutNewBest = 1e6) {
    double CurrentSimilarity = 0.0;

    std::vector<Chromosome> CurrentPopulation =
        generateFirstPopulation(PopulationSize);
    Chromosome Best = CurrentPopulation[0];
    Cycle = 0;
    for (int64_t CyclesWithouNewBest = 0;
         CurrentSimilarity < TargetSimilarity && Cycle < MaximalCyclesNumber &&
         CyclesWithouNewBest < MaximalCyclesWithoutNewBest;
         ++Cycle) {
      std::vector<Chromosome> Chromosomes;

      std::copy(std::begin(CurrentPopulation),
                std::begin(CurrentPopulation) + TopSize,
                std::back_inserter(Chromosomes));

      for (int I = 0; I < PopulationSize - TopSize - RandomGeneratedSize; ++I) {

        int Parent1Index = randomNumber(0, TopSize);
        int Parent2Index = randomNumber(TopSize, CurrentPopulation.size());

        std::pair<Chromosome, Chromosome> CrossoverRes =
            CrossoverAlg->crossover(CurrentPopulation[Parent1Index],
                                    CurrentPopulation[Parent2Index]);

        Chromosome Child1 = CrossoverRes.first, Child2 = CrossoverRes.second;

        double ProbMutateChild1 = DistProb(Engine);
        double ProbMutateChild2 = DistProb(Engine);

        if (ProbMutateChild1 < MutationProbabilty) {
          Child1 = MutationAlg->mutate(Child1);
        }
        if (ProbMutateChild2 < MutationProbabilty) {
          Child2 = MutationAlg->mutate(Child2);
        }

        Chromosomes.push_back(Child1);
      }

      for (int I = 0; I < RandomGeneratedSize; ++I) {
        Chromosomes.push_back(Chromosome(LS.getMachinesNumber()));
      }

      CurrentPopulation = calculateFitness(Chromosomes);
      std::sort(std::begin(CurrentPopulation), std::end(CurrentPopulation),
                [](const Chromosome &A, const Chromosome &B) {
                  return A.getFitness() > B.getFitness();
                });

      Chromosome PopulationBest = CurrentPopulation[0];
      CyclesWithouNewBest++;
      if (Best.getFitness() < PopulationBest.getFitness()) {
        Best = CurrentPopulation[0];
        CyclesWithouNewBest = 0;
      }

      CurrentSimilarity =
          percentageOfSimilarFitness(CurrentPopulation, SimilarityTolerance);

      if (Cycle % 1000 == 0) {
        std::cout << "Cycle " << Cycle << '\n';
        std::cout << "CyclesWithouNewBest " << CyclesWithouNewBest << '\n';
        std::cout << "CurrentSimilarity " << CurrentSimilarity << '\n';
        std::cout << "CyclesWithouNewBest " << CyclesWithouNewBest << '\n';
        std::cout << "Best " << Best.getFitness() << '\n';
        std::cout << "PopulationBest " << PopulationBest.getFitness() << '\n';
      }
    }
    return Best;
  }

  uint64_t getCycles() { return Cycle; }

  double similarity(double X, double Y) {
    return 1 - std::abs(X - Y) / std::max(X, Y);
  }

  double similarityFitness(const Chromosome &C1, const Chromosome &C2) {
    return similarity(C1.getFitness(), C2.getFitness());
  }

  double percentageOfSimilarFitness(const std::vector<Chromosome> &P,
                                    double SimilarityTolerance) {
    double Size = P.size();
    double Simillar = 0;
    for (int IndexI = 0; IndexI < Size - 1; ++IndexI) {
      for (int IndexJ = IndexI + 1; IndexJ < Size; ++IndexJ) {
        if (1 - similarityFitness(P[IndexI], P[IndexJ]) <=
            SimilarityTolerance) {
          Simillar++;
        }
      }
    }
    return Simillar / (Size * (Size - 1) / 2);
  }

  int randomNumber(int Start, int End) {
    return rand() % (End - Start) + Start;
  }
};
