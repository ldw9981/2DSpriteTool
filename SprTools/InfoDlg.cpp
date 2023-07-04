// InfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SprTools.h"
#include "InfoDlg.h"
#include "MainFrm.h"


// CInfoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
{
	m_ImageViewCursorPoint=CPoint(0,0);
}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CInfoDlg 메시지 처리기입니다.

void CInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	

	m_MemDC.TextOut(0,16*0,CString(_T("CInfoWnd")));

	CString temp;

	temp.Format(_T("%.4d,%.4d"),m_ImageViewCursorPoint.x,m_ImageViewCursorPoint.y );
	m_MemDC.TextOut(0,16*1,temp);
	temp.Empty();

	CImageView* pImageView=(CImageView*)pMainFrame->GetCurrImageView();


	temp = _T("Image: ");


	if ((pImageView!=NULL)&&(::IsWindow(pImageView->m_hWnd)!=NULL))
	{
		CImageDoc* pImageDoc=(CImageDoc*)pImageView->GetDocument();
		//		pImageDoc->GetFirstViewPosition()
		temp += pImageDoc->GetTitle();		
		m_MemDC.TextOut(0,16*2,temp);

		temp.Empty();
		temp.Format(_T("투명색 [%3d,%3d,%3d][%6X]"),
			GetRValue(pImageDoc->GetImage().GetTransparentColor()),
			GetGValue(pImageDoc->GetImage().GetTransparentColor()),
			GetBValue(pImageDoc->GetImage().GetTransparentColor()),
			pImageDoc->GetImage().GetTransparentColor());
		m_MemDC.TextOut(16,16*3,temp);

		CBrush Brush;
		Brush.CreateSolidBrush(pImageDoc->GetImage().GetTransparentColor());

		m_MemDC.SelectObject(Brush);
		m_MemDC.Rectangle(CRect(0,16*3,16,16*4));
	}
	else
	{
		pMainFrame->SetCurrImageView(NULL);
		temp += _T("선택 창 없음");
		m_MemDC.TextOut(0,16*2,temp);
	}

	
	temp.Empty();

	CMotionView* pMotionView=(CMotionView*)pMainFrame->GetCurrMotionView();
	temp = _T("Motion: ");
	if((pMotionView!=NULL)&&(::IsWindow(pMotionView->m_hWnd)!=NULL))
	{
		CMotionDoc* pMotionDoc=(CMotionDoc*)pMotionView->GetDocument();
		temp += pMotionDoc->GetTitle();		
	}
	else
	{
		pMainFrame->SetCurrMotionView(NULL);
		temp += _T("선택 창 없음");
	}
	m_MemDC.TextOut(0,16*4,temp);
	temp.Empty();

	
	dc.BitBlt(0,0,300,200,&m_MemDC,0,0,SRCCOPY);

	
}

BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int cx=300+::GetSystemMetrics(SM_CXDLGFRAME)*2;
	int	cy=200+::GetSystemMetrics(SM_CYDLGFRAME)*2+::GetSystemMetrics(SM_CYSMCAPTION);

	CRect temp;

	GetParent()->GetClientRect(&temp);
	SetWindowPos(NULL,
		temp.right-cx,temp.top,
		0,0,SWP_NOSIZE|SWP_NOACTIVATE);
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOACTIVATE);

	CClientDC dc(this);
	
	m_MemDC.Init(&dc,300,200);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
