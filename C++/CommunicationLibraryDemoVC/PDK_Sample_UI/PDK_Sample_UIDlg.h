
// PDK_Sample_UIDlg.h: 헤더 파일
//

#pragma once

#include "PDK_Library.h"
#include <vcclr.h>
#using <mscorlib.dll>
#include "..\CommunicationWrapper\CommunicationWrapper\PmacMultiWrapper.h"

// CPDKSampleUIDlg 대화 상자
class CPDKSampleUIDlg : public CDialogEx
{
// 생성입니다.
public:
	CPDKSampleUIDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PDK_SAMPLE_UI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	void HandleJogCommand(CString pmacId, int motor, bool isPlus, bool isDown);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedSendcmd();
	afx_msg void OnBnClickedFileSelect();
	afx_msg void OnBnClickedFiledownload();
	afx_msg void OnBnClickedSvOn1();
	afx_msg void OnBnClickedSvOff1();
	afx_msg void OnBnClickedSvOn2();
	afx_msg void OnBnClickedSvOff2();
	afx_msg void OnBnClickedJogPlus1();
	afx_msg void OnBnClickedJogMinus1();
	afx_msg void OnBnClickedJogPlus2();
	afx_msg void OnBnClickedJogMinus2();
	CEdit m_IP_Address;
	CEdit mConnectStatus;
	CEdit mCommand;
	CEdit mCmdResponse;
	CEdit mEditFilePath;
	CEdit mEditPMACPath;
	CEdit mDownloadResult;
	CEdit mMotor1Pos;
	CEdit mMotor2Pos;
	CStatic mStaticImage1;
	CStatic mStaticImage2;
	CStatic mStaticImage3;
	CStatic mStaticImage4;
	CStatic mStaticImage5;
	CStatic mStaticImage6;
	HBITMAP mhBitmapGreen;
	HBITMAP mhBitmapRed;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CString sFilePath;
	CString sfileName;
	const char* mServerTextBox;
	const char* muploadFileName;
	const char* muploadFileNameWithFolder;
	const char* mFtpPathname;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool mIsConnected;
	bool gConnected;

	// Dual Panel Controls
	gcroot<LegacyPmacWrapper::PmacManagerWrapper^> m_manager;
	
	// --- PMAC A UI ---
	CStatic m_lblIpA; CEdit m_txtIpA; CButton m_btnConnectA; CButton m_btnDisconnectA;
	CStatic m_lblCmdA; CEdit m_txtCmdA; CButton m_btnSendA; 
	CButton m_btnSvOnA; CButton m_btnSvOffA;
	CButton m_btnSvOn2A; CButton m_btnSvOff2A;
	CStatic m_lblM1A; CButton m_btnJp1A; CButton m_btnJm1A; CEdit m_txtPos1A;
	CStatic m_lblM2A; CButton m_btnJp2A; CButton m_btnJm2A; CEdit m_txtPos2A;
	CStatic m_lblFileA; CEdit m_txtFileA; CButton m_btnSelectA; CButton m_btnFileA;
	CEdit m_txtLogA;

	// --- PMAC B UI ---
	CStatic m_lblIpB; CEdit m_txtIpB; CButton m_btnConnectB; CButton m_btnDisconnectB;
	CStatic m_lblCmdB; CEdit m_txtCmdB; CButton m_btnSendB; 
	CButton m_btnSvOnB; CButton m_btnSvOffB;
	CButton m_btnSvOn2B; CButton m_btnSvOff2B;
	CStatic m_lblM1B; CButton m_btnJp1B; CButton m_btnJm1B; CEdit m_txtPos1B;
	CStatic m_lblM2B; CButton m_btnJp2B; CButton m_btnJm2B; CEdit m_txtPos2B;
	CStatic m_lblFileB; CEdit m_txtFileB; CButton m_btnSelectB; CButton m_btnFileB;
	CEdit m_txtLogB;

	void Log(const CString& target, const CString& msg);

	// Handlers A
	afx_msg void OnBtnConnectA(); afx_msg void OnBtnDisconnectA(); afx_msg void OnBtnSendA();
	afx_msg void OnBtnSvOnA(); afx_msg void OnBtnSvOffA();
	afx_msg void OnBtnSvOn2A(); afx_msg void OnBtnSvOff2A();
	afx_msg void OnBtnJp1A(); afx_msg void OnBtnJm1A();
	afx_msg void OnBtnJp2A(); afx_msg void OnBtnJm2A();
	afx_msg void OnBtnSelectA(); afx_msg void OnBtnFileA();

	// Handlers B
	afx_msg void OnBtnConnectB(); afx_msg void OnBtnDisconnectB(); afx_msg void OnBtnSendB();
	afx_msg void OnBtnSvOnB(); afx_msg void OnBtnSvOffB();
	afx_msg void OnBtnSvOn2B(); afx_msg void OnBtnSvOff2B();
	afx_msg void OnBtnJp1B(); afx_msg void OnBtnJm1B();
	afx_msg void OnBtnJp2B(); afx_msg void OnBtnJm2B();
	afx_msg void OnBtnSelectB(); afx_msg void OnBtnFileB();
};
