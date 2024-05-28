#include "Mutation.h"
#include <cstdlib>
#include <vector>
class GenMutation : public Mutation {

private:
  double MutationProb;

public:
  GenMutation(double MutationProb) : MutationProb(MutationProb){};

  Chromosome mutate(const Chromosome &C) {
    std::vector<double> Gen = C.getGen();
    int Range = C.getNumberOfMachines() + 1;
    int SelectedGen = rand() % Range;

    Gen[SelectedGen] = static_cast<double>(rand()) / RAND_MAX;
    return Chromosome(C.getNumberOfMachines(), Gen);
  };
};
