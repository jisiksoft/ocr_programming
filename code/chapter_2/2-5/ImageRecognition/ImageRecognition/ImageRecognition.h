
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

//---------------------------------------------------------------------------------------------------
class CImageRecognitionApp : public CWinAppEx
{
public:
	CImageRecognitionApp();

	public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CImageRecognitionApp theApp;