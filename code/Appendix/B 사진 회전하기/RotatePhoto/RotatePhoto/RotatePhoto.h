
// RotatePhoto.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CRotatePhotoApp:
// �� Ŭ������ ������ ���ؼ��� RotatePhoto.cpp�� �����Ͻʽÿ�.
//

class CRotatePhotoApp : public CWinAppEx
{
public:
	CRotatePhotoApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CRotatePhotoApp theApp;