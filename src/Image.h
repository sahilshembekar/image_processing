#include <stdint.h> //Since we are using unint8_t, size_t, etc
#include <cstdio> // Since we are using printf

#define STEG_HEADER_SIZE sizeof(uint32_t) * 8 // set to 32
enum ImageType {
   PNG, JPG, BMP, TGA 
};

struct Image {
   uint8_t* data = NULL; // This is 1 byte of data
   size_t size = 0; // To keep track of everything
   int w;
   int h;
   int channels; // How many color values per pixel RGB =3 and so on 

   Image(const char* filename); // Constrcutor
   Image(int w, int h, int channels); // B lank black Image
   Image(const Image& img); // Ability to coopy Images, hence copy constructor
   ~Image(); 

   bool read(const char* filename); // Read in from a file // will be called by the constructor
   bool write(const char* filename); // Ability to write to some location of a file 

   ImageType getFileType(const char* filename);  // Function that returns an Image type

   Image& grayscale_avg();
   Image& grayscale_lum();

   Image& colorMask(float r, float g, float b);

   Image& encodeMessage(const char* message);
   Image& decodeMessage(char* buffer, size_t* messageLength);

   Image& diffmap(Image& img);
   Image& diffmap_scale(Image& img, uint8_t scl = 0); // if scl is not given then default it to 0 

 };
 