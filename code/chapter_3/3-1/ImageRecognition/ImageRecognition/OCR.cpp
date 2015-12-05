/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Jan 1, 2015                                //
// eMail      : kimsehoon@hotmail.com                      //
// Description: OCR (Optical Character Recognition)        //
/////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "OCR.h"

//---------------------------------------------------------------------------------------------------
COCR::COCR(void)
{
	GetStandardImageDataFromBinaryFile("standard.bin");
}

//---------------------------------------------------------------------------------------------------
COCR::~COCR(void)
{
}

//---------------------------------------------------------------------------------------------------
void COCR::RunOCR(CImage *newImage, CString outFileName, int colorLetter) {

	image = newImage;
	colorToCheck = colorLetter;

	for (int i=0; i<MAX_COUNT_DATA; i++)
		allData.data[i].isFixed = false;

	ParsingStepFirst();
	MakeImageData();

	FindLetterValue();

	StoreLetterToTextFile(outFileName);
}

//---------------------------------------------------------------------------------------------------
void COCR::GetStandardImageDataFromBinaryFile(char * fileName) {
	
	FILE *fp;
	fp = fopen(fileName, "rb");

	int i, x, y;
	Letter *letter;

	fscanf(fp, "%d\n", &standard.count);

	for (i=0; i<standard.count; i++) {

		letter = &standard.letter[i];

		fscanf(fp, "%c\n", &letter->value);

		for (y=0; y<48; y++)
			fscanf(fp, "%d\n", &letter->image[y]);
	}

	fclose(fp);
}

//---------------------------------------------------------------------------------------------------
// Image Parsing을 진행한다.
void COCR::ParsingStepFirst() {

	allData.count = 0;
	data = &allData.data[0];

	int xMax = image->GetWidth();
	int yMax = image->GetHeight();

	int x, y;
	COLORREF rgb;
	int yTop, yBottom;
	bool isLetterLine;
	bool flagPrevLine;							
	
	int xStart = (int)(xMax * RATE_START_FOR_PARSING);
	int xEnd = (int)(xMax * RATE_END_FOR_PARSING);

	flagPrevLine = false;

	//----------- get letter lines and parse letters on each line ----------- 
	for (y=0; y<yMax; y++) {

		isLetterLine = false;

		for (x=xStart; x<xEnd; x++)	{

			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				isLetterLine = true;
				break;
			}
		}

		if (isLetterLine) {
			if (!flagPrevLine) {
				yTop = y;				//set the top of a letter line
			}
		}
		else {
			if (flagPrevLine) {
				yBottom = y-1;

				ParsingStepSecond(yTop, yBottom);
			}
		}

		flagPrevLine = isLetterLine;
	}
}

//-------------------------------------------------------------------------------------------------
void COCR::ParsingStepSecond(int yTop, int yBottom) {

	int xMax = image->GetWidth();

	int x, y;
	COLORREF rgb;
	bool isLetterLine;
	bool flagPrevLine;	

	flagPrevLine = false;

	for (x=0 ; x<xMax; x++) {

		isLetterLine = false;

		for (y=yTop; y<=yBottom; y++) {
			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {

				isLetterLine = true;
				break;
			}
		}

		if (isLetterLine) {
			if (!flagPrevLine) {
				data->rect.start.x = x;
				data->rect.start.y = yTop;
			}
		}
		else {
			if (flagPrevLine) {

				data->rect.end.x = x-1;
				data->rect.end.y = yBottom;

				ParsingStepThird(&data->rect);
				ParsingStepThird2(&data->rect);
				ParsingStepThird(&data->rect);

				allData.count += 1;
				data = &allData.data[allData.count];
			}
		}

		flagPrevLine = isLetterLine;
	}
}

