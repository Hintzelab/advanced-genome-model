#pragma once
#include <vector>
#include <stdint.h>
#include "AbstractGenome.h"

class TestGenome : public AbstractGenome {
  public:
    TestGenome();

    // required functions
    uint8_t* dataView() override;
    //GeneView geneView() override;

    // custom properties & functions
    std::vector<uint8_t> genome;
};
