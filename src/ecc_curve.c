#include "ecc.h"


void ecc_curve_init(ecc_curve *res) {
    res->a = malloc(sizeof(bi_t));
    res->b = malloc(sizeof(bi_t));
    res->p = malloc(sizeof(bi_t));
    res->n = malloc(sizeof(bi_t));
    res->g = malloc(sizeof(ecc_point));
    res->h = 0;
    ecc_point_init(res->g);
    bi_inits(res->a, res->b, res->p, res->n, NULL);
}

void ecc_curve_init_set(ecc_curve *res, const bi_pt a, const bi_pt b, const bi_pt p, const bi_pt n, int h,
                        const ecc_point *G) {
    res->a = malloc(sizeof(bi_t));
    res->b = malloc(sizeof(bi_t));
    res->p = malloc(sizeof(bi_t));
    res->n = malloc(sizeof(bi_t));
    res->g = malloc(sizeof(ecc_point));
    bi_init_set(res->a, a);
    bi_init_set(res->b, b);
    bi_init_set(res->p, p);
    bi_init_set(res->n, n);
    ecc_point_init_copy(res->g, G);
    res->h = h;
}

void ecc_curve_init_copy(ecc_curve *res, ecc_curve *a) {
    res->a = malloc(sizeof(bi_t));
    res->b = malloc(sizeof(bi_t));
    res->p = malloc(sizeof(bi_t));
    res->n = malloc(sizeof(bi_t));
    res->g = malloc(sizeof(ecc_point));
    bi_init_set(res->a, a->a);
    bi_init_set(res->b, a->b);
    bi_init_set(res->p, a->p);
    bi_init_set(res->n, a->n);
    ecc_point_init_copy(res->g, a->g);
    res->h = a->h;
}

void ecc_curve_clear(ecc_curve *res) {
    bi_clears(res->a, res->b, res->p, res->n, NULL);
    ecc_point_clear(res->g);
    free(res);
}

void ecc_curve_copy(ecc_curve *res, ecc_curve *a) {
    bi_set(res->a, a->a);
    bi_set(res->b, a->b);
    bi_set(res->p, a->p);
    bi_set(res->n, a->n);
    ecc_point_copy(res->g, a->g);
    res->h = a->h;
}

void
ecc_curve_set(ecc_curve *res, const bi_pt a, const bi_pt b, const bi_pt p, const bi_pt n, int h, const ecc_point *G) {
    bi_set(res->a, a);
    bi_set(res->b, b);
    bi_set(res->p, p);
    bi_set(res->n, n);
    ecc_point_copy(res->g, G);
    res->h = h;
}


void ecc_point_doubling(ecc_point *res, const ecc_point *P, const ecc_curve *curve) {
    if (P->y->size != 0) {
        bi_pt slope = malloc(sizeof(bi_t));
        bi_pt temp = malloc(sizeof(bi_t));
        bi_inits(slope, temp, NULL);

        bi_mul_ui(temp, P->y, 2);
        bi_inverse(temp, temp, curve->p);
        bi_mul(slope, P->x, P->x);
        bi_mul_ui(slope, slope, 3);
        bi_add(slope, slope, curve->a);
        bi_mul(slope, slope, temp);
        bi_mod(slope, slope, curve->p);
        bi_set(temp, P->x);

        bi_mul(res->x, slope, slope);
        bi_sub(res->x, res->x, temp);
        bi_sub(res->x, res->x, temp);

        bi_sub(temp, temp, res->x);
        bi_mul(temp, temp, slope);
        bi_sub(res->y, temp, P->y);

        bi_mod(res->x, res->x, curve->p);
        bi_mod(res->y, res->y, curve->p);
        bi_clears(slope, temp, NULL);
    } else {
        bi_resize(res->x, 0);
        bi_resize(res->y, 0);
    }
}

void ecc_point_addition(ecc_point *res, const ecc_point *P, const ecc_point *Q, const ecc_curve *curve) {

    if (P->x->size == 0 && P->y->size == 0) {
        return ecc_point_copy(res, Q);
    }
    if (Q->x->size == 0 && Q->y->size == 0) {
        return ecc_point_copy(res, P);
    }
    if (bi_cmp(P->x, Q->x) == 0) {
        if (bi_cmp(P->y, Q->y) == 0) {
            return ecc_point_doubling(res, P, curve);
        }
        bi_resize(res->x, 0);
        bi_resize(res->y, 0);
        return;
    }


    bi_pt slope = malloc(sizeof(bi_t));
    bi_pt temp = malloc(sizeof(bi_t));
    bi_pt temp1 = malloc(sizeof(bi_t));
    bi_inits(slope, temp, temp1, NULL);

    bi_sub(temp, P->x, Q->x);
    bi_mod(temp, temp, curve->p);
    bi_inverse(temp, temp, curve->p);
    bi_sub(slope, P->y, Q->y);
    bi_mul(slope, slope, temp);
    bi_mod(slope, slope, curve->p);

    bi_mul(temp, slope, slope);
    bi_sub(temp, temp, P->x);
    bi_sub(temp, temp, Q->x);

    bi_sub(temp1, P->x, temp);
    bi_mul(temp1, temp1, slope);
    bi_sub(temp1, temp1, P->y);

    bi_mod(res->x, temp, curve->p);
    bi_mod(res->y, temp1, curve->p);
    bi_clears(slope, temp, temp1, NULL);
}
//e687edac608b4ece31b128337c645a83c56c028e90229f2d36fe490387850abb

void ecc_scalar_multiplication_point(ecc_point *res, const ecc_point *P, const bi_pt m, const ecc_curve *curve) {
    if (m->size == 0) {
        bi_resize(res->x, 0);
        bi_resize(res->y, 0);
        return;
    }
    ecc_point *Q = malloc(sizeof(ecc_point));
    ecc_point_init_copy(Q, P);
    unsigned size = m->size * BIG_DIGIT_LEN;
    unsigned _size = 0;
    for (unsigned i = 0; i < size; i++)
        if (m->digits[i / BIG_DIGIT_LEN] & (1 << (i % BIG_DIGIT_LEN))) _size = i;
    for (unsigned i = 0; i <= _size; i++) {
        if (m->digits[i / BIG_DIGIT_LEN] & (1 << (i % BIG_DIGIT_LEN))) ecc_point_addition(res, res, Q, curve);
        ecc_point_doubling(Q, Q, curve);
    }
}

void ecc_scalar_multiplication(ecc_point *res, const bi_pt m, const ecc_curve *curve) {
    return ecc_scalar_multiplication_point(res, curve->g, m, curve);
}

void ecc_curve_set_secp256k1(ecc_curve *res) {
    res->h = 1;
    bi_set_ui(res->a, 0);
    bi_set_ui(res->b, 7);
    char p[] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
    bi_set_str_16(res->p, p, strlen(p));
    char n[] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
    bi_set_str_16(res->n, n, strlen(n));
    char G[] = "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
    ecc_point_form_string(res->g, G, strlen(G), res);
}

