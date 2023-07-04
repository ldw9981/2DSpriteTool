#pragma once


// CImageFrame frame

class CImageFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CImageFrame)
protected:
	CImageFrame();           // protected constructor used by dynamic creation
	virtual ~CImageFrame();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


