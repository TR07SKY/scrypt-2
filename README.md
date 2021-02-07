This epic soviet hash algorithm (maded to be used in crypto PoW) \
uses a communist only-CPU combination of scrypt, yespower and m7  \
that destroys any posibility of capitalist GPU/FPGA or ASIC. 

1st : do power2b over the input \
2st : do scrypt^2 over the output of power2b \
3st : do high precision floating point math over the two results \
4st : return uint (ready to be used in a coin)

This super epic soviet hash uses 128MB of RAM per mining thread!!!! \
the scrypt^2 do the high RAM usage, power2b uses L2 cache to prevent \
GPUs SM usage (too slow). m7 math fuck FPGAs / ASICs with the use of \
IEEE754

power2b  : N=1024, r=32   : 4MB of RAM usage \
scrypt^2 : N=1048576, r=1 :

- 1way : 128MB -> nr_hugepages = 65.
- SSE4 (3way) : 384MB -> nr_hugepages = 193.
- AVX (3way) : 384MB -> nr_hugepages = 193.
- AVX2 (6way) : 768MB -> nr_hugepages = 386.
- ARMv7 (3way) : 384MB -> nr_hugepages = 193.
- ARMv8 (3way) : 384MB -> nr_hugepages = 193.

Credits:
- volbil   : power2b      : https://github.com/volbil/yespower 
- verium   : scrypt^2     : https://github.com/VeriumReserve/verium/tree/master/src/crypto 
- joelao95 : float math's : https://github.com/m-pays/magi/blob/master/src/magimath.cpp 
- joelao95 : hash add sys : https://github.com/m-pays/magi/blob/master/src/hash_magi.h 
