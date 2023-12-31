#pragma once
#include "MemDC.h"

// CInfoDlg 대화 상자입니다.

class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INFODLG };


public:
	CPoint		m_ImageViewCursorPoint;
	CMemDC		m_MemDC;



public:
	void SetImageViewCursorPoint(CPoint& point) { m_ImageViewCursorPoint=point;  }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
