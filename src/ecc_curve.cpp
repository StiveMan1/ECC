#include <nlohmann/json.hpp>
#include "ecc.h"

using namespace ecc;
#ifndef gmp

curve::curve() {
    this->g = nullptr;
    this->h = 0;
    this->a = (bi_pt) malloc(sizeof(bi_t));
    this->b = (bi_pt) malloc(sizeof(bi_t));
    this->p = (bi_pt) malloc(sizeof(bi_t));
    this->n = (bi_pt) malloc(sizeof(bi_t));
    bi_inits(this->a, this->b, this->p, this->n, NULL);
}

curve::~curve() {
    delete this->g;
    bi_clears(this->a, this->b, this->p, this->n, NULL);
}

curve::curve(const bi_pt a, const bi_pt b, const bi_pt p, const bi_pt n, int h,
             const point &G) {
    this->g = new point(G);
    this->a = (bi_pt) malloc(sizeof(bi_t));
    this->b = (bi_pt) malloc(sizeof(bi_t));
    this->p = (bi_pt) malloc(sizeof(bi_t));
    this->n = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->a, a);
    bi_init_set(this->b, b);
    bi_init_set(this->p, p);
    bi_init_set(this->n, n);
    this->h = h;
}

curve::curve(const curve &a) {
    this->g = new point(a.g);
    this->a = (bi_pt) malloc(sizeof(bi_t));
    this->b = (bi_pt) malloc(sizeof(bi_t));
    this->p = (bi_pt) malloc(sizeof(bi_t));
    this->n = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->a, a.a);
    bi_init_set(this->b, a.b);
    bi_init_set(this->p, a.p);
    bi_init_set(this->n, a.n);
    this->h = a.h;
}

curve::curve(const curve *a) {
    this->g = new ecc::point(a->g);
    this->a = (bi_pt) malloc(sizeof(bi_t));
    this->b = (bi_pt) malloc(sizeof(bi_t));
    this->p = (bi_pt) malloc(sizeof(bi_t));
    this->n = (bi_pt) malloc(sizeof(bi_t));
    bi_init_set(this->a, a->a);
    bi_init_set(this->b, a->b);
    bi_init_set(this->p, a->p);
    bi_init_set(this->n, a->n);
    this->h = a->h;
}


void curve::copy(const curve &_a) {
    bi_set(this->a, _a.a);
    bi_set(this->b, _a.b);
    bi_set(this->p, _a.p);
    bi_set(this->n, _a.n);
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->copy(_a.g);
    this->h = _a.h;
}

void curve::copy(const curve *_a) {
    bi_set(this->a, _a->a);
    bi_set(this->b, _a->b);
    bi_set(this->p, _a->p);
    bi_set(this->n, _a->n);
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->copy(_a->g);
    this->h = _a->h;
}

void curve::set(const bi_pt _a, const bi_pt _b, const bi_pt _p, const bi_pt _n, int _h, const ecc::point &G) {
    bi_set(this->a, a);
    bi_set(this->b, b);
    bi_set(this->p, p);
    bi_set(this->n, n);
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->copy(G);
    this->h = _h;
}

void curve::set_secp256k1() {
    this->h = 1;
    bi_set_ui(this->a, 0);
    bi_set_ui(this->b, 7);
    std::string _p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
    bi_set_str_16(this->p, _p.c_str(), _p.size());
    std::string _n = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
    bi_set_str_16(this->n, _n.c_str(), _n.size());
    std::string G = "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->set_from_string(G, this);
}

ecc::curve curve::secp256k1() {
    curve _curve;
    _curve.h = 1;
    bi_set_ui(_curve.a, 0);
    bi_set_ui(_curve.b, 7);
    std::string _p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
    bi_set_str_16(_curve.p, _p.c_str(), _p.size());
    std::string _n = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
    bi_set_str_16(_curve.n, _n.c_str(), _n.size());
    std::string G = "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
    _curve.g = new ecc::point();
    _curve.g->set_from_string(G, _curve);
    return _curve;
}

