#include <stdio.h>
#include <stdlib.h>
#include "mystego.h"                                                //user defined function

#define IMAGE "america.bmp"                                         //file used as the basis for the encryption
#define HIDDEN "hidden_message.bmp"                                 //message to be hidden
#define OUTPUT "output.bmp"                                         //output of the encrypted file
FILE *image;                                                        //pointer to the america file
FILE *hidden;                                                       //pointer to the message file
FILE *output;                                                       //pointer to the output file

unsigned char ident[2];                                             //variable to hold the file identity
unsigned int fsize;                                                 //variable to hold the size of the file
unsigned int offset;                                                //variable to hold the location of the first pixel
int width;                                                          //variable to hold the width of the file
int height;                                                         //variable to hold the height of the file

void open_image()                                                   //function to open image file  in read mode
{
    image = fopen(IMAGE, "rb");                                     //open image file in binary into image pointer

    if (image == NULL)                                              //check if image file exists
    {
        printf("Error, some files might be missing");               //error message

    }
}
void open_hidden()                                                  //function to open hidden file in read mode
{
    hidden = fopen(HIDDEN, "rb");                                   //open hidden file in binary into image pointer

    if (hidden == NULL)                                             //check if hidden file exists
    {
        printf("\nFile was hidden and deleted.\n"                   //inform user about the state of the hidden file
        "Please decrypt output to recover message.\n");

    }
}
void open_output()                                                  //function to open output file  in read mode
{
    output = fopen(OUTPUT, "rb");                                   //open output file in binary into image pointer

    if (output == NULL)                                             //check if output file exists
    {
        printf("\nNo steganometry has taken place.\n");             //error message
    }
}
void close_files()                                                  //function to close all open files
{
    fclose(hidden);
    fclose(output);
    fclose(image);
}

void get_header(FILE *fp)                                           //function to get header information from any file
{
    rewind(fp);                                                     //set cursor the beginning of file
    fread(ident, sizeof(char), 2, fp);                              //get the file identity from pointer
    fread(&fsize,sizeof(int),1,fp);                                 //get the file size from pointer
    fseek(fp,0xA,SEEK_SET);                                         //set cursor to bytes containing the offset location
    fread(&offset,sizeof(int),1,fp);                                //get location of first pixel
    fseek(fp,0x12,SEEK_SET);                                        //set cursor to bytes containing the image width
    fread(&width,sizeof(int),1,fp);                                 //get image width
    fseek(fp,0x16,SEEK_SET);                                        //set cursor to bytes containing the image length
    fread(&height,sizeof(int),1,fp);                                //get image length

}

