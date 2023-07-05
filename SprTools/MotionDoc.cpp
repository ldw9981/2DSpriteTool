// MotionDoc.cpp : 구현 파일입니다.
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


// CMotionDoc 진단입니다.

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
// CMotionDoc serialization입니다.

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
		// TODO: 여기에 저장 코드를 추가합니다.
		std::ofstream file(ReplaceFileExtension(ar.m_strFileName, L".txt"), std::ios::binary);
		m_Motion.SerializeOut(file);
		file.close();
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.

	}
	
	// 중점 좌표도 봐야하므로 이미지 저장함.
	m_Motion.Serialize(ar);
}

void CMotionDoc::Modified()
{

}
#endif


// CMotionDoc 명령입니다.

