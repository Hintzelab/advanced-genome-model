#include "TestGenome.h"
#include <iostream>

TestGenome::TestGenome() {
  //initialize genome to 8 sites
  genome.resize(8);
  for (uint8_t i(0); i<8; ++i)
    genome[i] = i+1;
  genome[7] = 0b10000001; // 129, or -127
}

uint8_t* TestGenome::dataView() {
  return static_cast<uint8_t*>(genome.data()); // cast is for demonstration
}

//GeneView TestGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
