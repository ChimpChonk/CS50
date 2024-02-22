#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    //check for command line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //open file if error return 1
    FILE *raw_file = fopen(argv[1], "r");
    if (!raw_file)
    {
        printf("Error opening image file\n");
        return 1;
    }

    //buffer
    char buffer[BLOCK_SIZE];
    FILE *output_file = NULL;
    int count = 0;
    char filename[12];

    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        //check for JPEG signature
        if (buffer[0] == (char)0xff && buffer[1] == (char)0xd8 && buffer[2]file == (char)0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //close prev file if open
            if (output_file)
            {
                fclose(output_file);
            }

            //create new file
            sprintf(filename, "%03i.jpg", count);
            output_file = fopen(filename, "wb");

            if (!output_file)
            {
                printf("Error creating new file\n");
                fclose(raw_file);
                return 1;
            }
            count++;
        }

        if (output_file)
        {
            fwrite(buffer, 1, BLOCK_SIZE, output_file);
        }

    }

    //close output file if not closed
    if (output_file)
    {
        fclose(output_file);
    }
    fclose(raw_file);
}
