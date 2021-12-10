
// Project02_SpicyUdonDoc.cpp : implementation of the CProject02SpicyUdonDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Project02_SpicyUdon.h"
#endif

#include "Project02_SpicyUdonDoc.h"
#include "BackgroundRemovalDlg.h"
#include "RemoveDlg.h"
#include <map>
#include "ImageTool.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProject02SpicyUdonDoc

IMPLEMENT_DYNCREATE(CProject02SpicyUdonDoc, CDocument)

BEGIN_MESSAGE_MAP(CProject02SpicyUdonDoc, CDocument)
    ON_COMMAND(ID_BACKGROUNDREMOVAL, &CProject02SpicyUdonDoc::OnBackgroundRemoval)
    ON_COMMAND(ID_BACKGROUND_ADD, &CProject02SpicyUdonDoc::OnBackgroundAdd)
END_MESSAGE_MAP()


// CProject02SpicyUdonDoc construction/destruction

CProject02SpicyUdonDoc::CProject02SpicyUdonDoc() noexcept
{
    // TODO: add one-time construction code here
    m_picture = NULL;
    m_imageSet = FALSE;
}

CProject02SpicyUdonDoc::~CProject02SpicyUdonDoc()
{
}

void CProject02SpicyUdonDoc::dominantColor(cv::Mat mat, int& redColor, int& greenColor, int& blueColor)
{
    int rows = mat.rows;
    int cols = mat.cols;
    int maxCounter = 0;

    std::map<std::string, int> colorCounter;

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            int r = mat.at<cv::Vec3b>(j, i)[2];
            int g = mat.at<cv::Vec3b>(j, i)[1];
            int b = mat.at<cv::Vec3b>(j, i)[0];
            std::string red = std::to_string(r);
            std::string green = std::to_string(g);
            std::string blue = std::to_string(b);
            std::string rgb = red + ";" + green + ";" + blue;

            if (colorCounter.find(rgb) == colorCounter.end()) {
                colorCounter[rgb] = 1;
            } else {
                colorCounter[rgb] += 1;
            }
            int counter = colorCounter.at(rgb);
            if (counter > maxCounter) {
                redColor = r;
                greenColor = g;
                blueColor = b;
                maxCounter = counter;
            }
        }
    }
}

void CProject02SpicyUdonDoc::dominantColorAlpha(cv::Mat mat, int& redColor,
    int& greenColor, int& blueColor, int & alpha)
{
    int rows = mat.rows;
    int cols = mat.cols;
    int maxCounter = 0;

    std::map<std::string, int> colorCounter;

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            int a = mat.at<cv::Vec4b>(j, i)[3];
            int r = mat.at<cv::Vec4b>(j, i)[2];
            int g = mat.at<cv::Vec4b>(j, i)[1];
            int b = mat.at<cv::Vec4b>(j, i)[0];
            std::string red = std::to_string(r);
            std::string green = std::to_string(g);
            std::string blue = std::to_string(b);
            std::string alpha = std::to_string(a);
            std::string rgba = red + ";" + green + ";" + blue + ";" + alpha;

            if (colorCounter.find(rgba) == colorCounter.end()) {
                colorCounter[rgba] = 1;
            } else {
                colorCounter[rgba] += 1;
            }
            int counter = colorCounter.at(rgba);
            if (counter > maxCounter) {
                redColor = r;
                greenColor = g;
                blueColor = b;
                alpha = a;
                maxCounter = counter;
            }
        }
    }
}

