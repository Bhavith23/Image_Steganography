#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //------------------ argv[2] validation --------------------------//

    char *argv1 = argv[2];
    char *ext1 = strrchr(argv1, '.');
    if (ext1 == NULL || strcmp(ext1, ".bmp") != 0)
    {
        printf("File is not bmp\n");
        return e_failure;
    }
    decInfo->stego_image_fname = argv1;

    //------------------ argv[3] validation --------------------------//

    if (argv[3] == NULL)
    {
        decInfo->output_fname = "output";
        return e_success;
    }
    else
    {
        decInfo->output_fname = argv[3];    
    }

    return e_success;
}


Status open_files_dec(DecodeInfo *decInfo)
{
    // Stego image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }

    printf("Opening file done successfully\n");

    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    char arr[strlen(MAGIC_STRING) + 1];
    decode_data_from_image(arr, strlen(MAGIC_STRING), decInfo->fptr_stego_image);

    arr[strlen(MAGIC_STRING)] = '\0';
    if (strcmp(arr, MAGIC_STRING) != 0)
    {
        printf("Magic string mismatched\n");
        return e_failure;
    }
    printf("Decoding of magic string done successfully\n");
    return e_success;
}

Status decode_extn_size(DecodeInfo *decInfo)
{
    char arr[32];

    // Read 32 bytes from the image
    fread(arr, 32, 1, decInfo->fptr_stego_image);

    // Decode extension size
    decInfo->extn_secret_size= decode_size_from_lsb(arr);

    printf("Decoded extension size\n");
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extn[20];
    decode_data_from_image(extn, decInfo->extn_secret_size, decInfo->fptr_stego_image);
    extn[decInfo->extn_secret_size] = '\0';
    strcpy(decInfo->extn_secret_file, extn);

    // Build final name safely
    char final_name[100];
    strcpy(final_name, decInfo->output_fname);

    char *ext2 = strrchr(final_name, '.');
    if (ext2 == NULL) 
    {
        // No extension → append decoded one
        strcat(final_name, extn);
    }

    //open output file here only
    decInfo->fptr_output = fopen(final_name, "wb");
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", final_name);
        return e_failure;
    }

    printf("Decoded Secret File Extension successfully\n");
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    int size = decode_size_from_lsb(buffer);
    decInfo->size_secret_file = size;
    printf("Decoded secret file size successfully\n");
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        char buffer[8];
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        ch = decode_byte_from_lsb(buffer);
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }
    printf("Decoded Secret file data successfully\n");
    return e_success;
}

Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(buffer, 8, 1, fptr_stego_image);
        data[i] = decode_byte_from_lsb(buffer);
    }
    return e_success;
}

char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;
    for (int i = 0; i < 8; i++)
    {
        data = data | (image_buffer[i] & 1) << i;
    }
    return data;
}

int decode_size_from_lsb(char *image_buffer)
{
    int size = 0;
    for (int i = 0; i < 32; i++)
    {
        size = size | (image_buffer[i] & 1) << i;
    }
    return size;
}


Status do_decoding(DecodeInfo *decInfo)
{
    if (open_files_dec(decInfo) == e_failure)
        return e_failure;

    if (decode_magic_string(decInfo) == e_failure)
        return e_failure;

    if(decode_extn_size(decInfo) == e_failure)
    return e_failure;

    if (decode_secret_file_extn(decInfo) == e_failure)
        return e_failure;

    if (decode_secret_file_size(decInfo) == e_failure)
        return e_failure;

    if (decode_secret_file_data(decInfo) == e_failure)
        return e_failure;

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    return e_success;
}



