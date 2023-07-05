// MotionAnmDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SprTools.h"
#include "MotionAnmDlg.h"
#include "MainFrm.h"


// CMotionAnmDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMotionAnmDlg, CDialog)

CMotionAnmDlg::CMotionAnmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionAnmDlg::IDD, pParent)
{
	m_pMotionDoc=NULL;
	m_pMotionView=NULL;
	m_bAnimate=FALSE;
	m_ClientRect.SetRectEmpty();
}

CMotionAnmDlg::~CMotionAnmDlg()
{
}

void CMotionAnmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CMotionAnmDlg::IsValidCurrMotionView()
{
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	m_pMotionView=pMainFrame->GetCurrMotionView();
	m_pMotionDoc=pMainFrame->GetCurrMotionDoc();

	if ((m_pMotionView==NULL)||( !::IsWindow(m_pMotionView->m_hWnd)))
	{
		return FALSE;
	}
	if (m_pMotionDoc==NULL)
	{
		return FALSE;
	}
	return TRUE;
}

void CMotionAnmDlg::ReSize()
{
	if (!IsValidCurrMotionView())
		return;

	CSize size = m_pMotionDoc->GetMoiton().GetUnionRect().Size();

	size += CSize(1, 1);

	RECT rcClient = { 0, 0, size.cx * 4, size.cy * 4 };
	AdjustWindowRect(&rcClient, GetStyle(), FALSE);

	int CX = (rcClient.right - rcClient.left);
	int CY = (rcClient.bottom - rcClient.top);
	SetWindowPos(NULL, 0, 0, CX, CY, SWP_NOMOVE | SWP_NOACTIVATE);	
}

BEGIN_MESSAGE_MAP(CMotionAnmDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMotionAnmDlg 메시지 처리기입니다.

void CMotionAnmDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	TRACE(_T("CMotionAnmDlg::OnSize(%d,%d,%d)\n"),nType,cx,cy);

	m_MemDC.Uninit();
	CClientDC dc(this);
	m_MemDC.Init(&dc,cx,cy);
}

void CMotionAnmDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	m_MemDC.ClearBlack();


	m_MemDC.SelectStockObject(WHITE_PEN);
	m_MemDC.MoveTo(0					,m_MemDC.GetHeight()/2);
	m_MemDC.LineTo(m_MemDC.GetWidth()	,m_MemDC.GetHeight()/2);
	m_MemDC.MoveTo(m_MemDC.GetWidth()/2	,0);
	m_MemDC.LineTo(m_MemDC.GetWidth()/2	,m_MemDC.GetHeight());


	dc.BitBlt(0,0,m_MemDC.GetWidth(),m_MemDC.GetHeight(),&m_MemDC,0,0,SRCCOPY);
}

void CMotionAnmDlg::DrawTest()
{
	CClientDC dc(this);
	static	CPoint pt=CPoint(0,0);

	CRect rect(0,0,100,100);

	if (!m_bAnimate)
	{
		return;
	}

	pt.x++;
	if (pt.x>200)
	{
		pt.x=0;
	}
	

	rect.MoveToXY(pt);
	

	m_MemDC.Ellipse(&rect);

	dc.BitBlt(0,0,300,300,&m_MemDC,0,0,SRCCOPY);

}
BOOL CMotionAnmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.	

	int cx=300+::GetSystemMetrics(SM_CXDLGFRAME)*2;
	int	cy=300+::GetSystemMetrics(SM_CYDLGFRAME)*2+::GetSystemMetrics(SM_CYSMCAPTION);

	CRect temp;

	GetParent()->GetClientRect(&temp);
	SetWindowPos(NULL,
		temp.right-cx,temp.top,
		0,0,SWP_NOSIZE|SWP_NOACTIVATE);
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOACTIVATE);

	m_MemDC.Uninit();
	CClientDC dc(this);
	m_MemDC.Init(&dc,300,300);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMotionAnmDlg::DrawMotion()
{
	if (!m_bAnimate)
	{
		return;
	}
	CClientDC dc(this); // device context for painting

	m_MemDC.ClearBlack();
	if (IsValidCurrMotionView())
	{
		if(m_pMotionDoc->GetMoiton().CountFrame()!=0)
		{
			m_pMotionDoc->GetMoiton().DrawAnimation(&m_MemDC,m_MemDC.GetWidth()/2,m_MemDC.GetHeight()/2);
		}		
	}	

	

	m_MemDC.SelectStockObject(WHITE_PEN);
	m_MemDC.MoveTo(0					,m_MemDC.GetHeight()/2);
	m_MemDC.LineTo(m_MemDC.GetWidth()	,m_MemDC.GetHeight()/2);
	m_MemDC.MoveTo(m_MemDC.GetWidth()/2	,0);
	m_MemDC.LineTo(m_MemDC.GetWidth()/2	,m_MemDC.GetHeight());
	

	dc.BitBlt(0,0,m_MemDC.GetWidth(),m_MemDC.GetHeight(),&m_MemDC,0,0,SRCCOPY);
	Sleep(60);
}
void CMotionAnmDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bAnimate)
	{
		m_bAnimate=FALSE;
	}
	else
	{
		m_bAnimate=TRUE;
		ReSize();
	}
	CDialog::OnLButtonDown(nFlags, point);
}
