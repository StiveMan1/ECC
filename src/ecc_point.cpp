#include "ecc.h"
#include "big_int.h"

using namespace ecc;
#ifndef gmp

point::point() {
    this->x = (bi_pt) malloc(sizeof(bi_t));
    this->y = (bi_pt) malloc(sizeof(bi_t));
    bi_inits(this->x, this->y, NULL);
}

point::~point() {
    bi_clears(this->x, this->y, NULL);
}

point::point(const ecc::point *a) {
    this->x = (bi_pt) malloc(sizeof(bi_t));
    this->y = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->x, a->x);
    bi_init_set(this->y, a->y);
}

point::point(const bi_pt x, const bi_pt y) {
    this->x = (bi_pt) malloc(sizeof(bi_t));
    this->y = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->x, x);
    bi_init_set(this->y, y);
}

point::point(const ecc::point &a) {
    this->x = (bi_pt) malloc(sizeof(bi_t));
    this->y = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->x, a.x);
    bi_init_set(this->y, a.y);
}

void point::copy(const ecc::point &a) {
    bi_set(this->x, a.x);
    bi_set(this->y, a.y);
}

void point::copy(const ecc::point *a) {
    bi_set(this->x, a->x);
    bi_set(this->y, a->y);
}


void point::set(const bi_pt _x, const bi_pt _y) {
    bi_set(this->x, _x);
    bi_set(this->y, _y);
}

ecc::point point::from_string(const std::string &str, const curve &_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
    }
    ecc::point result;
    bi_pt temp2 = (bi_pt) malloc(sizeof(bi_t));
    bi_init(temp2);
    std::string tmp_str = str.substr(2);
    bi_set_str_16(result.x, tmp_str.c_str(), tmp_str.size());

    // y = (x^3 + ax + b) % p
    bi_pow_ui(result.y, result.x, 3);
    bi_mod(result.y, result.y, _curve.p);
    bi_add(result.y, result.y, _curve.b);
    bi_mul(temp2, result.x, _curve.a);
    bi_add(result.y, result.y, temp2);
    bi_mod(result.y, result.y, _curve.p);

    // y = y ^ ((p+1)//4) % p
    bi_add_ui(temp2, _curve.p, 1);
    bi_bool_rs(temp2, temp2, 2);
    bi_pow_mod(result.y, result.y, temp2, _curve.p);

    bi_mod_ui(temp2, result.y, 2);
    if ((str[1] - '0') - 2 != bi_get_ui(temp2)) {
        bi_sub(result.y, _curve.p, result.y);
    }
    bi_clear(temp2);
    return result;
}

ecc::point point::from_string(const std::string &str, const curve *_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
    }
    ecc::point result;
    bi_pt temp2 = (bi_pt) malloc(sizeof(bi_t));
    bi_init(temp2);
    std::string tmp_str = str.substr(2);
    bi_set_str_16(result.x, tmp_str.c_str(), tmp_str.size());

    // y = (x^3 + ax + b) % p
    bi_pow_ui(result.y, result.x, 3);
    bi_mod(result.y, result.y, _curve->p);
    bi_add(result.y, result.y, _curve->b);
    bi_mul(temp2, result.x, _curve->a);
    bi_add(result.y, result.y, temp2);
    bi_mod(result.y, result.y, _curve->p);

    // y = y ^ ((p+1)//4) % p
    bi_add_ui(temp2, _curve->p, 1);
    bi_bool_rs(temp2, temp2, 2);
    bi_pow_mod(result.y, result.y, temp2, _curve->p);

    bi_mod_ui(temp2, result.y, 2);
    if ((str[1] - '0') - 2 != bi_get_ui(temp2)) {
        bi_sub(result.y, _curve->p, result.y);
    }
    bi_clear(temp2);
    return result;
}

