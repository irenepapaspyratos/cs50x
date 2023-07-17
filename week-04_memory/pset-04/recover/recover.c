// Program to recover deleted JPEGs

#include <stdio.h>
#include <stdlib.h>

const int BLOCK = 512;
typedef unsigned char BYTE;

int main(int argc, char *argv[])
{
    // Quit, if no filename is given to recover from
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    // Quit, if file to recover from is not readable
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("The given file is not readable!");
        return 1;
    }


    // Get the size etc. of the file to recover from in bytes(chars) (7918080)
    fseek(file, 0, SEEK_END);
    int size_of_file = ftell(file);
    int block_count = size_of_file / BLOCK;
    rewind(file);

    BYTE *block_copy = malloc(BLOCK);
    if (block_copy == NULL)
    {
        printf("Not enough space for dynamic memory!");
        return 1;
    }

    // Read blocks in 512-buffer
    int read = 0;
    int pics = 0;
    int write = 0;
    char *filename = malloc(8);
    FILE *pic = NULL;
    while (fread(block_copy, BLOCK, 1, file) == 1)
    {
        //Check, if block is start of JPEG or not
        if (*block_copy == 0xff
            && *(block_copy + 1) == 0xd8
            && *(block_copy + 2) == 0xff
            && (*(block_copy + 3) & 0xf0) == 0xe0)
        {
            // Calculate name of new/actual jpg
            sprintf(filename, "%03i.jpg", pics);

            // Open new jpg, write first block and set write to 1 for following block
            pic = fopen(filename, "w");
            if (pic != NULL)
            {
                write = 1;
                fwrite(block_copy, sizeof(BYTE), BLOCK, pic);
            }
            fclose(pic);

            pics++;
        }
        else
        {
            // Write following block if allowed
            if (write == 1)
            {
                pic = fopen(filename, "a");
                if (pic != NULL)
                {
                    fwrite(block_copy, sizeof(BYTE), BLOCK, pic);
                    fclose(pic);
                }
            }
        }

        read++;
    }
    fclose(file);

    if (read != block_count)
    {
        printf("Looks like fread didn't work properly...\n");
        return 1;
    }

    free(block_copy);
    free(filename);
}
