#include "StdAfx.h"
#include "Motion.h"

CMotion::CMotion(void)
{
	memset(&m_MotionHeader,0,sizeof(m_MotionHeader));
	m_MotionHeader.BitCountNum=32;
	m_SpreadRect.SetRectEmpty();
	m_UnionRect.SetRectEmpty();
	m_DrawPos=NULL;
	m_nSelectedFrameIndex=0;
	m_bIsSelected=FALSE;
	m_bDrag=FALSE;
	m_nDragedFrameIndex=0;
}

CMotion::~CMotion(void)
{
	if (m_listFrame.GetSize()!=0)
	{
		DeleteAll();
	}
}

void CMotion::InsertFrame( CDIB& Src,CRect& SrcRect )
{
	CFrame* pItem;
	pItem=new CFrame(Src,SrcRect);
	m_listFrame.AddTail(pItem);	

	ExpandSpreadRect(SrcRect.Width(),SrcRect.Height());
	ExpandUnionRect(CRect(0,0,SrcRect.Width()-1,SrcRect.Height()-1));
}

void CMotion::Serialize( CArchive& ar )
{
	POSITION pos=NULL;
	CFrame* pFrame=NULL;

	if (ar.IsStoring())
	{
		m_MotionHeader.FrameCountNum=(int)m_listFrame.GetSize();
		ar.Write(&m_MotionHeader,sizeof(m_MotionHeader));		// m_MotionHeader 저장
		pos = m_listFrame.GetHeadPosition();
		while (pos!=NULL)
		{
			pFrame = m_listFrame.GetNext(pos);
			pFrame->Serialize(m_MotionHeader.BitCountNum,ar);	// CFrame::Serialize()호출 
		}
	} 
	else
	{
		ar.Read(&m_MotionHeader,sizeof(m_MotionHeader));		// m_MotionHeader 로드
		
		if (m_MotionHeader.FrameCountNum>0)
		{
			for (int i=0;i<m_MotionHeader.FrameCountNum;i++)
			{
				pFrame=new CFrame;
				pFrame->Serialize(m_MotionHeader.BitCountNum,ar);	// 리스트의 CFrame::Serialize()호출
				m_listFrame.AddTail(pFrame);

				//pFrame->SetTransparentColorAuto();
				ExpandSpreadRect(pFrame->GetWidth(),pFrame->GetHeight());
				ExpandUnionRect(CRect(0,0,pFrame->GetWidth()-1,pFrame->GetHeight()-1));
			}
		}		
	}
}

void CMotion::SerializeOut(std::ofstream& os)
{
	int FrameCountNum = (int)m_listFrame.GetSize();

	std::string strNumber = std::to_string(FrameCountNum);
	os << strNumber << std::endl;
	CFrame* pFrame = NULL;
	POSITION pos = NULL;
	pos = m_listFrame.GetHeadPosition();
	while (pos != NULL)
	{
		pFrame = m_listFrame.GetNext(pos);
		pFrame->SerializeOut(os);
	}
}

void CMotion::DrawSpread( CDC* pDC )
{
	int x=0,y=0;
	POSITION pos;
	CFrame* pFrame;
	pos = m_listFrame.GetHeadPosition();
	while (pos!=NULL)
	{
		pFrame = m_listFrame.GetNext(pos);
		pFrame->BitBlt(pDC->GetSafeHdc(),x,y,pFrame->GetWidth(),pFrame->GetHeight());
		x+=pFrame->GetWidth();
	}	

	DrawGauze(pDC,m_SelectedFrameRect);


	if (m_bDrag)
	{
		DrawGauze(pDC,m_InsertRect);
	}
}

int CMotion::CountFrame()
{
	return (int)m_listFrame.GetSize();
}

void CMotion::ExpandSpreadRect( int width,int height )
{
	m_SpreadRect.right	+= width;
	m_SpreadRect.bottom	 = max(height-1,m_SpreadRect.bottom);
}

void CMotion::DeleteAll()
{	
	if (m_listFrame.GetSize()==0)
		return;
	
	POSITION pos;
	CFrame* pFrame;
	pos = m_listFrame.GetHeadPosition();
	while (pos!=NULL)
	{
		pFrame = m_listFrame.GetNext(pos);
		delete pFrame;

	}
	m_listFrame.RemoveAll();
}

CFrame* CMotion::GetFrame( int index )
{
	POSITION pos;
	CFrame* pRet=NULL;

	if (index<0)
	{
		return pRet;
	}

	pos=m_listFrame.FindIndex(index);
	if (pos!=NULL)
	{
		pRet=m_listFrame.GetAt(pos);
	}
	return pRet;
}