std::string curve::to_json() const {
    nlohmann::json data;
    data["a"] = bi_str_get_16(this->a);
    data["b"] = bi_str_get_16(this->b);
    data["n"] = bi_str_get_16(this->n);
    data["p"] = bi_str_get_16(this->p);
    if (this->g != nullptr)
        data["G"] = this->g->to_string();
    data["h"] = this->h;
    return data.dump();
}


curve curve::from_json(const std::string &str) {
    curve _curve;
    nlohmann::json data = nlohmann::json::parse(str);
    if (!data.contains("a")) throw IncorrectJson();
    if (!data.contains("b")) throw IncorrectJson();
    if (!data.contains("n")) throw IncorrectJson();
    if (!data.contains("p")) throw IncorrectJson();
    if (!data.contains("h")) throw IncorrectJson();
    if (!data.contains("G")) throw IncorrectJson();
    std::string tmp_str = data["a"].get<std::string>();
    bi_set_str_16(_curve.a, tmp_str.c_str(), tmp_str.size());
    tmp_str = data["b"].get<std::string>();
    bi_set_str_16(_curve.b, tmp_str.c_str(), tmp_str.size());
    tmp_str = data["n"].get<std::string>();
    bi_set_str_16(_curve.n, tmp_str.c_str(), tmp_str.size());
    tmp_str = data["p"].get<std::string>();
    bi_set_str_16(_curve.p, tmp_str.c_str(), tmp_str.size());
    _curve.h = data["h"].get<int>();
    _curve.g = new ecc::point();
    _curve.g->set_from_string(data["G"].get<std::string>(), _curve);
    return _curve;
}

void curve::set_from_json(const std::string &str) {
    nlohmann::json data = nlohmann::json::parse(str);
    if (!data.contains("a")) throw IncorrectJson();
    if (!data.contains("b")) throw IncorrectJson();
    if (!data.contains("n")) throw IncorrectJson();
    if (!data.contains("p")) throw IncorrectJson();
    if (!data.contains("h")) throw IncorrectJson();
    if (!data.contains("G")) throw IncorrectJson();
    std::string tmp_str = data["a"].get<std::string>();
    bi_set_str_16(this->a, tmp_str.c_str(), tmp_str.size());
    tmp_str = data["b"].get<std::string>();
    bi_set_str_16(this->b, tmp_str.c_str(), tmp_str.size());
    tmp_str = data["n"].get<std::string>();
    bi_set_str_16(this->n, tmp_str.c_str(), tmp_str.size());
    tmp_str = data["p"].get<std::string>();
    bi_set_str_16(this->p, tmp_str.c_str(), tmp_str.size());
    this->h = data["h"].get<int>();
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->set_from_string(data["G"].get<std::string>(), this);
}


void curve::point_doubling(ecc::point *res, const ecc::point &P) {
    if (bi_cmp_ui(P.y, 0) == 0) {
        bi_set_ui(res->x, 0);
        bi_set_ui(res->y, 0);
        return;
    }
    bi_pt slope = (bi_pt) malloc(sizeof(bi_t));
    bi_pt temp = (bi_pt) malloc(sizeof(bi_t));
    bi_inits(slope, temp, NULL);

    bi_mul_ui(temp, P.y, 2);
    bi_inverse(temp, temp, this->p);
    bi_mul(slope, P.x, P.x);
    bi_mul_ui(slope, slope, 3);
    bi_add(slope, slope, this->a);
    bi_mul(slope, slope, temp);
    bi_mod(slope, slope, this->p);
    bi_set(temp, P.x);

    bi_mul(res->x, slope, slope);
    bi_sub(res->x, res->x, temp);
    bi_sub(res->x, res->x, temp);

    bi_sub(temp, temp, res->x);
    bi_mul(temp, temp, slope);
    bi_sub(res->y, temp, P.y);

    bi_mod(res->x, res->x, this->p);
    bi_mod(res->y, res->y, this->p);
    bi_clears(slope, temp, NULL);
}

