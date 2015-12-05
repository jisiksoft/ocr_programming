#pragma once

#define WIDTH_COMPRESSED_LETTER	32					//Compressed Letter Size 32x48 bits
#define HEIGHT_COMPRESSED_LETTER	48

//----------------- structure to store images -----------------
struct Letter
{
	char ch;																		//character of matched letter image
	unsigned int bytes[HEIGHT_COMPRESSED_LETTER];		//probability of matched pixels
};

struct StandardData
{
	Letter* letters;						//standard letters having image to compare
};

//----------------------------------------------------------------
class CImageRecognition
{
public:
	StandardData* standardData;			//standard letters having standard compressed image

	CImageRecognition(void);
	~CImageRecognition(void);
	void implementXOR(char charFirst, char charSecond);
	StandardData* LoadStandardData();
	Letter* LoadStandardLetters();
	void PrintLetterToFile(Letter* letters, int countToPrint);
};
