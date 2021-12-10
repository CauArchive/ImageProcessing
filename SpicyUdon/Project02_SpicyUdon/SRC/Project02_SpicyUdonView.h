
// Project02_SpicyUdonView.h : interface of the CProject02SpicyUdonView class
//

#pragma once


class CProject02SpicyUdonView : public CScrollView
{
protected: // create from serialization only
	CProject02SpicyUdonView() noexcept;
	DECLARE_DYNCREATE(CProject02SpicyUdonView)

// Attributes
public:
	CProject02SpicyUdonDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CProject02SpicyUdonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Project02_SpicyUdonView.cpp
inline CProject02SpicyUdonDoc* CProject02SpicyUdonView::GetDocument() const
   { return reinterpret_cast<CProject02SpicyUdonDoc*>(m_pDocument); }
#endif

