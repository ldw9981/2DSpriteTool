#pragma once

// CMotionDoc �����Դϴ�.
#include "Motion.h"
#include "MotionGroup.h"


class CMotionDoc : public CDocument
{
	DECLARE_DYNCREATE(CMotionDoc)

public:
	CMotionDoc();
	virtual ~CMotionDoc();

	CMotion&		GetMoiton() {	return m_Motion; }

	CMotionGroup&	GetMoitonGroup() {	return m_listMotionGroup; }

	void			Modified();
protected:
	CMotion			m_Motion;
	CMotionGroup	m_listMotionGroup;
	

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // ���� ��/����� ���� �����ǵǾ����ϴ�.
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:

};