//-------------------------------------------------------------------------------------------------
void COCR::ParsingStepThird(Rect *rect) {

	int x, y;
	COLORREF rgb;
	bool isLetterLine;

	//------------------------------ Letter Top -------------------------------
	for (y=rect->start.y; ; y--) {

		isLetterLine = false;

		for (x=rect->start.x; x<=rect->end.x; x++) {

			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				rect->start.y = y;
				isLetterLine = true;
				break;
			}
		}
		if (!isLetterLine)
			break;
	}
	for (y=rect->start.y; ; y++)	{

		isLetterLine = false;

		for (x=rect->start.x; x<=rect->end.x; x++) {

			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				rect->start.y = y;
				isLetterLine = true;
				break;
			}
		}
		if (isLetterLine)
			break;
	}
	//------------------------------ Letter Bottom -------------------------------
	for (y=rect->end.y; ; y++) {

		isLetterLine = false;

		for (x=rect->start.x; x<=rect->end.x; x++) {
			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				rect->end.y = y;
				isLetterLine = true;
				break;
			}
		}
		if (!isLetterLine)
			break;
	}
	for (y=rect->end.y;; y--) {

		isLetterLine = false;

		for (x=rect->start.x; x<=rect->end.x; x++) {
			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				rect->end.y = y;
				isLetterLine = true;
				break;
			}
		}
		if (isLetterLine)
			break;
	}
}

//-------------------------------------------------------------------------------------------------
void COCR::ParsingStepThird2(Rect *rect) {

	int x, y;
	COLORREF rgb;
	bool isLetterLine;

	//------------------------------ Letter Left -------------------------------
	for (x=rect->start.x; ; x--) {

		isLetterLine = false;

		for (y=rect->start.y; y<=rect->end.y; y++) {

			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				rect->start.x = x;
				isLetterLine = true;
				break;
			}
		}
		if (!isLetterLine)
			break;
	}
	for (x=rect->start.x; ; x++)	{

		isLetterLine = false;

		for (y=rect->start.y; y<=rect->end.y; y++) {

			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				rect->start.x = x;
				isLetterLine = true;
				break;
			}
		}
		if (isLetterLine)
			break;
	}
	//------------------------------ Letter Right -------------------------------
	for (x=rect->end.x; ; x++) {

		isLetterLine = false;

		for (y=rect->start.y; y<=rect->end.y; y++) {
			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				rect->end.x = x;
				isLetterLine = true;
				break;
			}
		}
		if (!isLetterLine)
			break;
	}
	for (x=rect->end.x;; x--) {

		isLetterLine = false;

		for (y=rect->start.y; y<=rect->end.y; y++) {
			rgb = image->GetPixel(x,y);

			if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				rect->end.x = x;
				isLetterLine = true;
				break;
			}
		}
		if (isLetterLine)
			break;
	}
}

//-------------------------------------------------------------------------------------------------
void COCR::MakeImageData() {

	int i, j;

	for (i=0; i<allData.count; i++) {

		Data *data = &allData.data[i];
		Letter *letter = &data->letter;
		Rect *rect = &data->rect;

		float xRate, yRate;
		int x, y;
		unsigned int buffer;
		COLORREF rgb;

		for (j=0; j<48; j++)
			letter->image[j] = 0x00000000;

		xRate = (float)(rect->end.x - rect->start.x) / (32 - 1);
		yRate = (float)(rect->end.y - rect->start.y) / (48 - 1);

		for (y=0; y<48; y++) {
			for (x=0; x<32; x++) {

				rgb = image->GetPixel((int)(rect->start.x + (x * xRate)), (int)(rect->start.y + (y * yRate)));

				if (GetRValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
					buffer = 0x80000000;
					buffer >>= x;
					letter->image[y] |= buffer;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
void COCR::FindLetterValue() {

	int count, maxCount;
	unsigned int buffer, bit;
	int i, j, x, y;


	for (i=0; i<allData.count; i++) {

		Data *data = &allData.data[i];
		Letter *letter = &data->letter;
		Rect *rect = &data->rect;

		maxCount = 0;

		for (j=0; j<standard.count; j++) {

			count = 0;

			for (y=0; y<48; y++) {

				buffer = letter->image[y] ^ standard.letter[j].image[y];

				for (x=0; x<32; x++) {

					bit = 0x80000000;
					bit >>= x;
					bit = bit & buffer;

					if (!bit)
						count += 1;
				}
			}

			if (count > maxCount) {
				letter->value = standard.letter[j].value; 
				maxCount = count;
			}
		}

	}
}

//---------------------------------------------------------------------------------------------------
void COCR::StoreLetterToTextFile(CString outFileName) {

	FILE *fp;
	fp = fopen((char*)((LPCSTR)(outFileName)), "wt");

	for (int i=0; i<allData.count; i++)
		fputc(allData.data[i].letter.value, fp);

	fclose(fp);
}