CFrame* CMotion::FindFrame( CPoint& ptInput,CRect& rtOuput,int& idxOutput )
{
	POSITION pos;
	CFrame* pRet=NULL;
	CFrame* pTemp;
	int	index=0;
	CRect ClientRect;
	CRect FrameRect;

	ClientRect.SetRectEmpty();
	FrameRect.SetRectEmpty();

	pos = m_listFrame.GetHeadPosition();

	while (pos!=NULL)
	{
		pTemp = m_listFrame.GetNext(pos);
		if(pTemp!=NULL)
		{
			FrameRect.left	= 0;
			FrameRect.top	= 0;
			FrameRect.right = pTemp->GetWidth()-1;
			FrameRect.bottom= pTemp->GetHeight()-1;

			ClientRect.right+=pTemp->GetWidth();			
			ClientRect.top=0;
			ClientRect.bottom=pTemp->GetHeight();
			
			if (ClientRect.PtInRect(ptInput))
			{
				rtOuput=ClientRect;
				idxOutput=index;
				pRet=pTemp;
				break;
			}
			ClientRect.left+=pTemp->GetWidth();
			index++;
		}	
	}
	return pRet;
}

void CMotion::DrawAnimation( CDC* pDC,int x,int y)
{		
	if (m_listFrame.GetCount()==0)
		return;

	if (m_DrawPos==NULL)
	{
		m_DrawPos=m_listFrame.GetHeadPosition();
	}	

	CFrame* pFrame;
	pFrame = m_listFrame.GetNext(m_DrawPos);
	if (pFrame!=NULL)
	{
		pFrame->TransparentBlt(pDC->GetSafeHdc(),x-pFrame->GetCenterX(),y-pFrame->GetCenterY(),pFrame->GetWidth(),pFrame->GetHeight());
	}
}

void CMotion::ExpandUnionRect( CRect& rect )
{
	m_UnionRect.UnionRect(&m_UnionRect,rect);
}

void CMotion::DrawGauze(CDC* pDC,CRect& rect )
{

	CPoint DrawPoint=rect.TopLeft();

	for (DrawPoint.y=rect.top;DrawPoint.y<=rect.bottom;DrawPoint.y++)
	{
		for (DrawPoint.x=rect.left;DrawPoint.x<=rect.right;DrawPoint.x++)
		{
			if (DrawPoint.y%2==0)
			{
				if (DrawPoint.x%2==1)
				{
					pDC->SetPixel(DrawPoint,RGB(0,0,0));
				}				
			}
			else
			{
				if (DrawPoint.x%2==0)
				{
					pDC->SetPixel(DrawPoint,RGB(0,0,0));
				}
			}

		}
	}	
}

CFrame* CMotion::SelectFrame( CPoint& pt )
{
	CFrame* pFrame=FindFrame(pt,m_SelectedFrameRect,m_nSelectedFrameIndex);	
	if (pFrame!=NULL)
	{
		m_bIsSelected=TRUE;
	}
	return pFrame;
}

CFrame* CMotion::SelectFrame( int index,CRect& rtClientRect )
{
	CFrame* pFrame;
	
	if ((index < 0)||(m_listFrame.GetCount()<=index ))
	{
		return NULL;
	}

	rtClientRect=GetFrameClientRect(index);
	pFrame=SelectFrame(CPoint(rtClientRect.left,rtClientRect.top));
	if (pFrame!=NULL)
	{
		m_bIsSelected=TRUE;
	}
	return pFrame;
}
CFrame* CMotion::GetFrameSelected()
{
	return GetFrame(m_nSelectedFrameIndex);
}

CFrame* CMotion::GetFrameSelectedPrev()
{
	return GetFrame(m_nSelectedFrameIndex-1);
}
/*
CRect CMotion::GetRectSelectedFrameClientRect()
{
	return GetFrameClientRect(m_nSelectedFrameIndex);
}
*/

CRect CMotion::GetFrameClientRect( int index )
{
	POSITION pos;
	CRect Ret;

	CFrame* pTemp;
	int	cntIndex=0;
	CRect ClientRect;
	CRect FrameRect;

	ClientRect.SetRectEmpty();
	FrameRect.SetRectEmpty();

	pos = m_listFrame.GetHeadPosition();

	while (pos!=NULL)
	{
		pTemp = m_listFrame.GetNext(pos);
		if(pTemp!=NULL)
		{
			FrameRect.left	= 0;
			FrameRect.top	= 0;
			FrameRect.right = pTemp->GetWidth()-1;
			FrameRect.bottom= pTemp->GetHeight()-1;

			ClientRect.right+=pTemp->GetWidth();			
			ClientRect.left= ClientRect.right - pTemp->GetWidth();
			ClientRect.top=0;
			ClientRect.bottom=pTemp->GetHeight();

			if (cntIndex==index)
			{
				Ret=ClientRect;				
				break;
			}
			ClientRect.left+=pTemp->GetWidth();
			cntIndex++;
		}	
	}
	return Ret;
}

