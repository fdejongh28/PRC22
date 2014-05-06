#include "parameter.h"


void SwapValues (int* a, int* b)
{
	int oldA = *a;
	int oldB = *b;

	a = &oldB;
	b = &oldA;

}

void SwapAddresses (int** a, int** b)
{
	int* oldA = *a;
	int* oldB = *b;

	a = &oldB;
	b = &oldA;
}

int GetSumReturn (int Arr[], int n)
{
	return 0;
}

void GetSumParameter (int* Arr, int n, int* Sum)
{

}
/*
char* Dutch (int i)
{
	char a = (char) "a";
	return &char;
}*/

void Translate (char* Language (int Index), int Arr[], int n, char** Translations)
{
	
}