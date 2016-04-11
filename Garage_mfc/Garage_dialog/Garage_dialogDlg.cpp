
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

#define ID_Timer_DOOR 100      // �����Ŷ���ʱ��
#define ID_Timer_Closing 101   // ����״̬���ض�ʱ���ڣ��޶������Զ����� 

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
	DDX_Control(pDX, IDC_BtnCat, m_BtnCat);
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
	ON_BN_CLICKED(IDC_BtnCat, &CGarage_dialogDlg::OnBnClickedBtnCat)
	ON_WM_SHOWWINDOW()
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

	// ʹ���򵯳�console������ʹ��printf("ok!!");
	AllocConsole();
	freopen("CONOUT$","a+",stdout);

	// �г��ĳ���ͼƬ(m_PicGarage)�����棬������(m_PicDoor)������
	//m_PicDoor.SetWindowPos(0,0,0,0,0,SWP_NOSIZE | SWP_NOZORDER);
	//m_PicGarage.SetWindowPos(0,0,0,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_PicDoor.SetWindowPos(&wndBottom,0,0,0,0,SWP_NOSIZE);
	m_PicGarage.SetWindowPos(&wndTop,0,0,0,0,SWP_NOSIZE);

	// ���ſ���λͼ���������С������λͼ��С
	CBitmap bitmap;  // ����λͼ����
	HBITMAP hBmp;    // ����λͼ����ľ��
	bitmap.LoadBitmap(IDB_bmpSwitch);
	hBmp = (HBITMAP)bitmap.GetSafeHandle();
	m_btnSwitch.SetBitmap(hBmp); 
	BITMAP bmSize;
	bitmap.GetBitmap(&bmSize);

	RECT rectDoor,rectGarage,rectSwitch;
	m_PicDoor.GetClientRect(&rectDoor);   // left,top=0;right,bottomΪ��С
	m_PicGarage.GetClientRect(&rectGarage);
	m_btnSwitch.GetClientRect(&rectSwitch);
	int x,y;
	x = rectDoor.right + 15; 
	y = (rectDoor.bottom - rectSwitch.bottom)/2;
	// SWP_NOCOPYBITS������ͻ�������������,SWP_NOMOVE��ά�ֵ�ǰλ�ã�����X��Y������
	// SWP_DRAWFRAME���ڴ�����Χ��һ���߿򣨶����ڴ����������У�,SWP_NOSIZE ���ֵ�ǰ�Ĵ�С������cx��cy������
	// SWP_NOZORDER: ���ı䴰����Z��λ��,���Ե�һ������
	//m_btnSwitch.SetWindowPos(&wndTop,x,y,bmSize.bmWidth,bmSize.bmHeight,SWP_SHOWWINDOW | SWP_NOZORDER);
	m_btnSwitch.SetWindowPos(&wndTop,x,y,bmSize.bmWidth,bmSize.bmHeight,SWP_SHOWWINDOW);
	

	// ����Cat��ťλͼ����ʹ��С��λͼһ��
	CBitmap catmap;     // ����λͼ����
	HBITMAP hcatmap;    // ����λͼ����ľ��
	catmap.LoadBitmap(IDB_bmpCat);
	hcatmap = (HBITMAP)catmap.GetSafeHandle();
	m_BtnCat.SetBitmap(hcatmap); 
	BITMAP catbmSize;
	catmap.GetBitmap(&catbmSize);

	// ����Cat��ťλ��(��ʼλ��)
	//RECT catRectOutDoor;
	catOutDoor_top = rectGarage.bottom - catbmSize.bmHeight;
	catOutDoor_left = rectGarage.right + 10;
	m_BtnCat.SetWindowPos(0,catOutDoor_left,catOutDoor_top,catbmSize.bmWidth,catbmSize.bmHeight,SWP_SHOWWINDOW | SWP_NOZORDER);
	
	// ����Cat��ťλ��
	catInDoor_top = catOutDoor_top;
    catInDoor_left = rectDoor.right/2; // ��Լ���ŵ��м�

	// ����ok,cancel,start,end��ť
	RECT rectBtnOk;
	m_BTN_OK.GetClientRect(&rectBtnOk);
	// �ر�
	x = rectGarage.right + 10;
	y = catInDoor_top - rectBtnOk.bottom - 10;
	m_BtnCancel.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// ����
	y = y - rectBtnOk.bottom - 10;
	m_BtnEnd.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// ��ʼ
	y = y - rectBtnOk.bottom - 10;
	m_BtnStart.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// ȷ��
	y = y - rectBtnOk.bottom - 10;
	m_BTN_OK.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// �Ŵ�Ի�����С
	int cx,cy; // �������ڱ߿�
	cx = catOutDoor_left +  rectBtnOk.right + 30;
	cy = rectGarage.bottom + 50;
	this->SetWindowPos(0,100,100,cx,cy,SWP_SHOWWINDOW | SWP_NOZORDER);
	
	// ��ʼֵ
	doorHeight = rectDoor.bottom-rectDoor.top;
	currentDoorPosition = 0;
	step = 10;
	interval = 100;
	OpenInterval = 10000;
	StartTimerClosing = false;
	State = DoorClosed;

	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	m_BTN_OK.SetWindowText(L"�����ʼ");
	m_BTN_OK.EnableWindow(false);

	printf("�������ʼ����ť����ʼ����...\n");
	printf("����ſ��أ����п����š�\n");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

