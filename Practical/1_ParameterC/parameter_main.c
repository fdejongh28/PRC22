#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "parameter.h"

static bool WithMenu = true;

char*
French (int i)
{
    static char * Translation [] =
    { "zero", "un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf", "dix" };

    return Translation [i];
}

char*
English (int i)
{
    switch (i)
    {
        case 10:
            return ("ten");
        case 9:
            return ("nine");
        case 8:
            return ("eight");
        case 7:
            return ("seven");
        case 6:
            return ("six");
        case 5:
            return ("five");
        case 4:
            return ("four");
        case 3:
            return ("three");
        case 2:
            return ("two");
        case 1:
            return ("one");
        case 0:
            return ("zero");
        default:
            return ("I don't know");
    }
}

static int
GetInt (const char* Message)
{
    char    Line[80];
    int     Value;

    if (WithMenu == true)
    {
        printf ("%s", Message);
    }
    if (fgets (Line, sizeof(Line), stdin) == NULL)
    {
        fprintf (stderr, "ERROR in fgets()\n");
    }
    sscanf (Line, "%d", &Value);

    return Value;
}

static void
PrintTranslations(const int Arr[], char* const Translations[], int n)
{
    int i = 0;

    for (i = 0; i < n; i++)
    {
        printf("%d: '%s'\n", Arr[i], Translations[i]);
    }
    printf("\n");
}

extern int
main (int argc, char* argv[])
{
    int     Value1 = 0;
    int     Value2 = 0;
    int *   ptrA = &Value1;
    int *   ptrB = &Value2;
    int     z[] = { 7, 4, 1, 8, 5, 2, 9, 6, 3, 0, 10 };
    char*   Translations[sizeof(z)/sizeof(z[0])] = {0};
    int     Sum = 0;
    int     n = 0;
    int     Choice = 0;
    bool    Quit = false;

    printf ("\n"
        "PRC - Parameter (version 5)\n"
        "---------------------------\n");

    if (argc != 1)
    {
        fprintf (stderr, "%s: argc=%d\n", argv[0], argc);
    }

    while (!Quit)
    {
        if (WithMenu)
        {
            printf ("\nMENU\n"
                "===========================\n"
                "[0] print welcome message\n"
                "[1] swap values\n"
                "[2] swap addresses\n"
                "[3] compute sum with return\n"
                "[4] compute sum with output-parameter\n"
                "[5] translate in English\n"
                "[6] translate in French\n"
                "[7] translate in Dutch\n"
                "[9] quit\n");
        }
        Choice = GetInt("choice: ");

        switch (Choice)
        {
            case 0:
				printf("Welcome at ParameterC\n");
				break;
            case 1:
                Value1 = GetInt("Geef waarde voor getal 1: ");
                Value2 = GetInt("Geef waarde voor getal 2: ");

                SwapValues(&Value1, &Value2);

                printf ("SwapValues(): %d, %d\n", Value1, Value2);
                break;
            case 2:
                Value1 = GetInt("Geef waarde voor getal 1: ");
                Value2 = GetInt("Geef waarde voor getal 2: ");

                SwapAddresses(&ptrA, &ptrB);

                printf("SwapAddresses(): %d, %d\n", *ptrA, *ptrB);
                break;
            case 3:
                n = GetInt("Geef waarde voor n: ");
                Sum = GetSumReturn (z, n);
                printf ("GetSumReturn(%d): %d\n", n, Sum);
                break;
            case 4:
                n = GetInt("Geef waarde voor n: ");
                GetSumParameter (z, n, &Sum);
                printf ("GetSumParameter(%d): %d\n", n, Sum);
                break;
            case 5:
                n = GetInt("Geef waarde voor n: ");
                Translate(English, z, n, Translations);
                PrintTranslations(z, Translations, n);
                break;
            case 6:
                n = GetInt("Geef waarde voor n: ");
                Translate(French, z, n, Translations);
                PrintTranslations(z, Translations, n);
                break;
            case 7:
                n = GetInt("Geef waarde voor n: ");
                Translate (Dutch, z, n, Translations);
                PrintTranslations(z, Translations, n);
                break;
            case 8:
                if (WithMenu == true)
                {
                    printf ("printing of MENU is disabled\n");
                }
                WithMenu = !WithMenu;
                break;
            case 9:
                Quit = true;
                break;
            default:
                printf ("invalid choice: %d\n", Choice);
                break;
        }

    }
    printf ("ready\n\n");
    return (0);
}

