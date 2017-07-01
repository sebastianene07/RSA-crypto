/* Simple Erathostene sieve with bit fields
 *
 * sebastian.ene07@gmail.com
 *
 * This solution stops at about 1000 000 upper limit.
 * We can use the disk to generate prime numbers 
 * greater than this but this will add some 
 * disk penalities.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

/* Get the byte offset from the start */
#define OFFSET(n)                       ((n) / 8)

/* Toogle bit helper */
#define TOOGLE_BIT(x, i, state)         ((~(1 << (i)) & (x)) | ((state) << (i))) 

/* Printing bits helper */
#define BYTE_TO_BINARY_PATTERN " %c%c%c%c%c%c%c%c\n"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

/* Name: 
 *      erathostene_sieve
 *
 * Description:
 *      Compute the prime numbers up to upper_limit and
 *      print them to stdout.
 *
 * Arguments:
 *      upper_limit -  the sieve upper limit
 *      filename    -  path where we store prime numbers
 */
static void erathostene_sieve(unsigned long upper_limit, const char *filename)
{
    unsigned long num_primes = 0;

    puts(filename);
    FILE *out_bin_file = fopen(filename, "a");
    assert(out_bin_file != NULL);

    /* Write the number of prime numbers .Right now is a placeholder */
    size_t nwrite = fwrite(&num_primes, sizeof(num_primes), 1, out_bin_file);
    assert(nwrite != 0);

    char *bit_list = calloc(OFFSET(upper_limit) + 1, 1);
    assert(bit_list != NULL);

    *bit_list |= TOOGLE_BIT(*bit_list, 0, 1);
    *bit_list |= TOOGLE_BIT(*bit_list, 1, 1);

    unsigned long last_p = 2;
    unsigned long p;

    while (last_p < upper_limit) {

        p = last_p;

        while (p < upper_limit) {
            p += last_p;
            *(bit_list + OFFSET(p)) |= TOOGLE_BIT(*(bit_list + OFFSET(p)), p % 8, 1);
        }

        /* Search for the next bit of 0 */
     
        for (last_p = last_p + 1; last_p < upper_limit; ++last_p) {

            /* If the number is prime the coresponding bit is '0' */

            if (!(*(bit_list + OFFSET(last_p)) & (1 << (last_p % 8)))) {
                printf("%ld\n", last_p);
                size_t nwrite = fwrite(&last_p, sizeof(last_p), 1, out_bin_file);
                assert(nwrite != 0);

                ++num_primes;       
                break; 
            }
        }
    }        

    /* Seek to the start and update number of prime numbers */
    int seek_stat = fseek(out_bin_file, 0L, SEEK_SET);
    assert(seek_stat >= 0);

    nwrite = fwrite(&num_primes, sizeof(num_primes), 1, out_bin_file);
    assert(nwrite != 0);

    /* Free resources, close output file */
    fclose(out_bin_file);
    free(bit_list);

    bit_list     = NULL;
    out_bin_file = NULL;
}

int main(int argc, char **argv)
{
    unsigned long upper_limit;
    char *msg = NULL, *filename = NULL;

    if (argc != 3) {
        printf("prime_gen <upper_limit> <output_file>\n");
        return EXIT_FAILURE;
    }

    filename = argv[2];
    upper_limit = strtol(argv[1], &msg, 10);
    erathostene_sieve(upper_limit, filename);

    return EXIT_SUCCESS;
}
