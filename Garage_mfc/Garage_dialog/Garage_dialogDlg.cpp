
// Garage_dialogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Garage_dialog.h"
#include "Garage_dialogDlg.h"
#include "afxdialogex.h"

#include "garageLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_Timer_DOOR 100    // �����Ŷ���ʱ��
#define ID_Timer_CLOCK 101   // ����ʱ��,��������������ڣ�

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGarage_dialogDlg �Ի���




CGarage_dialogDlg::CGarage_dialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGarage_dialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGarage_dialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BTN_OK);
	DDX_Control(pDX, IDC_btnSwitch, m_btnSwitch);
	DDX_Control(pDX, IDC_PicDoor, m_PicDoor);
	DDX_Control(pDX, IDC_PicGarage, m_PicGarage);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BtnStart, m_BtnStart);
	DDX_Control(pDX, IDC_BtnEnd, m_BtnEnd);
}

BEGIN_MESSAGE_MAP(CGarage_dialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGarage_dialogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_btnSwitch, &CGarage_dialogDlg::OnBnClickedbtnSwitch)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BtnStart, &CGarage_dialogDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDCANCEL, &CGarage_dialogDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BtnEnd, &CGarage_dialogDlg::OnBnClickedBtnEnd)
END_MESSAGE_MAP()


// CGarage_dialogDlg ��Ϣ�������

BOOL CGarage_dialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// ʹ���򵯳�console������ʹ��printf("ok!!");
	AllocConsole();
	freopen("CONOUT$","a+",stdout);

	// ���ſ���λͼ���������С������λͼ��С
	CBitmap bitmap;  // ����λͼ����
	HBITMAP hBmp;    // ����λͼ����ľ��
	bitmap.LoadBitmap(IDB_bmpSwitch);
	hBmp = (HBITMAP)bitmap.GetSafeHandle();
	m_btnSwitch.SetBitmap(hBmp); 
	BITMAP bmSize;
	bitmap.GetBitmap(&bmSize);
	printf("%d,%d",bmSize.bmWidth,bmSize.bmHeight);
	//m_btnSwitch.SetWindowPos(&wndTop,0,0,bmSize.bmWidth,bmSize.bmHeight,SWP_SHOWWINDOW);
	m_btnSwitch.SetWindowPos(&wndTop,0,0,bmSize.bmWidth,bmSize.bmHeight,SWP_NOMOVE);
	
	// �г��ĳ���ͼƬ(m_PicGarage)�����棬������(m_PicDoor)������
	RECT rectDoor,rectGarage,rectSwitch;
	m_PicDoor.GetWindowRect(&rectDoor);
	m_PicGarage.GetWindowRect(&rectGarage);
	m_PicGarage.SetWindowPos(&wndTop,0,0,rectGarage.right-rectGarage.left,
		rectGarage.bottom-rectGarage.top,SWP_SHOWWINDOW |  SWP_NOCOPYBITS);
    m_PicGarage.GetWindowRect(&rectGarage); // �Ѿ��ƶ������»�ȡ
	m_PicDoor.SetWindowPos(&wndBottom,0,0,rectDoor.right-rectDoor.left,
		rectGarage.bottom-rectGarage.top,SWP_SHOWWINDOW |  SWP_NOCOPYBITS);

	// ���µ��������ŵ���ʾλ��
	m_PicDoor.GetWindowRect(&rectDoor); // �Ѿ��ƶ������»�ȡ
	m_btnSwitch.GetWindowRect(&rectSwitch);
	m_btnSwitch.SetWindowPos(&wndTop,rectDoor.right,rectSwitch.top,0,0,SWP_NOSIZE);

	// ��ʼֵ
	doorHeight = rectDoor.bottom-rectDoor.top;
	currentDoorPosition = 0;
	step = 10;
	interval = 100;
	samplingInterval = 100;
	State = DoorClosed;

	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	// SWP_NOCOPYBITS������ͻ�������������,SWP_NOMOVE��ά�ֵ�ǰλ�ã�����X��Y������
	// SWP_DRAWFRAME���ڴ�����Χ��һ���߿򣨶����ڴ����������У�,SWP_NOSIZE ���ֵ�ǰ�Ĵ�С������cx��cy������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGarage_dialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGarage_dialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGarage_dialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGarage_dialogDlg::OnBnClickedOk()
{
	m_BTN_OK.ModifyStyle(1,WS_BORDER,0);
	//static int y = 0;
	//static bool border = false;
	//y -= 100;
	//printf("OnBnClickedOk()\n");
	//
	//HWND hwnd = m_PicDoor.GetSafeHwnd();
	//
	//// SWP_NOCOPYBITS������ͻ�������������,SWP_NOMOVE��ά�ֵ�ǰλ�ã�����X��Y������
	//// SWP_DRAWFRAME���ڴ�����Χ��һ���߿򣨶����ڴ����������У�,SWP_NOSIZE ���ֵ�ǰ�Ĵ�С������cx��cy������
	////m_PicDoor.SetWindowPos(&wndTop,0,y,0,0,SWP_NOSIZE |  SWP_NOCOPYBITS);
	//m_PicDoor.SetWindowPos(&wndTop,0,y,0,0,SWP_NOSIZE);
	////m_PicDoor.SetWindowPos(&wndTop,0,y,0,0,SWP_NOSIZE | SWP_DRAWFRAME | SWP_NOCOPYBITS);
	//
	////if (border) m_PicDoor.ModifyStyle(1,WS_BORDER,0);
	//border != border;
	////::SetWindowPos(hwnd,HWND_TOP,0,y,0,0,SWP_NOSIZE);  // ��Чm_PicDoor.SetWindowPos(&wndTop,0,y,0,0,SWP_NOSIZE);
	//
	////m_btnSwitch.SetWindowPos(&wndTop,0,0,100,200,SWP_NOMOVE);

	////m_PicDoor.UpdateWindow(); // ��Ч
	////UpdateWindow();
	
	
	//CDialogEx::OnOK();
}

