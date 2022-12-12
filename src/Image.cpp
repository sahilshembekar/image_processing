#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)
#include "Image.h"

#include "stb_image.h"
#include "stb_image_write.h"

Image::Image(const char* filename) {
// Constrcutor

  if (read(filename)) {
    printf("Read %s\n", filename);
    size = w * h * channels;
  } else {
    printf("Failed to read %s\n", filename);
  }
}

Image::Image(int w, int h, int channels)
    : w(w),
      h(h),
      channels(channels){  // Using initializer list
// Blank black Image

  size = w * h * channels;
  data = new uint8_t[size];
}

Image::Image(const Image& img)
    : Image(img.w, img.h, img.channels)  { // Calling the above constuctor here
// Image::Image(const Image& img) : w(img.w), h(img.h), channels(img.channels)
// // Calling the above constuctor here
// Ability to coopy Images, hence copy constructor
  memcpy(data, img.data, size);  // copy over the data from other Image here //
                                 // can also be img.size
}
Image::~Image() {
  stbi_image_free(
      data);  // deletes the data; basically the Image gets destructed
}

bool Image::read(const char* filename) {
// Read in from a file // will be called by the constructor
  data = stbi_load(filename, &w, &h, &channels,
                   0);  // Check explanation on line 103 of stb_Image.h file
  return data != NULL;
}

