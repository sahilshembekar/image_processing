#include "image.h"

int main(int argc, char** argv) {
  // Pass the test image here
  image test("test.png");
  test.write("new.jpg");
  image copy = test;  // to check if out copy constructor works

  for (int i = 0; i < copy.w * copy.channels; ++i) {
    // set top row of pixels in that image to white
    copy.data[i] = 255;
  }
  copy.write("copy.png");

  image blank(100, 100, 3);  // Test our blank image
  blank.write("blank.jpg");  // write it to blank.jpg

  image gray_avg = test; // Make a copy of our test image
  gray_avg.grayscale_avg(); // then grayscale it
  gray_avg.write("gray_avg.png"); //  write that to a file

  image gray_lum = test; // Make a copy of our test image
  gray_lum.grayscale_avg(); // then grayscale it
  gray_lum.write("gray_lum.png"); //  write that to a file

  return 0;
}