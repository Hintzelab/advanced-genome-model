#pragma once
#include <stdint.h>

class AbstractGenome {
  public:
    virtual uint8_t* dataView() = 0;
    //virtual GeneView geneView() = 0;
};

/* genome read */

template <class T>
auto genomeRead(AbstractGenome& genome, int index) -> T& {
  return reinterpret_cast<T*>(genome.dataView()+index)[0];
}

// python-like named arguments
struct GenomeReadParams {
  AbstractGenome& genome;
  int index;
};
template <class T>
auto genomeRead(const GenomeReadParams& params) -> T& {
  return genomeRead<T>(params.genome, params.index);
}

/* genome write */

template <class T>
auto genomeWrite(AbstractGenome& genome, int index, T value) -> void {
  reinterpret_cast<T*>(genome.dataView()+index)[0] = value;
}

// python-like named arguments
template <class T>
struct GenomeWriteParams {
  AbstractGenome& genome;
  int index;
  T value;
};
template <class T>
auto genomeWrite(const GenomeWriteParams<T>& params) -> void {
  genomeWrite<T>(params.genome, params.index, params.value);
}