void curve::point_doubling(ecc::point *res, const ecc::point *P) {
    if (bi_cmp_ui(P->y, 0) == 0) {
        bi_set_ui(res->x, 0);
        bi_set_ui(res->y, 0);
        return;
    }
    bi_pt slope = (bi_pt) malloc(sizeof(bi_t));
    bi_pt temp = (bi_pt) malloc(sizeof(bi_t));
    bi_inits(slope, temp, NULL);

    bi_mul_ui(temp, P->y, 2);
    bi_inverse(temp, temp, this->p);
    bi_mul(slope, P->x, P->x);
    bi_mul_ui(slope, slope, 3);
    bi_add(slope, slope, this->a);
    bi_mul(slope, slope, temp);
    bi_mod(slope, slope, this->p);
    bi_set(temp, P->x);

    bi_mul(res->x, slope, slope);
    bi_sub(res->x, res->x, temp);
    bi_sub(res->x, res->x, temp);

    bi_sub(temp, temp, res->x);
    bi_mul(temp, temp, slope);
    bi_sub(res->y, temp, P->y);

    bi_mod(res->x, res->x, this->p);
    bi_mod(res->y, res->y, this->p);
    bi_clears(slope, temp, NULL);
}

void curve::point_addition(ecc::point *res, const ecc::point &P, const ecc::point &Q) {

    if (bi_cmp_ui(P.x, 0) == 0 && bi_cmp_ui(P.y, 0) == 0) {
        return res->copy(Q);
    }
    if (bi_cmp_ui(Q.x, 0) == 0 && bi_cmp_ui(Q.y, 0) == 0) {
        return res->copy(P);
    }
    if (bi_cmp(P.x, Q.x) == 0) {
        if (bi_cmp(P.y, Q.y) == 0) {
            return point_doubling(res, P);
        }
        bi_set_ui(res->x, 0);
        bi_set_ui(res->y, 0);
        return;
    }


    bi_pt slope = (bi_pt) malloc(sizeof(bi_t));
    bi_pt temp = (bi_pt) malloc(sizeof(bi_t));
    bi_pt temp1 = (bi_pt) malloc(sizeof(bi_t));
    bi_inits(slope, temp, temp1, NULL);
    bi_sub(temp, P.x, Q.x);
    bi_mod(temp, temp, this->p);
    bi_inverse(temp, temp, this->p);
    bi_sub(slope, P.y, Q.y);
    bi_mul(slope, slope, temp);
    bi_mod(slope, slope, this->p);

    bi_mul(temp, slope, slope);
    bi_sub(temp, temp, P.x);
    bi_sub(temp, temp, Q.x);

    bi_sub(temp1, P.x, temp);
    bi_mul(temp1, temp1, slope);
    bi_sub(temp1, temp1, P.y);

    bi_mod(res->x, temp, this->p);
    bi_mod(res->y, temp1, this->p);
    bi_clears(slope, temp, temp1, NULL);
}

