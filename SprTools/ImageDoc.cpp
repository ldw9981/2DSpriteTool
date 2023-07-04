// ImageDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SprTools.h"
#include "ImageDoc.h"


// CImageDoc

IMPLEMENT_DYNCREATE(CImageDoc, CDocument)

CImageDoc::CImageDoc()
{
}

BOOL CImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CImageDoc::~CImageDoc()
{
}


BEGIN_MESSAGE_MAP(CImageDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CImageDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CImageDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CImageDoc diagnostics

#ifdef _DEBUG
void CImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CImageDoc serialization

void CImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CImageDoc commands

BOOL CImageDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	TRACE("CImageDoc::OnOpenDocument(LPCTSTR lpszPathName)\n");	

//	m_Image.Load(lpszPathName);
	if (!m_Image.Load(lpszPathName))
	{
		return FALSE;
	}
	
	
	return TRUE;
}

//BOOL CImageDoc::OnSaveDocument(LPCTSTR lpszPathName)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDocument::OnSaveDocument(lpszPathName);
//}

void CImageDoc::OnFileSave()
{
	// TODO: Add your command handler code here
}

void CImageDoc::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	
}
