#pragma once

// CImageDoc document
#include "DIB.h"

class CImageDoc : public CDocument
{
	DECLARE_DYNCREATE(CImageDoc)

public:
	CImageDoc();
	virtual ~CImageDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()

protected:
//	CList	<CDIB*,CDIB*> m_listDIB;	//테스트용
	CDIB	m_Image;
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	CDIB& GetImage() { return m_Image; }

//	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
};