void curve::point_addition(ecc::point *res, const ecc::point *P, const ecc::point *Q) {

    if (bi_cmp_ui(P->x, 0) == 0 && bi_cmp_ui(P->y, 0) == 0) {
        return res->copy(Q);
    }
    if (bi_cmp_ui(Q->x, 0) == 0 && bi_cmp_ui(Q->y, 0) == 0) {
        return res->copy(P);
    }
    if (bi_cmp(P->x, Q->x) == 0) {
        if (bi_cmp(P->y, Q->y) == 0) {
            return point_doubling(res, P);
        }
        bi_set_ui(res->x, 0);
        bi_set_ui(res->y, 0);
        return;
    }


    bi_pt slope = (bi_pt) malloc(sizeof(bi_t));
    bi_pt temp = (bi_pt) malloc(sizeof(bi_t));
    bi_pt temp1 = (bi_pt) malloc(sizeof(bi_t));
    bi_inits(slope, temp, temp1, NULL);
    bi_sub(temp, P->x, Q->x);
    bi_mod(temp, temp, this->p);
    bi_inverse(temp, temp, this->p);
    bi_sub(slope, P->y, Q->y);
    bi_mul(slope, slope, temp);
    bi_mod(slope, slope, this->p);

    bi_mul(temp, slope, slope);
    bi_sub(temp, temp, P->x);
    bi_sub(temp, temp, Q->x);

    bi_sub(temp1, P->x, temp);
    bi_mul(temp1, temp1, slope);
    bi_sub(temp1, temp1, P->y);

    bi_mod(res->x, temp, this->p);
    bi_mod(res->y, temp1, this->p);
    bi_clears(slope, temp, temp1, NULL);
}
int set_char_16(char x) {
    if (x >= '0' && x <= '9') {
        return (x - '0');
    }
    if (x >= 'a' && x <= 'f') {
        return (x - 'a' + 10);
    }
    if (x >= 'A' && x <= 'F') {
        return (x - 'A' + 10);
    }
    return 0;
}
void curve::scalar_multiplication_point(ecc::point *res, const ecc::point &P, const bi_pt m) {
    if (bi_cmp_ui(m, 0) == 0) {
        bi_set_ui(res->x, 0);
        bi_set_ui(res->y, 0);
        return;
    }
    ecc::point Q = ecc::point(P);
    std::string bin_str = bi_str_get_16(m);
    unsigned _size = 0;
    for (unsigned i = bin_str.size(), x; i > 0; i--){
        x = set_char_16(bin_str[i - 1]);
        for(unsigned j=1;j<16;j<<=1)
            if ((x&j)==j) _size = i;
    }
    for (unsigned i = bin_str.size(), x; i >=_size; i--){
        x = set_char_16(bin_str[i - 1]);
        for(unsigned j=1;j<16;j<<=1){
            if ((x&j)==j)  point_addition(res, res, &Q);
            point_doubling(&Q, Q);
        }
    }
}

void curve::scalar_multiplication_point(ecc::point *res, const ecc::point *P, const bi_pt m) {
    if (bi_cmp_ui(m, 0) == 0) {
        bi_set_ui(res->x, 0);
        bi_set_ui(res->y, 0);
        return;
    }
    ecc::point Q = ecc::point(P);
    std::string bin_str = bi_str_get_16(m);
    unsigned _size = 0;
    for (unsigned i = bin_str.size(), x; i > 0; i--){
        x = set_char_16(bin_str[i - 1]);
        for(unsigned j=1;j<16;j<<=1) {
            if ((x & j) == j) _size = i;
        }
    }
    for (unsigned i = bin_str.size(), x; i >=_size; i--){
        x = set_char_16(bin_str[i - 1]);
        for(unsigned j=1;j<16;j<<=1){
            if ((x&j)==j)  point_addition(res, res, &Q);
            point_doubling(&Q, Q);
        }
    }
}

void curve::scalar_multiplication(ecc::point *res, const bi_pt m) {
    return scalar_multiplication_point(res, g, m);
}

std::string ecc::bi_str_get_16(const bi_pt rop) {
    char *res = bi_get_str_16(rop);
    std::string result = res;
    delete res;
    return result;
}

#else

curve::curve() {
    this->g = nullptr;
    this->h = 0;
    mpz_inits(this->a, this->b, this->p, this->n, NULL);
}

curve::~curve() {
    delete this->g;
    mpz_clears(this->a, this->b, this->p, this->n, NULL);
}

curve::curve(const mpz_t &a, const mpz_t &b, const mpz_t &p, const mpz_t &n, int h,
             const point &G) {
    this->g = new point(G);
    mpz_init_set(this->a, a);
    mpz_init_set(this->b, b);
    mpz_init_set(this->p, p);
    mpz_init_set(this->n, n);
    this->h = h;
}

curve::curve(const curve &a) {
    this->g = new point(a.g);
    mpz_init_set(this->a, a.a);
    mpz_init_set(this->b, a.b);
    mpz_init_set(this->p, a.p);
    mpz_init_set(this->n, a.n);
    this->h = a.h;
}

