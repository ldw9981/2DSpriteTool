// ImageFrame.cpp : implementation file
//

#include "stdafx.h"
#include "SprTools.h"
#include "ImageFrame.h"


// CImageFrame

IMPLEMENT_DYNCREATE(CImageFrame, CMDIChildWnd)

CImageFrame::CImageFrame()
{

}

CImageFrame::~CImageFrame()
{
}


BEGIN_MESSAGE_MAP(CImageFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CImageFrame message handlers

BOOL CImageFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
//	cs.style = WS_CHILD| WS_VISIBLE | WS_BORDER |WS_MINIMIZEBOX|WS_CAPTION|WS_SYSMENU;
	cs.style ^= WS_MAXIMIZEBOX;	
	return CMDIChildWnd::PreCreateWindow(cs);
}
