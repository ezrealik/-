
// Left4Dead2Plugins.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLeft4Dead2PluginsApp: 
// �йش����ʵ�֣������ Left4Dead2Plugins.cpp
//

class CLeft4Dead2PluginsApp : public CWinApp
{
public:
	CLeft4Dead2PluginsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLeft4Dead2PluginsApp theApp;