/***

BOOL CGarage_dialogDlg::OnInitDialog1111()
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
	// SWP_NOCOPYBITS������ͻ�������������,SWP_NOMOVE��ά�ֵ�ǰλ�ã�����X��Y������
	// SWP_DRAWFRAME���ڴ�����Χ��һ���߿򣨶����ڴ����������У�,SWP_NOSIZE ���ֵ�ǰ�Ĵ�С������cx��cy������
	// SWP_NOZORDER: ���ı䴰����Z��λ��,���Ե�һ������
	m_btnSwitch.SetWindowPos(&wndTop,0,0,bmSize.bmWidth,bmSize.bmHeight,SWP_NOMOVE);

	// �г��ĳ���ͼƬ(m_PicGarage)�����棬������(m_PicDoor)������
	RECT rectDoor,rectGarage,rectSwitch;
	// ����ָ�����ڵı߿���εĳߴ硣
	// 1 ���ڻ�û�г�ʼ�����ʱ��ԭ�����������ڵ����Ͻǡ�OnInitDialog( )�¼��С�
	// 2 ��ʼ����ɺ�ԭ�����豸��Ļ���Ͻǡ�OnShowWindow( )�¼��С�
	m_PicDoor.GetWindowRect(&rectDoor); 
	printf("rectDoor=%d,%d,%d,%d",rectDoor.left,rectDoor.top,rectDoor.right,rectDoor.bottom);
	m_PicGarage.GetWindowRect(&rectGarage);
	m_PicGarage.SetWindowPos(&wndTop,0,0,rectGarage.right-rectGarage.left,
		rectGarage.bottom-rectGarage.top,SWP_SHOWWINDOW |  SWP_NOCOPYBITS);
	m_PicGarage.GetWindowRect(&rectGarage); // �Ѿ��ƶ������»�ȡ
	m_PicDoor.SetWindowPos(&wndBottom,0,0,rectDoor.right-rectDoor.left,
		rectGarage.bottom-rectGarage.top,SWP_SHOWWINDOW |  SWP_NOCOPYBITS);

	// ���µ��������ŵ���ʾλ��
	WINDOWPLACEMENT placement;
	m_PicDoor.GetWindowPlacement(&placement); // �Ѿ��ƶ������»�ȡ
	rectDoor = placement.rcNormalPosition;
	m_btnSwitch.GetWindowPlacement(&placement);
	rectSwitch = placement.rcNormalPosition;
	m_btnSwitch.SetWindowPos(0,rectDoor.right + 20 ,rectSwitch.top,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// ����Cat��ťλͼ����ʹ��С��λͼһ��
	CBitmap catmap;     // ����λͼ����
	HBITMAP hcatmap;    // ����λͼ����ľ��
	catmap.LoadBitmap(IDB_bmpCat);
	hcatmap = (HBITMAP)catmap.GetSafeHandle();
	m_BtnCat.SetBitmap(hcatmap); 
	BITMAP catbmSize;
	catmap.GetBitmap(&catbmSize);
	m_BtnCat.SetWindowPos(&wndTop,0,0,catbmSize.bmWidth,catbmSize.bmHeight,SWP_NOMOVE);

	// ����Cat��ťλ��(��ʼλ��)
	//RECT catRectOutDoor;
	catOutDoor_top = rectGarage.bottom - catbmSize.bmHeight;
	catOutDoor_left = rectDoor.right + 10;
	m_BtnCat.SetWindowPos(&wndTop,catOutDoor_top,catOutDoor_left,0,0,SWP_NOSIZE);
	// m_BtnCat.GetWindowPlacement(&placement); // ������ʾǰ����Ը����ڿͻ��������Ͻ�Ϊԭ��
	// catRectOutDoor = placement.rcNormalPosition;
	//catOutDoor_top = catRectOutDoor.top;
	//catOutDoor_left = catRectOutDoor.left; 

	// ����Cat��ťλ��
	//catInDoor_top = catOutDoor_top;
	//catInDoor_left = rectDoor.right/2; // ��Լ���ŵ��м�

	// ��ʼֵ
	doorHeight = rectDoor.bottom-rectDoor.top;
	currentDoorPosition = 0;
	step = 10;
	interval = 100;
	OpenInterval = 10000;
	StartTimerClosing = false;
	State = DoorClosed;

	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	m_BTN_OK.SetWindowText(L"�����ʼ");
	m_BTN_OK.EnableWindow(false);

	printf("�������ʼ����ť����ʼ����...\n");
	printf("����ſ��أ����п����š�\n");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
****/

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
	Lib_Running = !Lib_Running;
	if(Lib_Running) m_BTN_OK.SetWindowText(L"����...");
	else m_BTN_OK.SetWindowText(L"��ͣ...");
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
			main_control(&State);
			DoorUpDown(State);
			break;
		case ID_Timer_Closing: // ����״̬���ض�ʱ���ڣ��޶������Զ�����
			SetMotorPower(-1);
			State = DoorClosing;
			printf("����״̬��10����޶������Զ�����...\n");
			break;
		defaule:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// �����Ŷ���,