curve::curve(const curve *a) {
    this->g = new ecc::point(a->g);
    mpz_init_set(this->a, a->a);
    mpz_init_set(this->b, a->b);
    mpz_init_set(this->p, a->p);
    mpz_init_set(this->n, a->n);
    this->h = a->h;
}


void curve::copy(const curve &_a) {
    mpz_set(this->a, _a.a);
    mpz_set(this->b, _a.b);
    mpz_set(this->p, _a.p);
    mpz_set(this->n, _a.n);
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->copy(_a.g);
    this->h = _a.h;
}

void curve::copy(const curve *_a) {
    mpz_set(this->a, _a->a);
    mpz_set(this->b, _a->b);
    mpz_set(this->p, _a->p);
    mpz_set(this->n, _a->n);
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->copy(_a->g);
    this->h = _a->h;
}

void curve::set(const mpz_t &_a, const mpz_t &_b, const mpz_t &_p, const mpz_t &_n, int _h, const ecc::point &G) {
    mpz_set(this->a, a);
    mpz_set(this->b, b);
    mpz_set(this->p, p);
    mpz_set(this->n, n);
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->copy(G);
    this->h = _h;
}

void curve::set_secp256k1() {
    this->h = 1;
    mpz_set_ui(this->a, 0);
    mpz_set_ui(this->b, 7);
    std::string _p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
    mpz_set_str(this->p, _p.c_str(), 16);
    std::string _n = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
    mpz_set_str(this->n, _n.c_str(), 16);
    std::string G = "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->set_from_string(G, this);
}

ecc::curve curve::secp256k1() {
    curve _curve;
    _curve.h = 1;
    mpz_set_ui(_curve.a, 0);
    mpz_set_ui(_curve.b, 7);
    std::string _p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
    mpz_set_str(_curve.p, _p.c_str(), 16);
    std::string _n = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";
    mpz_set_str(_curve.n, _n.c_str(), 16);
    std::string G = "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
    _curve.g = new ecc::point();
    _curve.g->set_from_string(G, _curve);
    return _curve;
}

std::string curve::to_json() const {
    nlohmann::json data;
    data["a"] = mpz_str_get(this->a);
    data["b"] = mpz_str_get(this->b);
    data["n"] = mpz_str_get(this->n);
    data["p"] = mpz_str_get(this->p);
    if (this->g != nullptr)
        data["G"] = this->g->to_string();
    data["h"] = this->h;
    return data.dump();
}


curve curve::from_json(const std::string &str) {
    curve _curve;
    nlohmann::json data = nlohmann::json::parse(str);
    if (!data.contains("a")) throw IncorrectJson();
    if (!data.contains("b")) throw IncorrectJson();
    if (!data.contains("n")) throw IncorrectJson();
    if (!data.contains("p")) throw IncorrectJson();
    if (!data.contains("h")) throw IncorrectJson();
    if (!data.contains("G")) throw IncorrectJson();
    mpz_set_str(_curve.a, data["a"].get<std::string>().c_str(), 16);
    mpz_set_str(_curve.b, data["b"].get<std::string>().c_str(), 16);
    mpz_set_str(_curve.n, data["n"].get<std::string>().c_str(), 16);
    mpz_set_str(_curve.p, data["p"].get<std::string>().c_str(), 16);
    _curve.h = data["h"].get<int>();
    _curve.g = new ecc::point();
    _curve.g->set_from_string(data["G"].get<std::string>(), _curve);
    return _curve;
}

