
// PDK_Sample_UIDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "PDK_Sample_UI.h"
#include "PDK_Sample_UIDlg.h"
#include "afxdialogex.h"
#include <vcclr.h>
#include "string.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPDKSampleUIDlg 대화 상자



CPDKSampleUIDlg::CPDKSampleUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PDK_SAMPLE_UI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPDKSampleUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// 듀얼 패널 구성을 위해 기존 리소스 기반의 DDX 매핑을 모두 무효화합니다.
	/*
	DDX_Control(pDX, IDC_IP_ADDR, m_IP_Address);
	DDX_Control(pDX, IDC_CONNECT_STATUS, mConnectStatus);
	DDX_Control(pDX, IDC_COMMAND, mCommand);
	DDX_Control(pDX, IDC_CMD_RESPONSE, mCmdResponse);
	DDX_Control(pDX, IDC_EDIT_FILE_PATH, mEditFilePath);
	DDX_Control(pDX, IDC_EDIT_PMAC_PATH, mEditPMACPath);
	DDX_Control(pDX, IDC_DOWNLOAD_RESULT, mDownloadResult);
	DDX_Control(pDX, IDC_MOTOR1_POS, mMotor1Pos);
	DDX_Control(pDX, IDC_MOTOR2_POS, mMotor2Pos);
	DDX_Control(pDX, IDC_STATIC_CS, mStaticImage1);
	DDX_Control(pDX, IDC_STATIC_CS2, mStaticImage2);
	DDX_Control(pDX, IDC_STATIC_CS3, mStaticImage3);
	DDX_Control(pDX, IDC_STATIC_CS4, mStaticImage4);
	DDX_Control(pDX, IDC_STATIC_CS5, mStaticImage5);
	DDX_Control(pDX, IDC_STATIC_CS6, mStaticImage6);
	*/
}