// �����ſ��ر�����
void CGarage_dialogDlg::OnBnClickedbtnSwitch()
{
	printf("�����ſ��ر�����\n");
	Lib_ButtonPressed = !Lib_ButtonPressed;
}

// ��ʱ���ص�����
void CGarage_dialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent) 
	{
		case ID_Timer_DOOR: // �����Ŷ���
			DoorUpDown(State);
			break;
		case ID_Timer_CLOCK: //����ʱ��
			main_control(&State);
			break;
		defaule:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// �ſ���������,
void CGarage_dialogDlg::DoorUpDown(int state)
{
	printf("state=%d\n",State);
	switch(state)
	{
	  case DoorClosed:
		  currentDoorPosition = 0;
		  return;
	  case DoorOpen:
		  currentDoorPosition = doorHeight;
		  return;
	  case DoorOpening:
	  case DoorClosing:
		  currentDoorPosition += Lib_Power*step;
		  break;
	  default:
		  printf("�����ڵ�״̬!!!\n");
		  break;
	}
	printf("currentDoorPosition=%d\n",currentDoorPosition);
	// �������λ��
	Lib_CurrentDoorPosition = (Lib_DoorHeight/doorHeight)*currentDoorPosition;
	// SWP_NOZORDER�����Ե�һ��������SWP_NOSIZE�����Ե�3��4������
	m_PicDoor.SetWindowPos(0,0,-currentDoorPosition,0,0,SWP_NOSIZE | SWP_NOZORDER);
}
// �ſ���������,
//void CGarage_dialogDlg::DoorUpDown111()
//{
//	int step=20;
//	if(uping && doorCurrentTop < -doorHeight) {
//		KillTimer(ID_Timer_DOOR); // ���Ŷ����������ŵ�����
//		doorCurrentTop = -doorHeight;
//		m_PicDoor.SetWindowPos(0,0,doorCurrentTop,0,0,SWP_NOSIZE | SWP_NOZORDER);
//	}
//	if(!uping && doorCurrentTop > 0) {
//		KillTimer(ID_Timer_DOOR); // ���Ŷ����������ŵ��ײ�
//		doorCurrentTop = 0;
//		m_PicDoor.SetWindowPos(0,0,doorCurrentTop,0,0,SWP_NOSIZE | SWP_NOZORDER);
//	}
//	if(uping) doorCurrentTop -= step;
//	else doorCurrentTop += step;
//	//m_PicDoor.SetWindowPos(0,0,doorCurrentTop,0,0,SWP_NOSIZE | SWP_NOCOPYBITS);
//	//m_PicDoor.SetWindowPos(0,0,doorCurrentTop,0,0,SWP_NOSIZE);
//	m_PicDoor.SetWindowPos(0,0,doorCurrentTop,0,0,SWP_NOSIZE | SWP_NOZORDER);
//	//m_PicDoor.SetWindowPos(0,0,doorCurrentTop,0,0,SWP_NOSIZE | SWP_NOCOPYBITS);
//}


// ��ʼ����
void CGarage_dialogDlg::OnBnClickedBtnStart()
{	
	// ��ʼ����
	SetTimer(ID_Timer_CLOCK,samplingInterval,NULL);
	// ��ʼ�����Ŷ���
	SetTimer(ID_Timer_DOOR,interval,NULL);
	m_BtnStart.EnableWindow(false);
	m_BtnEnd.EnableWindow(true);

	GarageStartup();
}

// ��������
void CGarage_dialogDlg::OnBnClickedBtnEnd()
{
	KillTimer(ID_Timer_CLOCK);
	KillTimer(ID_Timer_DOOR);
	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	GarageShutdown();
}

// �ر�
void CGarage_dialogDlg::OnBnClickedCancel()
{
	KillTimer(ID_Timer_CLOCK);
	KillTimer(ID_Timer_DOOR);
	GarageShutdown();

	CDialogEx::OnCancel();
}
