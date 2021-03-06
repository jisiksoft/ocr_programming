/////////////////////////////////////////////////////////////
// Programmer : SEHOON KIM                                 //
// Date       : Feb 28, 2015                               //
// eMail      : kimsehoon@hotmail.com                      //
// Description: Recognize Vehicle Plate                    //
/////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "OCR.h"

//---------------------------------------------------------------------------------------------------
COCR::COCR(void) {
	colorToCheck = 0;
}

//---------------------------------------------------------------------------------------------------
COCR::~COCR(void) {
}

//---------------------------------------------------------------------------------------------------
void COCR::RunOCR(CImage *newImage) {

	image = ResizeInBlackWhiteImage(newImage);

	ParsingStepFirst();

	for (int i=0; i<MAX_COUNT_DATA; i++)
		PrintImageToFile(i, &allData.data[i].rect);
}

//---------------------------------------------------------------------------------------------------
CImage * COCR::ResizeInBlackWhiteImage(CImage *newImage) {

	static CImage image;

	COLORREF rgb;
	BYTE byte;

	int width = 400;
	int height = 300;

	float xRate = (float)(newImage->GetWidth()) / width;
	float yRate = (float)(newImage->GetHeight()) / height;

	if (image != NULL)
		image.Destroy();

	image.Create(width, height, 32);

	for (int i=0; i<width; i++) {
		for(int j=0; j<height; j++) {

			rgb = newImage->GetPixel((int)(xRate * i), (int)(yRate * j));

			byte = (GetRValue(rgb) * 30 + 
					GetGValue(rgb) * 59 +
					GetBValue(rgb) * 11) / 100;

			rgb = RGB(byte, byte, byte);

			image.SetPixel(i,j, rgb);
		}
	}
	
	image.Save("resizeBlackWhite.jpg", Gdiplus::ImageFormatJPEG);

	return &image;
}
	
//---------------------------------------------------------------------------------------------------
// Image Parsing을 진행한다.
void COCR::ParsingStepFirst() {

	int i, x, y, count;
	COLORREF rgb;
	int yTop, yBottom;
	bool isLetterLine;							
	
	allData.count = 0;
	data = &allData.data[0];
	for(i=0; i<MAX_COUNT_DATA; i++) {
		allData.data[i].letter.value = "\0";
	}

	int xMax = image->GetWidth();
	int yMax = image->GetHeight();
	int yMiddle = (int)(yMax / 2);

	int xStart = (int)(xMax * RATE_START_FOR_PARSING);
	int xEnd = (int)(xMax * RATE_END_FOR_PARSING);

	//----------- get letter lines and parse letters on each line ----------- 
	for (y=yMiddle; y>0; y--) {

		isLetterLine = false;
		count = 0;

		for (x=xStart; x<xEnd; x++)	{

			rgb = image->GetPixel(x,y);

			if (GetGValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				count += 1;
			}
			if (count > 3) {
				isLetterLine = true;
				break;
			}
		}

		if (!isLetterLine) {
			yTop = y+1;
			break;
		}
	}
	//----------- get letter lines and parse letters on each line ----------- 
	for (y=yMiddle; y<yMax; y++) {

		isLetterLine = false;
		count = 0;

		for (x=xStart; x<xEnd; x++)	{

			rgb = image->GetPixel(x,y);

			if (GetGValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				count += 1;
			}
			if (count > 3) {
				isLetterLine = true;
				break;
			}
		}

		if (!isLetterLine) {
			yBottom = y-1;
			break;
		}
	}
	
	ParsingStepSecond(yTop, yBottom);
}

//-------------------------------------------------------------------------------------------------
void COCR::ParsingStepSecond(int yTop, int yBottom) {

	int xMax = image->GetWidth();

	int x, y, count;
	COLORREF rgb;
	bool isLetterLine;
	bool flagPrevLine;	

	flagPrevLine = false;

	for (x=0 ; x<xMax; x++) {

		isLetterLine = false;
		count = 0;

		for (y=yTop; y<=yBottom; y++) {
			rgb = image->GetPixel(x,y);

			if (GetGValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				count += 1;
			}
			if (count > 3) {
				isLetterLine = true;
				break;
			}
		}

		if (isLetterLine) {
			if (!flagPrevLine) {
				data->rect.start.x = x;
				data->rect.start.y = yTop;
			}
		} else {
			if (flagPrevLine) {
				data->rect.end.x = x-1;
				data->rect.end.y = yBottom;

				ParsingStepThird(&data->rect);

				if (IsLetterRect(&data->rect)) {
					allData.count += 1;
					data = &allData.data[allData.count];
				}

				if (allData.count == 7)
					break;

			}
		}

		flagPrevLine = isLetterLine;
	}

	if (allData.count != 7)
		::AfxMessageBox(_T("Image Parsing Error : Take Photo Again."));
}

//-------------------------------------------------------------------------------------------------
void COCR::ParsingStepThird(Rect *rect) {

	int x, y, count;
	COLORREF rgb;
	bool isLetterLine;

	int height = image->GetHeight();

	//------------------------------ Letter Top -------------------------------
	for (y=rect->start.y; y>=0; y--) {

		isLetterLine = false;
		count = 0;

		for (x=rect->start.x; x<=rect->end.x; x++) {

			rgb = image->GetPixel(x,y);

			if (GetGValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				count += 1;
			}
			if (count > 3) {
				rect->start.y = y;
				isLetterLine = true;
				break;
			}
		}
		if (!isLetterLine)
			break;
	}
	for (y=rect->start.y; y<height; y++)	{

		isLetterLine = false;
		count = 0;

		for (x=rect->start.x; x<=rect->end.x; x++) {

			rgb = image->GetPixel(x,y);

			if (GetGValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				count += 1;
			}
			if (count > 3) {
				rect->start.y = y;
				isLetterLine = true;
				break;
			}
		}
		if (isLetterLine)
			break;
	}
	//------------------------------ Letter Bottom -------------------------------
	for (y=rect->end.y; y<height; y++) {

		isLetterLine = false;
		count = 0;

		for (x=rect->start.x; x<=rect->end.x; x++) {

			rgb = image->GetPixel(x,y);

			if (GetGValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				count += 1;
			}
			if (count > 3) {
				rect->end.y = y;
				isLetterLine = true;
				break;
			}
		}
		if (!isLetterLine)
			break;
	}
	for (y=rect->end.y; y>=0; y--) {

		isLetterLine = false;
		count = 0;

		for (x=rect->start.x; x<=rect->end.x; x++) {

			rgb = image->GetPixel(x,y);

			if (GetGValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
				count += 1;
			}
			if (count > 3) {
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
bool COCR::IsLetterRect(Rect *rect) {

	int width = rect->end.x - rect->start.x;
	int height = rect->end.y - rect->start.y;

	if (height < 50)
		return false;

	if ((height > width) && (height < (4 * width)))
		return true;

	return false;
}

//---------------------------------------------------------------------------------------------------
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
