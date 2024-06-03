#pragma once

#include "Selection.h"
#include <cstdlib>

class RandomSelection : public Selection {
public:
  int select(const std::vector<Chromosome> &P, int Start, int End) override {
    return rand() % P.size();
  }
};
