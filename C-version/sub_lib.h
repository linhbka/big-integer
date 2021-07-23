#include <stdbool.h>
#define max_size 200

char *strrev(char *str);
typedef struct big_int{
	char bistr[max_size];
	//char orgstr[200];
	int bilen;
	int sign;

} big_int;

int str_len(const char *str);



big_int get_big();
big_int rand_big(int max_len);
void show(big_int b);

void d2str(char *str, int d);
void d2bin(big_int b, char *s_bin);
big_int assign(big_int b2);

bool check_valid(char *str);
big_int clean_int(big_int b);
int compare(big_int b1, big_int b2);

big_int add(big_int b1, big_int b2);
big_int add_ui(big_int b, int d);

big_int sub(big_int b1, big_int b2);
big_int sub_ui(big_int b, int d);

big_int mul(big_int b1, big_int b2);
big_int mul_10(big_int b);
big_int mul_digit(big_int b, int d);
big_int mul_ui(big_int b, int d);

big_int divd(big_int b1, big_int b2);
big_int div_digit(big_int b, int d);
big_int div_ui(big_int b, int d);