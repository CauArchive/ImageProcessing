
// Project02_SpicyUdonDoc.h : interface of the CProject02SpicyUdonDoc class
//


#pragma once

#include "pch.h"


class CProject02SpicyUdonDoc : public CDocument
{
protected: // create from serialization only
	CProject02SpicyUdonDoc() noexcept;
	DECLARE_DYNCREATE(CProject02SpicyUdonDoc)

// Attributes
public:
	cv::Mat getPicture() const;
	cv::Mat getPictureDisplay() const;
	BOOL isImageSet() const;
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	afx_msg void OnBackgroundRemoval();
	afx_msg void OnBackgroundAdd();
	void dominantColor(cv::Mat mat, int& redColor, int& greenColor, int& blueColor);
	void dominantColorAlpha(cv::Mat mat, int& redColor, int& greenColor, int& blueColor, int &alpha);
	cv::Mat blendForegroundAndBackground(cv::Mat foreground, cv::Mat background, int red, int blue, int green);
	cv::Mat blendForegroundAndBackgroundAlpha(cv::Mat foreground, cv::Mat background, int red, int blue, int green, int alpha);
	cv::Mat removeBackground(cv::Mat picture, int red, int green, int blue);
	cv::Mat removeBackgroundAlpha(cv::Mat picture, int red, int green, int blue, int alpha);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CProject02SpicyUdonDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	cv::Mat m_picture;
	cv::Mat m_pictureDisplay;
	BOOL m_imageSet;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
