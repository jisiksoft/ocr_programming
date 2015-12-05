#include "StdAfx.h"
#include "ImageRecognition.h"

CImageRecognition::CImageRecognition(void)
{
	standardData = LoadStandardData();		//load standard data for recognition.
}

CImageRecognition::~CImageRecognition(void)
{
}

//-------------------------------------------------------------------------------------------------
void CImageRecognition::implementXOR(char charFirst, char charSecond)
{
	int i;
	Letter letter[3];

	for(i=0; i<62; i++) {
		//if (i=3) {
		if (standardData->letters[i].ch == charFirst) {
			letter[0] = standardData->letters[i];
			break;
		}
	}
	for(i=0; i<62; i++) {
		//if (i=5) {
		if (standardData->letters[i].ch == charSecond) {
			letter[1] = standardData->letters[i];
			break;
		}
	}

	letter[2].ch = '-';
	for(i=0; i<48; i++) {
		letter[2].bytes[i] = letter[0].bytes[i] ^ letter[1].bytes[i];
	}

	PrintLetterToFile(letter, 3);
}

//-------------------------------------------------------------------------------------------------
// Load Standard Data from files
StandardData* CImageRecognition::LoadStandardData()
{
	static StandardData data;

	data.letters = LoadStandardLetters();			//Load Standard Letters

	return &data;
}

//-------------------------------------------------------------------------------------------------
// Load standard Letters having compressed image from file
// Image data is 0 or 1 in a text file (0 is background(white) and 1 is Letter(black)
Letter* CImageRecognition::LoadStandardLetters()
{
	static Letter letter[62];
	char ch;
	unsigned int checkByte;
	int i, j, k, nStandardLetters;

	FILE *in;

	in = fopen("./standardLetter.txt", "rt");           //open a file to load number images
	if (in == NULL)
		AfxMessageBox(_T("The file 'standardNumber.data' is not opened"));

	fscanf(in, "%d\n\n", &nStandardLetters);

	for (i=0; i<nStandardLetters; i++)
	{
		letter[i].ch = fgetc(in);
		ch = fgetc(in);

		for (j=0; j<48; j++)								       		//initialize images as zero
			letter[i].bytes[j] = 0x00000000;

		for (j=0; j<48; j++)
		{
			unsigned int buffer = letter[i].bytes[j];

			checkByte = 0x80000000;

			for (k=0; k<32; k++)
			{
				ch = fgetc(in);

				if (ch == '1')												//set a bit as 1 for black number
					letter[i].bytes[j] |= checkByte;

				checkByte >>= 1;
			}
			ch = fgetc(in);
		}
		ch = fgetc(in);
	}
	fclose(in);

	return letter;
}

//-------------------------------------------------------------------------------------------------
// print out compressed image to a file, and the number of images is the input "countToPrint".
void CImageRecognition::PrintLetterToFile(Letter* letters, int countToPrint)
{
	FILE *out;

	out = fopen("./result.txt", "wt");

	for (int i=0; i<countToPrint; i++)
	{
		fputc(letters[i].ch, out);
		fputc('\n', out);
		for (int j=0; j<HEIGHT_COMPRESSED_LETTER; j++)
		{
			unsigned int buffer = letters[i].bytes[j];

			for (int k=0; k<WIDTH_COMPRESSED_LETTER; k++)
			{
				if (buffer & 0x80000000)
					fputc('1', out);
				else
					fputc('0', out);
				buffer <<= 1;
			}
			fputc('\n', out);
		}
		fputc('\n', out);
		fputc('\n', out);
	}

	fclose(out);
}
