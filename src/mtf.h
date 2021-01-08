/**
* @file mtf.h
* @author Drahomír Dlabaja (xdlaba02)
* @date 23. 4. 2020
* @brief Funkce pro mtf transformaci.
*/

#ifndef MTF_H
#define MTF_H

#include <vector>
#include <cstdint>
#include <map>

/**
 * @brief Funkce provede move to front transformaci.
 * @param dictionary Seřazený slovník symbolů.
 * @param data Ukazatel na data, která mají být transformována.
 * @param size Velikost dat.
 * @param update Funkce, která aktualizuje slovník.
 */
template<typename F>
void move_to_front(std::vector<uint8_t> &dictionary, uint8_t *data, size_t size, F &&update) {
  for (size_t i {}; i < size; i++) {
    auto it = std::find(std::begin(dictionary), std::end(dictionary), data[i]);

    if (it != std::end(dictionary)) {
      size_t k = std::distance(std::begin(dictionary), it);
      data[i] = k;

      update(dictionary, k);
    }
  }
}

/**
 * @brief Funkce provede inverzní move to front transformaci.
 * @param dictionary Seřazený slovník symbolů.
 * @param data Ukazatel na data, která mají být inverzně transformována.
 * @param size Velikost dat.
 * @param update Funkce, která aktualizuje slovník.
 */
template<typename F>
void move_from_front(std::vector<uint8_t> &dictionary, uint8_t *data, size_t size, F &&update) {
  for (size_t i {}; i < size; i++) {
    uint8_t k = data[i];
    data[i] = dictionary[k];

    update(dictionary, k);
  }
}

/**
 * @brief Funkce, která aktualizuje slovník podle základní mtf.
 * @param dictionary Seřazený slovník symbolů.
 * @param i Index symbolu, jež má být aktualizován.
 */
void update_base(std::vector<uint8_t> &dictionary, uint8_t i);

/**
 * @brief Funkce, která aktualizuje slovník podle metody m1ff.
 * @param dictionary Seřazený slovník symbolů.
 * @param i Index symbolu, jež má být aktualizován.
 */
void update_m1ff(std::vector<uint8_t> &dictionary, uint8_t i);

/**
 * @brief Funkce, která aktualizuje slovník podle metody m1ff2.
 * @param dictionary Seřazený slovník symbolů.
 * @param i Index symbolu, jež má být aktualizován.
 * @param prev Předchozí symboly.
 */
void update_m1ff2(std::vector<uint8_t> &dictionary, uint8_t i, int16_t prev[2]);

/**
 * @brief Funkce, která aktualizuje slovník podle metody sticky mtf.
 * @param dictionary Seřazený slovník symbolů.
 * @param i Index symbolu, jež má být aktualizován.
 * @param v Poměr o kolik se má hodnota posunou dopředu.
 */
void update_sticky(std::vector<uint8_t> &dictionary, uint8_t i, double v);

/**
 * @brief Funkce, která aktualizuje slovník podle metody mtf k.
 * @param dictionary Seřazený slovník symbolů.
 * @param i Index symbolu, jež má být aktualizován.
 * @param k Hodnota o kterou se posune symbol dopředu.
 */
void update_k(std::vector<uint8_t> &dictionary, uint8_t i, size_t k);

/**
 * @brief Funkce, která aktualizuje slovník podle metody mtf c.
 * @param dictionary Seřazený slovník symbolů.
 * @param i Index symbolu, jež má být aktualizován.
 * @param c Četnost symbolu, která musí být překočena, aby se aktualizoval slovník.
 * @param count Četnosti symbolů.
 */
void update_c(std::vector<uint8_t> &dictionary, uint8_t i, size_t c, std::map<uint8_t, size_t> &counts);

#endif
