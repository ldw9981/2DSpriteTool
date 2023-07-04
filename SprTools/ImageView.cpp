// ImageView.cpp : implementation file
//

#include "stdafx.h"
#include "SprTools.h"
#include "ImageView.h"
#include "ImageDoc.h"
#include "MotionDoc.h"
#include "MainFrm.h"


// CImageView

IMPLEMENT_DYNCREATE(CImageView, CScrollView)

CImageView::CImageView()
{
	m_bRButtonDrag=FALSE;
	m_bLButtonDrag=FALSE;
	m_SelectRect=CRect(0,0,0,0);
	m_CurrPoint=CPoint(0,0);
	m_sizeScroll=CSize(0,0);
	m_ptOldPos=CPoint(0,0);
	m_rtClient=CRect(0,0,0,0);
	m_bSpoid=FALSE;

}

CImageView::~CImageView()
{
	m_rtClient.SetRectEmpty();
	m_MemDC.Uninit();
}


BEGIN_MESSAGE_MAP(CImageView, CScrollView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_ACTIVATE()	
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CImageView drawing

void CImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CSize sizeTotal;
	// TODO: calculate the total size of this view
	TRACE("CImageView::OnInitialUpdate()\n");
	CImageDoc* pDoc =(CImageDoc*) GetDocument();	
	
	m_sizeScroll.cx = pDoc->GetImage().GetWidth();
	m_sizeScroll.cy = pDoc->GetImage().GetHeight();
	SetScrollSizes(MM_TEXT, m_sizeScroll);


	CClientDC dc(this);
	
//	m_BufferBitmap.CreateCompatibleBitmap(&dc,m_sizeScroll.cx,m_sizeScroll.cy);	
//	m_BufferDC.CreateCompatibleDC(&dc);
//	m_BufferDC.SelectObject(&m_BufferBitmap);	

	m_MemDC.Init(&dc,m_sizeScroll.cx,m_sizeScroll.cy);

	m_SelectRectBrush.CreateStockObject( NULL_BRUSH ) ;
	m_SelectRectPen.CreatePen( PS_DOT, 1, RGB( 0, 0, 255 ) ) ;
	

	
	// Resize FrameWnd 
	ResizeParentToFit(FALSE);
}

void CImageView::OnDraw(CDC* pDC)
{
	CImageDoc* pDoc =(CImageDoc*) GetDocument();
	// TODO: add draw code here

	// bmp
	if (!pDoc->GetImage().IsNull())
	{
	
//		pDoc->GetImage().BitBlt(m_BufferDC.GetSafeHdc(),0,0,pDoc->GetImage().GetWidth(),pDoc->GetImage().GetHeight());

			
		pDoc->GetImage().StretchDIBits(m_MemDC.GetSafeHdc(),0,0,
		pDoc->GetImage().GetWidth(),pDoc->GetImage().GetHeight(),DIB_RGB_COLORS,SRCCOPY);
	

	/*
		pDoc->GetImage().TransparentBlt(m_BufferDC.GetSafeHdc(),
		0,0,pDoc->GetImage().GetWidth(),pDoc->GetImage().GetHeight());
	*/
	}

	if (!m_bRButtonDrag)
	{
		// Cross Line
		m_MemDC.MoveTo(m_CurrPoint.x,0);
		m_MemDC.LineTo(m_CurrPoint.x,m_sizeScroll.cy);
		m_MemDC.MoveTo(0,m_CurrPoint.y);
		m_MemDC.LineTo(m_sizeScroll.cx,m_CurrPoint.y);
	}



	// Select Rect
	if (m_bLButtonDrag)
	{
		CPen* pPenOld;
		CBrush* pBrushOld;
		pPenOld=m_MemDC.SelectObject(&m_SelectRectPen);
		pBrushOld=m_MemDC.SelectObject(&m_SelectRectBrush);

		m_MemDC.Rectangle(m_SelectRect);

		m_MemDC.SelectObject(pPenOld);
		m_MemDC.SelectObject(pBrushOld);

	}
/*
	m_tempDIB.StretchDIBits(m_BufferDC.GetSafeHdc(),
		GetScrollPosition().x,GetScrollPosition().y,m_tempDIB.GetWidth(),m_tempDIB.GetHeight(),
		DIB_RGB_COLORS,SRCCOPY);
*/
	if (!m_tempDIB.IsNull())
	{
		m_tempDIB.TransparentBlt(m_MemDC.GetSafeHdc(),
		GetScrollPosition().x,GetScrollPosition().y,m_tempDIB.GetWidth(),m_tempDIB.GetHeight());		
	}

	pDC->BitBlt(0,0,m_sizeScroll.cx,m_sizeScroll.cy,&m_MemDC,0,0,SRCCOPY);
	
}


// CImageView diagnostics

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageView message handlers

BOOL CImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("CImageView::PreCreateWindow(CREATESTRUCT& cs)\n");

	// Create a window without min/max buttons or sizable border


	return CScrollView::PreCreateWindow(cs);
}

int CImageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here	

	TRACE("CImageView::OnCreate(LPCREATESTRUCT lpCreateStruct)\n");
	return 0;
}

//void CImageView::OnSize(UINT nType, int cx, int cy)
//{
//	CScrollView::OnSize(nType, cx, cy);
//
//	// TODO: Add your message handler code here
//}

void CImageView::OnDestroy()
{
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here'	
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	if (pMainFrame->GetCurrImageView()==this)
	{
		pMainFrame->SetCurrImageView(NULL);
		pMainFrame->GetInfoDlg().Invalidate();
	}	
	TRACE("CImageView::OnDestroy(%x)\n",this);
}

void CImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CImageDoc* pDoc =(CImageDoc*) GetDocument();	

	if ((nFlags&MK_LBUTTON)&&(nFlags&MK_CONTROL))
	{
		pDoc->GetImage().SetTransparentColor(pDoc->GetImage().GetRGB(point.x,point.y));	
	}
	else
	{		
		m_bLButtonDrag=TRUE;
		SetCapture();
		m_SelectRect.left	= GetScrollPosition().x + point.x;
		m_SelectRect.top	= GetScrollPosition().y + point.y;		
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CImageDoc* pImageDoc =(CImageDoc*) GetDocument();
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	CMotionDoc* pMotionDoc;

	point.x+=GetScrollPosition().x;
	point.y+=GetScrollPosition().y;

	if (m_bLButtonDrag)
	{
		m_bLButtonDrag=FALSE;	
		ReleaseCapture();
		m_SelectRect.right	=  point.x;
		m_SelectRect.bottom	=  point.y;

		if (m_SelectRect.TopLeft()==point)
		{
			//Down과Up의 위치가 같을때 
			m_SelectRect=pImageDoc->GetImage().GetMaximizeRect(point);	
			
		}
		else
		{
			//박스영역일때  		
			m_SelectRect=pImageDoc->GetImage().GetMinimizeRect(m_SelectRect);			
		}

		

		// 가로,세로 크기가 있을때만 프레임 추가 
		if ((m_SelectRect.Height()!=0)&&(m_SelectRect.Width()!=0))
		{
			if (pMainFrame->GetCurrMotionView()->GetSafeHwnd()!=NULL)
			{
				pMotionDoc=(CMotionDoc*)pMainFrame->GetCurrMotionView()->GetDocument();
				pMotionDoc->GetMoiton().InsertFrame(pImageDoc->GetImage(),m_SelectRect);
				pMotionDoc->SetModifiedFlag(TRUE);
				pMotionDoc->UpdateAllViews(this);
				pMainFrame->GetMotionAnmDlg().ReSize();
			}
			else
				::AfxMessageBox(_T("현재 선택된 모션창이 없습니다."));			
		}		
		m_SelectRect.SetRectEmpty();
	}
	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ((m_sizeScroll.cx > m_rtClient.right)||( m_sizeScroll.cy > m_rtClient.bottom))
	{
		m_bRButtonDrag=TRUE;
		SetCapture();
		SetCursor(::AfxGetApp()->LoadCursor(IDC_HANDGRIP));
	}
	
	CScrollView::OnRButtonDown(nFlags, point);
}

void CImageView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bRButtonDrag)
	{
		m_bRButtonDrag=FALSE;	
		ReleaseCapture();
	}

	CScrollView::OnRButtonUp(nFlags, point);
}

void CImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();

	if (nFlags == MK_LBUTTON)
	{
		if (m_bLButtonDrag)
		{
			
			m_SelectRect.right	= GetScrollPosition().x + point.x;
			m_SelectRect.bottom	= GetScrollPosition().y + point.y;
			
		}
	}
	else if (nFlags == MK_RBUTTON)
	{
		if (m_bRButtonDrag)
		{
			//// 캡쳐 윈도우를 점검한다
			if (GetCapture() == this)
			{
				CPoint ptScroll = GetScrollPosition();
				ptScroll -= (point - m_ptOldPos);
				SetScrollPos(SB_HORZ, ptScroll.x);
				SetScrollPos(SB_VERT, ptScroll.y);	
			}
		}

	}
	if (m_bSpoid)
	{
		SetCursor(::AfxGetApp()->LoadCursor(IDC_CURSOR2));
	}
	else if (m_bRButtonDrag)
	{
		SetCursor(::AfxGetApp()->LoadCursor(IDC_HANDGRIP));
	}
	else
	{
		
		SetCursor(::AfxGetApp()->LoadCursor(IDC_CHOOSE));
	}
	
		
	

	m_CurrPoint= CPoint(GetScrollPosition().x + point.x,GetScrollPosition().y + point.y);
	m_ptOldPos = point;

	Invalidate(FALSE);	
	pMainFrame->GetInfoDlg().SetImageViewCursorPoint(m_CurrPoint);
	pMainFrame->GetInfoDlg().Invalidate(FALSE);
	pMainFrame->Invalidate(FALSE);

//	TRACE("%d,%d\n",point.x,point.y);
	CScrollView::OnMouseMove(nFlags, point);
}

void CImageView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	GetClientRect(&m_rtClient);
}


LRESULT CImageView::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	return CScrollView::OnNcHitTest(point);
}

void CImageView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CScrollView::OnActivate(nState, pWndOther, bMinimized);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	TRACE("CImageView::OnActivate(%d,%x,%d)\n",nState,pWndOther,bMinimized);

}

void CImageView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();	
		
	if (bActivate)
	{		
		TRACE("CImageView::OnActivateView(%d,%x,%x) this=%x\n",bActivate,pActivateView,pDeactiveView,this);
		pMainFrame->SetCurrImageView(this);	
		pMainFrame->GetInfoDlg().Invalidate(FALSE);
		
	}	
		
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CImageView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int cx=0,cy=0;
	CImageDoc* pDoc=(CImageDoc*)GetDocument();

	if (!pDoc->GetImage().IsNull())
	{
	}
}

void CImageView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar==VK_CONTROL)
	{
		m_bSpoid=TRUE;		
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CImageView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar==VK_CONTROL)
	{
		m_bSpoid=FALSE;		
	}
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}
