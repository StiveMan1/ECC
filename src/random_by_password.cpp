#include "ecc.h"

using namespace ecc;


random_by_password::random_by_password(const std::string &password) {
    this->master_key = hash::code(password + hash::code(password, 1), 1);
    this->counter = 0;
}

random_by_password::~random_by_password() = default;

void random_by_password::random_rand(mpz_t result, unsigned n) {
    this->counter++;
    std::string y, res;
    int size = 0;

    while (size < n) {
        y += this->master_key + std::to_string(this->counter);
        y = hash::code(y, 1);
        for (int j = 0; j < y.length() && size < n; j += 2, size++) {
            res += y[j];
            res += y[j + 1];
        }
    }
    mpz_set_str(result, res.c_str(), 16);
}
