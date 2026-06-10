#ifndef DECODE_H
#define DECODE_H
#include "types.h" 

typedef struct DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output file Info */
    char *output_fname;
    FILE *fptr_output;

    /* Decoded Secret File Info */
    char extn_secret_file[20];
    int extn_secret_size;   // to hold extension like .txt
    int size_secret_file;       // decoded size of secret data

}DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for I/P and O/P files */
Status open_files_dec(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real encoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);

/* Perform the Decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

//Decode function for integer that is 4bytes
int decode_size_from_lsb(char *image_buffer);

#endif



