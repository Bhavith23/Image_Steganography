/*******************************************
* Name        : Bhavith M Gowda
* Title       : LSB Image Stegnography
* Date        : 29-09-2025
* Description : LSB image steganography hides secret data within an image by replacing the least significant bits of its pixel values,
                creating a visually unchanged but data-embedded “stego” image
*********************************************/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
    {
        return e_encode;
    }
    if(strcmp(argv[1],"-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

int main(int argc,char *argv[])
{
    if(argc < 2)
    {
        printf("ERROR: Insufficient arguements\n ");
        printf("Correct arguement Encoding: ./a.out -e src.bmp secret.txt [filename.bmp]\n");
        printf("Correct arguement Decoding: ./a.out -d src.bmp [filename.txt]\n");
        return 0;
    }

    OperationType res = check_operation_type(argv);

    if(res == e_encode)
    {
        if(argc < 3)
        {
            printf("ERROR: Insufficient arguments for encoding\n");
            printf("Usage: ./a.out -e src.bmp secret.txt [output.bmp]\n");
            return 0;
        }
        EncodeInfo encInfo;
        Status res1 = read_and_validate_encode_args(argv,&encInfo);

        if(res1 == e_success)
        {
            do_encoding(&encInfo);
        }
        else if(res1 == e_failure)
        {
           return 0;
        }
    }

    else if(res == e_decode)
    {
        if(argc < 2)
        {
            printf("ERROR: Insufficient arguments for decoding\n");
            printf("Usage: ./a.out -d src.bmp [output.txt]\n");
            return 0;
        }
        DecodeInfo decInfo;
        Status res2 = read_and_validate_decode_args(argv,&decInfo);

        if(res2 == e_success)
        {
            do_decoding(&decInfo);
        }
        else if(res2 == e_failure)
        {
            return 0;
        }
    }
    
    else
    {
        printf("ERROR: Unsupported operation %s\n", argv[1]);
        printf("Use -e for encoding or -d for decoding\n");
        return 0;
    }
    return 0;
}
