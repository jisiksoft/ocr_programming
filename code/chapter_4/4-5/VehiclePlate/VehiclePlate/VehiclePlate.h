
// VehiclePlate.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CVehiclePlateApp:
// �� Ŭ������ ������ ���ؼ��� VehiclePlate.cpp�� �����Ͻʽÿ�.
//

class CVehiclePlateApp : public CWinAppEx
{
public:
	CVehiclePlateApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CVehiclePlateApp theApp;