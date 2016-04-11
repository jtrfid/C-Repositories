
// Elevator_dialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Elevator_dialog.h"
#include "Elevator_dialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CElevator_dialogDlg 对话框




CElevator_dialogDlg::CElevator_dialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CElevator_dialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CElevator_dialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PicElevatorBackground, m_PicElevatorBackground);
	DDX_Control(pDX, IDC_PicCar, m_PicCar);
}

BEGIN_MESSAGE_MAP(CElevator_dialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CElevator_dialogDlg 消息处理程序

BOOL CElevator_dialogDlg::OnInitDialog()
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

	// 放置电梯箱体和背景
	RECT back,car;
	m_PicElevatorBackground.GetClientRect(&back); 
	printf("elevatorBackground=%d,%d,%d,%d\n",back.top,back.left,back.bottom,back.right); // 0,0,664,429
	m_PicCar.GetClientRect(&car); 
	printf("Car=%d,%d,%d,%d\n",car.top,car.left,car.bottom,car.right); // Car=0,0,166,83
	m_PicElevatorBackground.SetWindowPos(0,0,0,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// m_Car_x = (back.right-car.right)/2;
	m_Car_x = 178 - car.right/2;
	m_Car_y = (back.bottom-car.bottom);
	m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// 启动仿真时钟
	m_Interval = 100;
	SetTimer(ID_ClOCK_TIMER,m_Interval,NULL);

	m_step = 10;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CElevator_dialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CElevator_dialogDlg::OnPaint()
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
HCURSOR CElevator_dialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnTimer(nIDEvent);
//}


void CElevator_dialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	static int y = m_Car_y;
	
	if(y<0) y = 0;
	else y =  y - m_step;
	//m_PicCar.SetWindowPos(0,m_Car_x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	printf("y=%d\n",y);

	CDialogEx::OnTimer(nIDEvent);
}
