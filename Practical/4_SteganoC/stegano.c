/*
 * stegano.c
 *
 *  Created on: Nov 6, 2011
 *      Author: J. Geurts
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "resource_detector.h"
#include "bmp.h"
#include "stegano.h"

#define    MAX_FILENAME    80

extern uint8_t
SteganoGetSubstring (uint8_t Src, uint8_t SrcPos, uint8_t NrOfBits, uint8_t DestPos)
/* description: get a substring of bits from a uint8_t (i.e. a byte)
 *
 * example: SteganoGetSubstring (Src, 3, 4, 1) with Src=ABCDEFGH (bit H is LSB)
 *          = 000BCDE0
 *
 * parameters:
 *      Src:       byte to get the substring from
 *      SrcPos:    position of the first bit of the substring (LSB=0)
 *      NrOfBits:  length of the substring
 *      DestPos:   position of the first bits of the destination substring
 *
 * return:
 *      substring, starting at DestPos
 */
{

    // init variables
    uint8_t Mask = 0;
    uint8_t Out = 0;

    // create the mask (just te number of 1's - example if NrOfBits = 4 it will generate 00001111)
    int i = 0;
    for (; i < NrOfBits; i++)
    {

        Mask |= (1 << i);
    }

    // shift the mask to the right startposition
    Mask = Mask << SrcPos;

    // apply the mask on the source, removing all unwanted bits
    Out = Src & Mask;

    // shift the bits to the right destination place
    if(SrcPos - DestPos > 0)
    {
        Out = Out >> (SrcPos - DestPos);
    }
    else
    {
        Out = Out << abs(SrcPos - DestPos);
    }
    
    return Out;
}


// should be static in real life, but would give a compiling error in the unchanged code because this function is not yet used
void
ReadHdr (FILE* FilePtr, BMP_MAGIC_t* Magic, BMP_FILE_t* File, BMP_INFO_t* Info)
/*
 * description: read the header of a bmp File, and store the data in the provided parameters
 *
 * parameters:
 *         FilePtr: file, opened for reading
 *         Magic:   output-parameter to store the read BMP_MAGIC_t structure
 *         File:    output-parameter to store the read BMP_FILE_t structure
 *         Info:    output-parameter to store the read BMP_INFO_t structure
 *
 * Note: caller should provide enough memory for parameters 'Magic', 'File' and 'Info'
 */
{
    if (FilePtr != NULL && Magic != NULL && File != NULL && Info != NULL)
    {
        if (-1 != fread(Magic, sizeof(BMP_MAGIC_t), 1, FilePtr))
        {
            if (-1 != fread(File, sizeof(BMP_FILE_t), 1, FilePtr))
            {
                if (-1 != fread(Info, sizeof(BMP_INFO_t), 1, FilePtr))
                {
                    
                }
            }
        }
    }
}


// should be static in real life, but would give a compiling error in the unchanged code because this function is not yet used
void
WriteHdr (FILE* FilePtr, BMP_MAGIC_t* Magic, BMP_FILE_t* File, BMP_INFO_t* Info)
/*
 * description: write the header of a bmp File, where the data comes from the provided parameters
 *
 * parameters:
 *         FilePtr: file, opened for writing
 *         Magic:   input-parameter with a BMP_MAGIC_t structure
 *         File:    input-parameter with a BMP_FILE_t structure
 *         Info:    input-parameter with a BMP_INFO_t structure
 *
 */
{
    if (FilePtr != NULL && Magic != NULL && File != NULL && Info != NULL)
    {
        if (-1 != fwrite(Magic, sizeof(BMP_MAGIC_t), 1, FilePtr))
        {
            if (-1 != fwrite(File, sizeof(BMP_FILE_t), 1, FilePtr))
            {
                if (-1 != fwrite(Info, sizeof(BMP_INFO_t), 1, FilePtr))
                {
                    
                }
                else
                {
                    printf("WriteHdr: Info read error\n");
                }
            }
            else
            {
                printf("WriteHdr: File read error\n");
            }
        }
        else
        {
            printf("WriteHdr: Magic read error\n");
        }
    }
}