void print_header(FILE *fp)                                         //function to print header information
{
    get_header(fp);                                                 //function to get header information
    printf("FILE INFORMATION:\n\n");
    printf("   File Type:   %2c%c\n", ident[0],ident[1]);           //print file type
    printf("   File Length:  %d Bytes\n",fsize);                    //print file size
    printf("   BMP Information Offset: 0x%x\n", offset);            //print first pixel location
    printf("   Image Width:  %d Pixels\n", width);                  //print image width
    printf("   Image Height: %d Pixels\n\n", height);               //print image length
    ident [0]= 0;                                                    //clear header infor after use
    ident [1] = 0;
    fsize = 0;
    offset = 0;
    width = 0;
    height  = 0;

}
void hide_message()                                                 //function to encrypt the message
{
    get_header(image);                                              //get header information
    rewind(hidden);                                                 //set cursor the beginning of file
    rewind(image);                                                  //set cursor the beginning of file

    output = fopen(OUTPUT, "wb");                                   //open output file in binary write mode
    if (output == NULL)                                             //check for error
    {
        printf("Error, some files might be missing");               //error message
        exit(0);                                                    //exit program
    }

    long sizeA, i,sizeB, h;                                         //variable to hold the size of the files
	char *a,*b,*header;                                             //pointer to hold file information
    unsigned char outMur;                                           //variable to hold a single byte
    int j,k;                                                        //variables for loops

    fseek(image , 0 , SEEK_END);                                    //set cursor to end of file
    sizeA = ftell(image);                                           //get cursor position, and file size
    fseek(hidden , 0 , SEEK_END);                                   //set cursor to end of file
    sizeB = ftell(hidden);                                          //get cursor position, and file size
    rewind(hidden);                                                 //set cursor the beginning of file
    rewind(image);                                                  //set cursor the beginning of file
    a = (char*)malloc(sizeA);                                       //allocate memory the size of the file
    b = (char*)malloc(sizeB);                                       //allocate memory the size of the file
    header = (char*)malloc(offset-1);                               //allocate memory the size of the offset - 1 byte

    fread(a,sizeof(char),sizeA,image);                              //read the file into the pointer
    fread(b,sizeof(char),sizeB,hidden);                             //read the file into the pointer
    rewind(hidden);                                                 //set cursor the beginning of file
    rewind(image);                                                  //set cursor the beginning of file

    fread(header,sizeof(char),(offset),image);                      //read header from image file
    fwrite(header,sizeof(char),(offset),output);                    //copy header into output file
    fseek(image,offset,SEEK_SET);                                   //set cursor to offset location

    i=offset;                                                       //set counter equals to offset
    for(h=0;h<sizeB;h++)                                            //for loop to loop the length of the hidden file
    {
        j = 7;                                                      //counter for bit shifting
        for(k=0;k<=7;k++)                                           //loop for the shifting of 8 bits
        {
            outMur = (a[i] & (~1)) | ((1) & (b[h] >> j));           //place the MSB if byte h into the LSB if byte i, and every subsequent bit of h into every subsequent byte i.
            j--;                                                    //decrement the number if shifts
            i++;                                                    //proceed to the next byte i of image file
            fputc(outMur,output);                                   //copy the altered byte i into the output file
        }
    }
    while (i != (sizeA))                                            //while loop to place remain unaltered byte of image, if any, into the output
    {
        fputc(a[i],output);
        i++;
    }
     free(a);                                                       //free the allocated memory
     free(b);                                                       //free the allocated memory
     free(header);                                                  //free the allocated memory
     fclose(hidden);                                                //close the hidden file
     unlink("hidden_message.bmp");                                  //for security, deleted the message

}
void unhide_message()                                               //function to get the hidden message from the output file
{
    long sizeA,sizeB,i,h;                                           //variable to hold the size of the files
    char *a,*b;                                                     //pointer to hold file information
    int k,j;                                                        //variables for loops

    open_output();                                                  //open output file
    hidden = fopen(HIDDEN, "wb");                                   //create hidden file in binary write mode
    if (output == NULL || hidden == NULL)                           //check for errors
    {
        printf("Error, some files might be missing");
        exit(0);
    }
    get_header(output);                                             //get output file header information
    rewind(output);                                                 //set cursor the beginning of file

    printf("\nPlease provide the length of the hidden message: ");  //obtain size of hidden file
    scanf("%ld", &sizeB);

    fseek(output , 0 , SEEK_END);                                   //set cursor to end of file
    sizeA = ftell(output);                                          //get cursor position, and file size
    a = (char*)malloc(sizeA);                                       //allocate memory the size of the file
    b = (char*)malloc(sizeA);                                       //allocate memory the size of the file
    rewind(output);                                                 //set cursor the beginning of file

    fread(a,sizeof(char),sizeA,output);                             //read the file into the pointer
    fseek(output,offset,SEEK_SET);                                  //set cursor to offset location
    i = offset;                                                     //set counter equals to offset
    h = 0;                                                          //initialize counter
    while(i <= sizeA)                                               //while loop that counts from the offset to the end of the output file
    {
        j=7;                                                        //bit shift counter
        for(k=0;k<=7;k++)                                           //loop for the shifting of 8 bits
        {
            b[h] = b[h] | ((a[i] & (1))<< j);                       //get LSB from output byte, shift it to the front and OR it to the hidden byte
            i++;                                                    //proceed to the next byte of the output file
            j--;                                                    //decrement the shift amount
        }
        fputc(b[h],hidden);                                         //copy the completed byte into the hidden file
        h++;                                                        //proceed to the next byte of the hidden file
        if (h == sizeB)                                             //once the hidden file reaches the appropriate size, stop adding bytes
            break;
    }
    free(a);                                                        //free the allocated memory
    free(b);                                                        //free the allocated memory
    close_files();                                                  //close all files.
    unlink("output.bmp");                                           //delete output file
}
