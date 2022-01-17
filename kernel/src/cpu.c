#include "cpu.h"

#include <cpuid.h>

char* get_cpu_vendor(void)
{
    int ebx = 0, edx = 0, ecx = 0, unused = 0;
	__cpuid(0, unused, ebx, ecx, edx);
    static char cpu_vendor[13];
    *(int*)&cpu_vendor[0] = ebx;
    *(int*)&cpu_vendor[4] = edx;
    *(int*)&cpu_vendor[8] = ecx;
    cpu_vendor[12] = '\0';
    return cpu_vendor;
}

void get_cpu_features(bool* avx512bf16, bool* sse3, bool* ssse3, bool* sse41, bool* sse42, bool* aes, bool* avx, bool* f16c, bool* rdrnd, bool* sse, bool* sse2)
{
    unsigned int eax = 0, edx = 0, ecx = 0, unused = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &edx);

    if (avx512bf16) *avx512bf16 = eax & bit_AVX512BF16;
    if (sse3) *sse3 = ecx & bit_SSE3;
    if (ssse3) *ssse3 = ecx & bit_SSSE3;
    if (sse41) *sse41 = ecx & bit_SSE4_1;
    if (sse42) *sse42 = ecx & bit_SSE4_2;
    if (aes) *aes = ecx & bit_AES;
    if (avx) *avx = ecx & bit_AVX;
    if (f16c) *f16c = ecx & bit_F16C;
    if (rdrnd) *rdrnd = ecx & bit_RDRND;
    if (sse) *sse = edx & bit_SSE;
    if (sse2) *sse2 = edx & bit_SSE2;
}
