/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki_toolkit.git
@file    /projects/script2_benchmark/benchmark/benchmark_itos.cc
@author  Cale McCollough <<https://calemccollough.github.io>>
@license Copyright (C) 2014-9 Cale McCollough <<calemccollough.github.io>>;
All right reserved (R). This Source Code Form is subject to the terms of the
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with
this file, You can obtain one at <https://mozilla.org/MPL/2.0/>. */

#include <module_config.h>

#if SEAM >= SCRIPT2_1
#include "itos_algorithms.h"

#include "../../script2/global.h"

#include <chrono>
#include <fstream>
#include <random>

typename const CH1* (*ItoS32)(const CH1* buffer, intptr_t size,
                               uint32_t value);

using namespace std;
using namespace std::chrono;
using namespace _;

template <typename UI>
void BenchmarkScriptItos() {
  enum {
    kNumTests = 1000 * 1000 * 1000,
    kSize = 24,
  };

  // Lookup table for powers of 10.
  static const uint32_t kPow10Ranges[11]{
      1,      10,      100,      1000,      10000,
      100000, 1000000, 10000000, 100000000, 1000000000};

  // static const CH1 kBenchmarkHeader[] = "\nScript ItoS Benchmarks,,,,"
  //                                       "\n#Bits, Null, sprintf_s, Mod10, "
  //                                       "Mod100, Script, % Faster\n";
  static const CH1 kBenchmarkDecimalHeader[] =
      "\nScript ItoS Benchmarks per Decimal,,,,"
      "\n#Bits, Null, Mod10, Mod100, Script, % Faster\n";
  static const CH1 kBenchmarkHeader[] =
      "\nScript ItoS Benchmarks per Bit,,,,"
      "\n#Bits, Null, Mod10, "
      "Mod100, Script, % Faster\n";
  CH1 buffer[kSize], expecting[kSize];
  CH1* result;
  uint32_t value;
  uint32_t bits_mask = ~(uint32_t)0, value_mask;
  SI4 count;
  auto start = high_resolution_clock::now(),
       stop = high_resolution_clock::now();
  auto delta = duration_cast<milliseconds>(stop - start).count();

  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<uint32_t> distr;

  double nil_time, cpu_times[5], percent_faster;

  Print("\n\nTesting random numbers...\n\n");

  for (SI4 i = 0; i < kNumTests / 10; ++i) {
    value = distr(eng);
    sprintf_s(expecting, kSize, "%u", value);
    result = _::Print<>(buffer, kSize, value);
    if (!result) {
      Pause("\nAn error occurred in Print :-(");
      break;
    }
    *result = 0;
    if (StringCompare<>(expecting, buffer)) {
      Pausef("\n\nERROR in Print: Expecting \"%s\":%u and found \"%s\":%u",
             expecting, StringLength<>(expecting), buffer, StringLength<>(buffer));
    }

    result = PrintMod10(buffer, kSize, value);
    if (!result) {
      Pause("\nAn error occurred in PrintMod10 :-(");
      break;
    }
    *result = 0;
    if (StringCompare<>(expecting, buffer)) {
      Pausef("\n\nERROR in PrintMod10: Expecting \"%s\":%u and found \"%s\":%u",
             expecting, StringLength<>(expecting), buffer, StringLength<>(buffer));
    }

    result = PrintMod100(buffer, kSize, value);
    if (!result) {
      Pausef("\n%i.) Expecting \"%s\":%u\nAn error occurred in PrintMod100:-(",
             i + 1, expecting, StringLength<>(expecting));
      break;
    }
    *result = 0;
    if (StringCompare<>(expecting, buffer)) {
      Pausef(
          "\n\nERROR in PrintMod100: Expecting \"%s\":%u and found \"%s\":%i",
          expecting, (SI4)StringLength<>(expecting), buffer,
          (SI4)StringLength<>(buffer));
    }
  }

  Print("\n\nBenchmarking ItoS algorithms...\n\n");

  ofstream file("script_itos_benchmarks.csv");

  if (!file.is_open()) {
    Print("Unable to open file");
    return;
  }

  Print(kBenchmarkDecimalHeader);

  file << kBenchmarkDecimalHeader;

  const uint32_t* power_of_ten = &kPow10Ranges[0];
  uint32_t min, max = *power_of_ten++, range;
  for (SI4 num_digits = 1; num_digits <= 10; ++num_digits) {
    uint32_t min = max, max = *power_of_ten++, range = max - min, last_value;
    COut("\n    Range:").Print(range);
    uint32_t num_loops = kNumTests / range;
    if (range > kNumTests) {
      max = kNumTests;
    }

    file << num_digits << ",";

    PrintLn(num_digits).Print(".) ");

    start = high_resolution_clock::now();
    for (uint32_t j = num_loops; j > 0; --j) {
      for (uint32_t value = min; value < max; ++value) {
        result = PrintNil(buffer, buffer + kSize, value);
      }
    }
    stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();
    nil_time = (double)delta;
    file << delta << ',';
    COut(delta).Print(',');

    /*
    start = high_resolution_clock::now ();
    for (uint32_t j = num_loops; j > 0; --j) {
        for (uint32_t value = min; value < max; ++value) {
            result = PrintSprintf (text, text + kSize - 1, value);
        }
    }
    stop = high_resolution_clock::now ();
    delta = duration_cast<milliseconds> (stop - start).count ();
    cpu_times[0] = (double)delta;
    file << delta << ',';
    cout << delta << ',';*/

    start = high_resolution_clock::now();
    for (uint32_t j = num_loops; j > 0; --j) {
      for (uint32_t value = min; value < max; ++value) {
        result = PrintMod10(buffer, buffer + kSize - 1, value);
      }
    }
    stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();
    cpu_times[1] = (double)delta;
    file << delta << ',';
    COut(delta).Print(',');

    start = high_resolution_clock::now();
    for (uint32_t j = num_loops; j > 0; --j) {
      for (uint32_t value = min; value < max; ++value) {
        result = PrintMod100(buffer, buffer + kSize - 1, value);
      }
    }
    stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();
    cpu_times[2] = (double)delta;
    file << delta << ',';
    COut(delta).Print(',');

    start = high_resolution_clock::now();
    for (uint32_t j = num_loops; j > 0; --j) {
      for (uint32_t value = min; value < max; ++value) {
        result = _::Print<CH1>(buffer, buffer + kSize, value);
      }
    }
    stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();
    cpu_times[3] = (double)delta;
    file << delta << ',';
    COut(delta).Print(',');
    percent_faster =
        ((cpu_times[2] - nil_time) / (cpu_times[3] - nil_time)) - 1.0;
    file << percent_faster << kLF;
    COut(percent_faster);
  }

  COut("\n\n").Print(kBenchmarkHeader);
  file << kBenchmarkHeader;
  for (SI4 num_bits = 1; num_bits <= 32; ++num_bits) {
    bits_mask = bits_mask << 1;
    value_mask = ~bits_mask;
    file << num_bits << ",";
    COut(kLF).Print(num_bits) << ".) ";

    start = high_resolution_clock::now();
    for (count = kNumTests; count > 0; --count) {
      value = distr(eng) & value_mask;
      result = PrintNil(buffer, buffer + kSize - 1, value);
    }
    stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();
    nil_time = (double)delta;
    file << delta << ',';
    COut(delta).Print(',');
    /*
    start = high_resolution_clock::now ();
    for (count = kNumTests; count > 0; --count) {
        value = distr (eng) & value_mask;
        result = PrintSprintf (text, text + kSize - 1, value);
    }
    stop = high_resolution_clock::now ();
    delta = duration_cast<milliseconds> (stop - start).count ();
    cpu_times[0] = (double)delta;
    file << delta << ',';
    cout << delta << ',';*/

    start = high_resolution_clock::now();
    for (count = kNumTests; count > 0; --count) {
      value = distr(eng) & value_mask;
      result = PrintMod10(buffer, buffer + kSize - 1, value);
    }
    stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();
    cpu_times[1] = (double)delta;
    file << delta << ',';
    COut(delta).Print(',');

    start = high_resolution_clock::now();
    for (count = kNumTests; count > 0; --count) {
      value = distr(eng) & value_mask;
      result = PrintMod100(buffer, buffer + kSize - 1, value);
    }
    stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();
    cpu_times[2] = (double)delta;
    file << delta << ',';
    COut(delta).Print(',');

    start = high_resolution_clock::now();
    for (count = kNumTests; count > 0; --count) {
      value = distr(eng) & value_mask;
      result = _::Print<CH1>(buffer, buffer + kSize, value);
    }
    stop = high_resolution_clock::now();
    delta = duration_cast<milliseconds>(stop - start).count();
    cpu_times[3] = (double)delta;
    file << delta << ',';
    COut(delta).Print(',');
    double percent_faster =
        ((cpu_times[2] - nil_time) / (cpu_times[3] - nil_time)) - 1.0;
    file << percent_faster << kLF;
    Print(percent_faster);
  }
  file.close();
}

void PrintDigits99To99Lut() {
  Print("\n\nstatic const uint16_t kDigits00To99[100] = {\n    ");
  enum { kDigitsLast = ('9' << 8) | '9' };
  uint16_t digits, tick = 0, tick_count = 9;
  uint16_t tens, ones;
  for (tens = '0'; tens <= '9'; ++tens) {
    for (ones = '0'; ones <= '9'; ++ones) {
      digits = (ones << 8) | tens;
      if (++tick == tick_count) {
        tick = 0;
        Printf("0x%x,\n    ", digits);
      } else {
        Printf("0x%x, ", digits);
      }
    }
  }
  Print(" }; \n\n");
  Pause();
}

SI4 main() {
  BenchmarkScriptItos();
  Pause();
  return 0;
}

#endif  //< #if SEAM >= SCRIPT2_1
