
// ImageRecognition.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CImageRecognitionApp:
// �� Ŭ������ ������ ���ؼ��� ImageRecognition.cpp�� �����Ͻʽÿ�.
//

class CImageRecognitionApp : public CWinAppEx
{
public:
	CImageRecognitionApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CImageRecognitionApp theApp;