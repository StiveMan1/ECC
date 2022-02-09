#include "ecc.h"

void ecc_signature_init(ecc_signature *res) {
    res->r = malloc(sizeof(bi_t));
    res->s = malloc(sizeof(bi_t));
    bi_inits(res->r, res->s, NULL);
}

void ecc_signature_init_set(ecc_signature *res, const bi_pt r, const bi_pt s) {
    res->r = malloc(sizeof(bi_t));
    res->s = malloc(sizeof(bi_t));
    bi_init_set(res->r, r);
    bi_init_set(res->s, s);
}

void ecc_signature_init_copy(ecc_signature *res, const ecc_signature *a) {
    res->r = malloc(sizeof(bi_t));
    res->s = malloc(sizeof(bi_t));
    bi_init_set(res->r, a->r);
    bi_init_set(res->s, a->s);
}

void ecc_signature_clear(ecc_signature *res) {
    bi_clears(res->r, res->s, NULL);
    free(res);
}

void ecc_signature_copy(ecc_signature *res, const ecc_signature *a) {
    bi_set(res->r, a->r);
    bi_set(res->s, a->s);
}

void ecc_signature_set(ecc_signature *res, const bi_pt r, const bi_pt s) {
    bi_set(res->r, r);
    bi_set(res->s, s);
}