
// MakeBlackWhitePhoto.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMakeBlackWhitePhotoApp:
// �� Ŭ������ ������ ���ؼ��� MakeBlackWhitePhoto.cpp�� �����Ͻʽÿ�.
//

class CMakeBlackWhitePhotoApp : public CWinAppEx
{
public:
	CMakeBlackWhitePhotoApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMakeBlackWhitePhotoApp theApp;