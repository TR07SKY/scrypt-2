#include <stdio.h>
#include <stdlib.h> /* for atoi() */
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sched.h>

#include "insertname.h"
#include "utils/uint256.h"

#define BEGIN(a)            ((char*)&(a))
#define END(a)              ((char*)&((&(a))[1]))

int main(int argc, const char * const *argv)
{
    static __thread union {
        uint8_t u8[80];
        uint32_t u32[20];
    } src;

    unsigned int i;

    for (i = 0; i < sizeof(src); i++)
        src.u8[i] = i * 3;

    puts("Benchmarking 1 thread ...");

    clock_t clk_tck = sysconf(_SC_CLK_TCK);
    struct tms start_tms, end_tms;
    clock_t start = times(&start_tms), end;
    unsigned int n = 1;
    unsigned long long count = 0;

    do
	{
        for (i = 0; i < n; i++)
			uint256 temp2 = hash_M7M_v2(BEGIN(src.u8), END(src.u8), sizeof(src));

        count += n;

        end = times(&end_tms);
        n <<= 1;
    } while (end - start < clk_tck * 2);

    clock_t start_v = start_tms.tms_utime + start_tms.tms_stime +
        start_tms.tms_cutime + start_tms.tms_cstime;
    clock_t end_v = end_tms.tms_utime + end_tms.tms_stime +
        end_tms.tms_cutime + end_tms.tms_cstime;

    printf("%llu H/s real, %llu H/s virtual "
        "(%llu hashes in %.2f seconds)\n",
        count * clk_tck / (end - start),
        count * clk_tck / (end_v - start_v),
        count, (double)(end - start) / clk_tck);

    return 0;
}
