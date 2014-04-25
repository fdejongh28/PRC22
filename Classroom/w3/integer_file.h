#ifndef INTEGER_FILE_H
#define INTEGER_FILE_H

#include <stdio.h>

void create_file(char* filename, int high, int low);

void delete_file(char* filename);

int read_value(FILE* fileptr, int pos);


#endif
