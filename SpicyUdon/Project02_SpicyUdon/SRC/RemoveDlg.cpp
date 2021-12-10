#include "pch.h"
#include "RemoveDlg.h"
#include "ImageTool.h"
#include "Project02_SpicyUdonDoc.h"

IMPLEMENT_DYNAMIC(RemoveDlg, CDialog)

RemoveDlg::RemoveDlg(CWnd* pParent)
	: CDialog(RemoveDlg::IDD, pParent)
{
	m_redValue = 0;
	m_greenValue = 0;
	m_checkState = 1;
	m_blueValue = 0;
}

RemoveDlg::~RemoveDlg()
{
}

BEGIN_MESSAGE_MAP(RemoveDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK2, &RemoveDlg::OnBnClickedCheck1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_1, &RemoveDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3, &RemoveDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_2, &RemoveDlg::OnDeltaposSpin2)
END_MESSAGE_MAP()

void RemoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RED1, m_red);
	DDX_Control(pDX, IDC_STATIC_GREEN1, m_green);
	DDX_Control(pDX, IDC_STATIC_BLUE_TEXT1, m_blue);
	DDX_Control(pDX, IDC_STATIC_R1, m_redText);
	DDX_Control(pDX, IDC_STATIC_G1, m_greenText);
	DDX_Control(pDX, IDC_STATIC_B1, m_blueText);
	DDX_Control(pDX, IDC_CHECK2, m_checkBox);
	DDX_Control(pDX, IDC_SPIN_1, m_spinRed);
	DDX_Control(pDX, IDC_SPIN_3, m_spinGreen);
	DDX_Control(pDX, IDC_SPIN_2, m_spinBlue);

}

BOOL RemoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CProject02SpicyUdonDoc* pDoc = (CProject02SpicyUdonDoc*)((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	m_mat = pDoc->getPicture();
	m_matDis = pDoc->getPictureDisplay();
	this->hideManual();
	this->m_checkBox.SetCheck(true);
	UpdateData(FALSE);
	return TRUE;
}

void RemoveDlg::hideManual()
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

void RemoveDlg::showManual()
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

int RemoveDlg::getRed() const
{
	return this->m_redValue;
}

int RemoveDlg::getGreen() const
{
	return this->m_greenValue;
}

int RemoveDlg::getBlue() const
{
	return this->m_blueValue;
}

int RemoveDlg::isManual() const
{
	return !this->m_checkState;
}

void RemoveDlg::OnBnClickedCheck1()
{
	if (m_checkBox.GetCheck()) {
		this->hideManual();
		m_checkState = 1;
	}
	else {
		this->showManual();
		m_checkState = 0;
	}
}

int RemoveDlg::getValueColor(int value, int delta)
{
	if (delta > 0) {
		value -= 1;
		if (value < 0)
			value = 0;
	}
	else {
		value += 1;
		if (value > 255)
			value = 255;
	}
	return value;
}


void RemoveDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_redValue = this->getValueColor(m_redValue, pNMUpDown->iDelta);
	CString valueStr;
	valueStr.Format(_T("%d"), this->m_redValue);
	this->m_red.SetWindowTextW(valueStr);
	*pResult = 0;
}


void RemoveDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_greenValue = this->getValueColor(m_greenValue, pNMUpDown->iDelta);
	CString valueStr;
	valueStr.Format(_T("%d"), this->m_greenValue);
	this->m_green.SetWindowTextW(valueStr);
	*pResult = 0;
}


void RemoveDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_blueValue = this->getValueColor(m_blueValue, pNMUpDown->iDelta);
	CString valueStr;
	valueStr.Format(_T("%d"), this->m_blueValue);
	this->m_blue.SetWindowTextW(valueStr);
	*pResult = 0;
}

CImage* RemoveDlg::paintPicture(cv::Mat mat)
{
	cv::Mat matResize = ImageTool::resizeMat(mat, 600, int((600.0 / float(mat.size().width)) * mat.size().height));
	int bitsPerPixel = 8 * matResize.elemSize();
	assert((bitsPerPixel == 8 || bitsPerPixel == 24 || bitsPerPixel == 32));
	int border = 0;
	RECT rect;
	GetClientRect(&rect);
	cv::Size size(matResize.size().width / 2, matResize.size().height / 2);

	ImageTool::setupInformation(border, matResize, bitsPerPixel);
	BITMAPINFO* bitInfo = ImageTool::fillBitmap(bitsPerPixel, matResize);
	CImage* image = ImageTool::matToImage(matResize, bitInfo, size, border);

	return image;
}

afx_msg void RemoveDlg::OnPaint() {
	CImage* first = this->paintPicture(m_matDis);

	//Display
	CPaintDC dc(this);
	first->BitBlt(dc, 80, 80);
	::ReleaseDC(::WindowFromDC(dc), dc);
	first->ReleaseDC();
	first->Destroy();
}