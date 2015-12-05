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
	GetStandardImageDataFromTextFile("standard.txt");
	PrintAllStandardImageToTextFile("standardout.txt");
	colorToCheck = 0;
}

//---------------------------------------------------------------------------------------------------
COCR::~COCR(void) {
}

//---------------------------------------------------------------------------------------------------
// Standard Image Data�� �����.
void COCR::RunOCR(CImage *newImage) {

	image = ResizeInBlackWhiteImage(newImage);

	ParsingStepFirst();

	//for (int i=0; i<MAX_COUNT_DATA; i++)
	//	PrintImageToFile(i, &allData.data[i].rect);

	MakeImageData();

	//for (int i=0; i<MAX_COUNT_DATA; i++)
	//	PrintImageDataToFile(i, &allData.data[i].letter);

	FindValue();

	//PrintEveryImageDataInTextFile("EveryImageData.txt");

	ShowResultMessage();
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
// Image Parsing�� �����Ѵ�.
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

				if (GetGValue(rgb) < colorToCheck + RANGE_OF_COLOR_TO_CHECK) {
					buffer = 0x80000000;
					buffer >>= x;
					letter->image[y] |= buffer;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
void COCR::FindValue() {

	int i;

	for (i=0; i<allData.count; i++) {

		Data *aData = &allData.data[i];

		if (i == 2)
			FindLetterValue(aData);
		else
			FindNumberValue(aData);

	}
}

//-------------------------------------------------------------------------------------------------
void COCR::FindNumberValue(Data *aData) {

	int count, maxCount;
	unsigned int buffer, bit;
	int i, x, y;

	Letter *letter = &(aData->letter);

	maxCount = 0;

	for (i=0; i<standardNumber.count; i++) {

		count = 0;

		for (y=0; y<48; y++) {

			buffer = letter->image[y] ^ standardNumber.letter[i].image[y];

			for (x=0; x<32; x++) {

				bit = 0x80000000;
				bit >>= x;
				bit = bit & buffer;

				if (!bit)
					count += 1;
			}
		}

		if (count > maxCount) {
			letter->value = standardNumber.letter[i].value;
			maxCount = count;
		}
	}
}

//-------------------------------------------------------------------------------------------------
void COCR::FindLetterValue(Data *aData) {

	int count, maxCount;
	unsigned int buffer, bit;
	int i, x, y;

	Letter *letter = &(aData->letter);

	maxCount = 0;

	for (i=0; i<standardLetter.count; i++) {

		count = 0;

		for (y=0; y<48; y++) {

			buffer = letter->image[y] ^ standardLetter.letter[i].image[y];

			for (x=0; x<32; x++) {

				bit = 0x80000000;
				bit >>= x;
				bit = bit & buffer;

				if (!bit)
					count += 1;
			}
		}

		if (count > maxCount) {
			letter->value = standardLetter.letter[i].value;
			maxCount = count;
		}
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


//---------------------------------------------------------------------------------------------------
// Image Data �Ϸ� ��, ��� ���� �̹��� �����͸� Text File�� ����
// �׽�Ʈ�� ���Ͽ� ������ �Լ�
void COCR::PrintImageDataToFile(int fileNo, Letter *letter) {

	CString strName;
	strName.Format(TEXT("./ImageData%d.txt"), fileNo);

	FILE *fp;
	fp = fopen(LPSTR(LPCTSTR(strName)), "wt");

	int x, y;
	unsigned int buffer;

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

		fprintf(fp, "\n%s\n", letter->value.GetString());

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
	char ch, str[4];
	unsigned int buffer;
	Letter *letter;

	fscanf(fp, "%d\n", &standardLetter.count);

	standardNumber.count = 10;
	standardLetter.count -= 10;

	for (i=0; i<standardNumber.count; i++) {

		letter = &standardNumber.letter[i];

		fscanf(fp, "\n%s\n", str);
		letter->value = str;

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

	for (i=0; i<standardLetter.count; i++) {

		letter = &standardLetter.letter[i];

		fscanf(fp, "\n%s\n", str);
		letter->value = str;

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
void COCR::GetStandardImageDataFromBinaryFile(char * fileName) {
	
	FILE *fp;
	fp = fopen(fileName, "rb");

	int i, y;
	char str[4];
	Letter *letter;

	fscanf(fp, "%d\n", &standardLetter.count);

	standardNumber.count = 10;
	standardLetter.count -= 10;

	for (i=0; i<standardNumber.count; i++) {

		letter = &standardNumber.letter[i];

		fscanf(fp, "\n%s\n", str);
		letter->value = str;

		for (y=0; y<48; y++)
			fscanf(fp, "%d\n", &letter->image[y]);
	}

	for (i=0; i<standardLetter.count; i++) {

		letter = &standardLetter.letter[i];

		fscanf(fp, "\n%s\n", str);
		letter->value = str;

		for (y=0; y<48; y++)
			fscanf(fp, "%d\n", &letter->image[y]);
	}

	fclose(fp);
}

//---------------------------------------------------------------------------------------------------
void COCR::PrintEveryImageDataInBinaryFile(char * fileName) {
	
	FILE *fp;
	fp = fopen(fileName, "wb");

	int i, y;
	Letter *letter;

	fprintf(fp, "%d\n", allData.count);

	for (i=0; i<allData.count; i++) {

		letter = &allData.data[i].letter;

		fprintf(fp, "\n%s\n", letter->value.GetString());

		for (y=0; y<48; y++)
			fprintf(fp, "%d\n", letter->image[y]);
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

	fprintf(fp, "%d\n", standardNumber.count + standardLetter.count);

	for (i=0; i<standardNumber.count; i++) {

		letter = &standardNumber.letter[i];

		fprintf(fp, "\n%s\n", letter->value.GetString());

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

	for (i=0; i<standardLetter.count; i++) {

		letter = &standardLetter.letter[i];

		fprintf(fp, "\n%s\n", letter->value.GetString());

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

//-------------------------------------------------------------------------------------------------
void COCR::ShowResultMessage() {

	CString result;

	for (int i=0; i<7; i++)
		result.AppendFormat(_T("%s"), allData.data[i].letter.value);

	::AfxMessageBox(result);
}

	//CString str;
	//str.Format(_T("%d"), gapSpace);
	//::AfxMessageBox(str);
