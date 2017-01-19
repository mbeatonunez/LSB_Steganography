#ifndef MYSTEGO_H_INCLUDED
#define MYSTEGO_H_INCLUDED

void open_image();                      //function to open image file
void open_hidden();                     //function to open hidden file
void open_output();                     //function to open output file
void close_files();                     //function to close all opened files
void get_header(FILE *fp);              //function to get header information from any file
void print_header(FILE *fp);            //function to print header information from any file
void hide_message();                    //function for to hide the hidden into the image
void unhide_message();                  //function to unhide hidden from image
#endif // MYSTEGO_H_INCLUDED
