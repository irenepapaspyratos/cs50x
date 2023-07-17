// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    uint8_t array_head[HEADER_SIZE];
    fread(array_head, sizeof(uint8_t), HEADER_SIZE, input);
    fwrite(array_head, sizeof(uint8_t), HEADER_SIZE, output);

    // Read samples from input file and write updated data to output file
    int16_t temp[1];
    int16_t f[1];
    int pos;
    pos = ftell(input);

    if (pos == HEADER_SIZE)
    {
        int y = 1;
        while (y == 1)
        {
            y = fread(temp, sizeof(uint16_t), 1, input);
            if (y == 1)
            {
                f[0] = (uint16_t)(temp[0] * atof(argv[3]));
                fwrite(f, sizeof(uint16_t), 1, output);
            }
        }
    }
    else
    {
        return 1;
    }

    // Close files
    fclose(input);
    fclose(output);
}