bool Image::write(const char* filename) {
// Ability to write to some location of a file
  // 5 types based on extension type //Check line 31 of stb_Image_write.h
  ImageType type = getFileType(filename);
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

ImageType Image::getFileType(const char* filename) {
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

Image& Image::grayscale_avg() {
    // r+g+b/3 -> then take this average and set each channel back to that value --> creating a gray pixel
    if(channels < 3 ) { // check if we have rgb i.e. 3 channels
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

Image& Image::grayscale_lum() {
    // same as grayscale_avg but here we take a weighted average --> preserves human perceved luminence of the Image
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

Image& Image::colorMask(float r, float g, float b) {
    if (channels < 3) {
        printf("\e[31m[ERROR] Color mask requires at least 3 channels, but this Image has %d channels\e[0m\n", channels); 
    }
    else {
        for (int i = 0;i < size; i+=channels) {
            data[i] *= r;
            data[i+1] *= g;
            data[i+2] *= b;
        }
    }

    return *this;
}

Image& Image::encodeMessage(const char* message) {
  // check how log our message is to tell the decode function how many bytes to read
  // Previously --> uint32_t  len = strlen(message) * sizeof(char); //first 4 bytes are length of the message
  
  uint32_t  len = strlen(message) * 8 ;
  //printf("LENGTH: %d\n", len);
  
  // If the message is too long to be encoded

  if (len + STEG_HEADER_SIZE > size) {
    printf ("\e[31m[ERROR] This message is too large (%lu bits / %zu bits)\e[0m\n", len+STEG_HEADER_SIZE, size);  
    return *this;
  }

  for(uint8_t i = 0; i <  STEG_HEADER_SIZE; ++i) {
    /*put this length value in our image data
    set the least significant bit of every byte in our image data to the corresponding bit in the message that we encoding 
    loop through our size first //defined beforehand
    */
    
    //first we need to extract a bit from length and put it into the least significant bit of data
    
    data[i] &= 0xFE; 
    data[i] |= (len >> (STEG_HEADER_SIZE - 1 -i)) & 1UL; 
    
    /* taking a length and shifting it over i bits, 
    so that we get ith bit of length and 
    then we are ANDing that with 1 to clear all of the upper bits, 
    so that we just have 1 or 0
    and then invert it to put the most significant bit 1st
    first we also need to clear the LSB hece we do 0xFE
    */ 
  }

  // To put our message into our image  
  // unint32 because messages can be longer that 255 characters   
  for(uint32_t i = 0; i <  len; ++i) {
    data[i + STEG_HEADER_SIZE] &= 0xFE; 
    data[i + STEG_HEADER_SIZE] |= (message[i/8] >> ((len-1-i)%8)) & 1; 
    /* instead of pulling from one singular value, we are pulling the info from an array  
    and then we need to take the right bit from this byte, and to do that we are gonna shift over, which is gonna
    give use the remainder 
    and then the entire thing 1 to clear all the upper bits 
    and we also need to put the MSB first
    */
  }
  
  return *this;
}

Image& Image::decodeMessage(char* buffer, size_t* messageLength) {
  uint32_t len = 0;
  for(uint8_t i = 0; i <  STEG_HEADER_SIZE; ++i) {
    // same as above

    /* 
    We want the LSB of the ith byte of our image
    Move these values into len
    and put the MSB first. Slowly shift the bit upwards through length 

    Start with the MSB and everytime we start the OR operation we also shift everything to the left 1 bit
    That way we are loading our value in, 1 bit at a time
    */ 
    len = (len << 1) | (data[i] & 1);  
  }
  
  //printf("LENGTH: %d\n", len);
  *messageLength = len / 8;
  // Should return the messagelength in bytes


  for(uint32_t i = 0; i <  len; ++i) {

    buffer[i/8] = (buffer[i/8] << 1) | (data[i+STEG_HEADER_SIZE] & 1);
 
    /*
    Put data into the buffer  
    First get which byte in the buffer we want to set
    Like above shift everything to the left everytime 
    and then we want to get the LSB of our image data 
    not i but i/8 because length is in bits   
    */  
  }


  return *this;
}

Image& Image::diffmap(Image& img) {
  // First we see how much are we gonna compare between the 2 images
  int compare_width = fmin(w, img.w);
  int compare_height = fmin(h, img.h);
  int compare_channels = fmin(channels, img.channels);

  for(uint32_t i=0; i<compare_height; ++i) {
    for(uint32_t j=0; j<compare_width; ++j) {
      for(uint32_t k=0; k<compare_channels; ++k) {
        /* 
        first get the pixel and then specific channel of that pixel can be accessed by doing channels+k
        Set that to ABS of the difference between data already at the pixel and data of the same pixel in the other image
        and then bound that value between 0 to 255 using the function BYTE_BOUND --> we make a macro
        */
       data[(i*w+j)*channels+k] = BYTE_BOUND(abs(data[(i*w+j)*channels+k] - img.data[(i*img.w+j)*img.channels+k]));
      }
    }
  }
  return *this;  
}

Image& Image::diffmap_scale(Image& img, u_int8_t scl) {
  // First we see how much are we gonna compare between the 2 images
  int compare_width = fmin(w, img.w);
  int compare_height = fmin(h, img.h);
  int compare_channels = fmin(channels, img.channels);
  // Rest is same as above , but it has the option to scale up the difference using the scale factor
  // This is for easy of viewing if the difference is really small, we can scale it up
  uint8_t largest = 0;
  for(uint32_t i=0; i<compare_height; ++i) {
    for(uint32_t j=0; j<compare_width; ++j) {
      for(uint32_t k=0; k<compare_channels; ++k) {
        /* 
        first get the pixel and then specific channel of that pixel can be accessed by doing channels+k
        Set that to ABS of the difference between data already at the pixel and data of the same pixel in the other image
        and then bound that value between 0 to 255 using the function BYTE_BOUND --> we make a macro
        */
       data[(i*w+j)*channels+k] = BYTE_BOUND(abs(data[(i*w+j)*channels+k] - img.data[(i*img.w+j)*img.channels+k]));
       largest = fmax(largest, data[(i*w+j)*channels+k]);     
      }
    }
  }
  scl = 255/ fmax(1, fmax(scl, largest));
  /* fmax between fmax and 1 ensures that -ve value or 0 never become the denominator
  Takes a range between 0 and scl or 0 and largest and rescales to be between 0 and 255  
  If largest is greater than 255 it brought down by scl
  */
  
  for (int i=0; i<size; ++i) {
    data[i] *= scl;
  } 
  return *this;  
}