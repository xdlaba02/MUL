/**
* @file main.cc
* @author Drahomír Dlabaja (xdlaba02)
* @date 21. 4. 2020
* @brief Vstupni bod programu, parsovani parametru a samotne kodovani a dekodovani.
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <cmath>

#include <getopt.h>

#include "libppm/ppm.h"
#include "bitstream.h"
#include "cabac.h"
#include "mtf.h"
#include "bwt.h"
#include "predict.h"

/**
 * @brief Funkce, která vytvoří složky tak, aby bylo možné otevřít soubor.
 * @param file_name Cesta k souboru, pro kterou mají být vytvořeny složky.
 * @return Nenulová hodnota v případě chyby.
 */
int create_directory(const char *file_name) {
  size_t last_slash_pos = std::string(file_name).find_last_of('/');
  if (last_slash_pos != std::string::npos) {
    std::string command = "mkdir -p '" + std::string(file_name).substr(0, last_slash_pos) + "'";
    return system(command.c_str());
  }
  return 0;
}

/**
 * @brief Funkce, která na výstup vypíše nápovědu.
 * @param argv0 Jméno programu.
 */
void print_usage(const char *argv0) {
  std::cout << "Usage: " << '\n';
  std::cout << argv0 << " -i <ifile> -o <ofile> (-c | -d)" << '\n';
}

/**
 * @brief Funkce, která parsuje argumenty.
 * @param argc argc.
 * @param argv argv.
 * @param input_file_name Reference na ukazatel, který by měl po návratu ukazovat na jméno vstupního souboru.
 * @param output_file_name Reference na ukazatel, který by měl po návratu ukazovat na jméno výstupního souboru.
 * @param compress Přepínač, který bude true, pokud se má komprimovat.
 * @param adaptive Přepínač, který bude true, pokud se má použít adaptivní kódování.
 * @param adaptive Přepínač, který bude true, pokud se má použít model.
 * @param width Hodnota bude obsahovat šířku obrázku.
 * @return Nenulová hodnota v případě chyby.
 */
