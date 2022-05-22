#include <nlohmann/json.hpp>
#include "ecc.h"

using namespace ecc;


key::key() {
    this->p = new ecc::point();
    this->curve = new ecc::curve();
    this->isPrivate = 0;
    mpz_init(this->d);
}

key::~key() {
    delete this->p;
    delete this->curve;
    mpz_clear(this->d);
}

key::key(const key &a) {
    this->p = new ecc::point(a.p);
    this->curve = new ecc::curve(a.curve);
    this->isPrivate = a.isPrivate;
    mpz_init_set(this->d, a.d);
}

key::key(const key *a) {
    this->p = new ecc::point(a->p);
    this->curve = new ecc::curve(a->curve);
    this->isPrivate = a->isPrivate;
    mpz_init_set(this->d, a->d);
}

void key::copy(const key &a) {
    this->p->copy(a.p);
    this->curve->copy(a.curve);
    this->isPrivate = a.isPrivate;
    if (this->isPrivate) {
        mpz_set(this->d, a.d);
    }
}

void key::copy(const key *a) {
    this->p->copy(a->p);
    this->curve->copy(a->curve);
    this->isPrivate = a->isPrivate;
    if (this->isPrivate) {
        mpz_set(this->d, a->d);
    }
}

ecc::key key::from_address(const std::string &str, const ecc::curve &curve) {
    ecc::key key;
    key.p->set_from_string(str, curve);
    key.curve->copy(curve);
    key.isPrivate = 0;
    return key;
}

ecc::key key::from_address(const std::string &str, const ecc::curve *curve) {
    ecc::key key;
    key.p->set_from_string(str, curve);
    key.curve->copy(curve);
    key.isPrivate = 0;
    return key;
}

ecc::key key::from_private_key(const std::string &str, const ecc::curve &curve) {
    if (str[0] != '1') {
        throw IncorrectKey();
    }
    ecc::key key;
    mpz_set_str(key.d, str.substr(1).c_str(), 16);
    key.curve->copy(curve);
    key.curve->scalar_multiplication(key.p, key.d);
    key.isPrivate = 1;
    return key;
}

ecc::key key::from_private_key(const std::string &str, const ecc::curve *curve) {
    if (str[0] != '1') {
        throw IncorrectKey();
    }
    ecc::key key;
    mpz_set_str(key.d, str.substr(1).c_str(), 16);
    key.curve->copy(curve);
    key.curve->scalar_multiplication(key.p, key.d);
    key.isPrivate = 1;
    return key;
}

void key::set_public(const point &_p, const ecc::curve &_curve) {
    this->p->copy(_p);
    this->curve->copy(_curve);
    this->isPrivate = 0;
}

void key::set_public(const point *_p, const ecc::curve *_curve) {
    this->p->copy(_p);
    this->curve->copy(_curve);
    this->isPrivate = 0;
}

void key::set_private(const std::string &_d, const ecc::curve &_curve) {
    mpz_set_str(this->d, _d.c_str(), 16);
    this->curve->copy(_curve);
    this->curve->scalar_multiplication(this->p, this->d);
    this->isPrivate = 1;
}

void key::set_private(const std::string &_d, const ecc::curve *_curve) {
    mpz_set_str(this->d, _d.c_str(), 16);
    this->curve->copy(_curve);
    this->curve->scalar_multiplication(this->p, this->d);
    this->isPrivate = 1;
}


std::string key::get_address() const {
    return this->p->to_string();
}

std::string key::get_private_key() const {
    if (!this->isPrivate) throw NotPrivateKey();
    std::string result = mpz_str_get(this->d);
    return "1" + result;
}

std::string key::encode_self(const std::string &str, int hash_type, int crypto_type) const {
    if (!this->isPrivate) throw NotPrivateKey();
    ecc::point point;
    this->curve->scalar_multiplication_point(&point, this->p, this->d);
    std::string result = crypto::encode(str, hash::code(point.to_string(), hash_type), crypto_type);
    result = ecc::base64_encode(result);
    return result;
}

