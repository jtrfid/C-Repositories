
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

#define ID_Timer_DOOR 100      // 开关门动画时钟
#define ID_Timer_Closing 101   // 开门状态，特定时间内，无动作，自动关门 

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
	// SWP_NOCOPYBITS：清除客户区的所有内容,SWP_NOMOVE：维持当前位置（忽略X和Y参数）
	// SWP_DRAWFRAME：在窗口周围画一个边框（定义在窗口类描述中）,SWP_NOSIZE 保持当前的大小（忽略cx和cy参数）
	// SWP_NOZORDER: 不改变窗口听Z轴位置,忽略第一个参数
	m_btnSwitch.SetWindowPos(&wndTop,0,0,bmSize.bmWidth,bmSize.bmHeight,SWP_NOMOVE);
	
	// 有车的车库图片(m_PicGarage)在下面，车库门(m_PicDoor)在上面
	RECT rectDoor,rectGarage,rectSwitch;
	// 返回指定窗口的边框矩形的尺寸。
	// 1 窗口还没有初始化完成时：原点是整个窗口的左上角【OnInitDialog( )事件中】
	// 2 初始化完成后，原点是设备屏幕左上角【OnShowWindow( )事件中】
	m_PicDoor.GetWindowRect(&rectDoor); 
	m_PicGarage.GetWindowRect(&rectGarage);
	m_PicGarage.SetWindowPos(&wndTop,0,0,rectGarage.right-rectGarage.left,
		rectGarage.bottom-rectGarage.top,SWP_SHOWWINDOW |  SWP_NOCOPYBITS);
    m_PicGarage.GetWindowRect(&rectGarage); // 已经移动，重新获取
	m_PicDoor.SetWindowPos(&wndBottom,0,0,rectDoor.right-rectDoor.left,
		rectGarage.bottom-rectGarage.top,SWP_SHOWWINDOW |  SWP_NOCOPYBITS);

	// 重新调整车库门的显示位置
	WINDOWPLACEMENT placement;
	m_PicDoor.GetWindowPlacement(&placement); // 已经移动，重新获取
	rectDoor = placement.rcNormalPosition;
	m_btnSwitch.GetWindowPlacement(&placement);
	rectSwitch = placement.rcNormalPosition;
	m_btnSwitch.SetWindowPos(0,rectDoor.right + 20 ,rectSwitch.top,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// 加载Cat按钮位图，并使大小与位图一致
	CBitmap catmap;     // 加载位图对象
	HBITMAP hcatmap;    // 保存位图对象的句柄
	catmap.LoadBitmap(IDB_bmpCat);
	hcatmap = (HBITMAP)catmap.GetSafeHandle();
	m_BtnCat.SetBitmap(hcatmap); 
	BITMAP catbmSize;
	catmap.GetBitmap(&catbmSize);
	m_BtnCat.SetWindowPos(&wndTop,0,0,catbmSize.bmWidth,catbmSize.bmHeight,SWP_NOMOVE);

	// Cat按钮位置
	RECT catRectOutDoor;
	m_BtnCat.GetWindowPlacement(&placement); // 窗口显示前后均以父窗口客户区的左上角为原点
	catRectOutDoor = placement.rcNormalPosition;
	catOutDoor_top = catRectOutDoor.top;
	catOutDoor_left = catRectOutDoor.left; 

	catInDoor_top = catOutDoor_top;
    catInDoor_left = rectDoor.right/2; // 大约在门的中间
	
	// 初始值
	doorHeight = rectDoor.bottom-rectDoor.top;
	currentDoorPosition = 0;
	step = 10;
	interval = 100;
	OpenInterval = 10000;
	StartTimerClosing = false;
	State = DoorClosed;

	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	m_BTN_OK.SetWindowText(L"点击开始");
	m_BTN_OK.EnableWindow(false);

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
	Lib_Running = !Lib_Running;
	if(Lib_Running) m_BTN_OK.SetWindowText(L"运行...");
	else m_BTN_OK.SetWindowText(L"暂停...");
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
			main_control(&State);
			DoorUpDown(State);
			break;
		case ID_Timer_Closing: // 开门状态，特定时间内，无动作，自动关门
			SetMotorPower(-1);
			State = DoorClosing;
			printf("开门状态，10秒后，无动作，自动关门...\n");
			break;
		defaule:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 开关门动画,
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
		  if(!StartTimerClosing) {  // 启动自动关门定时器
			  // 开门状态，特定时间内，无动作，自动关门 
			  SetTimer(ID_Timer_Closing,OpenInterval,NULL);
			  StartTimerClosing = true;
		  }
		  return;
	  case DoorOpening:
	  case DoorClosing:
		  currentDoorPosition += Lib_Power*step;
		  if(StartTimerClosing) { // 关闭自动关门定时器
			  KillTimer(ID_Timer_Closing);
			  StartTimerClosing = false;
		  }
		  break;
	  default:
		  printf("不存在的状态!!!\n");
		  break;
	}
	// printf("currentDoorPosition=%d\n",currentDoorPosition);
	// 更新相对位置
	Lib_CurrentDoorPosition = (Lib_DoorHeight/doorHeight)*currentDoorPosition;
	// SWP_NOZORDER：忽略第一个参数；SWP_NOSIZE：忽略第3、4个参数
	m_PicDoor.SetWindowPos(0,0,-currentDoorPosition,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);
}

// 开始仿真
void CGarage_dialogDlg::OnBnClickedBtnStart()
{	
	// 开始开关门动画
	SetTimer(ID_Timer_DOOR,interval,NULL);
	m_BtnStart.EnableWindow(false);
	m_BtnEnd.EnableWindow(true);

	GarageStartup();
	m_BTN_OK.SetWindowText(L"运行...");
	m_BTN_OK.EnableWindow(true);
}

// 结束仿真
void CGarage_dialogDlg::OnBnClickedBtnEnd()
{
	KillTimer(ID_Timer_Closing);
	KillTimer(ID_Timer_DOOR);
	m_BtnStart.EnableWindow(true);
	m_BtnEnd.EnableWindow(false);

	GarageShutdown();
	m_BTN_OK.SetWindowText(L"结束...");
	m_BTN_OK.EnableWindow(false);
}

// 关闭
void CGarage_dialogDlg::OnBnClickedCancel()
{
	KillTimer(ID_Timer_Closing);
	KillTimer(ID_Timer_DOOR);
	GarageShutdown();

	CDialogEx::OnCancel();
}

// Cat按钮
void CGarage_dialogDlg::OnBnClickedBtnCat()
{
	RECT catRect;
	WINDOWPLACEMENT placement;
	m_BtnCat.GetWindowPlacement(&placement);
	catRect = placement.rcNormalPosition;
	if(catRect.left < catOutDoor_left) // 门内-->门外
	{
		Lib_BeamBroken = false;  // 门下部的红外线【未】探测到Cat
		m_BtnCat.SetWindowPos(0,catOutDoor_left,catOutDoor_top,0,0,SWP_NOSIZE | SWP_NOZORDER);
	}
	else // 门外-->门内
	{
		Lib_BeamBroken = true;  // 门下部的红外线探测到Cat
		m_BtnCat.SetWindowPos(0,catInDoor_left,catInDoor_top,0,0,SWP_NOSIZE | SWP_NOZORDER);
	}
}

// 打印当前状态
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
		printf("不存在的状态!!!\n");
		break;
	}
}

void CGarage_dialogDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}