int parse_args(int argc, char *argv[], const char *&input_file_name, const char *&output_file_name, bool &compress) {
  int         mode      {};

  input_file_name  = nullptr;
  output_file_name = nullptr;

  char opt;
  while ((opt = getopt(argc, argv, "i:o:cd")) >= 0) {
    switch (opt) {
      case 'i':
        if (!input_file_name) {
          input_file_name = optarg;
          continue;
        }
        break;

      case 'o':
        if (!output_file_name) {
          output_file_name = optarg;
          continue;
        }
        break;

      case 'c':
        if (!mode) {
          mode = 1;
        }
        continue;

      case 'd':
        if (!mode) {
          mode = 2;
        }
        continue;

      default:
        break;
    }

    print_usage(argv[0]);
    return -1;
  }

  compress = mode == 1;

  if ((!input_file_name) || (!output_file_name) || (!mode)) {
    print_usage(argv[0]);
    return -2;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  const char *input_file_name  {};
  const char *output_file_name {};

  bool compress {};

  PPM ppm_data {};

  uint64_t width  {};
  uint64_t height {};

  std::vector<uint8_t> channel_data {};
  CABAC::ContextModel context[255] {};

  const double   MTF_V = 0.9;
  const size_t   EG_CONST = 10;

  std::vector<uint8_t> dictionary(1 << (sizeof(uint8_t) * 8));
  auto update_dict = [MTF_V](std::vector<uint8_t> &dictionary, uint8_t i) {
    update_sticky(dictionary, i, MTF_V);
  };

  if (parse_args(argc, argv, input_file_name, output_file_name, compress) < 0) {
    return 1;
  }

  if (create_directory(output_file_name) < 0) {
    std::cerr << "ERROR: CANNON OPEN " << output_file_name << " FOR WRITING\n";
    return 2;
  }


  if (compress) {
    std::ofstream output  {};
    OBitstream bit_output {};
    CABACEncoder encoder  {};

    if (ppm_data.mmapPPM(input_file_name) < 0) {
      std::cerr << "ERROR: CANNON OPEN " << input_file_name << " FOR READING\n";
      return 3;
    }

    if (ppm_data.color_depth() != 255) {
      std::cerr << "ERROR: UNSUPPORTED COLOR DEPTH " << ppm_data.color_depth() << "\n";
      return 3;
    }

    width  = ppm_data.width();
    height = ppm_data.height();

    if (!width || !height) {
      std::cerr << "ERROR: WEIRD IMAGE DIMENSIONS: " << width << "x" << height << "\n";
      return 3;
    }

    output.open(output_file_name, std::ios::binary);
    if (!output) {
      std::cerr << "ERROR: CANNON OPEN " << output_file_name << " FOR WRITING\n";
      return 4;
    }

    bit_output.open(&output);
    encoder.init(bit_output);

    encoder.encodeEGBypass(EG_CONST, width);
    encoder.encodeEGBypass(EG_CONST, height);

    for (size_t i {}; i < width * height; i++) {
      auto rgb = ppm_data.get(i);
      rgb[0] -= rgb[1];
      rgb[2] -= rgb[1];
      ppm_data.put(i, rgb);
    }

    for (size_t y = height - 1; y > 0; y--) {
      for (size_t x = width - 1; x > 0; x--) {
        auto a = ppm_data.get(y * width + (x - 1));
        auto b = ppm_data.get((y - 1) * width + x);
        auto c = ppm_data.get((y - 1) * width + (x - 1));

        auto d = ppm_data.get(y * width + x);

        for (size_t i {}; i < 3; i++) {
          d[i] -= paethPredictor(a[i], b[i], c[i]);
        }

        ppm_data.put(y * width + x, d);
      }
    }

    channel_data.resize(width * height);

    for (size_t c {}; c < 3; c++) {
      for (size_t i {}; i < channel_data.size(); i++) {
        channel_data[i] = ppm_data.get(i)[c];
      }

      int64_t pidx = bwt(channel_data.data(), channel_data.size());
      if (pidx < 0) {
        return 5;
      }

      encoder.encodeEGBypass(EG_CONST, pidx);

      for (size_t i {}; i < dictionary.size(); i++) {
        dictionary[i] = i;
      }

      move_to_front(dictionary, channel_data.data(), channel_data.size(), update_dict);

      for (size_t i {}; i < channel_data.size(); i++) {
        for (size_t j {}; j < channel_data[i]; j++) {
          encoder.encodeBit(context[j], 1);
        }
        if (channel_data[i] < 255) {
          encoder.encodeBit(context[channel_data[i]], 0);
        }
      }
    }

    encoder.terminate();
  }
  else {
    std::ifstream input  {};
    IBitstream bit_input {};
    CABACDecoder decoder {};

    input.open(input_file_name, std::ios::binary);
    if (!input) {
      std::cerr << "ERROR: CANNON OPEN " << input_file_name << " FOR READING\n";
      return 3;
    }

    bit_input.open(&input);
    decoder.init(bit_input);

    width  = decoder.decodeEG(EG_CONST);
    height = decoder.decodeEG(EG_CONST);

    if (ppm_data.createPPM(output_file_name, width, height, 255) < 0) {
      return 4;
    }

    channel_data.resize(width * height);

    for (size_t c {}; c < 3; c++) {
      uint64_t pidx = decoder.decodeEG(EG_CONST);

      for (size_t i {}; i < channel_data.size(); i++) {
        channel_data[i] = 0;
        while (channel_data[i] < 255 && decoder.decodeBit(context[channel_data[i]])) {
          channel_data[i]++;
        }
      }

      for (size_t i {}; i < dictionary.size(); i++) {
        dictionary[i] = i;
      }

      move_from_front(dictionary, channel_data.data(), channel_data.size(), update_dict);

      if (ibwt(channel_data.data(), channel_data.size(), pidx) < 0) {
        return 5;
      }

      for (size_t i {}; i < channel_data.size(); i++) {
        auto current = ppm_data.get(i);
        current[c] = channel_data[i];
        ppm_data.put(i, current);
      }
    }

    for (size_t y { 1 }; y < height; y++) {
      for (size_t x { 1 }; x < width; x++) {
        auto a = ppm_data.get(y * width + (x - 1));
        auto b = ppm_data.get((y - 1) * width + x);
        auto c = ppm_data.get((y - 1) * width + (x - 1));

        auto d = ppm_data.get(y * width + x);

        for (size_t i {}; i < 3; i++) {
          d[i] += paethPredictor(a[i], b[i], c[i]);
        }

        ppm_data.put(y * width + x, d);

      }
    }

    for (size_t i {}; i < width * height; i++) {
      auto rgb = ppm_data.get(i);
      rgb[0] += rgb[1];
      rgb[2] += rgb[1];
      ppm_data.put(i, rgb);
    }

    decoder.terminate();
  }

  return 0;
}
