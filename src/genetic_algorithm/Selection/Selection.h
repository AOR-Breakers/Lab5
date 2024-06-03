#pragma once

#include "../Chromosome.h"
#include <vector>

class Selection {
public:
  virtual int select(const std::vector<Chromosome> &P, int Start, int End) = 0;

  virtual ~Selection() {}
};
