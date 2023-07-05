// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SprTools.h"

#include "MainFrm.h"
#include "SprTools.h"

#include "FrameCenterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_DROPFILES()

	ON_COMMAND(ID_INFO_OPEN, &CMainFrame::OnInfoOpen)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
//	ON_WM_DESTROY()
//	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pCurrImageView=NULL;	
	m_pCurrMotionView=NULL;
//	m_ClientRect.SetRectEmpty();
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	DragAcceptFiles(TRUE);

	CRect ClientRect;
	GetClientRect(&ClientRect);


	int cx=300+::GetSystemMetrics(SM_CXDLGFRAME)*2;
	int	cy=300+::GetSystemMetrics(SM_CYDLGFRAME)*2+::GetSystemMetrics(SM_CYSMCAPTION);

	
	m_InfoDlg.Create(IDD_INFODLG,this);
	m_InfoDlg.ShowWindow(SW_SHOW);


	m_FrameCenterDlg.Create(IDD_FRAMECENTER,this);
	m_FrameCenterDlg.ShowWindow(SW_SHOW);

	m_MotionAnmDlg.Create(IDD_MOTIONANMDLG,this);	
	m_MotionAnmDlg.ShowWindow(SW_SHOW);	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	cs.style |=WS_MAXIMIZE;
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	int nFileNum;
	TCHAR strFilePath[MAX_PATH];

	nFileNum = ::DragQueryFile( hDropInfo, 0xFFFFFFFF, strFilePath, MAX_PATH );     // nFileNum = Drop된 File의 갯수

	for( int i=nFileNum-1; i >= 0; i-- )
	{
		::DragQueryFile( hDropInfo, i, strFilePath, MAX_PATH ); // File의 경로를 구한다.

		CString temp=CString(strFilePath).Right(3);

		if (temp.CompareNoCase(_T("BMP"))==0)
		{
			CSprToolsApp* pApp=(CSprToolsApp*)::AfxGetApp();
			pApp->OpenImageDocTemplate(strFilePath);
		}
		else
			::AfxMessageBox(_T("BMP파일만 지원합니다"));
		
	}

	::DragFinish( hDropInfo );  // Drag 의 끝을 알리는 API함수

//  첫번째 DocTemplate열리므로 주석처리
//	CMDIFrameWnd::OnDropFiles(hDropInfo);
}






void CMainFrame::OnInfoOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_InfoDlg.ShowWindow(SW_SHOW);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	TRACE(_T("CMainFrame::OnSize(%d,%d,%d)\n"),nType,cx,cy);

	CRect ClientRect;
	GetClientRect(&ClientRect);

	CRect temp;
	m_wndToolBar.GetClientRect(&temp);

	int addcy=::GetSystemMetrics(SM_CYBORDER)+::GetSystemMetrics(SM_CYMENU)+
		::GetSystemMetrics(SM_CYCAPTION)+temp.Height()+::GetSystemMetrics(SM_CYEDGE);

	m_InfoDlg.SetWindowPos(NULL,ClientRect.right-300,ClientRect.top+addcy,
		0,0,SWP_NOSIZE);

	m_FrameCenterDlg.SetWindowPos(NULL,ClientRect.right-300,ClientRect.top+100+200,
		0,0,SWP_NOSIZE);	
	m_MotionAnmDlg.SetWindowPos(NULL,ClientRect.right-300,ClientRect.Height()/5*4,
		0,0,SWP_NOSIZE);	

	

}


CMotionDoc* CMainFrame::GetCurrMotionDoc()
{
	CMotionDoc* pDoc=NULL;
	if ((m_pCurrMotionView!=NULL)&&(::IsWindow(m_pCurrMotionView->m_hWnd)!=NULL))
	{
		pDoc=(CMotionDoc*)m_pCurrMotionView->GetDocument();
	}
	return pDoc;	
}

CImageDoc* CMainFrame::GetCurrImageDoc()
{
	CImageDoc* pDoc=NULL;
	if ((m_pCurrImageView!=NULL)&&(::IsWindow(m_pCurrImageView->m_hWnd)!=NULL))
	{
		pDoc=(CImageDoc*)m_pCurrImageView->GetDocument();
	}
	return pDoc;	
}

void CMainFrame::SetCurrMotionView( CMotionView* pMotionView )
{
	m_pCurrMotionView=pMotionView; 
	if (m_pCurrMotionView==NULL)
	{
		m_FrameCenterDlg.Init(NULL);
		m_MotionAnmDlg.SetAnimate(FALSE);
	}
	else
	{		
		m_FrameCenterDlg.Init(GetCurrMotionDoc()->GetMoiton().GetFrameSelected());
		m_MotionAnmDlg.SetAnimate(TRUE);
	}
}

void CMainFrame::SetCurrImageView( CImageView* pImageView )
{ 
	m_pCurrImageView=pImageView; 
	if (m_pCurrImageView==NULL)
	{
//		m_FrameCenterDlg.Invalidate(TRUE);
	}
}
