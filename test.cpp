#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils/uint256.h"

#include "scrypt/scrypt.h"
#include "power2b/yespower.h"

int main(void)
{
    yespower_params_t params = { /* power2b init struct */
        .N = 1024,
        .r = 8
    };

    uint8_t test[80] = "test";

    uint256 hash_scrypt;
    yespower_binary_t hash_power2b;

    scryptHash((const char *)test, BEGIN(hash_scrypt));
    yespower_tls(test, sizeof(test), &params, &hash_power2b);

    printf("Scrypt^2 hash: %s\n", hash_scrypt.GetHex().c_str());
    printf("Power2b hash : ");

    for (int i = 0; i < sizeof(hash_power2b); i++) /* print output of power2b */
		printf("%02x", hash_power2b.uc[i]);

	printf("\n");
    return 0;
}
