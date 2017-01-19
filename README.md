# LSB Steganography
A simple console C program that uses LSB shifting to hide a message inside of a bmp image file. This was a project for school and I have tons of fun learning how to do it. It is very simple but it gets the job done.

## How does it work?
1 The program loads the message, in this case a .bmp file made in MS Paint.
2 The program loads the image where the file will be hidden.
3 The program goes into the image data, or Pixel Array, protion of the image file
4 While reading the image and message one byte at a time, the program places each bit of the message into the LSB of each bite of the image file's pixel array portion.
5 The message file is erased and the output file is generated.

## Notes:
Before running the encryption, should as the program to print out the size of the message being hidden. The size of the message is used as a password in order to decrypt the file.
Essentially, you could send the output file to someone else, and as long as they have this program and the password, they can decrypt the hidden message.

Since this is using LSB shifting, keep in mind that the message file has to be 8x smaller then the image data portion of the file.
The name of the message, image, files have to be specified in the code before compiling. To make things easer, they should also be located in the same directory as the program.
