/* auteur : P.A.M. Maessen, J.H.J. Geurts
 * datum  : 6 apr 2008
 * code   : Ansi C
 * versie : 5
 */

#include <stdlib.h>
#include <stdio.h>
#include "resource_detector.h"
#include "file.h"

#define FILENAME    "students.dat"

static bool WithMenu = true;

static void 
PrintStudentsFile (void)
/* pre :
 * post: alle gegevens van alle studenten in de file zijn afgedrukt.
 */
{
    FILE*   FilePtr = NULL;
    STUDENT Student = {0, "", 0};
    int     Number = 0;

    FilePtr = fopen (FILENAME, "rb"); /* binair lezen */
    if (FilePtr == NULL)
    {
        fprintf (stderr, "Unable to open '%s' for reading\n", FILENAME);
        return;
    }

    while (ReadElement(FilePtr, Number++, &Student) == 1)
    {
        printf ("Nummer       : %d\n", Student.StudentNumber);
        printf ("Naam         : %s\n", Student.Surname);
        printf ("Studiepunten : %d\n", Student.StudyCredits);
        printf ("----------------------------\n");
    }

    fclose (FilePtr);
}

static int 
GetInt (const char* Message)
{
    char Line[80];
    int Value;

    if (WithMenu)
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
GetString (const char* Message, char* Str)
{
    char Line[80];

    if (WithMenu)
    {
        printf ("%s", Message);
    }
    if (fgets (Line, sizeof(Line), stdin) == NULL)
    {
        fprintf (stderr, "ERROR in fgets()\n");
    }
    sscanf (Line, "%s", Str);
}

int main (int argc, char * argv[])
{
    bool Quit = false;

    printf ("PRC opdracht 2 'FileC' (version 5)\n"
            "----------------------------------\n");

    if (argc != 1)
    {
        fprintf (stderr, "%s: argc=%d\n", argv[0], argc);
    }

    while (!Quit)
    {
        int Choice = 0;
        int StudentNumber = 0;
        STUDENT Student = {0, "", 0};
       
        if (WithMenu)
        {
            printf ("\n\nMENU\n"
                "===========================================\n"
                "[1] Tonen van ALLE studenten\n"
                "[2] Binair opzoeken van een student\n"
                "[3] Lineair opzoeken van een student\n"
                "[4] Gesorteerd toevoegen van een student\n"
                "[5] Verwijderen van een student\n"
                "[6] Berekenen gemiddeld aantal studiepunten\n"
                "[8] show/hide menu\n"
                "[9] quit\n");
        }
        Choice = GetInt ("choice: ");

        switch (Choice)
        {
            case 1:
                PrintStudentsFile ();
                break;
            case 2:
                StudentNumber = GetInt ("Geef studentnummer: ");
                if (BinarySearchStudentsFile (FILENAME, StudentNumber, &Student) == 0)
                {
                    printf ("[BINARYSEARCH] student '%d':\n", StudentNumber);
                    printf ("Number       : %d\n", Student.StudentNumber);
                    printf ("Name         : '%s'\n", Student.Surname);
                    printf ("Studiepunten : %d\n", Student.StudyCredits);
                }
                else
                {
                    printf ("[BINARYSEARCH] student '%d' not available\n", StudentNumber);
                }
                break;
            case 3:
                StudentNumber = GetInt ("Geef studentnummer: ");
                if (LinearSearchStudentsFile (FILENAME, StudentNumber, &Student) == 0)
                {
                    printf ("[LINEARSEARCH] student '%d':\n", StudentNumber);
                    printf ("Nummer       : %d\n", Student.StudentNumber);
                    printf ("Naam         : \"%s\"\n", Student.Surname);
                    printf ("Studiepunten : %d\n", Student.StudyCredits);
                }
                else
                {
                    printf ("[LINEARSEARCH] student '%d' not available\n", StudentNumber);
                }
                break;
            case 4:
                Student.StudentNumber = GetInt ("Geef studentnummer              : ");
                GetString ("Geef Achternaam                 : ", Student.Surname);
                Student.StudyCredits = GetInt ("Geef aatal behaalde studiepunten: ");
                if (AddStudentSortedToFile (FILENAME, &Student) == 0)
                {
                    printf ("[ADD] studentnummer '%d' komt al voor\n", Student.StudentNumber);
                }
                else
                {
                    printf ("[ADD] studentnummer '%d' toegevoegd\n", Student.StudentNumber);
                }
                break;
            case 5:
                StudentNumber = GetInt ("Geef studentnummer: ");
                if (RemoveStudentFromFile (FILENAME, StudentNumber) == 0)
                {
                    printf ("[DELETE] studentnummer '%d' verwijderd uit file.\n", StudentNumber);
                }
                else
                {
                    printf ("[DELETE] studentnummer '%d' komt NIET voor\n", StudentNumber);
                }
                break;
            case 6:
                {
                    double Average = 0.0;
                    ComputeAverageStudyResults (FILENAME, &Average);
                    printf ("[AVERAGE] Gemiddeld aantal studiepunten : %5.2f\n", Average);
                }
                break;
            case 8:
                if (WithMenu)
                {
                    printf ("printing of MENU is diabled\n");
                }
                WithMenu = !WithMenu;
                break;
            case 9:
                Quit = true;
                break;
            default:
                printf ("ERROR: invalid Choice: %d\n", Choice);
                break;
        }
    }

    return (0);
}

