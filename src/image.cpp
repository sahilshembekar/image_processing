#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"

#include "stb_image.h"
#include "stb_image_write.h"

image::image(const char* filename) {
// Constrcutor

  if (read(filename)) {
    printf("Read %s\n", filename);
    size = w * h * channels;
  } else {
    printf("Failed to read %s\n", filename);
  }
}

image::image(int w, int h, int channels)
    : w(w),
      h(h),
      channels(channels){  // Using initializer list
// Blank black image

  size = w * h * channels;
  data = new uint8_t[size];
}

image::image(const image& img)
    : image(img.w, img.h, img.channels)  { // Calling the above constuctor here
// image::image(const image& img) : w(img.w), h(img.h), channels(img.channels)
// // Calling the above constuctor here
// Ability to coopy images, hence copy constructor
  memcpy(data, img.data, size);  // copy over the data from other image here //
                                 // can also be img.size
}
image::~image() {
  stbi_image_free(
      data);  // deletes the data; basically the image gets destructed
}

bool image::read(const char* filename) {
// Read in from a file // will be called by the constructor
  data = stbi_load(filename, &w, &h, &channels,
                   0);  // Check explanation on line 103 of stb_image.h file
  return data != NULL;
}

bool image::write(const char* filename) {
// Ability to write to some location of a file
  // 5 types based on extension type //Check line 31 of stb_image_write.h
  imageType type = getFileType(filename);
  int success;  // since the xyz function returns an int value which is store in
                // sucess and check if
  switch (type) {
    case PNG:
      success = stbi_write_png(filename, w, h, channels, data,
                               w * channels);  // line 33
      break;
    case BMP:
      success = stbi_write_bmp(filename, w, h, channels, data);  // line 33
      break;
    case JPG:
      success = stbi_write_jpg(filename, w, h, channels, data, 100);  // line 33
      break;
    case TGA:
      success = stbi_write_tga(filename, w, h, channels, data);  // line 33
      break;
  }
  return success != 0;
}

imageType image::getFileType(const char* filename) {
  const char* ext = strrchr(filename, '.');  // to get the extension
  if (ext != nullptr) {                      // to see if it is a valid filename
    if (strcmp(ext, ".png") == 0) {
      return PNG;
    } else if (strcmp(ext, ".jpg") == 0) {
      return JPG;
    } else if (strcmp(ext, ".bmp") == 0) {
      return BMP;
    } else if (strcmp(ext, ".tga") == 0) {
      return TGA; 
    }
  }
  return PNG;  // if there is an issue with extension just return the default as
               // PNG
}

image& image::grayscale_avg() {
    // r+g+b/3 -> then take this average and set each channel back to that value --> creating a gray pixel
    if(channels < 3 ) {
        printf("Image %p has less than 3 channels, it is asumed to be already grayscaled", this); 
    }
    else {
        for (int i = 0; i < size; i+=channels) {
            // (r+g+b)/3
            int gray = (data[i] + data[i+1] + data[i+2])/3;
            memset(data+i, gray, 3); // to set each of the channels to the same gray value  
        }
    }
    return *this;
}

image& image::grayscale_lum() {
    // same as grayscale_avg but here we take a weighted average --> preserves human perceved luminence of the image
    if(channels < 3 ) {
        printf("Image %p has less than 3 channels, it is asumed to be already grayscaled", this); 
    }
    else {
        for (int i = 0; i < size; i+=channels) {
            int gray = 0.2126*data[i] + 0.7152*data[i+1] + 0.0722*data[i+2]; // to ratio to match how the humans perceive different colors
            memset(data+i, gray, 3); // to set each of the channels to the same gray value  
        }
    }
    return *this;
}