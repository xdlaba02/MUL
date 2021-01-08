/**
* @file bwt.cc
* @author Drahomír Dlabaja (xdlaba02)
* @date 23. 4. 2020
* @brief Wrapper pro libduvsufsort pro provedeni btw.
*/

#include <iostream>
#include <vector>
#include <algorithm>

#include <libdivsufsort/divsufsort.h>

#include "bwt.h"

int64_t bwt(uint8_t *data, size_t size) {
  int64_t pidx = divbwt(data, data, nullptr, size);

  if (pidx < 0) {
    std::cerr << "divbwt failed: ";
    if (pidx == -1) {
      std::cerr << "Invalid arguments.";
    }
    else {
      std::cerr << "Cannot allocate memory.";
    }
    std::cerr << '\n';
  }

  return pidx;
}

int64_t ibwt(uint8_t *data, size_t size, uint64_t pidx) {
  int64_t err = inverse_bw_transform(data, data, nullptr, size, pidx);

  if (err < 0) {
    std::cerr << "inverse_bw_transform failed: ";
    if (err == -1) {
      std::cerr << "Invalid arguments.";
    }
    else {
      std::cerr << "Cannot allocate memory.";
    }
    std::cerr << '\n';
  }

  return err;
}
