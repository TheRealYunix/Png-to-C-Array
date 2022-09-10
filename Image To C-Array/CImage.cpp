#include "CImage.hpp"
#include <stb/stb_image.h>
#include <string>
IMG::IMG() {
	std::cout << "Konstruktor Called!";
}

int IMG::getWidth()
{
	return ImageWidth;
}

int IMG::getHeight()
{
	return ImageHeight;
}

int IMG::getChannels()
{
	return ImageChannels;
}

IMG::IMG(const char *filename){
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(filename, &ImageWidth, &ImageHeight, &ImageChannels, 0);
}

IMG::~IMG(){
	//Clear the last image
	//Closing the file

}

void IMG::WriteCImage(const char *filename, std::string name)
{
	CFile.open(filename);
	
	size_t lastindex = name.find_last_of(".");
	std::string rawname = name.substr(0, lastindex);

	switch (present)
	{
	case 0:
		CFile << "static unsigned int ImageWidth = " <<
			ImageWidth << ";\n" <<
			"static unsigned int ImageHeight = " <<
			ImageHeight << ";\n" <<
			"static const unsigned char "<< rawname <<"_array[" << ImageHeight * ImageWidth * ImageChannels << "] = \n{";
		break;
	case 1:
		CFile << "static const unsigned char " << rawname << "_array[" << ImageHeight * ImageWidth * ImageChannels << "] = \n{";
		break;
	case 2:
		CFile << "static const unsigned char " << rawname << "_array[" << ImageHeight * ImageWidth * ImageChannels << "] = \n{";
		break;
	}


	unsigned bytePerPixel = ImageChannels;
	//Writing the RGB values of the Image

	if (present == 2) {

		for (int x = 0; x < ImageWidth; x++) {
			for (int y = 0; y < ImageHeight; y++) {

				pixelOffset = data + (x + y * ImageWidth) * bytePerPixel;
				r = pixelOffset[0];		g = pixelOffset[1];		b = pixelOffset[2];




				if (IsHexadecimal)
					CFile << std::hex << "0x" << static_cast<int>(r) << " , " << "0x" << static_cast<int>(g) << " , " << "0x" << static_cast<int>(b) << " , " << std::endl;
				else
					CFile << static_cast<int>(r) << " , " << static_cast<int>(g) << " , " << static_cast<int>(b) << " , " << std::endl;
			}

		}
		
	}

else {
		for (int x = 0; x < ImageWidth; x++) {
			for (int y = 0; y < ImageHeight; y++) {

				pixelOffset = data + (x + y * ImageWidth) * bytePerPixel;
				r = pixelOffset[0];		g = pixelOffset[1];		b = pixelOffset[2];

				


				if (IsHexadecimal)
					CFile << std::hex << "0x" << static_cast<int>(b) << " , " << "0x" << static_cast<int>(g) << " , " << "0x" << static_cast<int>(r) << " , " << std::endl;
				else
					CFile << static_cast<int>(b) << " , " << static_cast<int>(g) << " , " << static_cast<int>(r) << " , " << std::endl;
			}

		}
	}
		//End of File
		CFile << "};\n\n\nconst int " << rawname <<"_array_size"<<" = " << std::dec << ImageHeight * ImageWidth * ImageChannels << ";\n";
		CFile << "unsigned char *" << rawname << " = (unsigned char *)" << rawname << "_array;\n";
		CFile.close();
}

