#include "pch.h"
#include "BackgroundRemovalDlg.h"
#include "Project02_SpicyUdonDoc.h"
#include "ImageTool.h"

IMPLEMENT_DYNAMIC(BackgroundRemovalDlg, CDialog)

BackgroundRemovalDlg::BackgroundRemovalDlg(CWnd* pParent)
	: CDialog(BackgroundRemovalDlg::IDD, pParent)
{
	m_redValue = 0;
	m_greenValue = 0;
	m_checkState = 1;
	m_blueValue = 0;
}

BackgroundRemovalDlg::~BackgroundRemovalDlg()
{
}

BEGIN_MESSAGE_MAP(BackgroundRemovalDlg, CDialog)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_PIC, &BackgroundRemovalDlg::OnCbnSelchangeCoSecondImage)
	ON_BN_CLICKED(IDC_CHECK1, &BackgroundRemovalDlg::OnBnClickedCheck1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &BackgroundRemovalDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &BackgroundRemovalDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &BackgroundRemovalDlg::OnDeltaposSpin2)
END_MESSAGE_MAP()

void BackgroundRemovalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PIC, m_combo);
	DDX_Control(pDX, IDC_STATIC_RED, m_red);
	DDX_Control(pDX, IDC_STATIC_GREEN, m_green);
	DDX_Control(pDX, IDC_STATIC_BLUE_TEXT, m_blue);
	DDX_Control(pDX, IDC_STATIC_R, m_redText);
	DDX_Control(pDX, IDC_STATIC_G, m_greenText);
	DDX_Control(pDX, IDC_STATIC_B, m_blueText);
	DDX_Control(pDX, IDC_CHECK1, m_checkBox);
	DDX_Control(pDX, IDC_SPIN1, m_spinRed);
	DDX_Control(pDX, IDC_SPIN3, m_spinGreen);
	DDX_Control(pDX, IDC_SPIN2, m_spinBlue);

}

void BackgroundRemovalDlg::OnCbnSelchangeCoSecondImage()
{
	//m_secondPicture = (*(cv::Mat *)m_combo.GetItemDataPtr(m_combo.GetCurSel()));

	int index = m_combo.GetCurSel();
	if (index < m_matSave.size()) {
		m_secondPicture = this->m_matSave.at(index);
		m_secondPictureDis = this->m_matSaveDis.at(index);
	}
	Invalidate();
}

