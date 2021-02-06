#ifndef XPI_H
#define XPI_H

#include <gmpxx.h>

#include "utils/uint256.h"
#include "utils/m7math.h"

#include "common/sph_sha2.h"
#include "scrypt/scrypt.h"
#include "power2b/yespower.h"

#define BITS_PER_DIGIT 3.32192809488736234787
#define EPS (std::numeric_limits<double>::epsilon())

#define NM7M 5
#define SW_DIVS 5

yespower_params_t params = { /* power2b init struct */
    .N = 1024,
    .r = 8
};

template<typename T1>
inline uint256 hash_M7M_v2(const T1 pbegin, const T1 pend, const unsigned int nnNonce)
{
    sph_sha256_context ctx_sha256;

    static unsigned char pblank[1];
    int bytes, nnNonce2 = (int)(nnNonce / 2);

    uint512 hash[2];
    uint256 finalhash;

    for(int i = 0 ; i < 2 ; i++)
        hash[i] = 0;

    const void* ptr = (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0]));
    size_t sz = (pend - pbegin) * sizeof(pbegin[0]);

    /* -------------------- POWER2B STUFF --------------------------------*/

    yespower_binary_t str_power2b;
    yespower_tls((const uint8_t *)ptr, sz, &params, &str_power2b);

    char str_power2b_2[65];
    for(int x = 0 ; x < 64 ; x++)
        sprintf(str_power2b_2 + (x * 2), "%02x", str_power2b.uc[x]);

    str_power2b_2[64] = '\0';
    hash[0].SetHex((char *)str_power2b_2);

    /* --------------------- END POWER2B ----------------------------------*/

    scryptHash(hash[0].GetHex().c_str(), BEGIN(hash[1]));

    //printf("power2b: %s\n", hash[0].GetHex().c_str());
    //printf("scrypt^2: %s\n", hash[1].GetHex().c_str());

    mpz_t bns[3];
    //Take care of zeros and load gmp
    for(int i = 0 ; i < 2 ; i++)
    {
        if(hash[i] == 0)
            hash[i] = 1;

        mpz_init(bns[i]);
        mpz_set_uint512(bns[i], hash[i]);
    }

    mpz_init(bns[2]);
    mpz_set_ui(bns[2], 0);
    for(int i = 0 ; i < 2 ; i++)
        mpz_add(bns[2], bns[2], bns[i]);

    mpz_t product;
    mpz_init(product);
    mpz_set_ui(product, 1);

    //mpz_pow_ui(bns[7], bns[7], 2);
    for(int i = 0 ; i < 3 ; i++)
        mpz_mul(product, product, bns[i]);

    mpz_pow_ui(product, product, 2);

    bytes = mpz_sizeinbase(product, 256);
    //printf("M7M data space: %iB\n", bytes);
    char *data = (char*)malloc(bytes);
    mpz_export(data, NULL, -1, 1, 0, 0, product);

    sph_sha256_init(&ctx_sha256);
    //ZSHA256;
    sph_sha256 (&ctx_sha256, data, bytes);
    sph_sha256_close(&ctx_sha256, static_cast<void*>(&finalhash));
    //printf("finalhash = %s\n", hash[6].GetHex().c_str());
    free(data);

    int digits = (int)((sqrt((double)(nnNonce2)) * (1. + EPS)) / 9000 + 75);
    //int iterations=(int)((sqrt((double)(nnNonce2))+EPS)/500+350); // <= 500
    //int digits=100;
    int iterations = 20; // <= 500
    mpf_set_default_prec((long int)(digits * BITS_PER_DIGIT + 16));

    mpz_t magipi;
    mpz_t magisw;
    mpf_t magifpi;
    mpf_t mpa1, mpb1, mpt1, mpp1;
    mpf_t mpa2, mpb2, mpt2, mpp2;
    mpf_t mpsft;

    mpz_init(magipi);
    mpz_init(magisw);
    mpf_init(magifpi);
    mpf_init(mpsft);
    mpf_init(mpa1);
    mpf_init(mpb1);
    mpf_init(mpt1);
    mpf_init(mpp1);

    mpf_init(mpa2);
    mpf_init(mpb2);
    mpf_init(mpt2);
    mpf_init(mpp2);

    uint32_t usw_;
    usw_ = sw_(nnNonce2, SW_DIVS);
    if (usw_ < 1) usw_ = 1;
        mpz_set_ui(magisw, usw_);

    uint32_t mpzscale = mpz_size(magisw);
    for(int i = 0 ; i < NM7M ; i++)
    {
        if (mpzscale > 1000)
            mpzscale = 1000;
        else if (mpzscale < 1)
            mpzscale = 1;

        //if(fDebugMagi) printf("mpzscale: %d\n", mpzscale);

        mpf_set_ui(mpa1, 1);
        mpf_set_ui(mpb1, 2);
        mpf_set_d(mpt1, 0.25 * mpzscale);
        mpf_set_ui(mpp1, 1);
        mpf_sqrt(mpb1, mpb1);
        mpf_ui_div(mpb1, 1, mpb1);
        mpf_set_ui(mpsft, 10);

        for(int i = 0 ; i <= iterations ; i++)
        {
            mpf_add(mpa2, mpa1, mpb1);
            mpf_div_ui(mpa2, mpa2, 2);
            mpf_mul(mpb2, mpa1, mpb1);
            mpf_abs(mpb2, mpb2);
            mpf_sqrt(mpb2, mpb2);
            mpf_sub(mpt2, mpa1, mpa2);
            mpf_abs(mpt2, mpt2);
            mpf_sqrt(mpt2, mpt2);
            mpf_mul(mpt2, mpt2, mpp1);
            mpf_sub(mpt2, mpt1, mpt2);
            mpf_mul_ui(mpp2, mpp1, 2);
            mpf_swap(mpa1, mpa2);
            mpf_swap(mpb1, mpb2);
            mpf_swap(mpt1, mpt2);
            mpf_swap(mpp1, mpp2);
        }

        mpf_add(magifpi, mpa1, mpb1);
        mpf_pow_ui(magifpi, magifpi, 2);
        mpf_div_ui(magifpi, magifpi, 4);
        mpf_abs(mpt1, mpt1);
        mpf_div(magifpi, magifpi, mpt1);

        //mpf_out_str(stdout, 10, digits+2, magifpi);

        mpf_pow_ui(mpsft, mpsft, digits / 2);
        mpf_mul(magifpi, magifpi, mpsft);

        mpz_set_f(magipi, magifpi);

        //mpz_set_ui(magipi,1);

        mpz_add(product,product,magipi);
        mpz_add(product,product,magisw);

        if(finalhash == 0) finalhash = 1;
            mpz_set_uint256(bns[0], finalhash);

        mpz_add(bns[2], bns[2], bns[0]);

        mpz_mul(product, product, bns[2]);
        mpz_cdiv_q (product, product, bns[0]);
        if (mpz_sgn(product) <= 0)
            mpz_set_ui(product, 1);

        bytes = mpz_sizeinbase(product, 256);
        mpzscale = bytes;
        //printf("M7M data space: %iB\n", bytes);
        char *bdata = (char*)malloc(bytes);
        mpz_export(bdata, NULL, -1, 1, 0, 0, product);

        sph_sha256_init(&ctx_sha256);
        // ZSHA256;
        sph_sha256 (&ctx_sha256, bdata, bytes);
        sph_sha256_close(&ctx_sha256, static_cast<void*>(&finalhash));
        free(bdata);
        //    printf("finalhash = %s\n", finalhash.GetHex().c_str());
    }
    //Free the memory
    for(int i = 0 ; i < 3 ; i++)
        mpz_clear(bns[i]);

    //mpz_clear(dSpectralWeight);
    mpz_clear(product);

    mpz_clear(magipi);
    mpz_clear(magisw);
    mpf_clear(magifpi);
    mpf_clear(mpsft);
    mpf_clear(mpa1);
    mpf_clear(mpb1);
    mpf_clear(mpt1);
    mpf_clear(mpp1);

    mpf_clear(mpa2);
    mpf_clear(mpb2);
    mpf_clear(mpt2);
    mpf_clear(mpp2);

    return finalhash;
}

#endif // XPI_H
