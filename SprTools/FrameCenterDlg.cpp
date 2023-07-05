// FrameCenterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SprTools.h"
#include "FrameCenterDlg.h"
#include "MainFrm.h"
#include "MotionFrame.h"

// CFrameCenterDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFrameCenterDlg, CDialog)

CFrameCenterDlg::CFrameCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrameCenterDlg::IDD, pParent)
{
	m_ClientPoint.SetPoint(0,0);
	m_CenterPoint.SetPoint(0,0);

	m_pMotionView=NULL;
	m_pMotionDoc=NULL;
	m_bDrag=FALSE;
	m_FrameSize.SetSize(0,0);
//	pFrame=NULL;
}

CFrameCenterDlg::~CFrameCenterDlg()
{
}

void CFrameCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFrameCenterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
//	ON_WM_ACTIVATE()
//ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CFrameCenterDlg �޽��� ó�����Դϴ�.

void CFrameCenterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.


	

	m_MemDC.ClearBlack();
	if (IsValidCurrMotionView())
	{
		CFrame *pFrame,*pFramePrev;		
		// ���� ������ 
		/*
		pFramePrev=m_pMotionDoc->GetMoiton().GetFrameSelectedPrev();
		if (pFramePrev!=NULL)
		{
			pFramePrev->TransparentBlt(m_MemDC.GetSafeHdc(),
				m_ClientPoint.x - pFramePrev->GetCenterX(),
				m_ClientPoint.y - pFramePrev->GetCenterY(),
				pFramePrev->GetWidth(),pFramePrev->GetHeight());			
		}			
		*/
		
		//���õ� ������
		pFrame=m_pMotionDoc->GetMoiton().GetFrameSelected();
		if (pFrame!=NULL)
		{
			pFrame->TransparentBlt(m_MemDC.GetSafeHdc(),
				m_ClientPoint.x - m_CenterPoint.x,
				m_ClientPoint.y - m_CenterPoint.y,
				pFrame->GetWidth(),pFrame->GetHeight());			
		}	
	}	

	CString str;
	str.Format(_T("%d,%d"),m_CenterPoint.x,m_CenterPoint.y);
	m_MemDC.TextOut(0,0,str);

	
	m_MemDC.SelectStockObject(WHITE_PEN   );
	m_MemDC.MoveTo(0					,m_MemDC.GetHeight()/2);
	m_MemDC.LineTo(m_MemDC.GetWidth()	,m_MemDC.GetHeight()/2);
	m_MemDC.MoveTo(m_MemDC.GetWidth()/2	,0);
	m_MemDC.LineTo(m_MemDC.GetWidth()/2	,m_MemDC.GetHeight());


	dc.BitBlt(0,0,m_MemDC.GetWidth(),m_MemDC.GetHeight(),&m_MemDC,0,0,SRCCOPY);
}

BOOL CFrameCenterDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
		LoadCursor(NULL, MAKEINTRESOURCE(IDC_HANDARROW)),
		(HBRUSH) GetStockObject(GRAY_BRUSH),
		LoadIcon(NULL, IDI_APPLICATION));


	return CDialog::PreCreateWindow(cs);
}

void CFrameCenterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	TRACE(_T("CFrameCenterDlg::OnSize(%d,%d,%d)\n"),nType,cx,cy);

	m_MemDC.Uninit();
	CClientDC dc(this);
	m_MemDC.Init(&dc,cx,cy);
}

BOOL CFrameCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMainFrame* pMainFrame;
	pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetWindowText(_T("CFrameCenterDlg"));
	int cx=300+::GetSystemMetrics(SM_CXDLGFRAME)*2;
	int	cy=300+::GetSystemMetrics(SM_CYDLGFRAME)*2+::GetSystemMetrics(SM_CYSMCAPTION);
	
	CRect temp;

	GetParent()->GetClientRect(&temp);
	SetWindowPos(NULL,
		temp.right-cx,temp.top,
		0,0,SWP_NOSIZE|SWP_NOACTIVATE);
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOACTIVATE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CFrameCenterDlg::ReSize( int width,int height )
{
	CSize size = m_pMotionDoc->GetMoiton().GetUnionRect().Size();

	if (size.cx == 0 || size.cy ==0)
		return;

	size += CSize(1,1);
	
	RECT rcClient = { 0, 0, size.cx *4, size.cy *4 };
	AdjustWindowRect(&rcClient, GetStyle(), FALSE);

	m_FrameSize.cx = width;
	m_FrameSize.cy = height;


	int CX = (rcClient.right - rcClient.left) ;
	int CY = (rcClient.bottom - rcClient.top) ;
	SetWindowPos(NULL, 0, 0, CX, CY, SWP_NOMOVE | SWP_NOACTIVATE);

	m_ClientPoint.SetPoint(size.cx *2, size.cy *2);
	m_CenterPoint.SetPoint(0, 0);
}
void CFrameCenterDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect temp;
	temp.left	=m_ClientPoint.x-m_CenterPoint.x;
	temp.top	=m_ClientPoint.y-m_CenterPoint.y;
	temp.right	=temp.left+m_FrameSize.cx;
	temp.bottom	=temp.top+m_FrameSize.cy;
	if (temp.PtInRect(point))
	{
		if (!m_bDrag)
		{
			SetCursor(::AfxGetApp()->LoadCursor(IDC_HANDARROW));
		}
		else
		{
			TRACE(_T("%d,%d\n"),point.x,point.y);
			m_CenterPoint += m_PrevPoint - point;			
			Invalidate(FALSE);					
			SetCursor(::AfxGetApp()->LoadCursor(IDC_HANDPICKUP));	
		}
		
	}	
	m_PrevPoint=point;		
	CDialog::OnMouseMove(nFlags, point);
}

void CFrameCenterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (IsValidCurrMotionView())
	{
		CRect temp;
		temp.left	=m_ClientPoint.x-m_CenterPoint.x;
		temp.top	=m_ClientPoint.y-m_CenterPoint.y;
		temp.right	=temp.left+m_FrameSize.cx;
		temp.bottom	=temp.top+m_FrameSize.cy;
		if (temp.PtInRect(point))
		{
			m_bDrag=TRUE;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CFrameCenterDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(IsValidCurrMotionView())
	{
		m_bDrag=FALSE;
		CFrame* pFrame;
		pFrame=m_pMotionDoc->GetMoiton().GetFrameSelected();
		if (pFrame!=NULL)
		{
			pFrame->SetCenterX(m_CenterPoint.x);
			pFrame->SetCenterY(m_CenterPoint.y);
			((CMotionFrame*)m_pMotionView->GetParent())->SetLeftDlgBarCenterX(m_CenterPoint.x);
			((CMotionFrame*)m_pMotionView->GetParent())->SetLeftDlgBarCenterY(m_CenterPoint.y);
			m_pMotionDoc->SetModifiedFlag(TRUE);
			Invalidate(FALSE);	
		}
	}


	CDialog::OnLButtonUp(nFlags, point);
}

void CFrameCenterDlg::Init(CFrame* pFrame )
{
	if (pFrame!=NULL)
	{
		ReSize(pFrame->GetWidth(),pFrame->GetHeight());
		m_CenterPoint.x=pFrame->GetCenterX();
		m_CenterPoint.y=pFrame->GetCenterY();
	}
	else
	{
		m_CenterPoint.SetPoint(0,0);
		m_ClientPoint.SetPoint(0,0);
		m_FrameSize.SetSize(0,0);
	}

	Invalidate(FALSE);
}

BOOL CFrameCenterDlg::IsValidCurrMotionView()
{
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	if ((pMainFrame==NULL)||(!::IsWindow(pMainFrame->m_hWnd)))
	{
		return FALSE;
	}
	m_pMotionView=pMainFrame->GetCurrMotionView();
	if ((m_pMotionView==NULL)||(!::IsWindow(m_pMotionView->m_hWnd)))
	{
		return FALSE;
	}	
	m_pMotionDoc=pMainFrame->GetCurrMotionDoc();	
	if (m_pMotionDoc==NULL)
	{
		return FALSE;
	}
	return TRUE;
}
void CFrameCenterDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(!IsValidCurrMotionView())
		return;

	BOOL bChange=FALSE;

	switch(nChar)
	{
	case VK_DELETE:
		m_CenterPoint.SetPoint(0,0);
		bChange=TRUE;
		break;
	case VK_NUMPAD2:
		m_CenterPoint.y--;
		bChange=TRUE;
		break;
	case VK_NUMPAD8:
		m_CenterPoint.y++;
		bChange=TRUE;
		break;
	case VK_NUMPAD6:
		m_CenterPoint.x--;
		bChange=TRUE;
		break;
	case VK_NUMPAD4:
		m_CenterPoint.x++;
		bChange=TRUE;
		break;
	}

	if (bChange)
	{
		CFrame* pFrame;
		pFrame=m_pMotionDoc->GetMoiton().GetFrameSelected();
		if (pFrame!=NULL)
		{
			pFrame->SetCenter(m_CenterPoint);
			m_pMotionDoc->SetModifiedFlag(TRUE);
			Invalidate(FALSE);
		}		
	}
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFrameCenterDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CFrameCenterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	BOOL bChange=FALSE;	

	if (pMsg->message == WM_KEYDOWN)
	{

		if(IsValidCurrMotionView())
		{
			switch(pMsg->wParam)
			{
			case VK_LEFT:
				m_CenterPoint.x++;
				bChange=TRUE;
				break;
			case VK_RIGHT:
				m_CenterPoint.x--;
				bChange=TRUE;
				break;
			case VK_UP:
				m_CenterPoint.y++;
				bChange=TRUE;
				break;
			case VK_DOWN:
				m_CenterPoint.y--;
				bChange=TRUE;
				break;		
			}
			if (bChange)
			{
				CFrame* pFrame;
				pFrame=m_pMotionDoc->GetMoiton().GetFrameSelected();
				if (pFrame!=NULL)
				{
					pFrame->SetCenter(m_CenterPoint);
					m_pMotionDoc->SetModifiedFlag(TRUE);
					Invalidate(FALSE);
				}		
			}
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

//void CFrameCenterDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialog::OnActivate(nState, pWndOther, bMinimized);
//	
//	if (nState==)
//	{
//	}
//
//	// TODO: Add your message handler code here
//}

//BOOL CFrameCenterDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//	// TODO: Add your message handler code here and/or call default
//	return CDialog::OnSetCursor(pWnd, nHitTest, message);
//}
