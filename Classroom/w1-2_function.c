#include <stdio.h>

int val1;
int val2;
int sum;
int prod;

void hallo(int* v1, int* v2, int* s, int* p)
{
	s = &sum;
	p = &prod;

	*s = *v1 + *v2;
	*p = *v1 * *v2;
}

int main(int argc, char const *argv[])
{
	val1 = 5;
	val2 = 2;

	hallo(&val1, &val2, &sum, &prod);
	
	printf("Som: %d - Product: %d\n", sum, prod);
	return 0;
}