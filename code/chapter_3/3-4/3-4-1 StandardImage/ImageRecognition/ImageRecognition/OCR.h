#pragma once

#define MAX_COUNT_STANDARD			80
#define MAX_COUNT_DATA				2000
#define RANGE_OF_COLOR_TO_CHECK		100

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
	Standard standard;
	AllData allData;
	Data *data;

	int colorToCheck;

public:
	COCR(void);
	~COCR(void);

	void CreateStandard(CImage *image);
	void ParsingStepFirst();
	void ParsingStepSecond(int yTop, int yBottom);
	void ParsingStepThird(Rect *rect);
	void ParsingStepThird2(Rect *rect);
	void PrintImageToFile(int fileNo, Rect *rect);
	void MakeImageData();
	void MakeLetterData(Rect *rect);
	void PrintImageDataToFile(int fileNo, Letter *letter);
	void PrintEveryImageDataInTextFile(char * fileName);
	void GetStandardImageDataFromTextFile(char * fileName);
	void PrintEveryImageDataInBinaryFile(char * fileName);
	void GetStandardImageDataFromBinaryFile(char * fileName);
	void PrintAllStandardImageToTextFile(char * fileName);
};
