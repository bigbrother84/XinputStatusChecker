
// XinputStatusCheckerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XinputStatusChecker.h"
#include "XinputStatusCheckerDlg.h"
#include "afxdialogex.h"
#include <Xinput.h>
#pragma comment(lib,"Xinput.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXinputStatusCheckerDlg dialog



CXinputStatusCheckerDlg::CXinputStatusCheckerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_XINPUTSTATUSCHECKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXinputStatusCheckerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_LEFT_VIB, _left_vib_slider);
	DDX_Control(pDX, IDC_SLIDER_RIGHT_VIB, _right_vib_slider);
}

BEGIN_MESSAGE_MAP(CXinputStatusCheckerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LEFT_VIB, &CXinputStatusCheckerDlg::OnNMReleasedcaptureSliderLeftVib)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RIGHT_VIB, &CXinputStatusCheckerDlg::OnNMReleasedcaptureSliderRightVib)
	ON_BN_CLICKED(IDC_BUTTON_RETRY, &CXinputStatusCheckerDlg::OnBnClickedButtonRetry)
END_MESSAGE_MAP()


// CXinputStatusCheckerDlg message handlers

BOOL CXinputStatusCheckerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	initialize();
	return TRUE;  
}


void CXinputStatusCheckerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXinputStatusCheckerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXinputStatusCheckerDlg::initialize()
{
	if (check_pad_connected() == true) {
		welcome_vib();
		init_ui_values();
	}
	else
	{
		disable_ui_values();
	}
}
bool CXinputStatusCheckerDlg::check_pad_connected()
{
	DWORD dwResult = 0;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			_xinpu_number = i;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void CXinputStatusCheckerDlg::welcome_vib()
{
	XINPUT_VIBRATION xinput_vib = { 0, };
	xinput_vib.wLeftMotorSpeed = 32000;
	xinput_vib.wRightMotorSpeed = 32000;
	DWORD dRes = XInputSetState(_xinpu_number, &xinput_vib);
}

void CXinputStatusCheckerDlg::init_ui_values()
{
	//battery status
	XINPUT_BATTERY_INFORMATION battery_information{ 0, };
	if (XInputGetBatteryInformation(_xinpu_number, BATTERY_DEVTYPE_GAMEPAD, &battery_information) == ERROR_SUCCESS) {
		CString battery_level("UNKNOWN");
		switch (battery_information.BatteryLevel)
		{
		case BATTERY_LEVEL_EMPTY: battery_level = "EMPTY"; break;
		case BATTERY_LEVEL_LOW: battery_level = "LOW"; break;
		case BATTERY_LEVEL_MEDIUM: battery_level = "MEDUIM"; break;
		case BATTERY_LEVEL_FULL: battery_level = "FULL"; break;
		default:
			break;
		}
		SetDlgItemText(IDC_EDIT_BATTERY, battery_level);
	}
	else {
		SetDlgItemText(IDC_EDIT_BATTERY, CString("UNKNOWN"));
	}

	//slider bar
	_left_vib_slider.SetRange(0, 10);
	_right_vib_slider.SetRange(0, 10);
	_left_vib_slider.SetPos(5);
	_right_vib_slider.SetPos(5);

	SetDlgItemText(IDC_EDIT_STATUS, CString("Conneted"));
}

void CXinputStatusCheckerDlg::disable_ui_values()
{
	SetDlgItemText(IDC_EDIT_STATUS, CString("Disconneted"));
	SetDlgItemText(IDC_EDIT_BATTERY, CString("Unknown"));
}

void CXinputStatusCheckerDlg::test_left_vib(const int &level)
{
	XINPUT_VIBRATION xinput_vib = { 0, };
	xinput_vib.wLeftMotorSpeed = static_cast<int>((65535/10)*level);
	
	xinput_vib.wRightMotorSpeed = 0;
	DWORD dRes = XInputSetState(_xinpu_number, &xinput_vib);
}

void CXinputStatusCheckerDlg::test_right_vib(const int &level)
{
	XINPUT_VIBRATION xinput_vib = { 0, };
	xinput_vib.wLeftMotorSpeed = 0;
	xinput_vib.wRightMotorSpeed = static_cast<int>((65535 / 10)*level);
	DWORD dRes = XInputSetState(_xinpu_number, &xinput_vib);
}

void CXinputStatusCheckerDlg::OnNMReleasedcaptureSliderLeftVib(NMHDR *pNMHDR, LRESULT *pResult)
{
	test_left_vib(_left_vib_slider.GetPos());
	*pResult = 0;
}


void CXinputStatusCheckerDlg::OnNMReleasedcaptureSliderRightVib(NMHDR *pNMHDR, LRESULT *pResult)
{
	test_left_vib(_right_vib_slider.GetPos());
	*pResult = 0;
}


void CXinputStatusCheckerDlg::OnBnClickedButtonRetry()
{
	initialize();
}
