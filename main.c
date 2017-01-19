/*
Name:        Moises Beato Nunez
Project:     LSB Steganography
Description: This program gets a message and hides/unhides it to and from an image 8 times its size.
             It shifts each bit of every byte of the message into the LSB position, then it ORs into the LSB of the image file.
             I'm sure there are easer ways to do this, but this is the only way i could manage it.
*/
#include <stdio.h>
#include <stdlib.h>
#include "mystego.h"                                                                    //user define library

#define IMAGE "america.bmp"                                                             //file used as the basis for the steganometry
#define HIDDEN "hidden_message.bmp"                                                     //message to be hidden
#define OUTPUT "output.bmp"                                                             //output of the hidden file
FILE *image;                                                                            //pointer to the america file
FILE *hidden;                                                                           //pointer to the message file
FILE *output;                                                                           //pointer to the output file

int main()
{
    int sel;                                                                            //variable for the menu selection
    char yesno;                                                                         //variable for submenu
    printf("\t\t* * * * * * * * * * * * * * * * * * * * * * * * * *\n"                  //Program introduction
           "\t\t*           Welcome to Image Steganometry!!!      *\n"
           "\t\t*    Where we hide your messages from mediocre    *\n"
           "\t\t*              programmers like myself ^_-        *\n"
           "\t\t* * * * * * * * * * * * * * * * * * * * * * * * * *\n\n");
    printf("What would you like to do?\n"                                               //menu
           "\t1)Print image header.\n"
           "\t2)Print message header.\n"
           "\t3)Hide message.\n"
           "\t4)Read output header.\n"
           "\t5)Retrieve hidden message.\n"
           "\t6)Quit.\n\n"
           "Selection: ");                                                              //obtain menu selection
    scanf("%d", &sel);

    while (sel != 6)                                                                    //while loop for all selections other than 6
    {
        switch(sel)
        {
          case 1:                                                                       //in case option 1 was selected
              open_image();                                                             //open the image file into the pointer
              print_header(image);                                                      //read the header information for the image file
              fclose(image);                                                            //closes the image file
          break;
          case 2:                                                                       //in case option 2 was selected
                open_hidden();                                                          //open the hidden file into the pointer
                print_header(hidden);                                                   //print header for the pointer
                fclose(hidden);                                                         //close hidden file
          break;
          case 3:                                                                       //in case option 3 was selected
              printf("\t\t\t* * * * * * * *\n"                                          //warning message advising the user that if the steganometry
                     "\t\t\t*   WARNING   *\n"                                          //takes place, the hidden image will be deleted
                     "\t\t\t* * * * * * * *\n\n"
                     "For security purposes, once the file has been hidden,\n"
                     "the original file will be deleted. Please remember the length\n"
                     "of the message you wish to hide.\n"
                     "Do you wish to proceed (y/n)? ");
                    scanf(" %c",&yesno);                                                //obtain the user decision
                    while (yesno)                                                       //while loop for the answer
                    {
                        if (yesno == 'n' || yesno == 'N')                               //if no was selected, skip to the end
                        {
                            printf("\nNo steganometry has taken place\n.");
                         break;
                        }
                        else if (yesno == 'y' || yesno == 'Y')                          //if yes was selected, perform the steganometry
                        {
                            open_image();                                               //open image file into pointer
                            open_hidden();                                              //open hidden file into pointer
                            hide_message();                                             //hide hidden file into image file

                            printf("\nFile has been hidden.\n");                        //inform user steganometry has finished
                            printf("Exit the program (6) to view the file\n.");
                            close_files();                                              //close all opened files
                            break;
                        }
                    }
          break;
          case 4:
                open_output();                                                          //open output file into pointer
                print_header(output);                                                   //print header information for output file
                fclose(output);                                                         //close output file
          break;
          case 5:                                                                       //in case option 5 was selected
            unhide_message();                                                           //unhide the hidden file
          break;
          default: printf("\nInvalid selection.\n");                                    //in case invalid chose was selected
        }
        printf("\nWhat would you like to do?\n"                                         //menu
               "\t1)Print image header.\n"
               "\t2)Print message header.\n"
               "\t3)Hide message.\n"
               "\t4)Read output header.\n"
               "\t5)Retrieve hidden message.\n"
               "\t6)Quit.\n\n"
               "Selection: ");
        scanf(" %d", &sel);
    }
    close_files();                                                                      //close all files.
return 0;
}



