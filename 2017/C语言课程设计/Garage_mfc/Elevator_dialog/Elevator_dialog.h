
// Elevator_dialog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CElevator_dialogApp:
// �йش����ʵ�֣������ Elevator_dialog.cpp
//

class CElevator_dialogApp : public CWinApp
{
public:
	CElevator_dialogApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CElevator_dialogApp theApp;