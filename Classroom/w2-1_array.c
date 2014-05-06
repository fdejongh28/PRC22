#include <stdio.h>

int main() {
	int array[] = { 0, 1, 2, 3, 4, 5 };
	int *p = &array[5];
	p--;
	printf("Waarde: %d\n", *p);
	return 0;
}
