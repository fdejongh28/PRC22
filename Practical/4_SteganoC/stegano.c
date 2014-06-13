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
extern uint8_t SteganoGetSubstring (uint8_t Src, uint8_t SrcPos, uint8_t NrOfBits, uint8_t DestPos)
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
* FilePtr: file, opened for reading
* Magic: output-parameter to store the read BMP_MAGIC_t structure
* File: output-parameter to store the read BMP_FILE_t structure
* Info: output-parameter to store the read BMP_INFO_t structure
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
* FilePtr: file, opened for writing
* Magic: input-parameter with a BMP_MAGIC_t structure
* File: input-parameter with a BMP_FILE_t structure
* Info: input-parameter with a BMP_INFO_t structure
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
            int32_t widthI = 0;
            for (; widthI < image0Info.width; widthI++)
            {
                // Read from each column the pixel
                uint8_t pixel0[3]; // From the source
                uint8_t pixel1[3]; // From the hidden image
                uint8_t pixel2[3]; // Result goes here
                

                if(0 != fread(pixel0, 3, 1, FilePtr0)) // Read the pixel from the source
                {
                    if(0 != fread(pixel1, 3, 1, FilePtr1)) // Read the pixel from the hidden image
                    {
                        // Get for each color a new color based on NrBits
                        pixel2[0] = SteganoGetSubstring(pixel0[0], NrBits, 8 - NrBits, NrBits) | SteganoGetSubstring(pixel1[0], 8 - NrBits, NrBits, 0);
                        pixel2[1] = SteganoGetSubstring(pixel0[1], NrBits, 8 - NrBits, NrBits) | SteganoGetSubstring(pixel1[1], 8 - NrBits, NrBits, 0);
                        pixel2[2] = SteganoGetSubstring(pixel0[2], NrBits, 8 - NrBits, NrBits) | SteganoGetSubstring(pixel1[2], 8 - NrBits, NrBits, 0);

                        // Write the resutl to the new image
                        fwrite(pixel2, 3, 1, FilePtr2);                        
                    }
                }

                // Clear the arrays
                memset(pixel0, 0, sizeof(pixel0));
                memset(pixel1, 0, sizeof(pixel1));
                memset(pixel2, 0, sizeof(pixel2));
                
            }
        }

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
    // (FILE* FilePtr, BMP_MAGIC_t* Magic, BMP_FILE_t* File, BMP_INFO_t* Info)

    BMP_MAGIC_t imageMagic;
    BMP_FILE_t  imageFile;
    BMP_INFO_t  imageInfo;
    ReadHdr(FilePtr0, &imageMagic, &imageFile, &imageInfo);

    fseek(FilePtr1, SEEK_END, 0);
    size_t textSize = ftell(FilePtr1);
    rewind(FilePtr1);
    
    imageFile.creator1 = textSize;

    WriteHdr(FilePtr0, &imageMagic, &imageFile, &imageInfo);

    // uint8_t textI = 0;

    int32_t heightI = 0;
    for (; heightI < imageInfo.height; heightI++)
    {
        int32_t widthI = 0;
        for (; widthI < imageInfo.width; widthI++)
        {

            // printf("&d\n", widthI);
            // uint8_t pixel[3];
            // uint8_t pixelNew[3];
            // fread(pixel, 1, 3, FilePtr0);


            // pixelNew[0] = SteganoGetSubstring()

        }
    }

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

    // Read header information from the source image
    BMP_MAGIC_t image0Magic;
    BMP_FILE_t image0File;
    BMP_INFO_t image0Info;
    ReadHdr(FilePtr0, &image0Magic, &image0File, &image0Info);
    uint32_t NrBits = image0Info.nimpcolors;

    BMP_MAGIC_t image1Magic = image0Magic;
    BMP_FILE_t image1File = image0File;
    BMP_INFO_t image1Info = image0Info;
    image1File.creator1 = 0;
    image1File.creator2 = 0;
    image1Info.nimpcolors = 0;
    WriteHdr(FilePtr1, &image1Magic, &image1File, &image1Info);

    BMP_MAGIC_t image2Magic = image0Magic;
    BMP_FILE_t image2File = image0File;
    BMP_INFO_t image2Info = image0Info;
    image2File.creator1 = 0;
    image2File.creator2 = 0;
    image2Info.nimpcolors = 0;
    WriteHdr(FilePtr2, &image2Magic, &image2File, &image2Info);

    int32_t heightI = 0;
    for (; heightI < image0Info.height; heightI++)
    {

        uint8_t pixel0[3]; // From the source
        uint8_t pixel1[3]; // For the visible image
        uint8_t pixel2[3]; // For the hidden image

        int32_t widthI = 0;
        for (; widthI < image0Info.width; widthI++)
        {
            fread(pixel0, 1, 3, FilePtr0);  // Read the pixel from the source

            uint8_t mask;
            
            // Get the visible image
            mask = SteganoGetSubstring(0xff, 0, NrBits, 0);
            pixel1[0] = pixel0[0] | mask;
            pixel1[1] = pixel0[1] | mask;
            pixel1[2] = pixel0[2] | mask;
            fwrite(pixel1, 1, 3, FilePtr1);

            // Get the hidden image
            // mask = SteganoGetSubstring(0xff, NrBits, 8 - NrBits, 8 - NrBits);
            pixel2[0] = SteganoGetSubstring(pixel0[0], 8 - NrBits, NrBits, 8 - NrBits);
            pixel2[1] = SteganoGetSubstring(pixel0[1], 8 - NrBits, NrBits, 8 - NrBits);
            pixel2[2] = SteganoGetSubstring(pixel0[2], 8 - NrBits, NrBits, 8 - NrBits);
            // pixel2[0] = pixel0[0] | mask;
            // pixel2[1] = pixel0[1] | mask;
            // pixel2[2] = pixel0[2] | mask;
            fwrite(pixel2, 1, 3, FilePtr2);

            // Clear the arrays
            memset(pixel0, 0, sizeof(pixel0));
            memset(pixel1, 0, sizeof(pixel1));
            memset(pixel2, 0, sizeof(pixel2));
            
        }
    }

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


    fclose (FilePtr0);
    fclose (FilePtr1);
    fclose (FilePtr2);
}

