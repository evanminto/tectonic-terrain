#include <string>

// Holds data and dimensions for loaded BMP file
struct BitMapFile
{
  int sizeX;
  int sizeY;
  unsigned char *data;
};

// Load the data from the specified BMP file
BitMapFile *getBMPData(std::string filename);

