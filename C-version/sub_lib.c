#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#include "sub_lib.h"

int str_len(const char *str)
{
    int len;
    for (len = 0; str[len]; (len)++);
    return len;
}

char *strrev(char *str)
{
    char *p1, *p2;
    if (!str || ! *str)
        return str;
    int i = str_len(str) - 1, j = 0;
    char ch;
    while(i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

bool check_valid(char *str)
{
    int i, len;
    len = str_len(str);
    if (str[0] != '+' && str[0] != '-' && !isdigit(str[0]))
        return false;
    for (i = 1; i < len; i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

big_int get_big()
{
    big_int b;
    int len;
    char temp[max_size];
    do
    {
        gets(temp);
    } while (!check_valid(temp));
    len = str_len(temp);
    strrev(temp);
    if (temp[len - 1] == '-')
    {
        b.sign = -1;
        temp[len - 1] = '\0';
    } else if (temp[len - 1] == '+') {
        b.sign = 1;
        temp[len - 1] = '\0';
    } else {
        b.sign = 1;
    }
    strcpy(b.bistr, temp);
    b.bilen = str_len(b.bistr);

    return clean_int(b);
}


big_int rand_big(int max_len)
{
    big_int temp;
    int len = rand() % max_len + 1;
    char str[len];
    int i;
    //temp.sign = rand() % 2 - rand() % 2;
    temp.sign = 1;

    str[0] = rand()%9 + 1 + '0';

    for (i = 1; i < len; i++)
    {
        str[i] = rand()%9 + '0';
    }
    str[len] = '\0';
    strcpy(temp.bistr, strrev(str));
    temp.bilen = str_len(temp.bistr);

    return clean_int(temp);
}


big_int assign(big_int b)
{
    big_int temp;
    strcpy(temp.bistr, b.bistr);
    temp.bilen = b.bilen;
    temp.sign = b.sign;

    return temp;
}

void show(big_int b)
{
    char temp[b.bilen];
    char sign_c = '\0';
    if (b.sign == -1)
        sign_c = '-';
    temp[b.bilen] = '\0';
    strcpy(temp, b.bistr);
    printf("%c%s", sign_c, strrev(temp));
    printf("\n");
}

big_int clean_int(big_int b)
{
    int len, i = 0;
    char temp[max_size];
    strcpy(temp, b.bistr);
    len = str_len(temp);

    while (i >= 0)
    {
        //take and check the last bit
        i = len - 1;
        if (temp[i] == '0')
        {
            temp[i] = '\0';
            len--;
        }
        else
            break;
    }
    if (str_len(temp) == 0)
    {
        strcpy(temp, "0");
        b.sign = 0;
    }
    strcpy(b.bistr, temp);
    b.bilen = str_len(b.bistr);

    return b;
}

void d2str(char *str, int d)
{
    int digit, i = 0;
    char temp[10];
    do
    {
        digit = d%10;
        temp[i] = digit + '0';
        d = d/10;
        i++;
    } while(d > 0);
    temp[i] = '\0';
    strcpy(str, temp);
}

void d2bin(big_int b, char *s_bin)
{
    big_int temp, quotient, mul_t, ref;
    temp = assign(b);
    int len = temp.bilen, i, j;
    int max_bin = (int)(len * log2(10) ) + 1;  //ceil
    //printf("Size bin: %d %d\n",len, max_bin);
    char bin[max_bin];

    //set const 2
    ref.bistr[0] = '2';
    ref.bistr[1] = '\0';
    ref.bilen = 1;
    ref.sign = 1;

    //quotient = assign(temp);
    for (i = 0; i < max_bin; i++)
    {
        //printf("The compare value is %d. \n", compare(temp, ref));
        if (compare(temp, ref) < 0)
            break;
        quotient = div_digit(temp, 2);
        mul_t = mul_digit(quotient, 2);
        //remain = sub(temp, mul_t);
        if (compare(mul_t, temp) == 0)
        {
            bin[i] = '0';
        } else {
            bin[i] = '1';
        }
        temp = assign(quotient);
    /*
        printf("Binary char [%d]: %c\n", i, bin[i]);
        show(temp);
        printf("\n");
    */
    }
    if (temp.bistr[0] == '1')
        bin[i] = '1';
    else 
        bin[i] = '0';
    bin[i + 1] = '\0';
    bin[max_bin] = '\0';
    
    strcpy(s_bin, strrev(bin));
/*
    for (i = 0; i < max_bin; i++)
        printf("%c", bin[i]);
    printf("\n");
*/
}


//compare the absolute
int compare(big_int b1, big_int b2)
{
    big_int tmp1, tmp2;
    tmp1 = clean_int(assign(b1));
    tmp2 = clean_int(assign(b2));
    strrev(tmp1.bistr);
    strrev(tmp2.bistr);

    if(tmp1.bilen > b2.bilen)
        return 1;
    else if (tmp1.bilen < tmp2.bilen)
        return -1;
    else {
        if (strcmp((tmp1.bistr), (tmp2.bistr)) > 0)
            return 1;
        else if (strcmp((tmp1.bistr), (tmp2.bistr)) < 0)
            return -1;
        else 
            return 0;
    }
}

big_int add(big_int b1, big_int b2)
{
    big_int big, small, sum;
    int max_len, carry = 0;
    int d1, d2, dsum, i;

    if (compare(b1, b2) >= 0)
    {
        big = clean_int(assign(b1));
        small = clean_int(assign(b2));
    } else {
        big = clean_int(assign(b2));
        small = clean_int(assign(b1));
    }
    sum.sign = big.sign;
    max_len = big.bilen;
    char temp[max_len + 1];

    if (big.sign == small.sign || small.sign == 0)
    {
        for (i = 0; i < max_len; i++)
        {
            d1 = big.bistr[i] - '0';
            d2 = (i >= small.bilen) ? 0: small.bistr[i] - '0';

            dsum = d1 + d2 + carry;
            if (dsum >= 10)
            {
                dsum -= 10;
                carry = 1;
            } else 
                carry = 0;
            temp[i] = dsum + '0';
        }
        if (carry == 1)
            temp[i++] = '1';
        temp[i] = '\0';
        strcpy(sum.bistr, temp);
        sum.bilen = str_len(sum.bistr);
    } else {
        small.sign = !small.sign;
        //sum = sub(big, small);
    }
    return sum;
}

big_int add_ui(big_int b, int d)
{
    big_int temp, sum;
    temp = assign(b);
    char str_sum[temp.bilen + 1];
    if (temp.sign < 0)
    {
        temp.sign = -temp.sign;
        sum = sub_ui(temp, d);
        sum.sign = -1;
    } else {
        char str_d[10];
        int i, d1, d2, carry = 0, dsum;
        sprintf(str_d, "%d", d);
        int len_d = str_len(str_d);

        strrev(str_d);

        for (i = 0; i < temp.bilen; i++)
        {
            d1 = (i >= temp.bilen) ? 0 : temp.bistr[i] - '0';
            d2 = (i >= len_d) ? 0 : str_d[i] - '0';

            dsum = d1 + d2 +carry;
            if (dsum >= 10)
            {
                dsum -= 10;
                carry = 1;
            } else 
                carry = 0;
            str_sum[i] = dsum + '0';
        }
        if (carry == 1)
            str_sum[i++] = '1';
        str_sum[i] = '\0';
        strcpy(sum.bistr, str_sum);
        sum.bilen = str_len(sum.bistr);
        sum.sign = 1;
    }

    return clean_int(sum);
}

big_int mul_10(big_int b)
{
    big_int temp;
    int j, len = b.bilen;
    char str[len + 1];
    for (j = len - 1; j >= 0; j--)
        temp.bistr[j + 1] = b.bistr[j];
    temp.bistr[0] = '0';
    temp.bistr[b.bilen + 1] = '\0';
    temp.bilen = strlen(temp.bistr);

    temp.sign = b.sign;
    return clean_int(temp);
}

big_int mul_digit(big_int b, int d2)
{
    big_int temp;
    int carry = 0, j, d1, digitprod;
    for (j = 0; j < b.bilen; j++)
    {
        d1 = b.bistr[j] - '0';
        digitprod = d1 * d2;
        digitprod += carry;
        if (digitprod >= 10)
        {
            carry = digitprod / 10;
            digitprod -= carry * 10;
        }
        else
            carry = 0;
        temp.bistr[j] = digitprod + '0';
    }
    if (carry != 0)
        temp.bistr[j++] = carry + '0';
    temp.bistr[j] = '\0';
    temp.bilen = strlen(temp.bistr);
    temp.sign = b.sign;
    return clean_int(temp);
}

big_int mul(big_int b1, big_int b2)
{
    big_int temp, p_prod;
    int i, digit, k, len2 = b2.bilen;
/*
    temp.bistr[0] = '0';
    temp.bistr[1] = '\0';
    temp.bilen = 1;
    temp.sign = 0;
*/
    char str_m[len2];
    strcpy(str_m, b2.bistr);

    //printf("\nFor debug mul: b2 = %s\n", str_m);

    for (i = 0; i < len2; i++)
    {
        digit = str_m[i] - '0';
        p_prod = mul_digit(b1, digit);
/*
        printf("\n");
        show(p_prod);
        printf("\n");
*/

        for (k = 0; k < i; k++)
        {
            p_prod = mul_10(p_prod);
        /*
            printf("\n");
            show(p_prod);
            printf("\n");
        */
        }
        if (i == 0)
            temp = assign(p_prod);
        else
            temp = add(temp, p_prod);
    /*
        printf("\n");
        show(temp);
        printf("\n------------------\n");
    */
    }
    temp.bilen = str_len(temp.bistr);

    return clean_int(temp);
}

big_int mul_ui(big_int b, int d)
{
    big_int temp, d_convert;
    int i, len = b.bilen;
    char s[len];

    d2str(d_convert.bistr, d);
    //strrev(d_convert.bistr);
    d_convert.bilen = str_len(d_convert.bistr);
    d_convert.sign = 1;

    show(d_convert);

    return mul(b, d_convert);
}


big_int sub(big_int b1, big_int b2)
{
    //b1 - b2 = b1 + (-b2)
    big_int temp, big, small;
    strcpy(temp.bistr, "0");
    temp.bilen = str_len(temp.bistr);
    temp.sign = 0;

    int i, dsub, d1, d2, borrow = 0;

    if (compare(b1, b2) >= 0)
    {
        big = assign(b1);
        small = assign(b2);
        small.sign = -small.sign;
    }
    else {
        big = assign(b2);
        big.sign = -big.sign;
        small = assign(b1);
    }

    temp.sign = big.sign;
    if (small.sign == big.sign)
    {
        temp = add(big, small);
    }
    else
    {
        temp.bilen = big.bilen;
        for (i = 0; i < big.bilen; i++)
        {
            d1 = (i >= big.bilen) ? 0 : big.bistr[i] - '0';
            d2 = (i >= small.bilen) ? 0 : small.bistr[i] - '0';
            dsub = d1 - borrow - d2;
            if (dsub < 0)
            {
                dsub += 10;
                borrow = 1;
            }
            else
                borrow = 0;
            temp.bistr[i] = dsub + '0';
        }
        temp.bistr[big.bilen] = '\0';
        temp.sign = big.sign;
    }

    return clean_int(temp);
}

big_int div_digit(big_int b, int d)
{
    int cmp = 0, check = 0, i;
    int len = b.bilen;
    char divided[len], s_quotient[len];

    big_int temp;
    temp.sign = b.sign;
    strcpy(divided, b.bistr);
    strrev(divided);

    for (i = 0; i < len; i++)
    {
        cmp = (cmp*10 + (divided[i] - '0'));
        if (cmp >= d)
        {
            check = 1;
            s_quotient[i] = (cmp/d + '0');
            cmp %= d;
        } else
            s_quotient[i] = '0';
    }
    s_quotient[len] = '\0';
    strcpy(temp.bistr, strrev(s_quotient));
    temp.bilen = str_len(temp.bistr);

    return clean_int(temp);
}

big_int divd(big_int b1, big_int b2)
{
    big_int divided, divisor, quotient, temp;
    int i, j, k, d_div, diff;
    
    divided = clean_int(assign(b1));
    divisor = clean_int(assign(b2));

    int len1 = divided.bilen, len2 = divisor.bilen;
    quotient.sign = divided.sign * divisor.sign;
    
    if (compare(divided, divisor) < 0)
    {
        quotient.bistr[0] = '0';
        quotient.bistr[1] = '\0';
        quotient.bilen = 1;
        quotient.sign = 0;

        return quotient;
    } else {
        diff = len1 - len2;
        char s_quotient[diff + 1];
        int d_quotient[diff + 1];
        int temp_quot = 0;
        for (i = diff - 1; i >= 0; i--)
        {
            divisor = mul_10(divisor);
        }
        divisor.bistr[len1] = '\0';
        divisor.bilen = len1;
        //printf("\n");
        //show(divisor);


        for (j = 0; j <= diff; j++)
        {
            temp_quot = 0;

            while(compare(divided, divisor) >= 0)
            {
            /*
                printf("\nsign: %d %d\n", divided.sign, divisor.sign);
                printf("Length: %d %d\n", divided.bilen, divisor.bilen);
                show(divided);
                printf(" vs ");
                show(divisor);
                printf("\nThe compare is: %d\n", compare(divided, divisor));
            */
                divided = sub(divided, divisor);
                d_quotient[diff - j]++;
                temp_quot++;
            /*
                printf("\n divided: d_quo[%d]: %d\n", diff-j, temp_quot);
                show(divided);
                printf("---t---\n");
            */
            }
            divisor = div_digit(divisor, 10);
            //s_quotient[diff - j] = d_quotient[diff - j] + '0';
            s_quotient[diff - j] = temp_quot + '0';
        }
        s_quotient[diff + 1] = '\0';
        strcpy(quotient.bistr, s_quotient);
        quotient.bistr[diff + 1] = '\0';
        quotient.bilen = str_len(quotient.bistr);

        return clean_int(quotient);
    }

}


big_int sub_ui(big_int b, int d)
{
    big_int temp, sub;
    temp = assign(b);
    char str_sum[temp.bilen + 1];
    if (temp.sign <= 0)
    {
        temp.sign = -temp.sign;
        sub = add_ui(temp, d);
        sub.sign = -temp.sign;
    }
    else
    {
        
    }

    return clean_int(sub);
}
