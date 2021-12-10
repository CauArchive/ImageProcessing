
// Project02_SpicyUdonView.cpp : implementation of the CProject02SpicyUdonView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Project02_SpicyUdon.h"
#endif

#include "Project02_SpicyUdonDoc.h"
#include "Project02_SpicyUdonView.h"
#include "ImageTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProject02SpicyUdonView

IMPLEMENT_DYNCREATE(CProject02SpicyUdonView, CScrollView)

BEGIN_MESSAGE_MAP(CProject02SpicyUdonView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CProject02SpicyUdonView construction/destruction

CProject02SpicyUdonView::CProject02SpicyUdonView() noexcept
{
	// TODO: add construction code here

}

CProject02SpicyUdonView::~CProject02SpicyUdonView()
{
}

BOOL CProject02SpicyUdonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CProject02SpicyUdonView drawing

void CProject02SpicyUdonView::OnDraw(CDC* pdc)
{
	CProject02SpicyUdonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->isImageSet()) {
		cv::Mat mat = pDoc->getPictureDisplay();
		int bitsPerPixel = 8 * mat.elemSize();
		int border = 0;
		RECT rect;
		GetClientRect(&rect);
		cv::Size size(rect.right, rect.bottom);

		ImageTool::setupInformation(border, mat, bitsPerPixel);
		BITMAPINFO* bitInfo = ImageTool::fillBitmap(bitsPerPixel, mat);
		CImage * image = ImageTool::matToImage(mat, bitInfo, size, border);
		//Display
		HDC dc = pdc->GetSafeHdc();
		image->BitBlt(dc, 0, 0);
		::ReleaseDC(::WindowFromDC(dc), dc);
		image->ReleaseDC();
		image->Destroy();
	}
}

void CProject02SpicyUdonView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal(10, 10);
	CProject02SpicyUdonDoc* pDoc = GetDocument();
	if (pDoc->isImageSet())
		sizeTotal = CSize(pDoc->getPictureDisplay().size().width, pDoc->getPictureDisplay().size().height);

	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
}


// CProject02SpicyUdonView printing

BOOL CProject02SpicyUdonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CProject02SpicyUdonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CProject02SpicyUdonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CProject02SpicyUdonView diagnostics

#ifdef _DEBUG
void CProject02SpicyUdonView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CProject02SpicyUdonView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CProject02SpicyUdonDoc* CProject02SpicyUdonView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProject02SpicyUdonDoc)));
	return (CProject02SpicyUdonDoc*)m_pDocument;
}
#endif //_DEBUG


// CProject02SpicyUdonView message handlers
