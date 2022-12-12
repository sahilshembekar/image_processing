#include <stdint.h> //Since we are using unint8_t, size_t, etc
#include <cstdio> //Since we are using printf

enum imageType {
   PNG, JPG, BMP, TGA 
};

struct image
 {
    uint8_t* data = NULL; // This is 1 byte of data
    size_t size = 0; // To keep track of everything
    int w;
    int h;
    int channels; // How many color values per pixel RGB =3 and so on 

    image(const char* filename); // Constrcutor
    image(int w, int h, int channels); // B lank black image
    image(const image& img); // Ability to coopy images, hence copy constructor
    ~image(); 

    bool read(const char* filename); // Read in from a file // will be called by the constructor
    bool write(const char* filename); // Ability to write to some location of a file 

   imageType getFileType(const char* filename);  // Function that returns an image type
   
   image& grayscale_avg();
   image& grayscale_lum();

 };
 