BEGIN_MESSAGE_MAP(CPDKSampleUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	// --- PMAC A ---
	ON_BN_CLICKED(101, &CPDKSampleUIDlg::OnBtnConnectA)
	ON_BN_CLICKED(102, &CPDKSampleUIDlg::OnBtnDisconnectA)
	ON_BN_CLICKED(103, &CPDKSampleUIDlg::OnBtnSendA)
	ON_BN_CLICKED(111, &CPDKSampleUIDlg::OnBtnSvOnA)
	ON_BN_CLICKED(112, &CPDKSampleUIDlg::OnBtnSvOffA)
	ON_BN_CLICKED(113, &CPDKSampleUIDlg::OnBtnSvOn2A)
	ON_BN_CLICKED(114, &CPDKSampleUIDlg::OnBtnSvOff2A)
	ON_BN_CLICKED(121, &CPDKSampleUIDlg::OnBtnJp1A)
	ON_BN_CLICKED(122, &CPDKSampleUIDlg::OnBtnJm1A)
	ON_BN_CLICKED(123, &CPDKSampleUIDlg::OnBtnJp2A)
	ON_BN_CLICKED(124, &CPDKSampleUIDlg::OnBtnJm2A)
	ON_BN_CLICKED(131, &CPDKSampleUIDlg::OnBtnFileA)
	ON_BN_CLICKED(132, &CPDKSampleUIDlg::OnBtnSelectA)

	// --- PMAC B ---
	ON_BN_CLICKED(201, &CPDKSampleUIDlg::OnBtnConnectB)
	ON_BN_CLICKED(202, &CPDKSampleUIDlg::OnBtnDisconnectB)
	ON_BN_CLICKED(203, &CPDKSampleUIDlg::OnBtnSendB)
	ON_BN_CLICKED(211, &CPDKSampleUIDlg::OnBtnSvOnB)
	ON_BN_CLICKED(212, &CPDKSampleUIDlg::OnBtnSvOffB)
	ON_BN_CLICKED(213, &CPDKSampleUIDlg::OnBtnSvOn2B)
	ON_BN_CLICKED(214, &CPDKSampleUIDlg::OnBtnSvOff2B)
	ON_BN_CLICKED(221, &CPDKSampleUIDlg::OnBtnJp1B)
	ON_BN_CLICKED(222, &CPDKSampleUIDlg::OnBtnJm1B)
	ON_BN_CLICKED(223, &CPDKSampleUIDlg::OnBtnJp2B)
	ON_BN_CLICKED(224, &CPDKSampleUIDlg::OnBtnJm2B)
	ON_BN_CLICKED(231, &CPDKSampleUIDlg::OnBtnFileB)
	ON_BN_CLICKED(232, &CPDKSampleUIDlg::OnBtnSelectB)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPDKSampleUIDlg 메시지 처리기

BOOL CPDKSampleUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

    // 윈도우 크기를 강제로 키워서 듀얼 패널을 넉넉하게 배치합니다.
    MoveWindow(0, 0, 1200, 800);

	// ==========================================
	// 기존 리소스 컨트롤 찌꺼기 숨기기
	// (rc 파일에 남아있어서 겹쳐보이는 버튼들을 모두 가립니다)
	// ==========================================
	CWnd* pChild = GetWindow(GW_CHILD);
	while (pChild != nullptr)
	{
		pChild->ShowWindow(SW_HIDE);
		pChild = pChild->GetNextWindow();
	}

	// ==========================================
	// 다중 기기 (듀얼 패널) 동적 UI 생성 코드 시작
	// ==========================================
	m_manager = gcnew LegacyPmacWrapper::PmacManagerWrapper();

    // 공통 폰트
    CFont* pFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));

    // ================= [ PMAC A Area (좌측) ] =================
    int ax = 20, ay = 20, aw = 550; // 좌측 영역 (너비 550)
    m_lblIpA.Create(_T("IP A:"), WS_CHILD | WS_VISIBLE, CRect(ax, ay, ax+40, ay+25), this);
    m_txtIpA.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(ax+45, ay, ax+200, ay+25), this, 10);
    m_txtIpA.SetWindowText(_T("192.168.0.200"));
    m_btnConnectA.Create(_T("Connect"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+210, ay, ax+300, ay+25), this, 101);
    m_btnDisconnectA.Create(_T("Disconnect"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+310, ay, ax+400, ay+25), this, 102);

    ay += 40;
    m_lblCmdA.Create(_T("CMD:"), WS_CHILD | WS_VISIBLE, CRect(ax, ay, ax+40, ay+25), this);
    m_txtCmdA.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(ax+45, ay, ax+300, ay+25), this, 11);
    m_btnSendA.Create(_T("Send"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+310, ay, ax+400, ay+25), this, 103);

    ay += 40;
    ay += 40;
    // Motor 1 제어
    m_lblM1A.Create(_T("Motor 1:"), WS_CHILD | WS_VISIBLE, CRect(ax, ay+5, ax+60, ay+25), this);
    m_btnSvOnA.Create(_T("SV ON"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+65, ay, ax+125, ay+30), this, 111);
    m_btnSvOffA.Create(_T("SV OFF"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+130, ay, ax+190, ay+30), this, 112);
    m_btnJp1A.Create(_T("JOG +"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+195, ay, ax+260, ay+30), this, 121);
    m_btnJm1A.Create(_T("JOG -"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+265, ay, ax+330, ay+30), this, 122);
    m_txtPos1A.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, CRect(ax+335, ay+5, ax+440, ay+25), this, 151);

    ay += 40;
    // Motor 2 제어
    m_lblM2A.Create(_T("Motor 2:"), WS_CHILD | WS_VISIBLE, CRect(ax, ay+5, ax+60, ay+25), this);
    m_btnSvOn2A.Create(_T("SV ON"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+65, ay, ax+125, ay+30), this, 113);
    m_btnSvOff2A.Create(_T("SV OFF"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+130, ay, ax+190, ay+30), this, 114);
    m_btnJp2A.Create(_T("JOG +"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+195, ay, ax+260, ay+30), this, 123);
    m_btnJm2A.Create(_T("JOG -"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+265, ay, ax+330, ay+30), this, 124);
    m_txtPos2A.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, CRect(ax+335, ay+5, ax+440, ay+25), this, 152);

    ay += 40;
    m_lblFileA.Create(_T("File:"), WS_CHILD | WS_VISIBLE, CRect(ax, ay+5, ax+40, ay+25), this);
    m_txtFileA.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(ax+45, ay, ax+260, ay+25), this, 13);
    m_txtFileA.SetWindowText(_T("C:\\Temp\\test.pmc"));
    m_btnSelectA.Create(_T("..."), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+265, ay, ax+305, ay+25), this, 132);
    m_btnFileA.Create(_T("Download"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(ax+310, ay, ax+440, ay+25), this, 131);

    ay += 40;
    m_txtLogA.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, CRect(ax, ay, ax+aw, ay+400), this, 14);

    // ================= [ PMAC B Area (우측) ] =================
    int bx = 600, by = 20, bw = 550; // 우측 영역 (너비 550)
    m_lblIpB.Create(_T("IP B:"), WS_CHILD | WS_VISIBLE, CRect(bx, by, bx+40, by+25), this);
    m_txtIpB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(bx+45, by, bx+200, by+25), this, 20);
    m_txtIpB.SetWindowText(_T("192.168.0.201"));
    m_btnConnectB.Create(_T("Connect"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+210, by, bx+300, by+25), this, 201);
    m_btnDisconnectB.Create(_T("Disconnect"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+310, by, bx+400, by+25), this, 202);

    by += 40;
    m_lblCmdB.Create(_T("CMD:"), WS_CHILD | WS_VISIBLE, CRect(bx, by, bx+40, by+25), this);
    m_txtCmdB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(bx+45, by, bx+300, by+25), this, 21);
    m_btnSendB.Create(_T("Send"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+310, by, bx+400, by+25), this, 203);

    by += 40;
    by += 40;
    // Motor 1 제어
    m_lblM1B.Create(_T("Motor 1:"), WS_CHILD | WS_VISIBLE, CRect(bx, by+5, bx+60, by+25), this);
    m_btnSvOnB.Create(_T("SV ON"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+65, by, bx+125, by+30), this, 211);
    m_btnSvOffB.Create(_T("SV OFF"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+130, by, bx+190, by+30), this, 212);
    m_btnJp1B.Create(_T("JOG +"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+195, by, bx+260, by+30), this, 221);
    m_btnJm1B.Create(_T("JOG -"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+265, by, bx+330, by+30), this, 222);
    m_txtPos1B.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, CRect(bx+335, by+5, bx+440, by+25), this, 251);

    by += 40;
    // Motor 2 제어
    m_lblM2B.Create(_T("Motor 2:"), WS_CHILD | WS_VISIBLE, CRect(bx, by+5, bx+60, by+25), this);
    m_btnSvOn2B.Create(_T("SV ON"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+65, by, bx+125, by+30), this, 213);
    m_btnSvOff2B.Create(_T("SV OFF"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+130, by, bx+190, by+30), this, 214);
    m_btnJp2B.Create(_T("JOG +"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+195, by, bx+260, by+30), this, 223);
    m_btnJm2B.Create(_T("JOG -"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+265, by, bx+330, by+30), this, 224);
    m_txtPos2B.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, CRect(bx+335, by+5, bx+440, by+25), this, 252);

    by += 40;
    m_lblFileB.Create(_T("File:"), WS_CHILD | WS_VISIBLE, CRect(bx, by+5, bx+40, by+25), this);
    m_txtFileB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(bx+45, by, bx+260, by+25), this, 23);
    m_txtFileB.SetWindowText(_T("C:\\Temp\\test.pmc"));
    m_btnSelectB.Create(_T("..."), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+265, by, bx+305, by+25), this, 232);
    m_btnFileB.Create(_T("Download"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(bx+310, by, bx+440, by+25), this, 231);

    by += 40;
    m_txtLogB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, CRect(bx, by, bx+bw, by+400), this, 24);
    // ==========================================

	// 생성된 모든 컨트롤에 폰트 적용
	CWnd* pChildNode = GetWindow(GW_CHILD);
	while (pChildNode != nullptr)
	{
		pChildNode->SetFont(pFont);
		pChildNode = pChildNode->GetNextWindow();
	}

	// 시작 시 200ms 주기로 폴링 타이머 설정
	SetTimer(1, 200, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPDKSampleUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CPDKSampleUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CPDKSampleUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ---------------------------------------------------------------------------------
// 듀얼 패널 사용자 추가 이벤트 핸들러 및 헬퍼 함수
// ---------------------------------------------------------------------------------
void CPDKSampleUIDlg::Log(const CString& target, const CString& msg)
{
    CString timeStr = CTime::GetCurrentTime().Format(_T("[%H:%M:%S] "));
    CString fullMsg = timeStr + msg + _T("\r\n");

    CEdit* pLog = (target == _T("A")) ? &m_txtLogA : &m_txtLogB;
    int len = pLog->GetWindowTextLength();
    pLog->SetSel(len, len);
    pLog->ReplaceSel(fullMsg);
}

// ==========================================
// PMAC A Handlers
// ==========================================
void CPDKSampleUIDlg::OnBtnConnectA()
{
    CString ip; m_txtIpA.GetWindowText(ip);
    System::String^ sysIp = gcnew System::String(ip);
    
    m_manager->AddDevice(sysIp);
    auto dev = m_manager->GetDevice(sysIp);
    
    Log(_T("A"), ip + _T(" 에 연결 시도..."));
    bool res = dev->Connect("root", "deltatau");
    if(res) Log(_T("A"), _T("연결 성공!"));
    else Log(_T("A"), _T("연결 실패!"));
}

void CPDKSampleUIDlg::OnBtnDisconnectA()
{
    CString ip; m_txtIpA.GetWindowText(ip);
    auto dev = m_manager->GetDevice(gcnew System::String(ip));
    if(dev != nullptr && dev->IsConnected) {
        dev->Disconnect();
        Log(_T("A"), _T("연결 해제됨."));
    }
}

void CPDKSampleUIDlg::OnBtnSendA()
{
    CString ip; m_txtIpA.GetWindowText(ip);
    CString cmd; m_txtCmdA.GetWindowText(cmd);
    
    auto dev = m_manager->GetDevice(gcnew System::String(ip));
    if(dev == nullptr || !dev->IsConnected) {
        Log(_T("A"), _T("Error: Device not connected."));
        return;
    }
    
    System::String^ sysRes = dev->SendCommand(gcnew System::String(cmd));
    CString cstrRes(sysRes);

    // PMAC 제어 문자 필터링 (ACK: \x06, CR: \r)
    cstrRes.Replace(_T("\x06"), _T(""));
    cstrRes.Replace(_T("\r"), _T(""));
    cstrRes.Trim(); // 앞뒤 공백 및 개행문자 제거

    Log(_T("A"), _T("TX: ") + cmd + _T(" | RX: ") + cstrRes);
}

void CPDKSampleUIDlg::OnBtnSvOnA()
{
	CString ip; m_txtIpA.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#1j/"));
		Log(_T("A"), _T("SV ON (#1j/)"));
	}
}
void CPDKSampleUIDlg::OnBtnSvOffA()
{
	CString ip; m_txtIpA.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#1k"));
		Log(_T("A"), _T("SV OFF (#1k)"));
	}
}
void CPDKSampleUIDlg::OnBtnJp1A()
{
	CString ip; m_txtIpA.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#1j+"));
		Log(_T("A"), _T("JOG+ M1 (#1j+)"));
	}
}
void CPDKSampleUIDlg::OnBtnJm1A()
{
	CString ip; m_txtIpA.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#1j-"));
		Log(_T("A"), _T("JOG- M1 (#1j-)"));
	}
}
void CPDKSampleUIDlg::OnBtnJp2A()
{
	CString ip; m_txtIpA.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#2j+"));
		Log(_T("A"), _T("JOG+ M2 (#2j+)"));
	}
}
void CPDKSampleUIDlg::OnBtnJm2A()
{
	CString ip; m_txtIpA.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#2j-"));
		Log(_T("A"), _T("JOG- M2 (#2j-)"));
	}
}
void CPDKSampleUIDlg::OnBtnSelectA()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("PMAC Files (*.pmc;*.cfg;*.txt)|*.pmc;*.cfg;*.txt|All Files (*.*)|*.*||"), this);
	if (dlg.DoModal() == IDOK) {
		m_txtFileA.SetWindowText(dlg.GetPathName());
	}
}

