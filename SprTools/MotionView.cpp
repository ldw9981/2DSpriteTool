// CMotionView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SprTools.h"
#include "MotionView.h"
#include "MainFrm.h"
#include "MotionDoc.h"
#include "MotionFrame.h"

// CMotionView

IMPLEMENT_DYNCREATE(CMotionView, CScrollView)

CMotionView::CMotionView()
{
	m_ClientRect.SetRectEmpty();

}

CMotionView::~CMotionView()
{
	m_MemDC.Uninit();
}


BEGIN_MESSAGE_MAP(CMotionView, CScrollView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMotionView 그리기입니다.

void CMotionView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();


	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	TRACE(_T("CMotionView::OnInitialUpdate()\n"));
	CMotionDoc* pDoc=(CMotionDoc*)GetDocument();
	sizeTotal.cx = pDoc->GetMoiton().GetSpreadRect().Width();
	sizeTotal.cy = pDoc->GetMoiton().GetSpreadRect().Height();
	SetScrollSizes(MM_TEXT, sizeTotal);
	



//	((CMotionFrame*)GetParent())->m_LeftDlgBar.GetClientRect(&m_LeftDlgRect);
//	((CMotionFrame*)GetParent())->m_TopDlgBar.GetClientRect(&m_TopDlgRect);

/*
	GetParent()->SetWindowPos(NULL,0,0,
		m_TopDlgRect.Width()+::GetSystemMetrics(SM_CXBORDER)*2,
		m_TopDlgRect.Height()+m_LeftDlgRect.Height()+::GetSystemMetrics(SM_CYBORDER)*2+::GetSystemMetrics(SM_CYCAPTION),
		SWP_NOMOVE);
//		max(300,sizeTotal.cx+50),max(100,sizeTotal.cy+50),SWP_NOMOVE);
*/
}

void CMotionView::OnDraw(CDC* pDC)
{
	CMotionDoc* pMotionDoc = (CMotionDoc*)GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	m_MemDC.ClearBlack();
	if (pMotionDoc->GetMoiton().CountFrame()==0)
	{
		m_MemDC.TextOut(0,0,_T("프레임 없음."));
		m_MemDC.TextOut(0,16,_T("24비트 비트맵 파일을"));
		m_MemDC.TextOut(0,32,_T("열고 프레임을 추가하세요."));
	}
	else
	{
		pMotionDoc->GetMoiton().DrawSpread(&m_MemDC);	
		//DrawGauze(&m_MemDC,m_SelectedFrameRect);		
	}

	pDC->BitBlt(0,0,m_MemDC.GetWidth(),m_MemDC.GetHeight(),&m_MemDC,0,0,SRCCOPY);
}


// CMotionView 진단입니다.

#ifdef _DEBUG
void CMotionView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CMotionView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG



void CMotionView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	if (bActivate)
	{
		pMainFrame->SetCurrMotionView(this);		
		pMainFrame->GetInfoDlg().Invalidate(FALSE);		
	}	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CMotionView::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	CMotionDoc* pDoc=(CMotionDoc*)GetDocument();
	pDoc->GetMoiton().DeleteAll();


	if(pMainFrame->GetCurrMotionView()==this)
	{
		
		pMainFrame->SetCurrMotionView(NULL);
		pMainFrame->GetInfoDlg().Invalidate();
	}
		
	
}

void CMotionView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE(_T("CMotionView::OnUpdate()\n"));

	int cx=0,cy=0;
	CMotionDoc* pDoc=(CMotionDoc*)GetDocument();
	CMotionFrame *pMotionFrame=(CMotionFrame *)GetParent();
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();

	m_MemDC.Uninit();	
	CClientDC dc(this);

	if (pDoc->GetMoiton().CountFrame()==0)
	{
		m_MemDC.Init(&dc,400,400);
		GetParent()->SetWindowPos(NULL,0,0,	
		//cx
		pMotionFrame->m_TopDlgBar.m_sizeDefault.cx+
		::GetSystemMetrics(SM_CXFRAME)*2+pDoc->GetMoiton().GetSpreadRect().Width(),
		//cy	
		pMotionFrame->m_TopDlgBar.m_sizeDefault.cy+pMotionFrame->m_LeftDlgBar.m_sizeDefault.cy+
		::GetSystemMetrics(SM_CYFRAME)*2+::GetSystemMetrics(SM_CYCAPTION),
		//
		SWP_NOMOVE|SWP_NOACTIVATE);

	}
	else
	{
		m_MemDC.Init(&dc,pDoc->GetMoiton().GetSpreadRect().Width(),pDoc->GetMoiton().GetSpreadRect().Height()+50);

		//cx
		cx=pMotionFrame->m_LeftDlgBar.m_sizeDefault.cx+
			::GetSystemMetrics(SM_CXFRAME)*2+pDoc->GetMoiton().GetSpreadRect().Width();

		CRect MainClientRect;
		pMainFrame->GetClientRect(&MainClientRect);
		if (cx>=MainClientRect.Width()-300)
		{
			cx=MainClientRect.Width()-300;
		}

		//cy
		cy=pMotionFrame->m_TopDlgBar.m_sizeDefault.cy+
			::GetSystemMetrics(SM_CYFRAME)*2+::GetSystemMetrics(SM_CYCAPTION)+pDoc->GetMoiton().GetSpreadRect().Height()+50;
				

		GetParent()->SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOACTIVATE);

		SetScrollSizes(MM_TEXT,pDoc->GetMoiton().GetSpreadRect().Size());
		SendMessage(WM_HSCROLL,SB_PAGERIGHT,0);	
	}	
}	

