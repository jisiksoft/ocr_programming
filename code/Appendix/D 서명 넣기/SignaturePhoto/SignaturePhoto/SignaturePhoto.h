
// SignaturePhoto.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSignaturePhotoApp:
// �� Ŭ������ ������ ���ؼ��� SignaturePhoto.cpp�� �����Ͻʽÿ�.
//

class CSignaturePhotoApp : public CWinAppEx
{
public:
	CSignaturePhotoApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSignaturePhotoApp theApp;