void CPDKSampleUIDlg::OnBtnFileA()
{
	CString ip, localPath;
	m_txtIpA.GetWindowText(ip);
	m_txtFileA.GetWindowText(localPath);

	if(localPath.IsEmpty()) return;

	int pos = localPath.ReverseFind('\\');
	CString fileName = (pos != -1) ? localPath.Mid(pos + 1) : localPath;
	CString remotePath = _T("/var/ftp/usrflash/Temp/") + fileName;
	
	Log(_T("A"), _T("[File Download] 시도: ") + localPath + _T(" -> ") + remotePath);

	CT2A asciiIp(ip);
	CT2A asciiLocal(localPath);
	CT2A asciiRemote(remotePath);

	ICreateFTPClient(1, NULL); 
	if(IConnectFTPClient(asciiIp.m_psz)) {
		// PDK FTP 라이브러리: IFTPDownloadFile이 PC->PMAC으로 동작할 수 있음 (원본 코드 참조)
		bool bRes = IFTPDownloadFile(asciiLocal.m_psz, asciiRemote.m_psz);
		if(!bRes) {
			// 실패 시 IFTPUploadFile 로 재시도
			bRes = IFTPUploadFile(asciiLocal.m_psz);
		}

		if(bRes) {
			Log(_T("A"), _T("다운로드 성공. PMAC 버퍼 적용(Compile) 시작..."));
			auto dev = m_manager->GetDevice(gcnew System::String(ip));
			if (dev != nullptr && dev->IsConnected) {
				CString cmd = _T("system gpascii -i") + remotePath + _T(" -e/var/ftp/usrflash/Project/Log/filednlderror.log");
				System::String^ sysRes = dev->SendCommand(gcnew System::String(cmd));
				CString cstrRes(sysRes);
				cstrRes.Replace(_T("\x06"), _T("")); cstrRes.Replace(_T("\r"), _T("")); cstrRes.Trim();
				Log(_T("A"), _T("적용 응답: ") + cstrRes);
			} else {
				Log(_T("A"), _T("에러: 장비 미연결 상태라 gpascii 명령 전송 실패"));
			}
		}
		else Log(_T("A"), _T("다운로드 실패!"));
		
		IDisconnectFTPConnection();
	} else {
		Log(_T("A"), _T("FTP 연결 실패!"));
	}
}