cv::Mat CProject02SpicyUdonDoc::blendForegroundAndBackground(cv::Mat foreground,
    cv::Mat background, int red, int blue, int green) {
    int row = foreground.rows;
    int col = foreground.cols;
    cv::Mat im(row, col, CV_8UC3, cv::Scalar(0, 0, 0));
    if (row > background.rows || col > background.cols)
        background = ImageTool::resizeMat(background, row, col);
    assert((row <= background.rows) && (col <= background.cols));
    for (int y = 0; y < col; y++){
        for (int x = 0; x < row; x++){
            cv::Vec3b& color = foreground.at<cv::Vec3b>(x, y);
            cv::Vec3b& color2 = background.at<cv::Vec3b>(x, y);

            if (color[2] == red && color[1] == green && color[0] == blue) {
                im.at<cv::Vec3b>(x, y) = color2;
            } else {
                im.at<cv::Vec3b>(x, y) = color;
            }
        }
    }
    return im;
}

cv::Mat CProject02SpicyUdonDoc::blendForegroundAndBackgroundAlpha(cv::Mat foreground,
    cv::Mat background, int red, int blue, int green, int alpha) {
    int row = foreground.rows;
    int col = foreground.cols;
    cv::Mat im(row, col, CV_8UC4, cv::Scalar(0, 0, 0, 0));
    if (row > background.rows || col > background.cols)
        background = ImageTool::resizeMat(background, row, col);
    assert((row <= background.rows) && (col <= background.cols));
    for (int y = 0; y < col; y++) {
        for (int x = 0; x < row; x++) {
            cv::Vec4b& color = foreground.at<cv::Vec4b>(x, y);
            cv::Vec3b& color2 = background.at<cv::Vec3b>(x, y);

            if (color[2] == red && color[1] == green && color[0] == blue) {
                cv::Vec4b newColor;
                if (background.type() == CV_8UC4) {
                    cv::Vec4b& color2 = background.at<cv::Vec4b>(x, y);
                    newColor = color2;
                } else {
                    cv::Vec3b& color2 = background.at<cv::Vec3b>(x, y);
                    newColor = cv::Vec4b(color2[0], color2[1], color2[2], 1);
                }
                im.at<cv::Vec4b>(x, y) = newColor;
            } else {
                im.at<cv::Vec4b>(x, y) = color;
            }
        }
    }
    return im;
}

cv::Mat CProject02SpicyUdonDoc::removeBackground(cv::Mat picture,
    int red, int green, int blue) {
    int row = picture.rows;
    int col = picture.cols;
    cv::Mat im(row, col, CV_8UC4, cv::Scalar(0, 0, 0, 0));
    for (int y = 0; y < col; y++) {
        for (int x = 0; x < row; x++) {
            cv::Vec3b& color = picture.at<cv::Vec3b>(x, y);

            if (color[2] == red && color[1] == green && color[0] == blue) {
                cv::Vec4b newColor(255, 255, 255, 0);
                im.at<cv::Vec4b>(x, y) = newColor;
            } else {
                cv::Vec4b newColor(color[0], color[1], color[2], 1);
                im.at<cv::Vec4b>(x, y) = newColor;
            }
        }
    }
    return im;
}

cv::Mat CProject02SpicyUdonDoc::removeBackgroundAlpha(cv::Mat picture,
    int red, int green, int blue, int alpha) {
    int row = picture.rows;
    int col = picture.cols;
    cv::Mat im(row, col, CV_8UC4, cv::Scalar(0, 0, 0, 0));
    for (int y = 0; y < col; y++) {
        for (int x = 0; x < row; x++) {
            cv::Vec4b& color = picture.at<cv::Vec4b>(x, y);

            if (color[2] == red && color[1] == green && color[0] == blue) {
                cv::Vec4b newColor(255, 255, 255, 0);
                im.at<cv::Vec4b>(x, y) = newColor;
            } else {
                cv::Vec4b newColor(color[0], color[1], color[2], color[3]);
                im.at<cv::Vec4b>(x, y) = newColor;
            }
        }
    }
    return im;
}

cv::Mat CProject02SpicyUdonDoc::getPictureDisplay() const
{
    return this->m_pictureDisplay;
}

BOOL CProject02SpicyUdonDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;
    std::string path = CT2A(lpszPathName);
    m_picture = cv::imread(path);
    m_pictureDisplay = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (m_picture.empty())
        return FALSE;
    m_imageSet = TRUE;
    return TRUE;
}

