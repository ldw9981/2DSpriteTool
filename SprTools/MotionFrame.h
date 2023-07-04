#pragma once


#define	 WM_MOTIONDLGBAR WM_USER+100
// CMotionFrame �������Դϴ�.

class CMotionFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMotionFrame)
protected:
	CMotionFrame();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMotionFrame();

public:
	CDialogBar  m_TopDlgBar;
	CDialogBar	m_LeftDlgBar;

	CMotion*	m_pCurrMotion;
public:
	int		GetLeftDlgBarCenterX();
	void	SetLeftDlgBarCenterX(int val);
	int		GetLeftDlgBarCenterY();
	void	SetLeftDlgBarCenterY(int val);

	int		GetLeftDlgBarDelayTime();
	void	SetLeftDlgBarDelayTime(int val);

//	CMotion* GetTopDlgBarTabCurSel();

	void	ReNameTabItem();
protected:
	DECLARE_MESSAGE_MAP()
//	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMotionAdd();  //���� �ڵ�
	afx_msg void OnMotionDel();	//���� �ڵ�
	afx_msg void OnDelaySet();
	afx_msg void OnDelayBatch();
	afx_msg void OnCenterSet();
	afx_msg void OnCenterBatch();

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
//	virtual void ActivateFrame(int nCmdShow = -1);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};


