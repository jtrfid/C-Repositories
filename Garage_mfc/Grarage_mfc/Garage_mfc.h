
// Grarage_mfc.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGrarage_mfcApp:
// �йش����ʵ�֣������ Grarage_mfc.cpp
//

class CGarage_mfcApp : public CWinApp
{
public:
	CGarage_mfcApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGarage_mfcApp theApp;