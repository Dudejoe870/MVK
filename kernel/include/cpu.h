#pragma once

#include <stdcommon.h>

char* get_cpu_vendor(void);
void get_cpu_features(bool* avx512bf16, bool* sse3, bool* ssse3, bool* sse41, bool* sse42, bool* aes, bool* avx, bool* f16c, bool* rdrnd, bool* sse, bool* sse2);
