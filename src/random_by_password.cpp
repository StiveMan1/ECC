#include "ecc.h"

using namespace ecc;


#ifndef gmp

random_by_password::random_by_password(const std::string &password) {
    this->master_key = hash::code(password + hash::code(password, 1), 1);
    this->counter = 0;
}

random_by_password::~random_by_password() = default;

void random_by_password::random_rand(bi_pt result, unsigned n) {
    this->counter++;
    std::string y, res;
    unsigned size = 0;

    while (size < n) {
        y += this->master_key + std::to_string(this->counter);
        y = hash::code(y, 1);
        for (unsigned j = 0; j < y.length() && size < n; j += 2, size++) {
            res += y[j];
            res += y[j + 1];
        }
    }
    bi_set_str_16(result, res.c_str());
}

#else

random_by_password::random_by_password(const std::string &password) {
    this->master_key = hash::code(password + hash::code(password, 1), 1);
    this->counter = 0;
}

random_by_password::~random_by_password() = default;

void random_by_password::random_rand(mpz_t result, unsigned n) {
    this->counter++;
    std::string y, res;
    unsigned size = 0;

    while (size < n) {
        y += this->master_key + std::to_string(this->counter);
        y = hash::code(y, 1);
        for (unsigned j = 0; j < y.length() && size < n; j += 2, size++) {
            res += y[j];
            res += y[j + 1];
        }
    }
    mpz_set_str(result, res.c_str(), 16);
}

#endif
