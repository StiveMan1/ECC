#include "ecc.h"

void ecc_key_init_set_public(ecc_key *res, const ecc_point *p, ecc_curve *curve) {
    res->p = malloc(sizeof(ecc_point));
    ecc_point_init_copy(res->p, p);
    res->d = malloc(sizeof(bi_t));
    bi_init(res->d);
    res->curve = curve;
    res->isPrivate = 0;
}

void ecc_key_init_set_private(ecc_key *res, const bi_pt d, ecc_curve *curve) {
    res->p = malloc(sizeof(ecc_point));
    res->d = malloc(sizeof(bi_t));
    res->isPrivate = 1;

    bi_init_set(res->d, d);
    ecc_point_init(res->p);
    ecc_scalar_multiplication(res->p, d, curve);
    res->curve = curve;
}

void ecc_key_init_copy(ecc_key *res, const ecc_key *a) {
    res->p = malloc(sizeof(ecc_point));
    res->isPrivate = a->isPrivate;
    res->d = malloc(sizeof(bi_t));
    bi_init(res->d);

    if (res->isPrivate) {
        res->d = malloc(sizeof(bi_t));
        bi_set(res->d, a->d);
    }
    ecc_point_init_copy(res->p, a->p);
    res->curve = a->curve;
}

void ecc_key_init_from_address(ecc_key *res, const char str[], size_t size, ecc_curve *curve) {
    res->p = malloc(sizeof(ecc_point));
    ecc_point_init(res->p);
    res->d = malloc(sizeof(bi_t));
    bi_init(res->d);
    ecc_point_form_string(res->p, str, size, curve);
    res->curve = curve;
}

void ecc_key_init_from_private_key(ecc_key *res, const char str[], size_t size, ecc_curve *curve) {
    if (str[0] != '1') {
//        throw;
    }
    res->d = malloc(sizeof(bi_t));
    bi_init(res->d);
    res->p = malloc(sizeof(ecc_point));
    ecc_point_init(res->p);
    bi_set_str_16(res->d, &str[1], size - 1);
    ecc_scalar_multiplication(res->p, res->d, curve);
    res->curve = curve;
}

void ecc_key_clear(ecc_key *res) {
    bi_clear(res->d);
    ecc_point_clear(res->p);
    free(res);
}

void ecc_key_copy(ecc_key *res, const ecc_key *a) {
    res->isPrivate = a->isPrivate;
    if (res->isPrivate) {
        bi_set(res->d, a->d);
    }
    ecc_point_copy(res->p, a->p);
    res->curve = a->curve;
}

void ecc_key_set_public(ecc_key *res, const ecc_point *p, ecc_curve *curve) {
    ecc_point_copy(res->p, p);
    res->curve = curve;
    res->isPrivate = 0;
}

void ecc_key_set_private(ecc_key *res, const bi_pt d, ecc_curve *curve) {
    bi_set(res->d, d);
    ecc_scalar_multiplication(res->p, d, curve);
    res->curve = curve;
    res->isPrivate = 1;
}


void ecc_key_from_address(ecc_key *res, const char str[], size_t size, ecc_curve *curve) {
    ecc_point_form_string(res->p, str, size, curve);
    res->curve = curve;
}

void ecc_key_from_private_key(ecc_key *res, const char str[], size_t size, ecc_curve *curve) {
    if (str[0] != '1') {
//        throw;
    }
    bi_set_str_16(res->d, &str[1], size - 1);
    res->curve = curve;
}

char *ecc_key_get_address(ecc_key *res, size_t *res_size) {
    return ecc_point_to_string(res->p, res_size);
}

char *ecc_key_get_private_key(ecc_key *res, size_t *res_size) {
    char *res1 = bi_get_str_16(res->d, res_size);
    char *result = malloc(sizeof(char) * (*res_size + 1));
    for (unsigned i = 0; i < *res_size; i++) {
        result[i + 1] = res1[i];
    }
    free(res1);
    result[0] = '1';
    *res_size += 1;
    return result;
}

char *
ecc_encode_self(const char str[], size_t size, int hash_type, int crypto_type, const ecc_key *key, size_t *res_size) {
    ecc_point *_point = malloc(sizeof(ecc_point));
    ecc_point_init(_point);
    ecc_scalar_multiplication_point(_point, key->p, key->d, key->curve);
    size_t _hash_size = 0;
    char *_hash = ecc_point_to_string(_point, &_hash_size);
    size_t _key_size = 0;
    char *_key = code(_hash, _hash_size, 1, &_key_size);
    free(_hash);
    char *result = encode(str, size, _key, 1, res_size);
    free(_key);
    ecc_point_clear(_point);
    return result;
}