void curve::set_from_json(const std::string &str) {
    nlohmann::json data = nlohmann::json::parse(str);
    if (!data.contains("a")) throw IncorrectJson();
    if (!data.contains("b")) throw IncorrectJson();
    if (!data.contains("n")) throw IncorrectJson();
    if (!data.contains("p")) throw IncorrectJson();
    if (!data.contains("h")) throw IncorrectJson();
    if (!data.contains("G")) throw IncorrectJson();
    mpz_set_str(this->a, data["a"].get<std::string>().c_str(), 16);
    mpz_set_str(this->b, data["b"].get<std::string>().c_str(), 16);
    mpz_set_str(this->n, data["n"].get<std::string>().c_str(), 16);
    mpz_set_str(this->p, data["p"].get<std::string>().c_str(), 16);
    this->h = data["h"].get<int>();
    if (this->g == nullptr) this->g = new ecc::point();
    this->g->set_from_string(data["G"].get<std::string>(), this);
}


void curve::point_doubling(ecc::point *res, const ecc::point &P) {
    if (mpz_cmp_ui(P.y, 0) == 0) {
        mpz_set_ui(res->x, 0);
        mpz_set_ui(res->y, 0);
        return;
    }
    mpz_t slope;
    mpz_t temp;
    mpz_inits(slope, temp, NULL);

    mpz_mul_ui(temp, P.y, 2);
    mpz_invert(temp, temp, this->p);
    mpz_mul(slope, P.x, P.x);
    mpz_mul_ui(slope, slope, 3);
    mpz_add(slope, slope, this->a);
    mpz_mul(slope, slope, temp);
    mpz_mod(slope, slope, this->p);
    mpz_set(temp, P.x);

    mpz_mul(res->x, slope, slope);
    mpz_sub(res->x, res->x, temp);
    mpz_sub(res->x, res->x, temp);

    mpz_sub(temp, temp, res->x);
    mpz_mul(temp, temp, slope);
    mpz_sub(res->y, temp, P.y);

    mpz_mod(res->x, res->x, this->p);
    mpz_mod(res->y, res->y, this->p);
    mpz_clears(slope, temp, NULL);
}

void curve::point_doubling(ecc::point *res, const ecc::point *P) {
    if (mpz_cmp_ui(P->y, 0) == 0) {
        mpz_set_ui(res->x, 0);
        mpz_set_ui(res->y, 0);
        return;
    }
    mpz_t slope;
    mpz_t temp;
    mpz_inits(slope, temp, NULL);

    mpz_mul_ui(temp, P->y, 2);
    mpz_invert(temp, temp, this->p);
    mpz_mul(slope, P->x, P->x);
    mpz_mul_ui(slope, slope, 3);
    mpz_add(slope, slope, this->a);
    mpz_mul(slope, slope, temp);
    mpz_mod(slope, slope, this->p);
    mpz_set(temp, P->x);

    mpz_mul(res->x, slope, slope);
    mpz_sub(res->x, res->x, temp);
    mpz_sub(res->x, res->x, temp);

    mpz_sub(temp, temp, res->x);
    mpz_mul(temp, temp, slope);
    mpz_sub(res->y, temp, P->y);

    mpz_mod(res->x, res->x, this->p);
    mpz_mod(res->y, res->y, this->p);
    mpz_clears(slope, temp, NULL);
}

void curve::point_addition(ecc::point *res, const ecc::point &P, const ecc::point &Q) {

    if (mpz_cmp_ui(P.x, 0) == 0 && mpz_cmp_ui(P.y, 0) == 0) {
        return res->copy(Q);
    }
    if (mpz_cmp_ui(Q.x, 0) == 0 && mpz_cmp_ui(Q.y, 0) == 0) {
        return res->copy(P);
    }
    if (mpz_cmp(P.x, Q.x) == 0) {
        if (mpz_cmp(P.y, Q.y) == 0) {
            return point_doubling(res, P);
        }
        mpz_set_ui(res->x, 0);
        mpz_set_ui(res->y, 0);
        return;
    }


    mpz_t slope;
    mpz_t temp;
    mpz_t temp1;
    mpz_inits(slope, temp, temp1, NULL);
    mpz_sub(temp, P.x, Q.x);
    mpz_mod(temp, temp, this->p);
    mpz_invert(temp, temp, this->p);
    mpz_sub(slope, P.y, Q.y);
    mpz_mul(slope, slope, temp);
    mpz_mod(slope, slope, this->p);

    mpz_mul(temp, slope, slope);
    mpz_sub(temp, temp, P.x);
    mpz_sub(temp, temp, Q.x);

    mpz_sub(temp1, P.x, temp);
    mpz_mul(temp1, temp1, slope);
    mpz_sub(temp1, temp1, P.y);

    mpz_mod(res->x, temp, this->p);
    mpz_mod(res->y, temp1, this->p);
    mpz_clears(slope, temp, temp1, NULL);
}