void CProject02SpicyUdonDoc::OnBackgroundRemoval()
{
    if (isImageSet()) {
        RemoveDlg dlg;
        if (dlg.DoModal() == IDOK) {
            int red = 0;
            int green = 0;
            int blue = 0;
            int alpha = 0;
            cv::Mat image;
            if (dlg.isManual()) {
                red = dlg.getRed();
                green = dlg.getGreen();
                blue = dlg.getBlue();
            } else if (this->m_picture.type() == CV_8UC4) {
                this->dominantColorAlpha(this->m_picture, red, green, blue, alpha);
            } else {
                this->dominantColor(this->m_picture, red, green, blue);
            }
            if (this->m_picture.type() == CV_8UC4) {
                image = this->removeBackgroundAlpha(this->m_picture, red, green, blue, alpha);
            } else {
                image = this->removeBackground(this->m_picture, red, green, blue);
            }
            cv::namedWindow("Display remove background", cv::WINDOW_AUTOSIZE);
            cv::imshow("Display remove background", image);
        }
    }
}

void CProject02SpicyUdonDoc::OnBackgroundAdd()
{
    if (isImageSet()) {
        BackgroundRemovalDlg dlg;

        if (dlg.DoModal() == IDOK) {
            cv::Mat foreground = this->m_picture;
            cv::Mat background = dlg.GetSecondPicture();
            int red = 0;
            int green = 0;
            int blue = 0;
            int alpha = 0;
            if (dlg.isManual()) {
                red = dlg.getRed();
                green = dlg.getGreen();
                blue = dlg.getBlue();
            } else if (this->m_picture.type() == CV_8UC4) {
                this->dominantColorAlpha(this->m_picture, red, green, blue, alpha);
            } else {
                this->dominantColor(this->m_picture, red, green, blue);
            }
            if (this->m_picture.type() == CV_8UC4) {
                foreground = blendForegroundAndBackgroundAlpha(foreground, background, red, blue, green, alpha);
            }
            else {
                foreground = blendForegroundAndBackground(foreground, background, red, blue, green);
            }
            cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
            cv::imshow("Display window", foreground);
        }
    }
}

cv::Mat CProject02SpicyUdonDoc::getPicture() const
{
    return this->m_picture;
}

BOOL CProject02SpicyUdonDoc::isImageSet() const
{
    return this->m_imageSet;
}


BOOL CProject02SpicyUdonDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    return TRUE;
}




// CProject02SpicyUdonDoc serialization

void CProject02SpicyUdonDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CProject02SpicyUdonDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
    // Modify this code to draw the document's data
    dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

    CString strText = _T("TODO: implement thumbnail drawing here");
    LOGFONT lf;

    CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
    pDefaultGUIFont->GetLogFont(&lf);
    lf.lfHeight = 36;

    CFont fontDraw;
    fontDraw.CreateFontIndirect(&lf);

    CFont* pOldFont = dc.SelectObject(&fontDraw);
    dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
    dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CProject02SpicyUdonDoc::InitializeSearchContent()
{
    CString strSearchContent;
    // Set search contents from document's data.
    // The content parts should be separated by ";"

    // For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
    SetSearchContent(strSearchContent);
}

void CProject02SpicyUdonDoc::SetSearchContent(const CString& value)
{
    if (value.IsEmpty())
    {
        RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
    }
    else
    {
        CMFCFilterChunkValueImpl *pChunk = nullptr;
        ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
        if (pChunk != nullptr)
        {
            pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
            SetChunkValue(pChunk);
        }
    }
}

#endif // SHARED_HANDLERS

// CProject02SpicyUdonDoc diagnostics

#ifdef _DEBUG
void CProject02SpicyUdonDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CProject02SpicyUdonDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


// CProject02SpicyUdonDoc commands