BOOL BackgroundRemovalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CProject02SpicyUdonDoc* pDoc = (CProject02SpicyUdonDoc*)((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	m_mat = pDoc->getPicture();
	m_matDis = pDoc->getPictureDisplay();
	// Set second image & combobox
	POSITION docTemplatePosiotion = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = AfxGetApp()->GetNextDocTemplate(docTemplatePosiotion);

	POSITION docPosition = pDocTemplate->GetFirstDocPosition();
	while (docPosition != NULL) {
		pDoc = (CProject02SpicyUdonDoc*)pDocTemplate->GetNextDoc(docPosition);
		m_combo.AddString(pDoc->GetTitle());
		m_matSave.push_back(pDoc->getPicture());
		m_matSaveDis.push_back(pDoc->getPictureDisplay());
	}
	m_combo.SetCurSel(0);
	m_secondPicture = this->m_matSave.at(0);
	m_secondPictureDis = this->m_matSaveDis.at(0);
	this->hideManual();
	this->m_checkBox.SetCheck(true);
	UpdateData(FALSE);
	return TRUE;
}

cv::Mat BackgroundRemovalDlg::GetMat() const
{
	return this->m_mat;
}

cv::Mat BackgroundRemovalDlg::GetSecondPicture() const
{
	return this->m_secondPicture;
}

CImage * BackgroundRemovalDlg::paintPicture(cv::Mat mat)
{
	int t = 0;

	cv::Mat matResize = ImageTool::resizeMat(mat, 400, int((400.0 / float(mat.size().width)) * mat.size().height));
	int bitsPerPixel = 8 * matResize.elemSize();
	assert((bitsPerPixel == 8 || bitsPerPixel == 24 || bitsPerPixel == 32));
	int border = 0;
	RECT rect;
	GetClientRect(&rect);
	cv::Size size(matResize.size().width, matResize.size().height);

	ImageTool::setupInformation(border, matResize, bitsPerPixel);
	BITMAPINFO* bitInfo = ImageTool::fillBitmap(bitsPerPixel, matResize);
	CImage* image = ImageTool::matToImage(matResize, bitInfo, size, border);

	return image;
}

afx_msg void BackgroundRemovalDlg::OnPaint() {
	CImage * first = this->paintPicture(m_matDis);
	CImage * second = this->paintPicture(m_secondPictureDis);

	//Display
	CPaintDC dc(this);
	first->BitBlt(dc, 80, 80);
	second->BitBlt(dc, 620, 80);
	::ReleaseDC(::WindowFromDC(dc), dc);
	first->ReleaseDC();
	second->ReleaseDC();
	first->Destroy();
	second->Destroy();
}

void BackgroundRemovalDlg::hideManual()
{
	this->m_spinBlue.ShowWindow(SW_HIDE);
	this->m_spinRed.ShowWindow(SW_HIDE);
	this->m_spinGreen.ShowWindow(SW_HIDE);
	this->m_red.ShowWindow(SW_HIDE);
	this->m_blue.ShowWindow(SW_HIDE);
	this->m_green.ShowWindow(SW_HIDE);
	this->m_redText.ShowWindow(SW_HIDE);
	this->m_blueText.ShowWindow(SW_HIDE);
	this->m_greenText.ShowWindow(SW_HIDE);
}

void BackgroundRemovalDlg::showManual()
{
	this->m_spinBlue.ShowWindow(SW_SHOW);
	this->m_spinRed.ShowWindow(SW_SHOW);
	this->m_spinGreen.ShowWindow(SW_SHOW);
	this->m_red.ShowWindow(SW_SHOW);
	this->m_blue.ShowWindow(SW_SHOW);
	this->m_green.ShowWindow(SW_SHOW);
	this->m_redText.ShowWindow(SW_SHOW);
	this->m_blueText.ShowWindow(SW_SHOW);
	this->m_greenText.ShowWindow(SW_SHOW);
}

int BackgroundRemovalDlg::getRed() const
{
	return this->m_redValue;
}

int BackgroundRemovalDlg::getGreen() const
{
	return this->m_greenValue;
}

int BackgroundRemovalDlg::getBlue() const
{
	return this->m_blueValue;
}

int BackgroundRemovalDlg::isManual() const
{
	return !this->m_checkState;
}

void BackgroundRemovalDlg::OnBnClickedCheck1()
{
	if (m_checkBox.GetCheck()) {
		this->hideManual();
		m_checkState = 1;
	} else {
		this->showManual();
		m_checkState = 0;
	}
}

int BackgroundRemovalDlg::getValueColor(int value, int delta)
{
	if (delta > 0) {
		value -= 1;
		if (value < 0)
			value = 0;
	} else {
		value += 1;
		if (value > 255)
			value = 255;
	}
	return value;
}


void BackgroundRemovalDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_redValue = this->getValueColor(m_redValue, pNMUpDown->iDelta);
	CString valueStr;
	valueStr.Format(_T("%d"), this->m_redValue);
	this->m_red.SetWindowTextW(valueStr);
	*pResult = 0;
}


void BackgroundRemovalDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_greenValue = this->getValueColor(m_greenValue, pNMUpDown->iDelta);
	CString valueStr;
	valueStr.Format(_T("%d"), this->m_greenValue);
	this->m_green.SetWindowTextW(valueStr);
	*pResult = 0;
}


void BackgroundRemovalDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_blueValue = this->getValueColor(m_blueValue, pNMUpDown->iDelta);
	CString valueStr;
	valueStr.Format(_T("%d"), this->m_blueValue);
	this->m_blue.SetWindowTextW(valueStr);
	*pResult = 0;
}