// ==========================================
// PMAC B Handlers
// ==========================================
void CPDKSampleUIDlg::OnBtnConnectB()
{
    CString ip; m_txtIpB.GetWindowText(ip);
    System::String^ sysIp = gcnew System::String(ip);
    
    m_manager->AddDevice(sysIp);
    auto dev = m_manager->GetDevice(sysIp);
    
    Log(_T("B"), ip + _T(" 에 연결 시도..."));
    bool res = dev->Connect("root", "deltatau");
    if(res) Log(_T("B"), _T("연결 성공!"));
    else Log(_T("B"), _T("연결 실패!"));
}

void CPDKSampleUIDlg::OnBtnDisconnectB()
{
    CString ip; m_txtIpB.GetWindowText(ip);
    auto dev = m_manager->GetDevice(gcnew System::String(ip));
    if(dev != nullptr && dev->IsConnected) {
        dev->Disconnect();
        Log(_T("B"), _T("연결 해제됨."));
    }
}

void CPDKSampleUIDlg::OnBtnSendB()
{
    CString ip; m_txtIpB.GetWindowText(ip);
    CString cmd; m_txtCmdB.GetWindowText(cmd);
    
    auto dev = m_manager->GetDevice(gcnew System::String(ip));
    if(dev == nullptr || !dev->IsConnected) {
        Log(_T("B"), _T("Error: Device not connected."));
        return;
    }
    
    System::String^ sysRes = dev->SendCommand(gcnew System::String(cmd));
    CString cstrRes(sysRes);

    // PMAC 제어 문자 필터링 (ACK: \x06, CR: \r)
    cstrRes.Replace(_T("\x06"), _T(""));
    cstrRes.Replace(_T("\r"), _T(""));
    cstrRes.Trim(); // 앞뒤 공백 및 개행문자 제거

    Log(_T("B"), _T("TX: ") + cmd + _T(" | RX: ") + cstrRes);
}

