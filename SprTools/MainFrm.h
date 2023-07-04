// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "ImageView.h"
#include "MotionView.h"
#include "FrameCenterDlg.h"
#include "MotionAnmDlg.h"
#include "InfoDlg.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

protected:
	

	
// Operations
public:	
	
	// 메인프레임에 현재 선택된 ImageView를 등록한다.
	void			SetCurrImageView(CImageView* pImageView);
	
	// 현재 선택된 ImageView를 얻는다.
	CImageView*		GetCurrImageView() { return m_pCurrImageView; }
	
	// 현재 선택된 ImageDoc를 얻는다.
	CImageDoc*		GetCurrImageDoc();

	// 메인프레임에 현재 선택된 MotionView를 등록한다.
	void			SetCurrMotionView(CMotionView* pMotionView) ;
	
	// 현재 선택된 MotionView를 얻는다.
	CMotionView*	GetCurrMotionView() { return m_pCurrMotionView; }

	// 현재 선택된 MotionDoc를 얻는다.
	CMotionDoc*		GetCurrMotionDoc();


	CFrameCenterDlg&	GetFrameCenterDlg() { return m_FrameCenterDlg; } 
	CMotionAnmDlg&		GetMotionAnmDlg()	{ return m_MotionAnmDlg; } 
	CInfoDlg&			GetInfoDlg()		{ return m_InfoDlg; }

//	CRect&			GetClientRect() { return m_ClientRect; }
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;

//	CRect			m_ClientRect;

	CImageView*		m_pCurrImageView;
	CMotionView*	m_pCurrMotionView;
	
	CFrameCenterDlg	m_FrameCenterDlg;
	CMotionAnmDlg	m_MotionAnmDlg;
	CInfoDlg		m_InfoDlg;

// Gen
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnInfoOpen();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


