#include <stdlib.h>
#include <stdio.h>

#include "uint256.h"
#include "scrypt.h"

int main(void)
{
    const char *test = "test";
    uint256 hash;

    scryptHash(test, BEGIN(hash));

    printf("%s\n", hash.GetHex().c_str());
    return 0;
}