void point::set_from_string(const std::string &str, const curve *_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
    }
    bi_pt temp2 = (bi_pt) malloc(sizeof(bi_t));
    bi_init(temp2);
    std::string tmp_str = str.substr(2);
    bi_set_str_16(this->x, tmp_str.c_str(), tmp_str.size());

    // y = (x^3 + ax + b) % p
    bi_pow_ui(this->y, this->x, 3);
    bi_mod(this->y, this->y, _curve->p);
    bi_add(this->y, this->y, _curve->b);
    bi_mul(temp2, this->x, _curve->a);
    bi_add(this->y, this->y, temp2);
    bi_mod(this->y, this->y, _curve->p);

    // y = y ^ ((p+1)//4) % p
    bi_add_ui(temp2, _curve->p, 1);
    bi_bool_rs(temp2, temp2, 2);
    bi_pow_mod(this->y, this->y, temp2, _curve->p);

    bi_mod_ui(temp2, this->y, 2);
    if ((str[1] - '0') - 2 != bi_get_ui(temp2)) {
        bi_sub(this->y, _curve->p, this->y);
    }
    bi_clear(temp2);
}

void point::set_from_string(const std::string &str, const curve &_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
    }
    bi_pt temp2 = (bi_pt) malloc(sizeof(bi_t));
    bi_init(temp2);
    std::string tmp_str = str.substr(2);
    bi_set_str_16(this->x, tmp_str.c_str(), tmp_str.size());

    // y = (x^3 + ax + b) % p
    bi_pow_ui(this->y, this->x, 3);
    bi_mod(this->y, this->y, _curve.p);
    bi_add(this->y, this->y, _curve.b);
    bi_mul(temp2, this->x, _curve.a);
    bi_add(this->y, this->y, temp2);
    bi_mod(this->y, this->y, _curve.p);

    // y = y ^ ((p+1)//4) % p
    bi_add_ui(temp2, _curve.p, 1);
    bi_bool_rs(temp2, temp2, 2);
    bi_pow_mod(this->y, this->y, temp2, _curve.p);

    bi_mod_ui(temp2, this->y, 2);
    if ((str[1] - '0') - 2 != bi_get_ui(temp2)) {
        bi_sub(this->y, _curve.p, this->y);
    }
    bi_clear(temp2);
}

std::string point::to_string() const {
    std::string result = bi_str_get_16(this->x);
    std::string prefix = "0";
    bi_pt temp2 = (bi_pt) malloc(sizeof(bi_t));
    bi_init(temp2);
    bi_mod_ui(temp2, this->y, 2);
    prefix += (char) (bi_get_ui(temp2) + 2 + '0');
    bi_clear(temp2);
    return prefix + result;
}

#else

point::point() {
    mpz_inits(this->x, this->y, NULL);
}

point::~point() {
    mpz_clears(this->x, this->y, NULL);
}

point::point(const ecc::point *a) {
    mpz_init_set(this->x, a->x);
    mpz_init_set(this->y, a->y);
}

point::point(const mpz_t &x, const mpz_t &y) {
    mpz_init_set(this->x, x);
    mpz_init_set(this->y, y);
}

point::point(const ecc::point &a) {
    mpz_init_set(this->x, a.x);
    mpz_init_set(this->y, a.y);
}

void point::copy(const ecc::point &a) {
    mpz_set(this->x, a.x);
    mpz_set(this->y, a.y);
}

void point::copy(const ecc::point *a) {
    mpz_set(this->x, a->x);
    mpz_set(this->y, a->y);
}


void point::set(const mpz_t &_x, const mpz_t &_y) {
    mpz_set(this->x, _x);
    mpz_set(this->y, _y);
}

ecc::point point::from_string(const std::string &str, const curve &_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
    }
    for(char i : str){
        if((i < '0' || i > '9') && (i < 'a' || i > 'z') && (i < 'A' || i > 'Z')){
            throw IncorrectPoint();
        }
    }
    ecc::point result;
    mpz_t temp2;
    mpz_init(temp2);
    mpz_set_str(result.x, str.substr(2).c_str(), 16);

    // y = (x^3 + ax + b) % p
    mpz_pow_ui(result.y, result.x, 3);
    mpz_mod(result.y, result.y, _curve.p);
    mpz_add(result.y, result.y, _curve.b);
    mpz_mul(temp2, result.x, _curve.a);
    mpz_add(result.y, result.y, temp2);
    mpz_mod(result.y, result.y, _curve.p);

    // y = y ^ ((p+1)//4) % p
    mpz_add_ui(temp2, _curve.p, 1);
    mpz_tdiv_q_2exp(temp2, temp2, 2);
    mpz_powm(result.y, result.y, temp2, _curve.p);

    mpz_mod_ui(temp2, result.y, 2);
    if ((str[1] - '0') - 2 != mpz_get_si(temp2)) {
        mpz_sub(result.y, _curve.p, result.y);
    }
    mpz_clear(temp2);
    return result;
}

