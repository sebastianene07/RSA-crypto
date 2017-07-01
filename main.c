#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void print_usage(const char *progr_name)
{
    fprintf(stderr, "Usage: %s [-pged ]\n"
                "\t-p <upper_limit> <prime_file> generate prime numbers\n"
                "\t-g <prime_file> generate keys\n"
                "\t-e <message> encrypt message\n"
                "\t-d <message> decrypt message\n"
                ,progr_name);
}

int main(int argc, char **argv)
{
    char opt;
    
    if (argc == 1)
    {
        print_usage(argv[0]);
        exit(EXIT_FAILURE); 
    }

    while ((opt = getopt(argc, argv, "p:g:e:d")) != -1)
    {
        switch (opt)
        {
        case 'p':
            optind--;
            for(;optind < argc && *argv[optind] != '-'; optind++) {
                printf("%s\n", argv[optind]);
            }
            break;
        case 'g':
            break;
        case 'e':
            break;
        case 'd':
            break;
        case '?':
            break;
        default:
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}
