
// ResizePhoto.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CResizePhotoApp:
// �� Ŭ������ ������ ���ؼ��� ResizePhoto.cpp�� �����Ͻʽÿ�.
//

class CResizePhotoApp : public CWinAppEx
{
public:
	CResizePhotoApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CResizePhotoApp theApp;