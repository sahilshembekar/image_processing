#include "Image.h"

int main(int argc, char** argv) {
    // Pass the test Image here
    Image test("test.png");
    // test.write("new.jpg");
    // Image copy = test;  // to check if out copy constructor works

    // for (int i = 0; i < copy.w * copy.channels; ++i) {
    // // set top row of pixels in that Image to white
    // copy.data[i] = 255;
    // }
    // copy.write("copy.png");

    // Image blank(100, 100, 3);  // Test our blank Image
    // blank.write("blank.jpg");  // write it to blank.jpg

    // Image gray_avg = test; // Make a copy of our test Image
    // gray_avg.grayscale_avg(); // then grayscale it
    // gray_avg.write("gray_avg.png"); //  write that to a file

    // Image gray_lum = test; // Make a copy of our test Image
    // gray_lum.grayscale_avg(); // then grayscale it
    // gray_lum.write("gray_lum.png"); //  write that to a file

    // test.colorMask(0,0,1); // Since blue is 1 it will make the test Image blue
    // test.write("blue.png"); 

    test.encodeMessage("Created by Sahil Shembekar");
    test.write("secret_message.png");

    /* Initialize all elements to 0, that way whatever string we get is already null terminated 
    and we can use printf to print it easily
    */
    char buffer[4103] = {0}; // char array as our buffer
    size_t len = 0;
    test.decodeMessage(buffer, &len);

    // printout our message after we decode it // Verification
    printf("Message: %s (%zu)\n", buffer, len); 
    return 0;
}
