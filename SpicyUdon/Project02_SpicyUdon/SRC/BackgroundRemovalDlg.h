#pragma once
#include <afxwin.h>
#include "pch.h"
#include "Resource.h"
#include <vector>

class BackgroundRemovalDlg :
    public CDialog
{
	DECLARE_DYNAMIC(BackgroundRemovalDlg)

public:
	BackgroundRemovalDlg(CWnd* pParent = NULL);
	virtual ~BackgroundRemovalDlg();
	cv::Mat GetMat() const;
	cv::Mat GetSecondPicture() const;
	virtual BOOL OnInitDialog();
	enum { IDD = ID_BACK_DIALOG};
	int getRed() const;
	int getGreen() const;
	int getBlue() const;
	int isManual() const;

protected:
	cv::Mat m_mat;
	cv::Mat m_matDis;
	cv::Mat m_secondPicture;
	cv::Mat m_secondPictureDis;
	CComboBox m_combo;
	std::vector<cv::Mat> m_matSave;
	std::vector<cv::Mat> m_matSaveDis;
	int m_redValue;
	int m_greenValue;
	int m_blueValue;
	CStatic m_red;
	CStatic m_green;
	CStatic m_blue;
	CStatic m_redText;
	CStatic m_greenText;
	CStatic m_blueText;
	CButton m_checkBox;
	CSpinButtonCtrl m_spinRed;
	CSpinButtonCtrl m_spinGreen;
	CSpinButtonCtrl m_spinBlue;
	int m_checkState;

protected:
	CImage * paintPicture(cv::Mat mat);

protected:
	afx_msg void OnPaint();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnCbnSelchangeCoSecondImage();
	int getValueColor(int value, int delta);
	void hideManual();
	void showManual();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
};

