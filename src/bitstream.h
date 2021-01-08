/**
* @file bitstream.h
* @author Drahomír Dlabaja (xdlaba02)
* @date 23. 4. 2020
* @brief Wrapper pro c++ streamy pro cteni a zapis po jednotlivych bitech.
*/

#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <cstdint>

#include <istream>
#include <ostream>
#include <vector>

/**
 * @brief Bázová třída pro streamování bitů.
 */
class Bitstream {
public:
  /**
   * @brief Výchozí konstruktor.
   */
  Bitstream(): m_index{}, m_accumulator{} {};

protected:
  uint8_t m_index;
  uint8_t m_accumulator;
};

/**
 * @brief Třída pro čtení bitů ze streamu.
 */
class IBitstream: public Bitstream {
public:
  /**
   * @brief Výchozí konstruktor. Tento konstruktor inicializuje vnitřní ukazatel na nullptr.
   */
  IBitstream(): m_stream{} {}

  /**
   * @brief Konstruktor specifikující ukazatel na stream.
   * @param stream Ukazatel na stream, ze kterého se bude číst.
   */
  IBitstream(std::istream *stream): m_stream{ stream } { m_index = 8; }

  /**
   * @brief Metoda, která (znovu)otevře bitstream pro nový stream.
   * @param stream Ukazatel na stream, ze kterého se bude číst.
   */
  void open(std::istream *stream) { m_stream = stream; m_index = 8; }

  /**
   * @brief Metoda, která přečte specifický počet bitů ze streamu.
   * @param size Počet bitů, které se mají přečíst.
   * @return Bitový vektor.
   */
  std::vector<bool> read(const size_t size);

  /**
   * @brief Metoda, která přečte bit ze streamu.
   * @return Bit ze streamu.
   */
  bool readBit();

private:
  std::istream *m_stream;
};

/**
 * @brief Třída pro zápis bitů do streamu.
 */
class OBitstream: public Bitstream {
public:
  /**
   * @brief Výchozí konstruktor. Tento konstruktor inicializuje vnitřní ukazatel na nullptr.
   */
  OBitstream(): m_stream {} {}

  /**
   * @brief Konstruktor specifikující ukazatel na stream.
   * @param stream Ukazatel na stream, do kterého se bude zapisobat.
   */
  OBitstream(std::ostream *stream): m_stream{ stream } {}

  /**
   * @brief Destruktor, který na výstup spláchne buffer.
   */
  ~OBitstream() { flush(); }

  /**
   * @brief Metoda, která (znovu)otevře bitstream pro nový stream.
   * @param stream Ukazatel na stream, do kterého se bude zapisobat.
   */
  void open(std::ostream *stream) { m_stream = stream; }

  /**
   * @brief Metoda, která zapíše bitový vektor do streamu.
   * @param data Bitový vektor.
   */
  void write(const std::vector<bool> &data);

  /**
   * @brief Metoda, která zapíše jeden bit do streamu.
   * @param bit Bit, který má být zapsán do streamu.
   */
  void writeBit(const bool bit);

  /**
   * @brief Metoda, která spláchne vnitřní buffer.
   */
  void flush();

private:
  std::ostream *m_stream;
};

#endif
