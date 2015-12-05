/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Feb 28, 2015                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Recognize Vehicle Plate                    //
/////////////////////////////////////////////////////////////

#pragma once

//---------------------------------------------------------------------------------------------------
#define MAX_COUNT_STANDARD			80
#define MAX_COUNT_DATA				7
#define RANGE_OF_COLOR_TO_CHECK		80

#define RATE_START_FOR_PARSING		0.3
#define RATE_END_FOR_PARSING		0.7

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
	CString value;
	unsigned int image[48];
};

struct StandardNumber {
	int count;
	Letter letter[10];
};

struct StandardLetter {
	int count;
	Letter letter[MAX_COUNT_STANDARD];
};

struct Data {
	Letter letter;
	Rect rect;
};

struct AllData {
	int count;
	Data data[MAX_COUNT_DATA];
};

//---------------------------------------------------------------------------------------------------
class COCR {

private:
	CImage *image;
	StandardNumber standardNumber;
	StandardLetter standardLetter;
	AllData allData;
	Data *data;

	int colorToCheck;

public:
	COCR(void);
	~COCR(void);

	void RunOCR(CImage *newImage);
	CImage * ResizeInBlackWhiteImage(CImage *newImage);
	void ParsingStepFirst();
	void ParsingStepSecond(int yTop, int yBottom);
	void ParsingStepThird(Rect *rect);
	void ParsingStepThird2(Rect *rect);
	void MakeImageData();
	void MakeLetterData(Rect *rect);
	void FindValue();
	void FindNumberValue(Data *aData);
	void FindLetterValue(Data *aData);
	
	bool IsLetterRect(Rect *rect);
	
	void PrintImageToFile(int fileNo, Rect *rect);
	void PrintImageDataToFile(int fileNo, Letter *letter);
	void PrintEveryImageDataInTextFile(char * fileName);
	void GetStandardImageDataFromTextFile(char * fileName);
	void GetStandardImageDataFromBinaryFile(char * fileName);
	void PrintEveryImageDataInBinaryFile(char * fileName);
	void PrintAllStandardImageToTextFile(char * fileName);
	
	void ShowResultMessage();
};