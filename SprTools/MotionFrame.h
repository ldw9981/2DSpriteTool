#pragma once


#define	 WM_MOTIONDLGBAR WM_USER+100
// CMotionFrame 프레임입니다.

class CMotionFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMotionFrame)
protected:
	CMotionFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	afx_msg void OnMotionAdd();  //수동 코드
	afx_msg void OnMotionDel();	//수동 코드
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


