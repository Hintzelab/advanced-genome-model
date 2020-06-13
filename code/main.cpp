#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include "AbstractGenome.h"
#include "TestGenome.h"

int main() {
  TestGenome genome;

  /* proving no time is spent copying into num
   * if we use references.
   * Note: genomeRead<>() is defined in AbstractGenome.h
   */
  {
    uint8_t& num = genomeRead<uint8_t>(genome, 0);
    assert(num == 1); // same as above
    genome.dataView()[0] = 3;
    assert(num == 3);
    num = 1; // reset it to 1
  }

  int num;

  // 1-byte read
  num = genomeRead<uint8_t>(genome, 0);
  assert(num == 0b00000001);
  assert(num == 1); // same as above

  // 1-byte read (with named parameters for readability)
  num = genomeRead<uint8_t>({.genome=genome, .index=0});
  assert(num == 0b00000001);
  assert(num == 1); // same as above

  // 2-byte read
  num = genomeRead<uint16_t>(genome, 0);
  assert(num == 0b00000010'00000001);
  assert(num == 513); // same as above

  // 4-byte read
  num = genomeRead<uint32_t>(genome, 0);
  assert(num == 0b00000100'00000011'00000010'00000001);
  assert(num == 67'305'985); // same as above

  // 1-double read
  double& dbl = genomeRead<double>(genome, 0);
  uint64_t bitanswer = 0b10000001'00000111'00000110'00000101'00000100'00000011'00000010'00000001; // roughly, = -1.04917e-303
  assert(dbl == reinterpret_cast<double*>(&bitanswer)[0]);

  // 2's complement
  // left-most bit interpretation:
  // unsigned vs signed
  num = genomeRead<uint8_t>(genome, 7); // range [0,255] [00000000, 11111111]
  assert(num == 0b10000001);
  assert(num == 129); // same as above
  num = genomeRead<int8_t>(genome, 7); // range [-128,127] [10000000,01111111]
  assert(num == static_cast<int8_t>(0b10000001));
  assert(num == -127); // same as above

  // byte-array read with no copy
  {
    std::array<uint8_t,4>& arr = genomeRead<std::array<uint8_t,4>>(genome, 0);
    for (int i(0); i<arr.size(); i++)
      assert(arr[i] == i+1);
  }

  // int32-array read with no copy
  {
    std::array<unsigned int,2>& arr = genomeRead<std::array<unsigned int,2>>(genome, 0);
    assert(arr[0] == 67'305'985);
    assert(arr[1] == 2'164'721'157);
  }

  std::cout << "all read tests passed" << std::endl;

  // write single byte
  genomeWrite<uint8_t>(genome, 0, 255);
  assert(genomeRead<uint8_t>(genome, 0) == 255);

  // write several bytes
  for (int i=0; i<4; i++) { // rewrite first 4 bytes with (2,4,6,8)
    genomeWrite<uint8_t>({.genome=genome, .index=i, .value=uint8_t((i+1)*2)});
    assert(genomeRead<uint8_t>(genome, i) == (i+1)*2);
  }

  // write array of integers (and let compiler guess the type!)
  {
    std::array<uint32_t,2> arr = {42,213};
    genomeWrite(genome, 0, arr);
    auto& result = genomeRead<std::array<uint32_t,2>>(genome, 0);
    assert(result[0] == 42);
    assert(result[1] == 213);
  }

  std::cout << "all write tests passed" << std::endl;

  return(0);
}
