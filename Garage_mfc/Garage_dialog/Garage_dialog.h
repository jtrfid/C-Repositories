
// Garage_dialog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGarage_dialogApp:
// �йش����ʵ�֣������ Garage_dialog.cpp
//

class CGarage_dialogApp : public CWinApp
{
public:
	CGarage_dialogApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGarage_dialogApp theApp;