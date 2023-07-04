#pragma once
#include "Frame.h"

typedef struct _MOTIONHEADER_
{
	int FrameCountNum;
	int	BitCountNum;
} MOTIONHEADER;


class CMotion
{
public:
	CMotion(void);
	virtual ~CMotion(void);
	
protected:
	MOTIONHEADER m_MotionHeader;			// 모션 헤더 
	CList <CFrame*,CFrame*> m_listFrame;	// 프레임 리스트

	CRect m_SpreadRect;						// 모션이 그려질 클라이언트 영역
	CRect m_UnionRect;						// 모션의 최대 결합크기
	

	POSITION m_DrawPos;						// 애니메이션 프레임의 인덱스 정보

	int		m_nSelectedFrameIndex;
	CRect	m_SelectedFrameRect;

	CRect	m_InsertRect;

	BOOL	m_bIsSelected;
	BOOL	m_bDrag;
	int		m_nDragedFrameIndex;

public:
	void	InsertFrame(CDIB& Src,CRect& SrcRect);
	void	DeleteFrame(int index);

	CFrame* GetFrame(int index);	
	CFrame* GetFrameSelected();
	CFrame* GetFrameSelectedPrev();
	CRect   GetFrameClientRect(int index);


	int		CountFrame();
	void	Serialize(CArchive& ar);
	void	SerializeOut(std::ofstream& os);
	void	DeleteAll();

	int		GetSelectedFrameIndex() const { return m_nSelectedFrameIndex; }
	void	SetSelectedFrameIndex(int val) { m_nSelectedFrameIndex = val; }

	CFrame*	SelectFrame(CPoint& pt);
	CFrame* SelectFrame(int index,CRect& rtClientRect);
	

/* View가 더 적당한가?....*/
	void	DrawSpread(CDC* pDC);
	void	DrawAnimation(CDC* pDC,int x,int y);
	CRect&	GetSpreadRect() { return m_SpreadRect; }
	CRect&  GetUnionRect() { return m_UnionRect; }

	
//	CRect	GetRectSelectedFrameClientRect();
	BOOL	Drag(CPoint& pt);
	BOOL	Drop(CPoint& pt);

	BOOL	SetCenterAll(CPoint& point);
	BOOL	SetDelayTimeAll(int DelayTime);
	
protected:
	void	DrawGauze(CDC* pDC,CRect& rect);
	CFrame*	FindFrame( CPoint& ptInput,CRect& rtOuput,int& idxOutput);
	void	ExpandSpreadRect(int width,int height);
	void	ReCalcSpreadRect();

	void	ExpandUnionRect(CRect& rect);
	void	ReCalcUnionRect();

};
