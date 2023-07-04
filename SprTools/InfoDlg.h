#pragma once
#include "MemDC.h"

// CInfoDlg ��ȭ �����Դϴ�.

class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInfoDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_INFODLG };


public:
	CPoint		m_ImageViewCursorPoint;
	CMemDC		m_MemDC;



public:
	void SetImageViewCursorPoint(CPoint& point) { m_ImageViewCursorPoint=point;  }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