std::string key::decode_self(const std::string &str, int hash_type, int crypto_type) const {
    if (!this->isPrivate) throw NotPrivateKey();
    ecc::point point;
    this->curve->scalar_multiplication_point(&point, this->p, this->d);
    std::string result = ecc::base64_decode(str);
    result = crypto::decode(result, hash::code(point.to_string(), hash_type), crypto_type);
    return result;
}

std::string key::encode(const std::string &str, int hash_type, int crypto_type, const ecc::key *key) const {
    ecc::point point;
    this->curve->scalar_multiplication_point(&point, this->p, key->d);
    std::string result = crypto::encode(str, hash::code(point.to_string(), hash_type), crypto_type);
    result = ecc::base64_encode(result);
    return result;
}

std::string key::encode(const std::string &str, int hash_type, int crypto_type, const ecc::key &key) const {
    ecc::point point;
    this->curve->scalar_multiplication_point(&point, this->p, key.d);
    std::string result = crypto::encode(str, hash::code(point.to_string(), hash_type), crypto_type);
    result = ecc::base64_encode(result);
    return result;
}

std::string key::decode(const std::string &str, int hash_type, int crypto_type, const ecc::key *key) const {
    if (!this->isPrivate) throw NotPrivateKey();
    ecc::point point;
    this->curve->scalar_multiplication_point(&point, key->p, this->d);
    std::string result = ecc::base64_decode(str);
    result = crypto::decode(result, hash::code(point.to_string(), hash_type), crypto_type);
    return result;
}

std::string key::decode(const std::string &str, int hash_type, int crypto_type, const ecc::key &key) const {
    if (!this->isPrivate) throw NotPrivateKey();
    ecc::point point;
    this->curve->scalar_multiplication_point(&point, key.p, this->d);
    std::string result = ecc::base64_decode(str);
    result = crypto::decode(result, hash::code(point.to_string(), hash_type), crypto_type);
    return result;
}

key key::generate_key(const std::string &password, const ecc::curve &curve) {
    ecc::key key;
    key.curve->copy(curve);
    key.isPrivate = 1;
    {
        auto *rand = new random_by_password(password);
        unsigned _size = mpz_sizeinbase(key.curve->n, 2) / 8;
        while (true) {
            rand->random_rand(key.d, _size);
            mpz_mod(key.d, key.d, key.curve->n);
            key.curve->scalar_multiplication(key.p, key.d);
            if (mpz_cmp_ui(key.p->x, 0) != 0 && mpz_cmp_ui(key.d, 0) != 0) break;
        }
    }
    return key;
}

key key::generate_key(const std::string &password, const ecc::curve *curve) {
    ecc::key key;
    key.curve->copy(curve);
    key.isPrivate = 1;
    {
        auto *rand = new random_by_password(password);
        unsigned _size = mpz_sizeinbase(key.curve->n, 2) / 8;
        while (true) {
            rand->random_rand(key.d, _size);
            mpz_mod(key.d, key.d, key.curve->n);
            key.curve->scalar_multiplication(key.p, key.d);
            if (mpz_cmp_ui(key.p->x, 0) != 0 && mpz_cmp_ui(key.d, 0) != 0) break;
        }
    }
    return key;
}

ecc::key key::generate_key(const ecc::curve &curve) {
    ecc::key key;
    key.curve->copy(curve);
    key.isPrivate = 1;
    {
        unsigned long seed = time(nullptr);
        gmp_randstate_t state;
        gmp_randinit_mt(state);
        gmp_randseed_ui(state, seed);
        while (true) {
            mpz_urandomm(key.d, state, key.curve->n);
            mpz_mod(key.d, key.d, key.curve->n);
            key.curve->scalar_multiplication(key.p, key.d);
            if (mpz_cmp_ui(key.p->x, 0) != 0 && mpz_cmp_ui(key.d, 0) != 0) break;
        }
    }
    return key;
}