char *
ecc_decode_self(const char str[], size_t size, int hash_type, int crypto_type, const ecc_key *key, size_t *res_size) {
    ecc_point *_point = malloc(sizeof(ecc_point));
    ecc_point_init(_point);
    ecc_scalar_multiplication_point(_point, key->p, key->d, key->curve);
    size_t _hash_size = 0;
    char *_hash = ecc_point_to_string(_point, &_hash_size);
    size_t _key_size = 0;
    char *_key = code(_hash, _hash_size, 1, &_key_size);
    free(_hash);
    char *result = decode(str, size, _key, 1, res_size);
    free(_key);
    ecc_point_clear(_point);
    return result;
}

char *ecc_encode(const char str[], size_t size, int hash_type, int crypto_type, const ecc_key *key, const bi_pt d,
                 size_t *res_size) {
    ecc_point *_point = malloc(sizeof(ecc_point));
    ecc_point_init(_point);
    ecc_scalar_multiplication_point(_point, key->p, d, key->curve);
    size_t _hash_size = 0;
    char *_hash = ecc_point_to_string(_point, &_hash_size);
    size_t _key_size = 0;
    char *_key = code(_hash, _hash_size, 1, &_key_size);
    free(_hash);
    char *result = decode(str, size, _key, 1, res_size);
    free(_key);
    ecc_point_clear(_point);
    return result;
}

char *
ecc_decode(const char str[], size_t size, int hash_type, int crypto_type, const ecc_key *key, const ecc_point *point,
           size_t *res_size) {
    ecc_point *_point = malloc(sizeof(ecc_point));
    ecc_point_init(_point);
    ecc_scalar_multiplication_point(_point, point, key->d, key->curve);
    size_t _hash_size = 0;
    char *_hash = ecc_point_to_string(_point, &_hash_size);
    size_t _key_size = 0;
    char *_key = code(_hash, _hash_size, 1, &_key_size);
    free(_hash);
    char *result = decode(str, size, _key, 1, res_size);
    free(_key);
    ecc_point_clear(_point);
    return result;
}

void ecc_generate_key(ecc_key *res, const char *password, size_t size, const ecc_curve *curve) {
    res->p = malloc(sizeof(ecc_point));
    res->d = malloc(sizeof(bi_t));
    res->isPrivate = 1;
    ecc_point_init(res->p);
    bi_init(res->d);
    {
        random_by_password *rand = malloc(sizeof(random_by_password));
        random_by_password_init(rand, password, size);
        unsigned _size = curve->n->size;
        while (1) {
            random_rand(_size * 2, res->d, rand);
            bi_mod(res->d, res->d, curve->n);
            ecc_scalar_multiplication(res->p, res->d, curve);
            if (res->p->x->size != 0 && res->d->size != 0) break;
        }
        random_by_password_clear(rand);
    }
}

void ecc_signature_create(ecc_signature *res, const bi_pt hash, const ecc_key *key) {
    bi_pt k = malloc(sizeof(bi_t));
    bi_init(k);
    ecc_point *R = malloc(sizeof(ecc_point));
    ecc_point_init(R);
    do {
        do {
            bi_random(k, key->curve->n->size);
        } while (k->size == 0 || bi_cmp(key->d, k) == 0);
        ecc_scalar_multiplication(R, k, key->curve);

        bi_mul(res->s, res->s, key->d);
        bi_sub(res->s, k, res->s);
        bi_mod(res->s, res->s, key->curve->n);

    } while (res->s->size == 0);

    size_t _size = 0;
    char *str = ecc_point_to_string(R, &_size);
    bi_set_str_16(res->r, str, _size);

    ecc_point_clear(R);
    bi_clear(k);

}

char ecc_signature_check(const ecc_signature *sign, const bi_pt hash, const ecc_key *key) {
    bi_pt temp = malloc(sizeof(bi_t));
    bi_init(temp);
    ecc_point *_point1 = malloc(sizeof(ecc_point));
    ecc_point_init(_point1);
    ecc_point *_point2 = malloc(sizeof(ecc_point));
    ecc_point_init(_point2);

    ecc_scalar_multiplication_point(_point1, key->p, hash, key->curve);
    ecc_scalar_multiplication(_point2, sign->s, key->curve);

    ecc_point_addition(_point1, _point1, _point2, key->curve);

    size_t _size = 0;
    char *str = bi_get_str_16(sign->r, &_size);
    ecc_point_form_string(_point2, str, _size, key->curve);
    free(str);

    char result = (char) (bi_cmp(_point2->x, _point1->x) == 0);

    ecc_point_clear(_point1);
    ecc_point_clear(_point2);
    bi_clear(temp);
    return result;
}