void CMotionView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	point.x +=GetScrollPosition().x;
	point.y +=GetScrollPosition().y;

	CMotionDoc* pDoc=(CMotionDoc*)GetDocument();
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	
	CFrame* pFrame=pDoc->GetMoiton().SelectFrame(point);
	if (pFrame!=NULL)
	{	
		((CMotionFrame*)GetParent())->SetLeftDlgBarCenterX(pFrame->GetCenterX());
		((CMotionFrame*)GetParent())->SetLeftDlgBarCenterY(pFrame->GetCenterY());	
		((CMotionFrame*)GetParent())->SetLeftDlgBarDelayTime(pFrame->GetDelayTime());	

		Invalidate(FALSE);
		pMainFrame->GetFrameCenterDlg().Init(pFrame);
	}


	CScrollView::OnLButtonDown(nFlags, point);
}


void CMotionView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMotionDoc* pDoc=(CMotionDoc*)GetDocument();
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	CRect rtFrameClient;
	CFrame* pFrame=NULL;
	switch(nChar)
	{
	case VK_DELETE:
		pDoc->GetMoiton().DeleteFrame(pDoc->GetMoiton().GetSelectedFrameIndex());
		pDoc->UpdateAllViews(NULL);
		
		break;
	case VK_LEFT:
		pFrame=pDoc->GetMoiton().SelectFrame(pDoc->GetMoiton().GetSelectedFrameIndex()-1,rtFrameClient);	
		break;
	case VK_RIGHT:
		pFrame=pDoc->GetMoiton().SelectFrame(pDoc->GetMoiton().GetSelectedFrameIndex()+1,rtFrameClient);
		break;
	}

	if (pFrame!=NULL)
	{			
		Invalidate(FALSE);
		pMainFrame->GetFrameCenterDlg().Init(pFrame);
	}

	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMotionView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	point.x +=GetScrollPosition().x;
	point.y +=GetScrollPosition().y;

	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	CMotionDoc* pDoc=(CMotionDoc*)GetDocument();
	if (m_bDrag)
	{
		m_bDrag=FALSE;
		if (pDoc->GetMoiton().Drop(point))
		{
			pDoc->SetModifiedFlag(TRUE);
			pDoc->Modified();
			CFrame* pFrame=pDoc->GetMoiton().GetFrameSelected();
			if (pFrame!=NULL)
			{
				pMainFrame->GetFrameCenterDlg().Init(pFrame);
				Invalidate(FALSE);
			}
		}
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void CMotionView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	point.x +=GetScrollPosition().x;
	point.y +=GetScrollPosition().y;

	CMotionDoc* pDoc=(CMotionDoc*)GetDocument();
	if (nFlags==MK_LBUTTON)
	{
		m_bDrag=TRUE;	
		if (pDoc->GetMoiton().Drag(point))
		{
			SetCursor(::AfxGetApp()->LoadCursor(IDC_DRAGMOVE));
		}
		else
		{
			SetCursor(::AfxGetApp()->LoadCursor(IDC_NODROP));
		}
		
		
		
		Invalidate(FALSE);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

int CMotionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CMotionView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	TRACE(_T("CMotionView::OnSize(%d,%d,%d)\n"),nType,cx,cy);


//	CRect temp;
//	GetClientRect(&m_ClientRect);

//	if (cx>=temp.Width()-300)
//	{
//		SetWindowPos(NULL,0,0,temp.Width()-300,cy,SWP_NOMOVE|SWP_NOACTIVATE);
//	}
}