void CMotion::DeleteFrame( int index )
{
	if (m_listFrame.GetSize()==0)
		return;

	int cntIndex=0;
	POSITION pos;
	CFrame* pFrame;

	pos= m_listFrame.FindIndex(index);
	if (pos==NULL)
		return;

	pFrame=m_listFrame.GetAt(pos);
	if (pFrame==NULL)
		return;

	delete pFrame;
	m_listFrame.RemoveAt(pos);
	
	CRect temp;

	if (index == m_listFrame.GetCount())
	{
		index=(int)m_listFrame.GetCount()-1;
	}
	SelectFrame(index,temp);

	ReCalcSpreadRect();
	ReCalcUnionRect();

	m_DrawPos = m_listFrame.GetHeadPosition();
}

BOOL CMotion::Drag( CPoint& pt )
{
	CFrame* pFrame;
	CRect FrameClientRect;
	int index;
	
	
	pFrame=FindFrame(pt,FrameClientRect,index);
	if (pFrame==NULL) {
		return FALSE;
	}

	if (!m_bDrag)
	{
		m_nDragedFrameIndex=index;
		m_bDrag=TRUE;
	}	 
	m_InsertRect.left	= FrameClientRect.left-3;
	m_InsertRect.top	= FrameClientRect.top;
	m_InsertRect.right	= FrameClientRect.left+3;
	m_InsertRect.bottom	= FrameClientRect.bottom;
	return TRUE;	
}

BOOL CMotion::Drop( CPoint& pt )
{
	m_bDrag=FALSE;
	CRect FrameClientRect;
	int index;
	CFrame* pFrame;
	pFrame=FindFrame(pt,FrameClientRect,index);

	m_InsertRect.SetRectEmpty();
	if (pFrame==NULL)
	{
		return FALSE;
	}

	if (m_nDragedFrameIndex==index)
	{
		m_InsertRect.SetRectEmpty();
		return FALSE;
	}


	POSITION posSrc,posInsertBefore;
	CFrame* pDragedFrame;
	posInsertBefore=m_listFrame.FindIndex(index);
	if (posInsertBefore==NULL)
	{
		return FALSE;
	}
	posSrc=m_listFrame.FindIndex(m_nDragedFrameIndex);
	if (posSrc==NULL)
	{
		return FALSE;
	}
	pDragedFrame=GetFrame(m_nDragedFrameIndex);
	if (pDragedFrame==NULL)
	{
		return FALSE;
	}
	m_listFrame.RemoveAt(posSrc);
	m_listFrame.InsertBefore(posInsertBefore,pDragedFrame);	
	
	CRect temp;
	SelectFrame(index,temp);
	return TRUE;
}

void CMotion::ReCalcSpreadRect()
{
	m_SpreadRect.SetRect(0,0,0,0);

	POSITION pos;
	CFrame* pItem;
	pos = m_listFrame.GetHeadPosition();
	while (pos!=NULL)
	{
		pItem = m_listFrame.GetNext(pos);
		if(pItem!=NULL)
		{
			ExpandSpreadRect(pItem->GetWidth(),pItem->GetHeight());
		}	
	}
}

void CMotion::ReCalcUnionRect()
{
	m_UnionRect.SetRect(0,0,0,0);

	POSITION pos;
	CFrame* pItem;
	CRect temp;
	pos = m_listFrame.GetHeadPosition();
	while (pos!=NULL)
	{
		pItem = m_listFrame.GetNext(pos);
		if(pItem!=NULL)
		{
			temp.SetRect(0,0,pItem->GetWidth()-1,pItem->GetHeight()-1);
			ExpandUnionRect(temp);
		}	
	}
}

BOOL CMotion::SetCenterAll( CPoint& point )
{
	POSITION pos;
	CFrame* pItem;	
	pos = m_listFrame.GetHeadPosition();
	if (pos==NULL)
	{
		return FALSE;
	}
	while (pos!=NULL)
	{
		pItem = m_listFrame.GetNext(pos);
		if(pItem!=NULL)
		{
			pItem->SetCenter(point);
		}	
	}	
	return TRUE;
}

BOOL CMotion::SetDelayTimeAll( int DelayTime )
{
	POSITION pos;
	CFrame* pItem;	
	pos = m_listFrame.GetHeadPosition();
	if (pos==NULL)
	{
		return FALSE;
	}
	while (pos!=NULL)
	{
		pItem = m_listFrame.GetNext(pos);
		if(pItem!=NULL)
		{
			pItem->SetDelayTime(DelayTime);
		}	
	}	
	return TRUE;
}