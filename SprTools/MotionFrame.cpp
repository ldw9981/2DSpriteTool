// MotionFrame.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SprTools.h"
#include "MotionFrame.h"
#include "MotionDoc.h"
#include "MainFrm.h"

// CMotionFrame

IMPLEMENT_DYNCREATE(CMotionFrame, CMDIChildWnd)

CMotionFrame::CMotionFrame()
{
	m_pCurrMotion=NULL;
}

CMotionFrame::~CMotionFrame()
{
}


BEGIN_MESSAGE_MAP(CMotionFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_MOTION_ADD,OnMotionAdd)		// ���� �ڵ�
	ON_BN_CLICKED(IDC_MOTION_DEL,OnMotionDel)		// ���� �ڵ�

	ON_BN_CLICKED(IDC_DELAY_SET,OnDelaySet)		// ���� �ڵ�
	ON_BN_CLICKED(IDC_DELAY_BATCH,OnDelayBatch)		// ���� �ڵ�

	ON_BN_CLICKED(IDC_CENTER_SET,OnCenterSet)		// ���� �ڵ�
	ON_BN_CLICKED(IDC_CENTER_BATCH,OnCenterBatch)		// ���� �ڵ�
//	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CMotionFrame �޽��� ó�����Դϴ�.

//BOOL CMotionFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
//}

int CMotionFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//// ���� ������ ��ŷ ���
	EnableDocking(CBRS_ALIGN_ANY);

	//// ���̾�α׹� ����
	m_LeftDlgBar.Create(this,
		IDD_MOTION_LEFTDLGBAR,	// ���ҽ� ID 
		CBRS_LEFT,		// ��ŷ ��ġ
		WM_MOTIONDLGBAR);			// ���̾�α׹� ID

	//// ���̾�α׹� ��ŷ ���� ����
	m_LeftDlgBar.EnableDocking(CBRS_ALIGN_LEFT);

	m_TopDlgBar.Create(this,
		IDD_MOTION_TOPDLGBAR,	// ���ҽ� ID 
		CBRS_TOP,		// ��ŷ ��ġ
		WM_MOTIONDLGBAR);			// ���̾�α׹� ID

	//// ���̾�α׹� ��ŷ ���� ����
	m_TopDlgBar.EnableDocking(CBRS_ALIGN_TOP);


	//// ���̾�α׹� ��ŷ
	DockControlBar(&m_TopDlgBar);

	//// ���̾�α׹� ��ŷ
	DockControlBar(&m_LeftDlgBar);	
	return 0;
}

void CMotionFrame::OnMotionAdd()
{

	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("No Number");

	CTabCtrl *pTabCtrl=(CTabCtrl *)m_TopDlgBar.GetDlgItem(IDC_MOTION_TAB);
	pTabCtrl->InsertItem(0xfffffff, &tcItem);
	ReNameTabItem();

	m_pCurrMotion=((CMotionDoc*)GetActiveDocument())->GetMoitonGroup().AddMotion();	
	((CMotionDoc*)GetActiveDocument())->UpdateAllViews(NULL);
	//UpdateView
}

void CMotionFrame::OnMotionDel()
{
	int index;
	index=TabCtrl_GetCurSel( ((CTabCtrl *)m_TopDlgBar.GetDlgItem(IDC_MOTION_TAB))->GetSafeHwnd() );
	
	CTabCtrl *pTabCtrl=(CTabCtrl *)m_TopDlgBar.GetDlgItem(IDC_MOTION_TAB);
	pTabCtrl->DeleteItem(index);
	ReNameTabItem();


	((CMotionDoc*)GetActiveDocument())->GetMoitonGroup().DelMotion(index);
	m_pCurrMotion=((CMotionDoc*)GetActiveDocument())->GetMoitonGroup().GetMotionSelected();
}

void CMotionFrame::OnDelaySet()
{
	if (((CMotionDoc*)GetActiveDocument())->GetMoiton().CountFrame()==0)
	{
		return;
	}	

	CFrame *pFrame=((CMotionDoc*)GetActiveDocument())->GetMoiton().GetFrameSelected();
	pFrame->SetDelayTime(GetLeftDlgBarDelayTime());
}

void CMotionFrame::OnDelayBatch()
{
	if (((CMotionDoc*)GetActiveDocument())->GetMoiton().SetDelayTimeAll(GetLeftDlgBarDelayTime()))
	{
		((CMotionDoc*)GetActiveDocument())->SetModifiedFlag();
	}
}