extern void
SteganoMultiplex (const char* File0, const char* File1)
{
    FILE* FilePtr0 = NULL;
    FILE* FilePtr1 = NULL;
    FILE* FilePtr2 = NULL;
    char  buf[MAX_FILENAME];

    for (int NrBits = 0; NrBits <= 8; NrBits++)
    {
        // NrBits: number of bits for the hidden image

        // Open the files
        sprintf (buf, "mux_%s_%s_%d.bmp", File0, File1, NrBits);
        FilePtr0 = fopen (File0, "rb");
        FilePtr1 = fopen (File1, "rb");
        FilePtr2 = fopen (buf,   "wb");

        // Read header information from the source image
        BMP_MAGIC_t image0Magic;
        BMP_FILE_t image0File;
        BMP_INFO_t image0Info;
        ReadHdr(FilePtr0, &image0Magic, &image0File, &image0Info);
       
        // Read header information from the hidden image
        BMP_MAGIC_t image1Magic;
        BMP_FILE_t image1File;
        BMP_INFO_t image1Info;
        ReadHdr(FilePtr1, &image1Magic, &image1File, &image1Info);

        // Create the header for the new image
        BMP_MAGIC_t image2Magic = image0Magic;
        BMP_FILE_t image2File = image0File;
        BMP_INFO_t image2Info = image0Info;
        image2File.creator1 = image1Info.width;
        image2File.creator2 = image1Info.height;
        image2Info.nimpcolors = NrBits;
        WriteHdr(FilePtr2, &image2Magic, &image2File, &image2Info);

        // Read for each row the columns
        int32_t heightI = 0;
        for (; heightI < image0Info.height; heightI++)
        {

            // Read from each column the pixel

            uint8_t pixel0[3]; // From the source
            uint8_t pixel1[3]; // From the hidden image
            uint8_t pixel2[3]; // Result goes here

            int32_t widthI = 0;
            for (; widthI < image0Info.width; widthI++)
            {
                
                fread(pixel0, 1, 3, FilePtr0);  // Read the pixel from the source
                fread(pixel1, 1, 3, FilePtr1);  // Read the pixel from the hidden image
                
                // Get for each color a new color based on NrBits
                pixel2[0] = pixel0[0] | SteganoGetSubstring(pixel1[0], 0, NrBits, 0);
                pixel2[1] = pixel0[1] | SteganoGetSubstring(pixel1[1], 0, NrBits, 0);
                pixel2[2] = pixel0[2] | SteganoGetSubstring(pixel1[2], 0, NrBits, 0);
                
                // Write the resutl to the new image
                fwrite(pixel2, 1, 3, FilePtr2);

                // Clear the arrays
                memset(pixel0, 0, sizeof(pixel0));
                memset(pixel1, 0, sizeof(pixel1));
                memset(pixel2, 0, sizeof(pixel2));
                
            }
        }

        // Close the files
        fclose (FilePtr0);
        fclose (FilePtr1);
        fclose (FilePtr2);
    }
}


extern void
SteganoMultiplexText (const char* File0, const char* File1)
{
    FILE* FilePtr0 = NULL;
    FILE* FilePtr1 = NULL;
    FILE* FilePtr2 = NULL;
    char  buf[MAX_FILENAME];

    sprintf (buf, "mux_%s_%s.bmp", File0, File1);
    FilePtr0 = fopen (File0, "rb");
    FilePtr1 = fopen (File1, "rb");
    FilePtr2 = fopen (buf,   "wb");

    // to be implemented


    fclose (FilePtr0);
    fclose (FilePtr1);
    fclose (FilePtr2);
}


extern void
SteganoDemultiplex (const char* File0, const char* File1, const char* File2)
{
    FILE* FilePtr0 = NULL;
    FILE* FilePtr1 = NULL;
    FILE* FilePtr2 = NULL;

    FilePtr0 = fopen (File0, "rb");
    FilePtr1 = fopen (File1, "wb");
    FilePtr2 = fopen (File2, "wb");

    // to be implemented


    fclose (FilePtr0);
    fclose (FilePtr1);
    fclose (FilePtr2);
}


extern void
SteganoDemultiplexText (const char* File0, const char* File1, const char* File2)
{
    FILE* FilePtr0 = NULL;
    FILE* FilePtr1 = NULL;
    FILE* FilePtr2 = NULL;

    FilePtr0 = fopen (File0, "rb"); /* binair lezen */
    FilePtr1 = fopen (File1, "wb"); /* binair schrijven */
    FilePtr2 = fopen (File2, "wb"); /* binair schrijven */

    // to be implemented


    fclose (FilePtr0);
    fclose (FilePtr1);
    fclose (FilePtr2);
}

