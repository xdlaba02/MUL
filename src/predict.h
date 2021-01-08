/**
* @file predict.h
* @author Drahomír Dlabaja (xdlaba02)
* @date 23. 4. 2020
* @brief Implementace prediktorů.
*/

#ifndef PREDICT_H
#define PREDICT_H

#include <algorithm>

/**
 * @brief Funkce vrátí průměr ze sousedních hodnot.
 * @param a Levý soused.
 * @param b Horní soused.
 * @return Predikovaný vzorek.
 */
uint8_t avgPredictor(uint8_t a, uint8_t b) {
  return (static_cast<uint16_t>(a) + static_cast<uint16_t>(b)) >> 1;
}

/**
 * @brief Funkce vrátí vzorek predikovaný pomocí paethova prediktoru ze sousedních hodnot.
 * @param a Levý soused.
 * @param b Horní soused.
 * @param c Rohový soused.
 * @return Predikovaný vzorek.
 */
uint8_t paethPredictor(uint8_t a, uint8_t b, uint8_t c) {
  int64_t p {};

  p += a;
  p += b;
  p -= c;

  uint8_t avg = avgPredictor(a, b);

  uint64_t pavg = std::abs(p - avg);
  uint64_t pa   = std::abs(p - a);
  uint64_t pb   = std::abs(p - b);
  uint64_t pc   = std::abs(p - c);

  if (pavg <= pa && pavg <= pb && pavg <= pc) {
    return avg;
  }
  else if (pa <= pb && pa <= pc) {
    return a;
  }
  else if (pb <= pc) {
    return b;
  }
  else {
    return c;
  }
}

#endif