void CMotionFrame::OnCenterSet()
{
	if (((CMotionDoc*)GetActiveDocument())->GetMoiton().CountFrame()==0)
	{
		return;
	}
	CFrame *pFrame=((CMotionDoc*)GetActiveDocument())->GetMoiton().GetFrameSelected();
	pFrame->SetCenterX(GetLeftDlgBarCenterX());
	pFrame->SetCenterY(GetLeftDlgBarCenterY());
	((CMainFrame*)::AfxGetMainWnd())->GetFrameCenterDlg().Init(pFrame);
	
}

void CMotionFrame::OnCenterBatch()
{
	CPoint pt;
	pt.x=GetLeftDlgBarCenterX();
	pt.y=GetLeftDlgBarCenterY();
	if (((CMotionDoc*)GetActiveDocument())->GetMoiton().SetCenterAll(pt))
	{
		((CMotionDoc*)GetActiveDocument())->SetModifiedFlag();
	}
}

int CMotionFrame::GetLeftDlgBarCenterX()
{
	CString tempStr;
	int	nRet;
	m_LeftDlgBar.GetDlgItemText(IDC_CENTER_X,tempStr);
	nRet = _ttoi(tempStr);
	return nRet;
}

int CMotionFrame::GetLeftDlgBarCenterY()
{
	CString tempStr;
	int	nRet;
	m_LeftDlgBar.GetDlgItemText(IDC_CENTER_Y,tempStr);
	nRet = _ttoi(tempStr);
	return nRet;
}

void CMotionFrame::SetLeftDlgBarCenterX( int val )
{
	CString tempStr;
	tempStr.Format(_T("%d"),val);
	m_LeftDlgBar.SetDlgItemText(IDC_CENTER_X,tempStr);
}

void CMotionFrame::SetLeftDlgBarCenterY( int val )
{
	CString tempStr;
	tempStr.Format(_T("%d"),val);
	m_LeftDlgBar.SetDlgItemText(IDC_CENTER_Y,tempStr);
}

int CMotionFrame::GetLeftDlgBarDelayTime()
{
	CString tempStr;
	int	nRet;
	m_LeftDlgBar.GetDlgItemText(IDC_DELAY_TIME,tempStr);
	nRet = _ttoi(tempStr);
	return nRet;
}

void CMotionFrame::SetLeftDlgBarDelayTime( int val )
{
	CString tempStr;
	tempStr.Format(_T("%d"),val);
	m_LeftDlgBar.SetDlgItemText(IDC_DELAY_TIME,tempStr);
}
BOOL CMotionFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	TRACE(_T("CMotionFrame::OnNotify(%x,%x,%x)\n"),wParam,lParam,pResult);
	
	int index;
	LPNMHDR lpnmhdr;
	lpnmhdr = (LPNMHDR) lParam; 


	switch(wParam)
	{
	case IDC_MOTION_TAB:
		{
			switch (lpnmhdr->code) 
			{ 
			case TCN_SELCHANGE: 
				{ 				
					TRACE(_T("TCN_SELCHANGE\n"));
					index=TabCtrl_GetCurSel( ((CTabCtrl *)m_TopDlgBar.GetDlgItem(IDC_MOTION_TAB))->GetSafeHwnd() );
					m_pCurrMotion=((CMotionDoc*)GetActiveDocument())->GetMoitonGroup().GetMotion(index);
				} 
				break;
			case TCN_FOCUSCHANGE:
				{
					TRACE(_T("TCN_FOCUSCHANGE\n"));
				}
				break;
			} 	
		}
		break;	
	}
	

	return CMDIChildWnd::OnNotify(wParam, lParam, pResult);
}

void CMotionFrame::ReNameTabItem()
{
	int cnt;
	CTabCtrl* pTabCtrl=(CTabCtrl *)m_TopDlgBar.GetDlgItem(IDC_MOTION_TAB);
	cnt=pTabCtrl->GetItemCount();
	if (cnt==0)
	{
		return;
	}

	
	TCHAR	szBuffer[10]={0,};
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = szBuffer;

	for (int i=0;i<cnt;i++)	
	{
		memset(&szBuffer,0,sizeof(szBuffer));
		_itow_s(i,szBuffer,10);

	
		pTabCtrl->SetItem(i,&tcItem);
	}
	



}
//void CMotionFrame::ActivateFrame(int nCmdShow)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	CMDIChildWnd::ActivateFrame(nCmdShow);
//}

//void CMotionFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CMDIChildWnd::OnActivate(nState, pWndOther, bMinimized);
//
//	// TODO: Add your message handler code here
//}
