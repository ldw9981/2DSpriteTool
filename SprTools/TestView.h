#pragma once



// CTestView ���Դϴ�.

class CTestView : public CScrollView
{
//	DECLARE_DYNCREATE(CTestView)

public:
	CTestView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CTestView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();     // ������ �� ó���Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
	afx_msg void OnDestroy();
};


