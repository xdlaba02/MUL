/**
* @file bwt.h
* @author Drahomír Dlabaja (xdlaba02)
* @date 23. 4. 2020
* @brief Wrapper pro libduvsufsort pro provedeni btw.
*/

#ifndef BWT_H
#define BWT_H

/**
 * @brief Funkce, která provede Burrows–Wheelerovu transformaci.
 * @param data Ukazatel na data, na kterých má být provedena transformace.
 * @param size Velikost dat.
 * @return Index řádku originálního řetězce, záporné číslo v případě chyby.
 */
int64_t bwt(uint8_t *data, size_t size);

/**
 * @brief Funkce, která provede inverzní Burrows–Wheelerovu transformaci.
 * @param data Ukazatel na data, na kterých má být provedena inverzní transformace.
 * @param size Velikost dat.
 * @param pidx Index řádku originálního řetězce.
 * @return Nula, záporné číslo v případě chyby.
 */
int64_t ibwt(uint8_t *data, size_t size, uint64_t pidx);

#endif
