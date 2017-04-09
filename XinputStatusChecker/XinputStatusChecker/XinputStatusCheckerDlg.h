
// XinputStatusCheckerDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CXinputStatusCheckerDlg dialog
class CXinputStatusCheckerDlg : public CDialogEx
{
// Construction
public:
	CXinputStatusCheckerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XINPUTSTATUSCHECKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNMReleasedcaptureSliderLeftVib(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderRightVib(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRetry();
	DECLARE_MESSAGE_MAP()

private:
	void initialize();
	bool check_pad_connected();
	void welcome_vib();
	void init_ui_values();
	void disable_ui_values();
	void test_left_vib(const int &level);
	void test_right_vib(const int &level);

	int  _xinpu_number;
public:
	CSliderCtrl _left_vib_slider;
	CSliderCtrl _right_vib_slider;
	
};