ecc::point point::from_string(const std::string &str, const curve *_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
    }
    for(char i : str){
        if((i < '0' || i > '9') && (i < 'a' || i > 'z') && (i < 'A' || i > 'Z')){
            throw IncorrectPoint();
        }
    }
    ecc::point result;
    mpz_t temp2;
    mpz_init(temp2);
    mpz_set_str(result.x, str.substr(2).c_str(), 16);

    // y = (x^3 + ax + b) % p
    mpz_pow_ui(result.y, result.x, 3);
    mpz_mod(result.y, result.y, _curve->p);
    mpz_add(result.y, result.y, _curve->b);
    mpz_mul(temp2, result.x, _curve->a);
    mpz_add(result.y, result.y, temp2);
    mpz_mod(result.y, result.y, _curve->p);

    // y = y ^ ((p+1)//4) % p
    mpz_add_ui(temp2, _curve->p, 1);
    mpz_tdiv_q_2exp(temp2, temp2, 2);
    mpz_powm(result.y, result.y, temp2, _curve->p);

    mpz_mod_ui(temp2, result.y, 2);
    if ((str[1] - '0') - 2 != mpz_get_si(temp2)) {
        mpz_sub(result.y, _curve->p, result.y);
    }
    mpz_clear(temp2);
    return result;
}

void point::set_from_string(const std::string &str, const curve *_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
    }
    for(char i : str){
        if((i < '0' || i > '9') && (i < 'a' || i > 'z') && (i < 'A' || i > 'Z')){
            throw IncorrectPoint();
        }
    }
    mpz_t temp2;
    mpz_init(temp2);
    mpz_set_str(this->x, str.substr(2).c_str(), 16);

    // y = (x^3 + ax + b) % p
    mpz_pow_ui(this->y, this->x, 3);
    mpz_mod(this->y, this->y, _curve->p);
    mpz_add(this->y, this->y, _curve->b);
    mpz_mul(temp2, this->x, _curve->a);
    mpz_add(this->y, this->y, temp2);
    mpz_mod(this->y, this->y, _curve->p);

    // y = y ^ ((p+1)//4) % p
    mpz_add_ui(temp2, _curve->p, 1);
    mpz_tdiv_q_2exp(temp2, temp2, 2);
    mpz_powm(this->y, this->y, temp2, _curve->p);

    mpz_mod_ui(temp2, this->y, 2);
    if ((str[1] - '0') - 2 != mpz_get_si(temp2)) {
        mpz_sub(this->y, _curve->p, this->y);
    }
    mpz_clear(temp2);
}

void point::set_from_string(const std::string &str, const curve &_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
    }
    for(char i : str){
        if((i < '0' || i > '9') && (i < 'a' || i > 'z') && (i < 'A' || i > 'Z')){
            throw IncorrectPoint();
        }
    }
    mpz_t temp2;
    mpz_init(temp2);
    mpz_set_str(this->x, str.substr(2).c_str(), 16);

    // y = (x^3 + ax + b) % p
    mpz_pow_ui(this->y, this->x, 3);
    mpz_mod(this->y, this->y, _curve.p);
    mpz_add(this->y, this->y, _curve.b);
    mpz_mul(temp2, this->x, _curve.a);
    mpz_add(this->y, this->y, temp2);
    mpz_mod(this->y, this->y, _curve.p);

    // y = y ^ ((p+1)//4) % p
    mpz_add_ui(temp2, _curve.p, 1);
    mpz_tdiv_q_2exp(temp2, temp2, 2);
    mpz_powm(this->y, this->y, temp2, _curve.p);

    mpz_mod_ui(temp2, this->y, 2);
    if ((str[1] - '0') - 2 != mpz_get_si(temp2)) {
        mpz_sub(this->y, _curve.p, this->y);
    }
    mpz_clear(temp2);
}

std::string point::to_string() const {
    std::string result = mpz_str_get(this->x);
    std::string prefix = "0";
    mpz_t temp2;
    mpz_init(temp2);
    mpz_mod_ui(temp2, this->y, 2);
    prefix += (char) (mpz_get_ui(temp2) + 2 + '0');
    mpz_clear(temp2);
    return prefix + result;
}

#endif