#pragma once
#include "MemDC.h"
#include "MotionDoc.h"
#include "MotionView.h"


// CMotionAnmDlg 대화 상자입니다.

class CMotionAnmDlg : public CDialog
{
	DECLARE_DYNAMIC(CMotionAnmDlg)

public:
	CMotionAnmDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMotionAnmDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOTIONANMDLG };

public:
	CMemDC	m_MemDC;

	CMotionView*	m_pMotionView;
	CMotionDoc*		m_pMotionDoc;
	CRect			m_ClientRect;		
		
	BOOL			m_bAnimate;

public:

	BOOL GetAnimate() const { return m_bAnimate; }
	void SetAnimate(BOOL val) { m_bAnimate = val; }
	BOOL IsValidCurrMotionView();
//	void Init(CFrame* pFrame );

	void ReSize();
	void DrawTest();
	void DrawMotion();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
