// SprTools.h : main header file for the SprTools application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "ImageDoc.h"
#include "MotionDoc.h"

// CSprToolsApp:
// See SprTools.cpp for the implementation of this class
//

class CSprToolsApp : public CWinApp
{
public:
	CSprToolsApp();	

	// Attributes
public:

protected:
	CMultiDocTemplate* m_pImageDocTemplate;
	CMultiDocTemplate* m_pMotionDocTemplate;

	// Operations
public:
	CImageDoc* OpenImageDocTemplate(LPCTSTR lpszPathName);
	CMotionDoc* OpenMotionDocTemplate(LPCTSTR lpszPathName);

	CMultiDocTemplate* GetImageDocTemplatePtr() { return m_pImageDocTemplate; }
	CMultiDocTemplate* GetMotionDocTemplatePtr() { return m_pMotionDocTemplate;}

// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
// Implementation	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();	

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CSprToolsApp theApp;