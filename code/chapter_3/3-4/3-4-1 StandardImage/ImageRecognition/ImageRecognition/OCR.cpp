#include "StdAfx.h"
#include "OCR.h"

//---------------------------------------------------------------------------------------------------
COCR::COCR(void)
{
}

//---------------------------------------------------------------------------------------------------
COCR::~COCR(void)
{
}

//---------------------------------------------------------------------------------------------------
// Standard Image Data를 만든다.
void COCR::CreateStandard(CImage *newImage) {

	image = newImage;
	colorToCheck = 50;

	ParsingStepFirst();
	MakeImageData();

	int i = 0;
	for (i=0; i<26; i++)
		allData.data[i].letter.value = 'a' + i;
	for (i=0; i<26; i++)
		allData.data[i+26].letter.value = 'A' + i;
	for (i=0; i<10; i++)
		allData.data[i+52].letter.value = '0' + i;

	allData.data[62].letter.value = 0x3F;//'?';
	allData.data[63].letter.value = 0x21;//'!';
	allData.data[64].letter.value = 0x2C;//',';
	allData.data[65].letter.value = 0x27;//''';
	allData.data[66].letter.value = 0x3B;//';';
	allData.data[67].letter.value = 0x3A;//':';
	
	PrintEveryImageDataInTextFile("standard.txt");
	//GetStandardImageDataFromTextFile("standard.txt");

	PrintEveryImageDataInBinaryFile("standard.bin");
	GetStandardImageDataFromBinaryFile("standard.bin");

	PrintAllStandardImageToTextFile("standardout.txt");
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
	
	int xStart = (int)(xMax * 0.4);
	int xEnd = (int)(xMax * 0.6);

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

//---------------------------------------------------------------------------------------------------
// Image Parsing 완료 후, 모든 문자 이미지를 사진 file로 출력
// 테스트를 위하여 구현된 함수
void COCR::PrintImageToFile(int fileNo, Rect *rect) {

	CString strName;
	strName.Format(TEXT("./image%d.jpg"), fileNo);

	CImage newImage;
	int width, height;

	width = rect->end.x - rect->start.x + 1;
	height = rect->end.y - rect->start.y + 1;

	if (width <=0 || height <= 0)
	{
		AfxMessageBox(_T("Error: 사진의 크기가 0보다 작다."));
		return;
	}

	newImage.Create(width, height, 32);

	for (int i=0; i<width; i++)
		for(int j=0; j<height; j++)
			newImage.SetPixel(i,j, image->GetPixel(rect->start.x+i, rect->start.y+j));

	newImage.Save(strName, Gdiplus::ImageFormatJPEG);
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

//---------------------------------------------------------------------------------------------------
// Image Data 완료 후, 모든 문자 이미지 데이터를 Text File에 저장
// 테스트를 위하여 구현된 함수
void COCR::PrintImageDataToFile(int fileNo, Letter *letter) {

	CString strName;
	strName.Format(TEXT("./ImageData%d.txt"), fileNo);

	FILE *fp;
	fp = fopen(LPSTR(LPCTSTR(strName)), "wt");

	int i, j;
	unsigned int buffer;

	for (i=0; i<48; i++) {

		buffer = letter->image[i];

		for (int j=0; j<32; j++) {

			if (buffer & 0x80000000)
				fputc('1', fp);
			else
				fputc('0', fp);

			buffer <<= 1;
		}
		fputc('\n', fp);
	}

	fclose(fp);
}

//---------------------------------------------------------------------------------------------------
void COCR::PrintEveryImageDataInTextFile(char * fileName) {

	FILE *fp;
	fp = fopen(fileName, "wt");

	int i, x, y;
	unsigned int buffer;
	Letter *letter;

	fprintf(fp, "%d\n", allData.count);

	for (i=0; i<allData.count; i++) {

		letter = &allData.data[i].letter;

		fprintf(fp, "\n%c\n", letter->value);

		for (y=0; y<48; y++) {

			buffer = letter->image[y];

			for (x=0; x<32; x++) {

				if (buffer & 0x80000000)
					fputc('1', fp);
				else
					fputc('0', fp);

				buffer <<= 1;
			}
			fputc('\n', fp);
		}
	}

	fclose(fp);
}

//---------------------------------------------------------------------------------------------------
void COCR::GetStandardImageDataFromTextFile(char * fileName) {
	
	FILE *fp;
	fp = fopen(fileName, "rt");

	int i, x, y, temp;
	char ch;
	unsigned int buffer;
	Letter *letter;

	fscanf(fp, "%d\n", &standard.count);

	for (i=0; i<standard.count; i++) {

		letter = &standard.letter[i];

		fscanf(fp, "\n%c\n", &letter->value);

		for (y=0; y<48; y++) {

			letter->image[y] = 0x00000000;

			for (x=0; x<32; x++) {
				if ((ch = fgetc(fp)) == '1') {
					buffer = 0x80000000;
					buffer >>= x;
					letter->image[y] |= buffer;
				}
			}
			ch = fgetc(fp);
		}
	}

	fclose(fp);
}

//---------------------------------------------------------------------------------------------------
void COCR::PrintEveryImageDataInBinaryFile(char * fileName) {
	
	FILE *fp;
	fp = fopen(fileName, "wb");

	int i, x, y;
	Letter *letter;

	fprintf(fp, "%d\n", allData.count);

	for (i=0; i<allData.count; i++) {

		letter = &allData.data[i].letter;

		fprintf(fp, "%c\n", letter->value);

		for (y=0; y<48; y++)
			fprintf(fp, "%d\n", letter->image[y]);
	}

	fclose(fp);
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
void COCR::PrintAllStandardImageToTextFile(char * fileName) {
	
	FILE *fp;
	fp = fopen(fileName, "wt");

	int i, x, y;
	unsigned int buffer;
	Letter *letter;

	fprintf(fp, "%d\n", standard.count);

	for (i=0; i<standard.count; i++) {

		letter = &standard.letter[i];

		fprintf(fp, "\n%c\n", letter->value);

		for (y=0; y<48; y++) {

			buffer = letter->image[y];

			for (int x=0; x<32; x++) {

				if (buffer & 0x80000000)
					fputc('1', fp);
				else
					fputc('0', fp);

				buffer <<= 1;
			}
			fputc('\n', fp);
		}
	}

	fclose(fp);
}

