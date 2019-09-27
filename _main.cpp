/* Script^2 @version 0.x
@link    https://github.com/kabuki-starship/script2.git
@file    /script2/script2_benchmark.cc
@author  Cale McCollough <<https://calemccollough.github.io>>
@license Copyright (C) 2014-9 Cale McCollough <<calemccollough.github.io>>;
All right reserved (R). This Source Code Form is subject to the terms of the
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with
this file, You can obtain one at <https://mozilla.org/MPL/2.0/>. */

#include <module_config.h>

#if SEAM >= SCRIPT2_SEAM_ROOM
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

#include "rng.h"
#include "t_benchmark.h"
#include "t_test.h"
#include "t_utf.h"

using namespace std;
using namespace std::chrono;

#if SEAM == SCRIPT2_SEAM_BOOK
#include "module_debug.inl"
#else
#include "module_release.inl"
#endif

namespace _ {

BenchmarkCase::BenchmarkCase(const CH1* name, TestCase* cases, SI4 count)
    : name(name), cases(cases), count(count) {}

const CH1* BenchmarkCase::Run(CH1* cursor, CH1* stop, const CH1* args) {
  /*
  FP8 nil_reading;
  SI4 i;
  SI4 columns_max;
  A_ASSERT(TScan<>(args, columns_max));

  for (i = 0; i < count; ++i) {
    const CH1* result = (*cases[i])(nullptr, nullptr, nullptr);
    Print(result);
    if (i < count) Print(',', ' ');
  }
  for (; i < columns_max; ++i) Print(',');
  PrintNL();
  for (i = 0; i < count; ++i) {
    const CH1* result = (*cases[i])(nullptr, nullptr, nullptr);
    if (i < count) Print(',', ' ');
  }
  PrintNL();
  */
  return nullptr;
}

SI4 BenchmarkCase::GetCount() { return count; }

Benchmark::Benchmark(const CH1* name, const CH1* filename,
                     BenchmarkCase* groups, SI4 count)
    : name(name), filename(filename), groups(groups), count(count) {
  A_ASSERT(count > 0);
  SI4 l_test_count_max = 0;
  for (SIW i = 0; i < count; ++i) {
  }
}

const CH1* Benchmark::Run(CH1* cursor, CH1* stop, const CH1* args) {
  /*
  std::ofstream out(filename);

  auto* coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  for (SIW i = 0; i < count; ++i) {
    BenchmarkCase group = groups[i];
    const CH1* result = group.Run(cursor, stop, args);
    A_ASSERT(result);
    Print(result);
  }
  BenchmarkCase *benchmark_case = groups, *stop = groups + count - 1;
  for (SI4 i = 0; i <  {
    groups[i]->Run(cursor, stop, args);
  }
  std::cout.rdbuf(coutbuf);*/
  return nullptr;
}

}  // namespace _

#endif