void CPDKSampleUIDlg::OnBtnSvOnB()
{
	CString ip; m_txtIpB.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#1j/"));
		Log(_T("B"), _T("SV ON (#1j/)"));
	}
}
void CPDKSampleUIDlg::OnBtnSvOffB()
{
	CString ip; m_txtIpB.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#1k"));
		Log(_T("B"), _T("SV OFF (#1k)"));
	}
}
void CPDKSampleUIDlg::OnBtnJp1B()
{
	CString ip; m_txtIpB.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#1j+"));
		Log(_T("B"), _T("JOG+ M1 (#1j+)"));
	}
}
void CPDKSampleUIDlg::OnBtnJm1B()
{
	CString ip; m_txtIpB.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#1j-"));
		Log(_T("B"), _T("JOG- M1 (#1j-)"));
	}
}
void CPDKSampleUIDlg::OnBtnJp2B()
{
	CString ip; m_txtIpB.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#2j+"));
		Log(_T("B"), _T("JOG+ M2 (#2j+)"));
	}
}
void CPDKSampleUIDlg::OnBtnJm2B()
{
	CString ip; m_txtIpB.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#2j-"));
		Log(_T("B"), _T("JOG- M2 (#2j-)"));
	}
}
void CPDKSampleUIDlg::OnBtnSelectB()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("PMAC Files (*.pmc;*.cfg;*.txt)|*.pmc;*.cfg;*.txt|All Files (*.*)|*.*||"), this);
	if (dlg.DoModal() == IDOK) {
		m_txtFileB.SetWindowText(dlg.GetPathName());
	}
}

