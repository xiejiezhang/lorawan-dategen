
// lorawan-note-datagen.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// ClorawannotedatagenApp:
// �йش����ʵ�֣������ lorawan-note-datagen.cpp
//

class ClorawannotedatagenApp : public CWinApp
{
public:
	ClorawannotedatagenApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern ClorawannotedatagenApp theApp;