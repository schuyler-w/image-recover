#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;
 
    if (argc != 2) // Ensure proper usage and to remind user
    {
        printf("Usage: ./recover file_name.raw\n");
        return 1;
    }
 
    FILE *input = fopen(argv[1], "r");
 
    if (input == NULL) // checks if file is accessable
    {
        printf("Could not open file %s.\n", argv[1]);
        return 2;
    }
 
    FILE *output = NULL; // points output to NULL
    BYTE buffer[512]; // create buffer array of 512 elements to store 512 bytes from raw file
    int jpgcount = 0;
    char filename[8] = {0}; // string to hold filename
 
    while (fread(buffer, sizeof(BYTE) * 512, 1, input))
    {
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] & 0xF0) == 0xE0)
        {
            if (output != NULL) // if output isn't empty and already written into, close file
            {
                fclose(output);
            }
            sprintf(filename, "%03i.jpg", jpgcount++);
            output = fopen(filename, "w"); // opens a new output for writing a new jpg into
        }
 
        // while new jpeg isn't found, keep writing to jpg file
        if (output != NULL)
        {
            fwrite(buffer, sizeof(BYTE) * 512, 1, output);
        }
    }
 
    // close the last opened output
    if (output != NULL)
    {
        fclose(output);
    }
 
    fclose(input); // close the original raw file
 
    return 0;
}