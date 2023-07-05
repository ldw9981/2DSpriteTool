// MotionDoc.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SprTools.h"
#include "MotionDoc.h"
#include "MainFrm.h"


// CMotionDoc

IMPLEMENT_DYNCREATE(CMotionDoc, CDocument)

CMotionDoc::CMotionDoc()
{
}


CMotionDoc::~CMotionDoc()
{
	
}


BEGIN_MESSAGE_MAP(CMotionDoc, CDocument)

//	ON_COMMAND(ID_FILE_SAVE, &CMotionDoc::OnFileSave)
END_MESSAGE_MAP()


// CMotionDoc �����Դϴ�.

#ifdef _DEBUG
void CMotionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CMotionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CMotionDoc serialization�Դϴ�.

CString ReplaceFileExtension(const CString& filepath, const CString& newExtension)
{
	// Get the position of the last period (.) in the filepath
	int lastDotPos = filepath.ReverseFind('.');

	if (lastDotPos != -1)
	{
		// Remove the current extension
		CString filepathWithoutExtension = filepath.Left(lastDotPos);

		// Add the new extension
		CString newFilepath = filepathWithoutExtension + newExtension;

		return newFilepath;
	}

	// If there is no existing extension, simply add the new extension
	return filepath + newExtension;
}

void CMotionDoc::Serialize(CArchive& ar)
{
	TRACE(_T("CMotionDoc::Serialize(CArchive& ar)\n"));
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		std::ofstream file(ReplaceFileExtension(ar.m_strFileName, L".txt"), std::ios::binary);
		m_Motion.SerializeOut(file);
		file.close();
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.

	}
	
	// ���� ��ǥ�� �����ϹǷ� �̹��� ������.
	m_Motion.Serialize(ar);
}

void CMotionDoc::Modified()
{

}
#endif


// CMotionDoc ����Դϴ�.

