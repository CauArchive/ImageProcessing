#pragma once
#include <afxwin.h>
#include "pch.h"
#include "Resource.h"

class RemoveDlg :
    public CDialog
{
	DECLARE_DYNAMIC(RemoveDlg)

public:
	RemoveDlg(CWnd* pParent = NULL);
	virtual ~RemoveDlg();
	virtual BOOL OnInitDialog();
	enum { IDD = IDD_DIALOG_REMOVE };
	int getRed() const;
	int getGreen() const;
	int getBlue() const;
	int isManual() const;

protected:
	cv::Mat m_mat;
	cv::Mat m_matDis;
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
	CImage* paintPicture(cv::Mat mat);

protected:
	afx_msg void OnPaint();
	virtual void DoDataExchange(CDataExchange* pDX);
	int getValueColor(int value, int delta);
	void hideManual();
	void showManual();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeStaticRed1();
};