void CPDKSampleUIDlg::OnBtnFileB()
{
	CString ip, localPath;
	m_txtIpB.GetWindowText(ip);
	m_txtFileB.GetWindowText(localPath);

	if(localPath.IsEmpty()) return;

	int pos = localPath.ReverseFind('\\');
	CString fileName = (pos != -1) ? localPath.Mid(pos + 1) : localPath;
	CString remotePath = _T("/var/ftp/usrflash/Temp/") + fileName;
	
	Log(_T("B"), _T("[File Download] 시도: ") + localPath + _T(" -> ") + remotePath);

	CT2A asciiIp(ip);
	CT2A asciiLocal(localPath);
	CT2A asciiRemote(remotePath);

	ICreateFTPClient(1, NULL); 
	if(IConnectFTPClient(asciiIp.m_psz)) {
		bool bRes = IFTPDownloadFile(asciiLocal.m_psz, asciiRemote.m_psz);
		if(!bRes) {
			bRes = IFTPUploadFile(asciiLocal.m_psz);
		}

		if(bRes) {
			Log(_T("B"), _T("다운로드 성공. PMAC 버퍼 적용(Compile) 시작..."));
			auto dev = m_manager->GetDevice(gcnew System::String(ip));
			if (dev != nullptr && dev->IsConnected) {
				CString cmd = _T("system gpascii -i") + remotePath + _T(" -e/var/ftp/usrflash/Project/Log/filednlderror.log");
				System::String^ sysRes = dev->SendCommand(gcnew System::String(cmd));
				CString cstrRes(sysRes);
				cstrRes.Replace(_T("\x06"), _T("")); cstrRes.Replace(_T("\r"), _T("")); cstrRes.Trim();
				Log(_T("B"), _T("적용 응답: ") + cstrRes);
			} else {
				Log(_T("B"), _T("에러: 장비 미연결 상태라 gpascii 명령 전송 실패"));
			}
		}
		else Log(_T("B"), _T("다운로드 실패!"));
		
		IDisconnectFTPConnection();
	} else {
		Log(_T("B"), _T("FTP 연결 실패!"));
	}
}


void CPDKSampleUIDlg::OnBtnSvOn2A()
{
	CString ip; m_txtIpA.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#2j/"));
		Log(_T("A"), _T("SV ON M2 (#2j/)"));
	}
}

void CPDKSampleUIDlg::OnBtnSvOff2A()
{
	CString ip; m_txtIpA.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#2k"));
		Log(_T("A"), _T("SV OFF M2 (#2k)"));
	}
}