void curve::point_addition(ecc::point *res, const ecc::point *P, const ecc::point *Q) {

    if (mpz_cmp_ui(P->x, 0) == 0 && mpz_cmp_ui(P->y, 0) == 0) {
        return res->copy(Q);
    }
    if (mpz_cmp_ui(Q->x, 0) == 0 && mpz_cmp_ui(Q->y, 0) == 0) {
        return res->copy(P);
    }
    if (mpz_cmp(P->x, Q->x) == 0) {
        if (mpz_cmp(P->y, Q->y) == 0) {
            return point_doubling(res, P);
        }
        mpz_set_ui(res->x, 0);
        mpz_set_ui(res->y, 0);
        return;
    }


    mpz_t slope;
    mpz_t temp;
    mpz_t temp1;
    mpz_inits(slope, temp, temp1, NULL);
    mpz_sub(temp, P->x, Q->x);
    mpz_mod(temp, temp, this->p);
    mpz_invert(temp, temp, this->p);
    mpz_sub(slope, P->y, Q->y);
    mpz_mul(slope, slope, temp);
    mpz_mod(slope, slope, this->p);

    mpz_mul(temp, slope, slope);
    mpz_sub(temp, temp, P->x);
    mpz_sub(temp, temp, Q->x);

    mpz_sub(temp1, P->x, temp);
    mpz_mul(temp1, temp1, slope);
    mpz_sub(temp1, temp1, P->y);

    mpz_mod(res->x, temp, this->p);
    mpz_mod(res->y, temp1, this->p);
    mpz_clears(slope, temp, temp1, NULL);
}

void curve::scalar_multiplication_point(ecc::point *res, const ecc::point &P, const mpz_t &m) {
    if (mpz_cmp_ui(m, 0) == 0) {
        mpz_set_ui(res->x, 0);
        mpz_set_ui(res->y, 0);
        return;
    }
    ecc::point Q = ecc::point(P);
    std::string bin_str = mpz_str_get(m, 2);
    unsigned _size = 0;
    for (unsigned i = bin_str.size(); i > 0; i--)
        if (bin_str[i - 1] == '1') _size = i;
    for (unsigned i = bin_str.size(); i >= _size; i--) {
        if (bin_str[i - 1] == '1') {
            point_addition(res, res, &Q);
        }
        point_doubling(&Q, Q);
    }
}

void curve::scalar_multiplication_point(ecc::point *res, const ecc::point *P, const mpz_t &m) {
    if (mpz_cmp_ui(m, 0) == 0) {
        mpz_set_ui(res->x, 0);
        mpz_set_ui(res->y, 0);
        return;
    }
    ecc::point Q = ecc::point(P);
    std::string bin_str = mpz_str_get(m, 2);
    unsigned _size = 0;
    for (unsigned i = bin_str.size(); i > 0; i--)
        if (bin_str[i - 1] == '1') _size = i;
    for (unsigned i = bin_str.size(); i >= _size; i--) {
        if (bin_str[i - 1] == '1') {
            point_addition(res, res, &Q);
        }
        point_doubling(&Q, Q);
    }
}

void curve::scalar_multiplication(ecc::point *res, const mpz_t &m) {
    return scalar_multiplication_point(res, g, m);
}

std::string ecc::mpz_str_get(mpz_t const &rop, int base) {
    char *res = mpz_get_str(nullptr, base, rop);
    std::string result = res;
    delete res;
    return result;
}

#endif

