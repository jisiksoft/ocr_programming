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

	letterSpace.letter.value = ' ';
	letterSpace.isFixed = true;
	letterSpace.isDeleted = false;
}

//---------------------------------------------------------------------------------------------------
COCR::~COCR(void)
{
}

//---------------------------------------------------------------------------------------------------
// Standard Image Data를 만든다.
void COCR::RunOCR(CImage *newImage, CString outFileName, int colorLetter) {

	image = newImage;
	colorToCheck = colorLetter;

	for (int i=0; i<MAX_COUNT_DATA; i++) {
		allData.data[i].isFixed = false;
		allData.data[i].isDeleted = false;
	}

	ParsingStepFirst();
	MakeImageData();

	FindLetterValue();

	AddSpaceValue();
	MakePeriodApostrophe();
	MakeQuotationMark();

	ChangeBigSmallLetter();

	//StoreLetterToTextFile(outFileName);
	StoreLetterToTextFile2(outFileName);
}

//---------------------------------------------------------------------------------------------------
void COCR::GetStandardImageDataFromBinaryFile(char * fileName) {
	
	FILE *fp;
	fp = fopen(fileName, "rb");

	int i, y;
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

	data->isFixed = true;
	data->letter.value = '\n';

	allData.count += 1;
	data = &allData.data[allData.count];
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

		if (!data->isFixed) {

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

		if (!data->isFixed) {

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
}

//-------------------------------------------------------------------------------------------------
void COCR::AddSpaceValue() {

	int index, start, end;

	index = 0;
	start = 0;

	for (int i=0; i<allData.count; i++) {

		if (allData.data[i].letter.value == '\n') {

			end = i;
			AddSpaceValueInLine(&index, start, end);

			start = i + 1;
		}
	}

	allDataPointer.count = index;
}

//-------------------------------------------------------------------------------------------------
void COCR::AddSpaceValueInLine(int *index, int start, int end) {

	int i;
	int gapSpace = CalculateGapSpace(start, end);
	
	allDataPointer.data[*index] = &allData.data[start];
	*index += 1;

	for (i=start+1; i<end; i++) {

		Data *prevData = &allData.data[i-1];
		Data *currData = &allData.data[i];

		if ((currData->rect.start.x - prevData->rect.end.x) > gapSpace) {
			allDataPointer.data[*index] = &letterSpace;
			*index += 1;
		}
		
		allDataPointer.data[*index] = &allData.data[i];
		*index += 1;
	}
		
	allDataPointer.data[*index] = &allData.data[i];
	*index += 1;
}

//-------------------------------------------------------------------------------------------------
int COCR::CalculateGapSpace(int start, int end) {

	int totalGap, count;

	totalGap = 0;
	count = 0;

	for (int i=start+1; i<end; i++) {

		Data *prevData = &allData.data[i-1];
		Data *currData = &allData.data[i];

		totalGap += (currData->rect.start.x - prevData->rect.end.x);
		count += 1;
	}
		
	return ((int)(totalGap / (count * 0.7))); //한줄에 10개의 space가 있으면, 보통 40개 조금 넘는 문자들이 있다.
}

//-------------------------------------------------------------------------------------------------
int COCR::CalculateMidlineValue(int index) {

	int count = 0;
	float sumHeight = 0;

	for (int i=index; i<allData.count; i++) {
		
		if (allData.data[i].letter.value == '\n')
			break;

		sumHeight += ((allData.data[i].rect.end.y + allData.data[i].rect.start.y) / 2);
		count += 1;
	}

	return (int)(sumHeight / count);
}

//-------------------------------------------------------------------------------------------------
void COCR::MakePeriodApostrophe() {

	int avgMidline;

	for (int i=0; i<allData.count;) {
		
		avgMidline = CalculateMidlineValue(i);

		while (allData.data[i].letter.value != '\n') {

			if (allData.data[i].rect.start.y > avgMidline) {
				if ((allData.data[i].letter.value == 'I') || 
					(allData.data[i].letter.value == 'l')) {
					
						allData.data[i].letter.value = '.';
				}
			} else if (allData.data[i].rect.end.y < avgMidline) {
				if ((allData.data[i].letter.value == 'I') || 
					(allData.data[i].letter.value == 'l') || 
					(allData.data[i].letter.value == ',')) {
					
						allData.data[i].letter.value = 0x27;//''';
				}
			}

			i += 1;
		}
		i += 1;
	}
}

//-------------------------------------------------------------------------------------------------
void COCR::MakeQuotationMark() {

	for (int i=0; i<allData.count; i++) {
		if (!(allData.data[i].isDeleted) && allData.data[i].letter.value == 0x27) {
			if (allData.data[i+1].letter.value == 0x27) {
				allData.data[i].letter.value = 0x22;//'"'
				allData.data[i+1].isDeleted = true;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
bool COCR::isAlphabet(char value) {

	if (value >= 'A' && value <= 'Z')
		return true;
	else if (value >= 'a' && value <= 'z')
		return true;

	return false;
}

//-------------------------------------------------------------------------------------------------
bool COCR::isBigLetter(char value) {

	if (value >= 'A' && value <= 'Z')
		return true;

	return false;
}

//-------------------------------------------------------------------------------------------------
bool COCR::isSmallLetter(char value) {

	if (value >= 'a' && value <= 'z')
		return true;

	return false;
}

//-------------------------------------------------------------------------------------------------
void COCR::ChangeBigSmallLetter() {

	bool isFirstChar = true;
	char *ch, *chNext;

	for (int i=0; i<(allData.count-1); i++) {

		ch = &(allData.data[i].letter.value);
		chNext = &(allData.data[i+1].letter.value);

		if (isFirstChar) {

			if (isSmallLetter(*ch)) {
					
				ChangeSmallToBigLetter(ch);
				isFirstChar = false;

			} else if (isBigLetter(*ch)) {

				isFirstChar = false;
			
			} else if (*ch == '0') { //숫자 0
				if(isAlphabet(*chNext))
					*ch = '0'; //알파벳 O
				
				isFirstChar = false;
			}
		} else {

			if (isBigLetter(*ch)) {

				ChangeBigToSmallLetter(ch);

			} else if (*ch == '0') { //숫자 0
				if(isAlphabet(*chNext))
					*ch = 'o'; //알파벳 o
			}

			if ((*ch == '.') || (*ch == 0x22)) {
				isFirstChar = true;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------
void COCR::ChangeBigToSmallLetter(char *value) {

	if (*value == 'I')	//대문자 'I'
		*value = 'l';	//소문자 'ㅣ'
	else if (*value == 'C')
		*value = 'c';
	else if (*value == 'O')
		*value = 'o';
	else if (*value == 'S')
		*value = 's';
	else if (*value == 'U')
		*value = 'u';
	else if (*value == 'V')
		*value = 'v';
	else if (*value == 'W')
		*value = 'w';
	else if (*value == 'X')
		*value = 'x';
	else if (*value == 'Z')
		*value = 'z';
}

//---------------------------------------------------------------------------------------------------
void COCR::ChangeSmallToBigLetter(char *value) {
	
	if (*value == 'l')	//소문자 'ㅣ'
		*value = 'I';	//대문자 'I'
	else if (*value == 'c')
		*value = 'C';
	else if (*value == 'o')
		*value = 'O';
	else if (*value == 's')
		*value = 'S';
	else if (*value == 'u')
		*value = 'U';
	else if (*value == 'v')
		*value = 'V';
	else if (*value == 'w')
		*value = 'W';
	else if (*value == 'x')
		*value = 'X';
	else if (*value == 'z')
		*value = 'Z';
}

//---------------------------------------------------------------------------------------------------
void COCR::StoreLetterToTextFile(CString outFileName) {

	FILE *fp;
	fp = fopen((char*)((LPCSTR)(outFileName)), "wt");

	for (int i=0; i<allData.count; i++)
		fputc(allData.data[i].letter.value, fp);

	fclose(fp);
}

//---------------------------------------------------------------------------------------------------
void COCR::StoreLetterToTextFile2(CString outFileName) {

	FILE *fp;
	fp = fopen((char*)((LPCSTR)(outFileName)), "wt");

	for (int i=0; i<allDataPointer.count; i++) {
		if (!(allDataPointer.data[i]->isDeleted)) {
			fputc(allDataPointer.data[i]->letter.value, fp);
		}
	}

	fclose(fp);
}

/*
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
*/

	//CString str;
	//str.Format(_T("%d"), gapSpace);
	//::AfxMessageBox(str);
