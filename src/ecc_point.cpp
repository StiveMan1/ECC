#include "ecc.h"
#include "iostream"

using namespace ecc;

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
    if ((str[1] - '0') - 2 != mpz_get_ui(temp2)) {
        mpz_sub(result.y, _curve.p, result.y);
    }
    mpz_clear(temp2);
    return result;
}

ecc::point point::from_string(const std::string &str, const curve *_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
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
    if ((str[1] - '0') - 2 != mpz_get_ui(temp2)) {
        mpz_sub(result.y, _curve->p, result.y);
    }
    mpz_clear(temp2);
    return result;
}

void point::set_from_string(const std::string &str, const curve *_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
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
    if ((str[1] - '0') - 2 != mpz_get_ui(temp2)) {
        mpz_sub(this->y, _curve->p, this->y);
    }
    mpz_clear(temp2);
}

void point::set_from_string(const std::string &str, const curve &_curve) {
    if ('0' != str[0] || ('2' != str[1] && '3' != str[1])) {
        throw IncorrectPoint();
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
    if ((str[1] - '0') - 2 != mpz_get_ui(temp2)) {
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
