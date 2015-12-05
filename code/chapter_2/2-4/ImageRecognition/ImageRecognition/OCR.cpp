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
// Standard Image Data�� �����.
void COCR::CreateStandard(CImage *newImage) {

	image = newImage;
	colorToCheck = 50;

	ParsingStepFirst();
	MakeImageData();

	for (int i=0; i<allData.count; i++)
		PrintImageDataToFile(i, &allData.data[i].letter);
	
	//for (int i=0; i<allData.count; i++)
	//	PrintImageToFile(i, &allData.data[i].rect);
}

//---------------------------------------------------------------------------------------------------
// Image Parsing�� �����Ѵ�.
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
// Image Parsing �Ϸ� ��, ��� ���� �̹����� ���� file�� ���
// �׽�Ʈ�� ���Ͽ� ������ �Լ�
void COCR::PrintImageToFile(int fileNo, Rect *rect) {

	CString strName;
	strName.Format(TEXT("./image%d.jpg"), fileNo);

	CImage newImage;
	int width, height;

	width = rect->end.x - rect->start.x + 1;
	height = rect->end.y - rect->start.y + 1;

	if (width <=0 || height <= 0)
	{
		AfxMessageBox(_T("Error: ������ ũ�Ⱑ 0���� �۴�."));
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
// Image Data �Ϸ� ��, ��� ���� �̹��� �����͸� Text File�� ����
// �׽�Ʈ�� ���Ͽ� ������ �Լ�
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
