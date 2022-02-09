#include "ecc.h"


void ecc_point_init(ecc_point *res) {
    res->x = malloc(sizeof(bi_t));
    res->y = malloc(sizeof(bi_t));
    bi_inits(res->x, res->y, NULL);
}

void ecc_point_init_set(ecc_point *res, const bi_pt x, const bi_pt y) {
    res->x = malloc(sizeof(bi_t));
    res->y = malloc(sizeof(bi_t));
    bi_init_set(res->x, x);
    bi_init_set(res->y, y);
}

void ecc_point_init_copy(ecc_point *res, const ecc_point *a) {
    res->x = malloc(sizeof(bi_t));
    res->y = malloc(sizeof(bi_t));
    bi_init_set(res->x, a->x);
    bi_init_set(res->y, a->y);
}

void ecc_point_clear(ecc_point *res) {
    bi_clears(res->x, res->y, NULL);
    free(res);
}

void ecc_point_copy(ecc_point *res, const ecc_point *a) {
    bi_set(res->x, a->x);
    bi_set(res->y, a->y);
}

void ecc_point_set(ecc_point *res, const bi_pt x, const bi_pt y) {
    bi_set(res->x, x);
    bi_set(res->y, y);
}

void ecc_point_form_string(ecc_point *res, const char str[], const size_t size, const ecc_curve *curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
//        throw;
    }
    bi_pt temp2 = malloc(sizeof(bi_t));
    bi_init(temp2);

    bi_set_str_16(res->x, &str[2], size - 2);


    // y = (x^3 + ax + b) % p
    bi_pow_ui(res->y, res->x, 3);
    bi_mod(res->y, res->y, curve->p);
    bi_add(res->y, res->y, curve->b);
    bi_mul(temp2, res->x, curve->a);
    bi_add(res->y, res->y, temp2);
    bi_mod(res->y, res->y, curve->p);

    // y = y ^ ((p+1)//4) % p
    bi_add_ui(temp2, curve->p, 1);
    bi_right_shift(temp2, temp2, 2);
    bi_pow_mod(res->y, res->y, temp2, curve->p);

    bi_mod_ui(temp2, res->y, 2);
    if ((str[1] - '0') - 2 != bi_get_ui(temp2)) {
        bi_sub(res->y, curve->p, res->y);
    }
    bi_clear(temp2);
}

char *ecc_point_to_string(const ecc_point *res, size_t *res_size) {
    size_t _size = 0;
    char *res1 = bi_get_str_16(res->x, &_size);
    char *result = malloc(sizeof(char) * (_size + 2));
    for (unsigned i = 0; i < _size; i++) {
        result[i + 2] = res1[i];
    }
    free(res1);
    *res_size = _size + 2;
    result[0] = '0';
    bi_pt temp2 = malloc(sizeof(bi_t));
    bi_init(temp2);
    bi_mod_ui(temp2, res->y, 2);
    result[1] = (char) (bi_get_ui(temp2) + 2 + '0');
    bi_clear(temp2);
    return result;
}