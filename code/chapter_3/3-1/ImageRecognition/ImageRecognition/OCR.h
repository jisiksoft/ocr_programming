/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 1, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: OCR (Optical Character Recognition)        //
/////////////////////////////////////////////////////////////

#pragma once

//---------------------------------------------------------------------------------------------------
#define MAX_COUNT_STANDARD			80
#define MAX_COUNT_DATA				2000
#define RANGE_OF_COLOR_TO_CHECK		100

#define RATE_START_FOR_PARSING		0.1
#define RATE_END_FOR_PARSING		0.3

//---------------------------------------------------------------------------------------------------
struct Point {
	int x;
	int y;
};

struct Rect {
	Point start;
	Point end;
};

struct Letter {
	char value;
	unsigned int image[48];
};

struct Standard {
	int count;
	Letter letter[MAX_COUNT_STANDARD];
};

struct Data {
	bool isFixed;
	Letter letter;
	Rect rect;
};

struct AllData {
	int count;
	Data data[MAX_COUNT_DATA];
};

struct AllDataToPrint {
	int count;
	Data *data[MAX_COUNT_DATA + MAX_COUNT_DATA];
};

//---------------------------------------------------------------------------------------------------
class COCR {

private:
	CImage *image;
	Standard standard;
	AllData allData;
	Data *data;

	int colorToCheck;

public:
	COCR(void);
	~COCR(void);

	void RunOCR(CImage *image, CString outFileName, int colorLetter);
	void GetStandardImageDataFromBinaryFile(char * fileName);
	void ParsingStepFirst();
	void ParsingStepSecond(int yTop, int yBottom);
	void ParsingStepThird(Rect *rect);
	void ParsingStepThird2(Rect *rect);
	void MakeImageData();
	void FindLetterValue();
	void StoreLetterToTextFile(CString outFileName);
};
