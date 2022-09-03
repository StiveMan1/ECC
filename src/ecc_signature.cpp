#include "ecc.h"
#include <nlohmann/json.hpp>

using namespace ecc;
#ifndef gmp

signature::signature() {
    this->r = (bi_pt) malloc(sizeof(bi_t));
    this->s = (bi_pt) malloc(sizeof(bi_t));
    bi_inits(this->r, this->s, NULL);
}

signature::signature(const bi_pt r, const bi_pt s) {
    this->r = (bi_pt) malloc(sizeof(bi_t));
    this->s = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->r, r);
    bi_init_set(this->s, s);
}

signature::signature(const signature &a) {
    this->r = (bi_pt) malloc(sizeof(bi_t));
    this->s = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->r, a.r);
    bi_init_set(this->s, a.s);
}

signature::signature(const signature *a) {
    this->r = (bi_pt) malloc(sizeof(bi_t));
    this->s = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->r, a->r);
    bi_init_set(this->s, a->s);
}

signature::~signature() {
    bi_clears(this->r, this->s, NULL);
}

void signature::copy(const signature &a) {
    bi_set(this->r, a.r);
    bi_set(this->s, a.s);
}

void signature::copy(const signature *a) {
    bi_set(this->r, a->r);
    bi_set(this->s, a->s);
}

void signature::set(const bi_pt _r, const bi_pt _s) {
    bi_set(this->r, _r);
    bi_set(this->s, _s);
}

std::string signature::to_json() const {
    nlohmann::json data;
    data["s"] = bi_str_get_16(this->s);
    data["r"] = bi_str_get_16(this->r);
    return data.dump();
}

signature signature::from_json(const std::string &str) {
    signature _signature = signature();
    nlohmann::json data = nlohmann::json::parse(str);
    if (!data.contains("r")) throw IncorrectJson();
    if (!data.contains("s")) throw IncorrectJson();
    std::string tmp_str = data["s"].get<std::string>();
    bi_set_str_16(_signature.s, tmp_str.c_str(), tmp_str.size());
    tmp_str = data["r"].get<std::string>();
    bi_set_str_16(_signature.r, tmp_str.c_str(), tmp_str.size());
    return _signature;
}

void signature::set_from_json(const std::string &str) {
    nlohmann::json data = nlohmann::json::parse(str);
    if (!data.contains("r")) throw IncorrectJson();
    if (!data.contains("s")) throw IncorrectJson();
    std::string tmp_str = data["s"].get<std::string>();
    bi_set_str_16(this->s, tmp_str.c_str(), tmp_str.size());
    tmp_str = data["r"].get<std::string>();
    bi_set_str_16(this->r, tmp_str.c_str(), tmp_str.size());
}

#else

signature::signature() {
    mpz_inits(this->r, this->s, NULL);
}

signature::signature(const mpz_t &r, const mpz_t &s) {
    mpz_init_set(this->r, r);
    mpz_init_set(this->s, s);
}

signature::signature(const signature &a) {
    mpz_init_set(this->r, a.r);
    mpz_init_set(this->s, a.s);
}

signature::signature(const signature *a) {
    mpz_init_set(this->r, a->r);
    mpz_init_set(this->s, a->s);
}

signature::~signature() {
    mpz_clears(this->r, this->s, NULL);
}

void signature::copy(const signature &a) {
    mpz_set(this->r, a.r);
    mpz_set(this->s, a.s);
}

void signature::copy(const signature *a) {
    mpz_set(this->r, a->r);
    mpz_set(this->s, a->s);
}

void signature::set(const mpz_t &_r, const mpz_t &_s) {
    mpz_set(this->r, _r);
    mpz_set(this->s, _s);
}

std::string signature::to_json() const {
    nlohmann::json data;
    data["s"] = mpz_str_get(this->s);
    data["r"] = mpz_str_get(this->r);
    return data.dump();
}

signature signature::from_json(const std::string &str) {
    signature _signature = signature();
    nlohmann::json data = nlohmann::json::parse(str);
    if (!data.contains("r")) throw IncorrectJson();
    if (!data.contains("s")) throw IncorrectJson();
    mpz_set_str(_signature.s, data["s"].get<std::string>().c_str(), 16);
    mpz_set_str(_signature.r, data["r"].get<std::string>().c_str(), 16);
    return _signature;
}

void signature::set_from_json(const std::string &str) {
    nlohmann::json data = nlohmann::json::parse(str);
    if (!data.contains("r")) throw IncorrectJson();
    if (!data.contains("s")) throw IncorrectJson();
    mpz_set_str(this->s, data["s"].get<std::string>().c_str(), 16);
    mpz_set_str(this->r, data["r"].get<std::string>().c_str(), 16);
}

#endif