ecc::key key::generate_key(const ecc::curve *curve) {
    ecc::key key;
    key.curve->copy(curve);
    key.isPrivate = 1;
    {
        unsigned long seed = time(nullptr);
        gmp_randstate_t state;
        gmp_randinit_mt(state);
        gmp_randseed_ui(state, seed);
        while (true) {
            mpz_urandomm(key.d, state, key.curve->n);
            mpz_mod(key.d, key.d, key.curve->n);
            key.curve->scalar_multiplication(key.p, key.d);
            if (mpz_cmp_ui(key.p->x, 0) != 0 && mpz_cmp_ui(key.d, 0) != 0) break;
        }
    }
    return key;
}

signature key::signature_create(const mpz_t &hash) const {
    if (!this->isPrivate) throw NotPrivateKey();
    mpz_t k, s, r, temp;
    mpz_inits(k, s, r, temp, NULL);

    unsigned long seed = time(nullptr);

    gmp_randstate_t state;

    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);

    ecc::point R;
    do {
        do {
            mpz_urandomm(k, state, this->curve->n);
        } while (mpz_cmp_ui(k, 0) == 0 || mpz_cmp(this->d, k) == 0);
        this->curve->scalar_multiplication(&R, k);

        mpz_invert(s, k, this->curve->n);
        mpz_mul(temp, R.x, this->d);
        mpz_add(temp, temp, hash);
        mpz_mul(s, s, temp);
        mpz_mod(s, s, this->curve->n);

    } while (mpz_cmp_ui(s, 0) == 0);

    mpz_set(r, R.x);

    signature res = signature(r, s);
    mpz_inits(k, s, r, temp, NULL);
    return res;
}

signature key::signature_create(const std::string &hash) const {
    mpz_t temp;
    mpz_init_set_str(temp, hash.c_str(), 16);
    signature result = this->signature_create(temp);
    mpz_clear(temp);
    return result;
}

bool key::signature_check(const signature &sign, const mpz_t &hash) const {
    mpz_t s1, temp1, temp2;
    mpz_inits(s1, temp1, temp2, NULL);
    mpz_invert(s1, sign.s, this->curve->n);
    mpz_mul(temp1, hash, s1);
    mpz_mul(temp2, sign.r, s1);

    ecc::point _point1;
    ecc::point _point2;

    this->curve->scalar_multiplication(&_point1, temp1);
    this->curve->scalar_multiplication_point(&_point2, this->p, temp2);
    this->curve->point_addition(&_point1, &_point1, &_point2);

    bool result = (mpz_cmp(_point1.x, sign.r) == 0);

    mpz_clears(s1, temp1, temp2, NULL);
    return result;
}

bool key::signature_check(const signature *sign, const mpz_t &hash) const {
    mpz_t s1, temp1, temp2;
    mpz_inits(s1, temp1, temp2, NULL);
    mpz_invert(s1, sign->s, this->curve->n);
    mpz_mul(temp1, hash, s1);
    mpz_mul(temp2, sign->r, s1);

    ecc::point _point1;
    ecc::point _point2;

    this->curve->scalar_multiplication(&_point1, temp1);
    this->curve->scalar_multiplication_point(&_point2, this->p, temp2);
    this->curve->point_addition(&_point1, &_point1, &_point2);

    bool result = (mpz_cmp(_point1.x, sign->r) == 0);

    mpz_clears(s1, temp1, temp2, NULL);
    return result;
}

bool key::signature_check(const signature &sign, const std::string &hash) const {
    mpz_t temp;
    mpz_init_set_str(temp, hash.c_str(), 16);
    bool result = this->signature_check(sign, temp);
    mpz_clear(temp);
    return result;
}

bool key::signature_check(const signature *sign, const std::string &hash) const {
    mpz_t temp;
    mpz_init_set_str(temp, hash.c_str(), 16);
    bool result = this->signature_check(sign, temp);
    mpz_clear(temp);
    return result;
}