#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "binteger.h"
#include "gmp.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    big_int b1, b2, b3, b4, b5, b6;
    mpz_t p1, p2, p3, p4, p5, p6, p7, p8;

    double consum_1 = 0, consum_2 = 0;

    int i, point = 0;
    mpz_init_set_ui(p1, 1);
    mpz_init_set_ui(p2, 1);
    mpz_init_set_ui(p3, 1);
    mpz_init_set_ui(p4, 1);
    mpz_init_set_ui(p5, 1);
    mpz_init_set_ui(p6, 1);
    mpz_init_set_ui(p7, 1);
    mpz_init_set_ui(p8, 1);

    char t1[max_size], t2[max_size], t3[max_size], t4[max_size], t5[max_size];
    char s_bin_1[max_size], s_bin_2[max_size];
    //int sample = 1000000;

    int n = atoi(argv[1]);
    assert(n >= 0);

    for (i = 0; i < n; i++)
    {
        b1 = rand_big(39);
        b2 = rand_big(39);

        strcpy(t1, b1.bistr);
        strcpy(t2, b2.bistr);

        strrev(t1);
        strrev(t2);

        mpz_init_set_str(p1, t1, 10);
        mpz_init_set_str(p2, t2, 10);

        clock_t begin = clock();

        b3 = add(b1, b2);
        //b4 = sub(b1, b2);
        //b5 = mul(b1, b2);
        //b6 = divd(b1, b2);
        //d2bin(b1, s_bin_1);
        //d2bin(b2, s_bin_2);

        clock_t end = clock();
        consum_1 += (double)(end - begin)/CLOCKS_PER_SEC;


        clock_t begin_2 = clock();
        mpz_add(p3, p1, p2);
        //mpz_sub(p4, p1, p2);
        //mpz_mul(p5, p1, p2);
        //mpz_div(p6, p1, p2);

        //mpz_init_set_str(p7, s_bin_1, 2);
        //mpz_init_set_str(p8, s_bin_2, 2);

        clock_t end_2 = clock();
        consum_2 += (double)(end_2 - begin_2) / CLOCKS_PER_SEC;
        /*
		printf("b1 and b2: \n");
		show(b1);
		show(b2);
	*/
        strcpy(t3, b3.bistr);
        mpz_init_set_str(p4, strrev(t3), 10);
        if (mpz_cmp(p3, p4) == 0)
            point++;
    }

    mpz_clear(p1);
    mpz_clear(p2);
    mpz_clear(p3);
    mpz_clear(p4);
    mpz_clear(p5);
    mpz_clear(p6);
    mpz_clear(p7);
    mpz_clear(p8);

    printf("Done!\n");
    printf("Consuming 1: %lf\n", consum_1);
    printf("Time per 1 turn with libbig.a: %1.8f\n", consum_1/n);
    printf("-----------------------\n");
    printf("Consuming 2: %lf\n", consum_2);
    printf("Time per 1 turn with libgmp.a: %1.8f\n", consum_2/n);
    printf("***********************\n");
    printf("The result:%d/%d\n",point, n);
    return 0;
}