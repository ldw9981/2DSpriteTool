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
	MOTIONHEADER m_MotionHeader;			// ��� ��� 
	CList <CFrame*,CFrame*> m_listFrame;	// ������ ����Ʈ

	CRect m_SpreadRect;						// ����� �׷��� Ŭ���̾�Ʈ ����
	CRect m_UnionRect;						// ����� �ִ� ����ũ��
	

	POSITION m_DrawPos;						// �ִϸ��̼� �������� �ε��� ����

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
	

/* View�� �� �����Ѱ�?....*/
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
