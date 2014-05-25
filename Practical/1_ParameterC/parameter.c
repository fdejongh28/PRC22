#include <stdio.h>

#include "parameter.h"

void SwapValues (int* a, int* b)
{
	int oldA = *a;
	int oldB = *b;

	*a = oldB;
	*b = oldA;
}

void SwapAddresses (int** a, int** b)
{
	int* oldA = *a;
	int* oldB = *b;

	*a = oldB;
	*b = oldA;
}

int GetSumReturn (int Arr[], int n)
{
	int sum = 0;

	if(n > 0)
	{
		for(int i = 0; i < n; i++) {
			sum += Arr[i];
		}
	}

	return sum;
}

void GetSumParameter (int* Arr, int n, int* Sum)
{
	int sum = 0;

	if(n > 0)
	{
		for(int i = 0; i < n; i++) {
			sum += Arr[i];
		}
	}

	*Sum = sum;
}

char* Dutch (int i)
{
	static char * Translation [] =
    { "nul", "een", "twee", "drie", "vier", "vijf", "zes", "zeven", "acht", "negen", "tien" };

    return Translation [i];
}

void Translate (char* Language (int Index), int Arr[], int n, char** Translations)
{
	if(n > 0)
	{
		for(int i = 0; i < n; i++) {
			*Translations = Language(Arr[i]);
			Translations++;
		}		
	}
}