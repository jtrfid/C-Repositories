
// Garage_dialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Garage_dialog.h"
#include "Garage_dialogDlg.h"
#include "afxdialogex.h"

#include "garageLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_Timer_DOOR 100    // 开关门动画时钟
#define ID_Timer_CLOCK 101   // 仿真时钟,即采样间隔（周期）

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CGarage_dialogDlg 对话框




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


// CGarage_dialogDlg 消息处理程序

BOOL CGarage_dialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 使程序弹出console，可以使用printf("ok!!");
	AllocConsole();
	freopen("CONOUT$","a+",stdout);

	// 给门开关位图，并将其大小调整到位图大小
	CBitmap bitmap;  // 加载位图对象
	HBITMAP hBmp;    // 保存位图对象的句柄
	bitmap.LoadBitmap(IDB_bmpSwitch);
	hBmp = (HBITMAP)bitmap.GetSafeHandle();
	m_btnSwitch.SetBitmap(hBmp); 
	BITMAP bmSize;
	bitmap.GetBitmap(&bmSize);
	printf("%d,%d",bmSize.bmWidth,bmSize.bmHeight);
	//m_btnSwitch.SetWindowPos(&wndTop,0,0,bmSize.bmWidth,bmSize.bmHeight,SWP_SHOWWINDOW);
	m_btnSwitch.SetWindowPos(&wndTop,0,0,bmSize.bmWidth,bmSize.bmHeight,SWP_NOMOVE);
	
	// 有车的车库图片(m_PicGarage)在下面，车库门(m_PicDoor)在上面
	RECT rectDoor,rectGarage,rectSwitch;
	m_PicDoor.GetWindowRect(&rectDoor);
	m_PicGarage.GetWindowRect(&rectGarage);
	m_PicGarage.SetWindowPos(&wndTop,0,0,rectGarage.right-rectGarage.left,
		rectGarage.bottom-rectGarage.top,SWP_SHOWWINDOW |  SWP_NOCOPYBITS);
    m_PicGarage.GetWindowRect(&rectGarage); // 已经移动，重新获取
	m_PicDoor.SetWindowPos(&wndBottom,0,0,rectDoor.right-rectDoor.left,
		rectGarage.bottom-rectGarage.top,SWP_SHOWWINDOW |  SWP_NOCOPYBITS);

	// 重新调整车库门的显示位置
	m_PicDoor.GetWindowRect(&rectDoor); // 已经移动，重新获取
	m_btnSwitch.GetWindowRect(&rectSwitch);
	m_btnSwitch.SetWindowPos(&wndTop,rectDoor.right,rectSwitch.top,0,0,SWP_NOSIZE);

	// 初始值
	doorHeight = rectDoor.bottom-rectDoor.top;
	currentDoorPosition = 0;
	step = 10;
	interval = 100;
	samplingInterval = 100;
	State = DoorClosed;

	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	// SWP_NOCOPYBITS：清除客户区的所有内容,SWP_NOMOVE：维持当前位置（忽略X和Y参数）
	// SWP_DRAWFRAME：在窗口周围画一个边框（定义在窗口类描述中）,SWP_NOSIZE 保持当前的大小（忽略cx和cy参数）

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGarage_dialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	//// SWP_NOCOPYBITS：清除客户区的所有内容,SWP_NOMOVE：维持当前位置（忽略X和Y参数）
	//// SWP_DRAWFRAME：在窗口周围画一个边框（定义在窗口类描述中）,SWP_NOSIZE 保持当前的大小（忽略cx和cy参数）
	////m_PicDoor.SetWindowPos(&wndTop,0,y,0,0,SWP_NOSIZE |  SWP_NOCOPYBITS);
	//m_PicDoor.SetWindowPos(&wndTop,0,y,0,0,SWP_NOSIZE);
	////m_PicDoor.SetWindowPos(&wndTop,0,y,0,0,SWP_NOSIZE | SWP_DRAWFRAME | SWP_NOCOPYBITS);
	//
	////if (border) m_PicDoor.ModifyStyle(1,WS_BORDER,0);
	//border != border;
	////::SetWindowPos(hwnd,HWND_TOP,0,y,0,0,SWP_NOSIZE);  // 等效m_PicDoor.SetWindowPos(&wndTop,0,y,0,0,SWP_NOSIZE);
	//
	////m_btnSwitch.SetWindowPos(&wndTop,0,0,100,200,SWP_NOMOVE);

	////m_PicDoor.UpdateWindow(); // 无效
	////UpdateWindow();
	
	
	//CDialogEx::OnOK();
}

// 车库门开关被按下
void CGarage_dialogDlg::OnBnClickedbtnSwitch()
{
	printf("车库门开关被按下\n");
	Lib_ButtonPressed = !Lib_ButtonPressed;
}

// 定时器回调函数
void CGarage_dialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent) 
	{
		case ID_Timer_DOOR: // 开关门动画
			DoorUpDown(State);
			break;
		case ID_Timer_CLOCK: //仿真时钟
			main_control(&State);
			break;
		defaule:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 门开关锁动画,
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
		  printf("不存在的状态!!!\n");
		  break;
	}
	printf("currentDoorPosition=%d\n",currentDoorPosition);
	// 更新相对位置
	Lib_CurrentDoorPosition = (Lib_DoorHeight/doorHeight)*currentDoorPosition;
	// SWP_NOZORDER：忽略第一个参数；SWP_NOSIZE：忽略第3、4个参数
	m_PicDoor.SetWindowPos(0,0,-currentDoorPosition,0,0,SWP_NOSIZE | SWP_NOZORDER);
}
// 门开关锁动画,
//void CGarage_dialogDlg::DoorUpDown111()
//{
//	int step=20;
//	if(uping && doorCurrentTop < -doorHeight) {
//		KillTimer(ID_Timer_DOOR); // 开门动画结束，门到顶部
//		doorCurrentTop = -doorHeight;
//		m_PicDoor.SetWindowPos(0,0,doorCurrentTop,0,0,SWP_NOSIZE | SWP_NOZORDER);
//	}
//	if(!uping && doorCurrentTop > 0) {
//		KillTimer(ID_Timer_DOOR); // 关门动画结束，门到底部
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


// 开始仿真
void CGarage_dialogDlg::OnBnClickedBtnStart()
{	
	// 开始仿真
	SetTimer(ID_Timer_CLOCK,samplingInterval,NULL);
	// 开始开关门动画
	SetTimer(ID_Timer_DOOR,interval,NULL);
	m_BtnStart.EnableWindow(false);
	m_BtnEnd.EnableWindow(true);

	GarageStartup();
}

// 结束仿真
void CGarage_dialogDlg::OnBnClickedBtnEnd()
{
	KillTimer(ID_Timer_CLOCK);
	KillTimer(ID_Timer_DOOR);
	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	GarageShutdown();
}

// 关闭
void CGarage_dialogDlg::OnBnClickedCancel()
{
	KillTimer(ID_Timer_CLOCK);
	KillTimer(ID_Timer_DOOR);
	GarageShutdown();

	CDialogEx::OnCancel();
}
