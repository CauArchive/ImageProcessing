
// Project02_SpicyUdon.h : main header file for the Project02_SpicyUdon application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CProject02SpicyUdonApp:
// See Project02_SpicyUdon.cpp for the implementation of this class
//

class CProject02SpicyUdonApp : public CWinApp
{
public:
	CProject02SpicyUdonApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
};

extern CProject02SpicyUdonApp theApp;