void CGarage_dialogDlg::DoorUpDown(int state)
{
	printfState(state); 
	if (!Lib_Running) return;
	switch(state)
	{
	  case DoorClosed:
		  currentDoorPosition = 0;
		  return;
	  case DoorOpen:
		  currentDoorPosition = doorHeight;
		  if(!StartTimerClosing) {  // �����Զ����Ŷ�ʱ��
			  // ����״̬���ض�ʱ���ڣ��޶������Զ����� 
			  SetTimer(ID_Timer_Closing,OpenInterval,NULL);
			  StartTimerClosing = true;
		  }
		  return;
	  case DoorOpening:
	  case DoorClosing:
		  currentDoorPosition += Lib_Power*step;
		  if(StartTimerClosing) { // �ر��Զ����Ŷ�ʱ��
			  KillTimer(ID_Timer_Closing);
			  StartTimerClosing = false;
		  }
		  break;
	  default:
		  printf("�����ڵ�״̬!!!\n");
		  break;
	}
	// printf("currentDoorPosition=%d\n",currentDoorPosition);
	// �������λ��
	Lib_CurrentDoorPosition = (Lib_DoorHeight/doorHeight)*currentDoorPosition;
	// SWP_NOZORDER�����Ե�һ��������SWP_NOSIZE�����Ե�3��4������
	m_PicDoor.SetWindowPos(0,0,-currentDoorPosition,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);
}

// ��ʼ����
void CGarage_dialogDlg::OnBnClickedBtnStart()
{	
	// ��ʼ�����Ŷ���
	SetTimer(ID_Timer_DOOR,interval,NULL);
	m_BtnStart.EnableWindow(false);
	m_BtnEnd.EnableWindow(true);

	GarageStartup();
	m_BTN_OK.SetWindowText(L"����...");
	m_BTN_OK.EnableWindow(true);
}

// ��������
void CGarage_dialogDlg::OnBnClickedBtnEnd()
{
	KillTimer(ID_Timer_Closing);
	KillTimer(ID_Timer_DOOR);
	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	GarageShutdown();
	m_BTN_OK.SetWindowText(L"����...");
	m_BTN_OK.EnableWindow(false);
}

// �ر�
void CGarage_dialogDlg::OnBnClickedCancel()
{
	KillTimer(ID_Timer_Closing);
	KillTimer(ID_Timer_DOOR);
	GarageShutdown();

	CDialogEx::OnCancel();
}

// Cat��ť
void CGarage_dialogDlg::OnBnClickedBtnCat()
{
	RECT catRect;
	WINDOWPLACEMENT placement;
	m_BtnCat.GetWindowPlacement(&placement);
	catRect = placement.rcNormalPosition;
	if(catRect.left < catOutDoor_left) // ����-->����
	{
		Lib_BeamBroken = false;  // ���²��ĺ����ߡ�δ��̽�⵽Cat
		m_BtnCat.SetWindowPos(0,catOutDoor_left,catOutDoor_top,0,0,SWP_NOSIZE | SWP_NOZORDER);
	}
	else // ����-->����
	{
		Lib_BeamBroken = true;  // ���²��ĺ�����̽�⵽Cat
		m_BtnCat.SetWindowPos(0,catInDoor_left,catInDoor_top,0,0,SWP_NOSIZE | SWP_NOZORDER);
	}
}

// ��ӡ��ǰ״̬
void CGarage_dialogDlg::printfState(int state)
{
	switch(state)
	{
	case DoorClosed:
		printf("Current State is DoorClosed\n");
		break;
	case DoorOpen:
		printf("Current State is DoorOpen\n");
		break;
	case DoorOpening:
		printf("Current State is DoorOpening\n");
		break;
	case DoorClosing:
		printf("Current State is DoorClosing\n");
		break;
	default:
		printf("�����ڵ�״̬!!!\n");
		break;
	}
}

void CGarage_dialogDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}
