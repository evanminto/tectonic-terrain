#include <string>

// Array of texture indices
static unsigned int texture[1];

// Holds data and dimensions for loaded BMP file
struct BitMapFile
{
  int sizeX;
  int sizeY;
  unsigned char *data;
};

// Load the data from the specified BMP file
BitMapFile *getBMPData(std::string filename);

