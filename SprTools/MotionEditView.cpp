// MotionEditView.cpp : ���� �����Դϴ�.
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


// CMotionEditView �׸����Դϴ�.

void CMotionEditView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CMotionEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CMotionEditView �����Դϴ�.

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


// CMotionEditView �޽��� ó�����Դϴ�.
