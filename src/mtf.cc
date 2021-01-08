/**
* @file mtf.cc
* @author Drahomír Dlabaja (xdlaba02)
* @date 23. 4. 2020
* @brief Funkce pro mtf transformaci.
*/

#include <vector>
#include <string>
#include <algorithm>

#include <cmath>

#include "mtf.h"

void update_base(std::vector<uint8_t> &dictionary, uint8_t i) {
  auto it = std::begin(dictionary) + i;
  std::rotate(std::begin(dictionary), it, std::next(it));
}

void update_m1ff(std::vector<uint8_t> &dictionary, uint8_t i) {
  auto it = std::begin(dictionary) + i;
  if (it <= std::next(std::begin(dictionary))) {
    std::rotate(std::begin(dictionary), it, std::next(it));
  }
  else {
    std::rotate(std::next(std::begin(dictionary)), it, std::next(it));
  }
}

void update_m1ff2(std::vector<uint8_t> &dictionary, uint8_t i, int16_t prev[2]) {
  auto it = std::begin(dictionary) + i;

  uint8_t c = *it;

  if (it <= std::next(std::begin(dictionary))) {
    std::rotate(std::begin(dictionary), it, std::next(it));
  }
  else if (prev[0] == *std::begin(dictionary) || prev[1] == *std::begin(dictionary)) {
    std::rotate(std::next(std::begin(dictionary)), it, std::next(it));
  }
  else {
    std::rotate(std::begin(dictionary), it, std::next(it));
  }

  prev[1] = prev[0];
  prev[0] = c;
}

void update_sticky(std::vector<uint8_t> &dictionary, uint8_t i, double v) {
  auto it = std::begin(dictionary) + i;

  size_t new_pos = std::round(i * v);
  std::rotate(std::begin(dictionary) + new_pos, it, std::next(it));
}

void update_k(std::vector<uint8_t> &dictionary, uint8_t i, size_t k) {
  auto it = std::begin(dictionary) + i;

  size_t new_pos = std::max<int64_t>(0, static_cast<int64_t>(i) - static_cast<int64_t>(k));
  std::rotate(std::begin(dictionary) + new_pos, it, std::next(it));
}

void update_c(std::vector<uint8_t> &dictionary, uint8_t i, size_t c, std::map<uint8_t, size_t> &counts) {
  auto it = std::begin(dictionary) + i;

  if (counts[*it] >= c) {
    std::rotate(std::begin(dictionary), it, std::next(it));
  }

  counts[*it]++;
}
