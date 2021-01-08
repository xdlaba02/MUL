/**
* @file bitstream.cc
* @author Drahomír Dlabaja (xdlaba02)
* @date 23. 4. 2020
* @brief Wrapper pro c++ streamy pro cteni a zapis po jednotlivych bitech.
*/

#include "bitstream.h"

#include <fstream>

using namespace std;

vector<bool> IBitstream::read(const size_t size) {
  vector<bool> data {};
  for (size_t i = 0; i < size; i++) {
    data.push_back(readBit());
  }
  return data;
}

bool IBitstream::readBit() {
  if (m_index > 7) {
    m_accumulator = m_stream->get();
    m_index = 0;
  }

  bool data = m_accumulator & (1 << (7 - m_index));

  m_index++;

  return data;
}

void OBitstream::write(const vector<bool> &data) {
  for (auto &&bit: data) {
    writeBit(bit);
  }
}

void OBitstream::writeBit(bool bit) {
  if (m_index > 7) {
    flush();
  }

  m_accumulator |= bit << (7 - m_index);
  m_index++;
}

void OBitstream::flush() {
  if (m_index > 0) {
    m_stream->put(m_accumulator);
    m_index = 0;
    m_accumulator = 0;
  }
}
