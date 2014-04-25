#include <stdlib.h>
#include "integer_file.h"

void create_file(char* filename, int high, int low)
/* pre : filename contains a valid c string
 * post: a new file is created with name filename. The file contains integers,
 *       starting from high and counting down to low.
 *       thus create_file("test.txt", 3999, 3000) will result in a text file that contains 1000 integers
 *       starting at 3999 and counting down to 3000
 */
{
    // TODO: implement
}

void delete_file(char* filename)
/* pre : filename contains a valid c string
 * post: file is removed from disk
 */
{
    // TODO: implement
}

int read_value(FILE* fileptr, int pos)
/* pre : -
 * post: the value on position pos is read from the correct location in the already opened file
 *       if fileptr is NULL, -1 is returned.
 */
{
    // TODO: implement
    return 0;
}
