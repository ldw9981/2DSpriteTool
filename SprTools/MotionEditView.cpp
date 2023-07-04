// MotionEditView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SprTools.h"
#include "MotionEditView.h"


// CMotionEditView

IMPLEMENT_DYNCREATE(CMotionEditView, CScrollView)

CMotionEditView::CMotionEditView()
{

}

CMotionEditView::~CMotionEditView()
{
}


BEGIN_MESSAGE_MAP(CMotionEditView, CScrollView)
END_MESSAGE_MAP()


// CMotionEditView 그리기입니다.

void CMotionEditView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CMotionEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMotionEditView 진단입니다.

#ifdef _DEBUG
void CMotionEditView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CMotionEditView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMotionEditView 메시지 처리기입니다.