void CPDKSampleUIDlg::OnBtnSvOn2B()
{
	CString ip; m_txtIpB.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#2j/"));
		Log(_T("B"), _T("SV ON M2 (#2j/)"));
	}
}

void CPDKSampleUIDlg::OnBtnSvOff2B()
{
	CString ip; m_txtIpB.GetWindowText(ip);
	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		device->SendCommand(gcnew System::String("#2k"));
		Log(_T("B"), _T("SV OFF M2 (#2k)"));
	}
}

void CPDKSampleUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		CString ipA; m_txtIpA.GetWindowText(ipA);
		CString ipB; m_txtIpB.GetWindowText(ipB);

		// 메모리 우상향(GC Pressure) 방지를 위한 쿼리 문자열 정적 캐싱 (C++/CLI 에러 방지용 gcroot 적용)
		static gcroot<System::String^> cmdAct1 = gcnew System::String("Motor[1].ActPos");
		static gcroot<System::String^> cmdHome1 = gcnew System::String("Motor[1].HomePos");
		static gcroot<System::String^> cmdAct2 = gcnew System::String("Motor[2].ActPos");
		static gcroot<System::String^> cmdHome2 = gcnew System::String("Motor[2].HomePos");

		auto devA = m_manager->GetDevice(gcnew System::String(ipA));
		if (devA != nullptr && devA->IsConnected) {
			// 모터 1
			CString strAct1 = CString(devA->SendCommand(cmdAct1));
			strAct1.Replace(_T("\x06"), _T("")); strAct1.Replace(_T("\r"), _T("")); strAct1.Replace(_T("\n"), _T("")); strAct1.Trim();
			int eq1 = strAct1.Find(_T("=")); if(eq1 != -1) strAct1 = strAct1.Mid(eq1 + 1);

			CString strHome1 = CString(devA->SendCommand(cmdHome1));
			strHome1.Replace(_T("\x06"), _T("")); strHome1.Replace(_T("\r"), _T("")); strHome1.Replace(_T("\n"), _T("")); strHome1.Trim();
			int eh1 = strHome1.Find(_T("=")); if(eh1 != -1) strHome1 = strHome1.Mid(eh1 + 1);

			double valAct1 = _ttof(strAct1);
			double valHome1 = _ttof(strHome1);
			CString res1; res1.Format(_T("%.4f"), valAct1 - valHome1);
			m_txtPos1A.SetWindowText(res1);

			// 모터 2
			CString strAct2 = CString(devA->SendCommand(cmdAct2));
			strAct2.Replace(_T("\x06"), _T("")); strAct2.Replace(_T("\r"), _T("")); strAct2.Replace(_T("\n"), _T("")); strAct2.Trim();
			int eq2 = strAct2.Find(_T("=")); if(eq2 != -1) strAct2 = strAct2.Mid(eq2 + 1);

			CString strHome2 = CString(devA->SendCommand(cmdHome2));
			strHome2.Replace(_T("\x06"), _T("")); strHome2.Replace(_T("\r"), _T("")); strHome2.Replace(_T("\n"), _T("")); strHome2.Trim();
			int eh2 = strHome2.Find(_T("=")); if(eh2 != -1) strHome2 = strHome2.Mid(eh2 + 1);
			
			double valAct2 = _ttof(strAct2);
			double valHome2 = _ttof(strHome2);
			CString res2; res2.Format(_T("%.4f"), valAct2 - valHome2);
			m_txtPos2A.SetWindowText(res2);
		}

		auto devB = m_manager->GetDevice(gcnew System::String(ipB));
		if (devB != nullptr && devB->IsConnected) {
			// 모터 1
			CString strAct1 = CString(devB->SendCommand(cmdAct1));
			strAct1.Replace(_T("\x06"), _T("")); strAct1.Replace(_T("\r"), _T("")); strAct1.Replace(_T("\n"), _T("")); strAct1.Trim();
			int eq1 = strAct1.Find(_T("=")); if(eq1 != -1) strAct1 = strAct1.Mid(eq1 + 1);

			CString strHome1 = CString(devB->SendCommand(cmdHome1));
			strHome1.Replace(_T("\x06"), _T("")); strHome1.Replace(_T("\r"), _T("")); strHome1.Replace(_T("\n"), _T("")); strHome1.Trim();
			int eh1 = strHome1.Find(_T("=")); if(eh1 != -1) strHome1 = strHome1.Mid(eh1 + 1);
			
			double valAct1 = _ttof(strAct1);
			double valHome1 = _ttof(strHome1);
			CString res1; res1.Format(_T("%.4f"), valAct1 - valHome1);
			m_txtPos1B.SetWindowText(res1);

			// 모터 2
			CString strAct2 = CString(devB->SendCommand(cmdAct2));
			strAct2.Replace(_T("\x06"), _T("")); strAct2.Replace(_T("\r"), _T("")); strAct2.Replace(_T("\n"), _T("")); strAct2.Trim();
			int eq2 = strAct2.Find(_T("=")); if(eq2 != -1) strAct2 = strAct2.Mid(eq2 + 1);

			CString strHome2 = CString(devB->SendCommand(cmdHome2));
			strHome2.Replace(_T("\x06"), _T("")); strHome2.Replace(_T("\r"), _T("")); strHome2.Replace(_T("\n"), _T("")); strHome2.Trim();
			int eh2 = strHome2.Find(_T("=")); if(eh2 != -1) strHome2 = strHome2.Mid(eh2 + 1);
			
			double valAct2 = _ttof(strAct2);
			double valHome2 = _ttof(strHome2);
			CString res2; res2.Format(_T("%.4f"), valAct2 - valHome2);
			m_txtPos2B.SetWindowText(res2);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CPDKSampleUIDlg::HandleJogCommand(CString pmacId, int motor, bool isPlus, bool isDown)
{
	CString ip;
	if (pmacId == _T("A")) m_txtIpA.GetWindowText(ip);
	else m_txtIpB.GetWindowText(ip);

	auto device = m_manager->GetDevice(gcnew System::String(ip));
	if (device != nullptr && device->IsConnected) {
		CString cmd;
		if (isDown) {
			cmd.Format(_T("#%dj%s"), motor, isPlus ? _T("+") : _T("-"));
			Log(pmacId, CString(_T("JOG ")) + (isPlus ? _T("+") : _T("-")) + _T(" M") + CString(std::to_wstring(motor).c_str()) + _T(" (") + cmd + _T(")"));
		} else {
			cmd.Format(_T("#%dj/"), motor);
			Log(pmacId, CString(_T("JOG STOP M")) + CString(std::to_wstring(motor).c_str()) + _T(" (") + cmd + _T(")"));
		}
		device->SendCommand(gcnew System::String(cmd));
	}
}

BOOL CPDKSampleUIDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP)
	{
		HWND hWnd = pMsg->hwnd;
		bool isDown = (pMsg->message == WM_LBUTTONDOWN);
		
		// 버튼 핸들 매칭
		if (hWnd == m_btnJp1A.GetSafeHwnd())      { HandleJogCommand(_T("A"), 1, true, isDown); return TRUE; }
		else if (hWnd == m_btnJm1A.GetSafeHwnd()) { HandleJogCommand(_T("A"), 1, false, isDown); return TRUE; }
		else if (hWnd == m_btnJp2A.GetSafeHwnd()) { HandleJogCommand(_T("A"), 2, true, isDown); return TRUE; }
		else if (hWnd == m_btnJm2A.GetSafeHwnd()) { HandleJogCommand(_T("A"), 2, false, isDown); return TRUE; }
		else if (hWnd == m_btnJp1B.GetSafeHwnd()) { HandleJogCommand(_T("B"), 1, true, isDown); return TRUE; }
		else if (hWnd == m_btnJm1B.GetSafeHwnd()) { HandleJogCommand(_T("B"), 1, false, isDown); return TRUE; }
		else if (hWnd == m_btnJp2B.GetSafeHwnd()) { HandleJogCommand(_T("B"), 2, true, isDown); return TRUE; }
		else if (hWnd == m_btnJm2B.GetSafeHwnd()) { HandleJogCommand(_T("B"), 2, false, isDown); return TRUE; }
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
