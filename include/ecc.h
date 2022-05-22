#ifndef ECC_H
#define ECC_H

#include "gmp.h"
#include <hash_code.h>
#include <crypto_code.h>
#include <string>
#include <cstring>
#include <malloc.h>

namespace ecc {

    class EccException : public std::exception {
    };

    class NotPrivateKey : public EccException {
    };

    class IncorrectPoint : public EccException {
    };

    class IncorrectKey : public EccException {
    };

    class IncorrectJson : public EccException {
    };


    std::string base64_decode(std::string const &s);

    std::string base64_encode(std::string const &s);

    class point;

    class curve;

    class random_by_password;

    class signature;

    class key;

#define mpz_str_get _mpz_str_get

    std::string _mpz_str_get(const mpz_t &rop, int base = 16);

    class point {
    private:
        mpz_t x{};
        mpz_t y{};

        friend class ecc::curve;

        friend class ecc::key;

    public:
        point();

        ~point();

        point(const ecc::point &a);

        point(const mpz_t &x, const mpz_t &y);

        explicit point(const ecc::point *a);

        void copy(const ecc::point *a);

        void copy(const ecc::point &a);

        void set(const mpz_t &x, const mpz_t &y);

        static ecc::point from_string(const std::string &str, const ecc::curve &_curve);

        static ecc::point from_string(const std::string &str, const ecc::curve *_curve);

        void set_from_string(const std::string &str, const ecc::curve &_curve);

        void set_from_string(const std::string &str, const ecc::curve *_curve);

        std::string to_string() const;

    };

    class curve {
    private:
        ecc::point *g;
        unsigned h;
        mpz_t a{};
        mpz_t b{};
        mpz_t n{};
        mpz_t p{};

        friend class ecc::point;

        friend class ecc::key;

    public:
        ecc::point get_g() const { return point(this->g); }

        unsigned get_h() const { return this->h; }

        curve();

        ~curve();

        curve(const mpz_t &a, const mpz_t &b, const mpz_t &p, const mpz_t &n, int h, const ecc::point &G);

        curve(const ecc::curve &a);

        explicit curve(const ecc::curve *a);

        void copy(const ecc::curve &a);

        void copy(const ecc::curve *a);

        void set(const mpz_t &a, const mpz_t &b, const mpz_t &p, const mpz_t &n, int h, const ecc::point &G);

        void set_secp256k1();

        static ecc::curve secp256k1();

        std::string to_json() const;

        static ecc::curve from_json(const std::string &str);

        void set_from_json(const std::string &str);


        void point_doubling(ecc::point *res, const ecc::point &P);

        void point_doubling(ecc::point *res, const ecc::point *P);

        void point_addition(ecc::point *res, const ecc::point &P, const ecc::point &Q);

        void point_addition(ecc::point *res, const ecc::point *P, const ecc::point *Q);

        void scalar_multiplication_point(ecc::point *res, const ecc::point &P, const mpz_t &m);

        void scalar_multiplication_point(ecc::point *res, const ecc::point *P, const mpz_t &m);

        void scalar_multiplication(ecc::point *res, const mpz_t &m);

    };

    class random_by_password {
    private:
        int counter;
        std::string master_key;
    public:
        explicit random_by_password(const std::string &password);

        ~random_by_password();

        void random_rand(mpz_t result, unsigned n);
    };

    class signature {
    private:
        mpz_t s{};
        mpz_t r{};

        friend class ecc::key;

    public:

        signature();

        signature(const mpz_t &r, const mpz_t &s);

        signature(const ecc::signature &a);

        explicit signature(const ecc::signature *a);

        ~signature();

        void copy(const ecc::signature &a);

        void copy(const ecc::signature *a);

        void set(const mpz_t &r, const mpz_t &s);

        std::string to_json() const;

        static ecc::signature from_json(const std::string &str);

        void set_from_json(const std::string &str);
    };

    class key {
    private:
        ecc::point *p;
        ecc::curve *curve;
        char isPrivate;
        mpz_t d{};
    public:
        ecc::point get_p() const { return ecc::point(p); }

        ecc::curve get_curve() const { return ecc::curve(curve); }

        key();

        ~key();

        key(const key &a);

        explicit key(const key *a);


        void copy(const key &a);

        void copy(const key *a);

        static key from_address(const std::string &str, const ecc::curve &curve);

        static key from_address(const std::string &str, const ecc::curve *curve);

        static key from_private_key(const std::string &str, const ecc::curve &curve);

        static key from_private_key(const std::string &str, const ecc::curve *curve);

        void set_public(const ecc::point &p, const ecc::curve &curve);

        void set_public(const ecc::point *p, const ecc::curve *curve);

        void set_private(const std::string &d, const ecc::curve &curve);

        void set_private(const std::string &d, const ecc::curve *curve);

        std::string get_address() const;

        std::string get_private_key() const;

        std::string
        encode_self(const std::string &str, int hash_type, int crypto_type) const;

        std::string
        decode_self(const std::string &str, int hash_type, int crypto_type) const;

        std::string encode(const std::string &str, int hash_type, int crypto_type, const ecc::key *key) const;

        std::string encode(const std::string &str, int hash_type, int crypto_type, const ecc::key &key) const;

        std::string decode(const std::string &str, int hash_type, int crypto_type, const ecc::key *key) const;

        std::string decode(const std::string &str, int hash_type, int crypto_type, const ecc::key &key) const;

        static key generate_key(const std::string &password, const ecc::curve &_curve);

        static key generate_key(const std::string &password, const ecc::curve *_curve);

        static key generate_key(const ecc::curve &_curve);

        static key generate_key(const ecc::curve *_curve);

        ecc::signature signature_create(const mpz_t &hash) const;

        ecc::signature signature_create(const std::string &hash) const;

        bool signature_check(const ecc::signature &sign, const mpz_t &hash) const;

        bool signature_check(const ecc::signature *sign, const mpz_t &hash) const;

        bool signature_check(const ecc::signature &sign, const std::string &hash) const;

        bool signature_check(const ecc::signature *sign, const std::string &hash) const;
    };
}

#endif //ECC_H
