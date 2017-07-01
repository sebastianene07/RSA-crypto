#ifndef     __RSA_H
#define     __RSA_H

struct rsa_keys_s
{
    unsigned long public_exp;
    unsigned long private_exp;
    unsigned long modulo_n;
};

int rsa_generate_keys(struct rsa_keys_s *keys, unsigned long p, unsigned long v);

int rsa_decrypt(const struct rsa_keys_s *keys, char *msg, size_t msg_len, const unsigned long *encrypted_msg);

int rsa_encrypt(const struct rsa_keys_s *keys, const char *msg, size_t msg_len, unsigned long *encrypted_msg);

unsigned long rsa_modular_exponentiation(unsigned long a, unsigned long b, unsigned long n);

#endif /* __RSA_H */
