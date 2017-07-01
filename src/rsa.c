#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rsa.h"

/* Get the i-th bit from n */
#define BIT(n, i)   (((1 << (i)) & (n)) >> (i))


/* 
 * Name: rsa_generate_keys
 *
 * Description:
 *   Generate public/private key pairs.
 *
 * Arguments:
 *   keys   - place where we store keys
 *   p      - random prime number
 *   v      - random prime number
 */
int rsa_generate_keys(struct rsa_keys_s *keys, unsigned long p, unsigned long v)
{
   if (keys == NULL || p == v) {
       return -1;
   }

   unsigned long n, t;
   unsigned long e, d;
   char is_coprime_found = 0;

   n = p * v;
   t = (p - 1) * (v - 1);

   /* Find e so that t and e are coprime 1 < e < t */

   for (unsigned long i = 1; i < t; ++i) {
       e = t - i;

       /* Test if e is coprime with t */
       unsigned long de, imp, rest;
       de = t;
       imp = e;       

       while ((rest = de % imp) != 0) {
            de = imp;
            imp = rest;
       }

       /* If they are coprime cmmdc should be 1 */
       if (imp == 1) {
            is_coprime_found = 1;    
            break;
       }
   }

   assert(is_coprime_found);

   /* Find d so that (e * d) % t = 1 */ 
   d = 2;

   while ((((d % t) * (e % t)) % t) != 1) {
        ++d;
        if (d == e) ++d;
   }

   keys->public_exp  = e;
   keys->private_exp = d;
   keys->modulo_n    = n;

   return 0; 
}

/* Name: rsa_modular_exponentiation
 *
 * Descrition:
 *      Compute a ^ b mod n using Corman approach.
 *
 * Return:
 *      The modulus of a ^ b.
 */ 
unsigned long rsa_modular_exponentiation(unsigned long a, unsigned long b, unsigned long n)
{
    unsigned long d = 1;
    long i;

    for (i = (sizeof(b) << 3) - 1; i >= 0; i--) {
        d = (d * d) % n;

        if (BIT(b, i)) {
            d = (d * a) % n;
        }        
    }

    return d;
}

/* Name: rsa_encrypt
 *
 * Description:
 *      Encrypt the input message with the keys specified and
 *      store the output in encrypted_msg buffer.
 *
 * Arguments:
 *      keys    - structure used to hold keys information
 *      msg     - message to encrypt
 *      msg_len - size of the msg
 *      encrypted_msg - buffer where to store encrypted msg
 *
 * Return:
 *      On success returns 0 otherwise a negate number.
 */
int rsa_encrypt(const struct rsa_keys_s *keys, const char *msg, size_t msg_len, unsigned long *encrypted_msg)
{
    if (msg == NULL || keys == NULL || encrypted_msg == NULL) {
        return -1;
    }
 
    for (int i = 0; i < msg_len; ++i) {
        *(encrypted_msg + i) = rsa_modular_exponentiation(*(msg + i), keys->public_exp, keys->modulo_n);
    }

    return 0;   
}

/* Name: rsa_decrypt
 *
 * Description:
 *      Decrypt the encrypted message with the keys specified and
 *      store the output in the msg buffer.
 *
 * Arguments:
 *      keys    - structure used to hold keys information
 *      msg     - message where to store decrypted data
 *      msg_len - size of the msg
 *      encrypted_msg - buffer where to store encrypted msg
 *
 * Return:
 *      On success returns 0 otherwise a negate number.
 */
int rsa_decrypt(const struct rsa_keys_s *keys, char *msg, size_t msg_len, const unsigned long *encrypted_msg)
{
    if (msg == NULL || keys == NULL || encrypted_msg == NULL) {
        return -1;
    }

    /* Compute using modular exponentiation */   
    for (int i = 0; i < msg_len; ++i) {
        *(msg + i) = rsa_modular_exponentiation(*(encrypted_msg + i), keys->private_exp, keys->modulo_n);
    }

    return 0;   
}

int main(int argc, char **argv)
{
   struct rsa_keys_s keys; 
   const char *original_msg;
   char *decrypted_buffer;
   unsigned long *encrypted_buffer;

   if (argc != 2) {
        printf("rsa_example <message>\n");
        return -1;
   }
 
   original_msg = argv[1];

   rsa_generate_keys(&keys, 4703, 3529);

   /* Alocate some space for encrypted buffer */
   encrypted_buffer = calloc(strlen(original_msg), sizeof(unsigned long));
   assert(encrypted_buffer != NULL);
    
   /* Alocate some space for decrypted buffer */
   decrypted_buffer = calloc(strlen(original_msg), sizeof(char));
   assert(decrypted_buffer != NULL);

   rsa_encrypt(&keys, original_msg, strlen(original_msg), encrypted_buffer);

   rsa_decrypt(&keys, decrypted_buffer, strlen(original_msg), (const unsigned long *) encrypted_buffer);

   printf("Message to encrypt: %s\n", original_msg);
   printf("Encrypted msg:");

   for (int i = 0; i < strlen(original_msg); ++i) {
        printf("%ld, ", *(encrypted_buffer + i));
   } 

   printf("\nDecrypted message: %s\n", decrypted_buffer);

   for (int i = 0; i < strlen(original_msg); ++i) {
        printf("%c, ", *(decrypted_buffer + i));
   } 

   /* Free alocated memory */
   free(decrypted_buffer);
   free(encrypted_buffer); 
 
   return 0;
}
