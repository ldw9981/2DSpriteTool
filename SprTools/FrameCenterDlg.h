#pragma once
#include "MemDC.h"
#include "Frame.h"
#include "MotionView.h"
#include "MotionDoc.h"
// CFrameCenterDlg ��ȭ �����Դϴ�.

class CFrameCenterDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameCenterDlg)

public:
	CFrameCenterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFrameCenterDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FRAMECENTER };
public:
	CMemDC	m_MemDC;

	CPoint	m_ClientPoint;
	CPoint	m_CenterPoint;
	CPoint	m_PrevPoint;
	CSize	m_FrameSize;

	BOOL	m_bDrag;

	CMotionView*	m_pMotionView;
	CMotionDoc*		m_pMotionDoc;

//	CFrame* pFrame;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

public:
	BOOL IsValidCurrMotionView();
	void Init(CFrame* pFrame );
	void ReSize(int width,int height);
//	void SetFrame(CFrame* pFrame);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
