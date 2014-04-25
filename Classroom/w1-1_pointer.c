#include <stdio.h>

int i;
int* ip;
int* iq;

int main(int argc, char const *argv[])
{
	ip = &i;
	iq = &i;
	*ip = 10;
	printf("%d\n", *iq);
	return 0;
}