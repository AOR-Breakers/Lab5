#pragma once

#include "../Chromosome.h"

class Mutation {
public:
  virtual Chromosome mutate(const Chromosome &C) = 0;
  virtual ~Mutation(){};
};
