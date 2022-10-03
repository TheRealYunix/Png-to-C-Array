#include <iostream>
#include <fstream>	//for Writing the C Image file

#include <stb/stb_image.hpp>
#define STB_IMAGE_IMPLEMENTATION

static class IMG{

private:
	//Image RGB Data's
	unsigned char* pixelOffset;
	unsigned char r;	unsigned char g;	unsigned char b;

	//Other stuff
	int ImageWidth, ImageHeight, ImageChannels;

	//CImage
	unsigned char *data;	//for RGB values, and stbi loading Image
	std::ofstream CFile;

public:
	uint16_t present;
	bool IsHexadecimal = false;
	
	//Getting information returns a Integer
	int getWidth();
	int getHeight();
	int getChannels();

	IMG(const char *filename);
	IMG();
	~IMG();
	
	void WriteCImage(const char *filename, std::string name);
};
