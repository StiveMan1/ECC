#ifndef ECC_H
#define ECC_H

#include <big_int.h>
#include <string.h>
#include <hash_code.h>
#include <crypto_code.h>

typedef struct {
    bi_pt x;
    bi_pt y;
} ecc_point;
typedef struct {
    bi_pt a;
    bi_pt b;
    bi_pt n;
    bi_pt p;
    ecc_point *g;
    unsigned h;
} ecc_curve;

void ecc_point_init(ecc_point *res);

void ecc_point_init_set(ecc_point *res, const bi_pt x, const bi_pt y);

void ecc_point_init_copy(ecc_point *res, const ecc_point *a);

void ecc_point_clear(ecc_point *res);

void ecc_point_copy(ecc_point *res, const ecc_point *a);

void ecc_point_set(ecc_point *res, const bi_pt x, const bi_pt y);

void ecc_point_form_string(ecc_point *res, const char str[], size_t size, const ecc_curve *curve);

char *ecc_point_to_string(const ecc_point *res, size_t *res_size);

void ecc_curve_init(ecc_curve *res);

void ecc_curve_init_set(ecc_curve *res, const bi_pt a, const bi_pt b, const bi_pt p, const bi_pt n, int h,
                        const ecc_point *G);

void ecc_curve_init_copy(ecc_curve *res, ecc_curve *a);

void ecc_curve_clear(ecc_curve *res);

void ecc_curve_copy(ecc_curve *res, ecc_curve *a);

void
ecc_curve_set(ecc_curve *res, const bi_pt a, const bi_pt b, const bi_pt p, const bi_pt n, int h, const ecc_point *G);

void ecc_point_doubling(ecc_point *res, const ecc_point *P, const ecc_curve *curve);

void ecc_point_addition(ecc_point *res, const ecc_point *P, const ecc_point *Q, const ecc_curve *curve);

void ecc_scalar_multiplication_point(ecc_point *res, const ecc_point *P, const bi_pt m, const ecc_curve *curve);

void ecc_scalar_multiplication(ecc_point *res, const bi_pt m, const ecc_curve *curve);

void ecc_curve_set_secp256k1(ecc_curve *res);

char *ecc_curve_to_json(ecc_curve *curve, size_t *res_size);

ecc_curve *ecc_curve_from_json(const char *income, size_t size);

typedef struct {
    bi_pt s;
    bi_pt r;
} ecc_signature;

void ecc_signature_init(ecc_signature *res);

void ecc_signature_init_set(ecc_signature *res, const bi_pt r, const bi_pt s);

void ecc_signature_init_copy(ecc_signature *res, const ecc_signature *a);

void ecc_signature_clear(ecc_signature *res);

void ecc_signature_copy(ecc_signature *res, const ecc_signature *a);

void ecc_signature_set(ecc_signature *res, const bi_pt r, const bi_pt s);

char *ecc_signature_to_json(ecc_signature *curve, size_t *res_size);

ecc_signature *ecc_signature_from_json(const char *income, size_t size);

typedef struct {
    int counter;
    char *master_key;
    size_t size;
} random_by_password;

void random_by_password_init(random_by_password *res, const char *password, size_t size);

void random_by_password_clear(random_by_password *random);

void random_rand(unsigned n, bi_pt result, random_by_password *random);

typedef struct {
    bi_pt d;
    ecc_point *p;
    ecc_curve *curve;
    char isPrivate;
} ecc_key;

void ecc_key_init_set_public(ecc_key *res, const ecc_point *p, ecc_curve *curve);

void ecc_key_init_set_private(ecc_key *res, const bi_pt d, ecc_curve *curve);

void ecc_key_init_copy(ecc_key *res, const ecc_key *a);

void ecc_key_init_from_address(ecc_key *res, const char str[], size_t size, ecc_curve *curve);

void ecc_key_init_from_private_key(ecc_key *res, const char str[], size_t size, ecc_curve *curve);

void ecc_key_clear(ecc_key *res);

void ecc_key_copy(ecc_key *res, const ecc_key *a);

void ecc_key_set_public(ecc_key *res, const ecc_point *p, ecc_curve *curve);

void ecc_key_set_private(ecc_key *res, const bi_pt d, ecc_curve *curve);

void ecc_key_from_address(ecc_key *res, const char str[], size_t size, ecc_curve *curve);

void ecc_key_from_private_key(ecc_key *res, const char str[], size_t size, ecc_curve *curve);

char *ecc_key_get_address(ecc_key *res, size_t *res_size);

char *ecc_key_get_private_key(ecc_key *res, size_t *res_size);

void ecc_signature_create(ecc_signature *res, const bi_pt hash, const ecc_key *key);

char ecc_signature_check(const ecc_signature *sign, const bi_pt hash, const ecc_key *key);

char *
ecc_encode_self(const char str[], size_t size, int hash_type, int crypto_type, const ecc_key *key, size_t *res_size);

char *
ecc_decode_self(const char str[], size_t size, int hash_type, int crypto_type, const ecc_key *key, size_t *res_size);

char *ecc_encode(const char str[], size_t size, int hash_type, int crypto_type, const ecc_key *key, const bi_pt d,
                 size_t *res_size);

char *
ecc_decode(const char str[], size_t size, int hash_type, int crypto_type, const ecc_key *key, const ecc_point *point,
           size_t *res_size);

void ecc_generate_key(ecc_key *res, const char *password, size_t size, const ecc_curve *curve);

#endif //ECC_H
