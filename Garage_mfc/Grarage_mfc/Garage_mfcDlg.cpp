
// Grarage_mfcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Garage_mfc.h"
#include "Garage_mfcDlg.h"
#include "afxdialogex.h"

#include "Garage.h"

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


// CGarage_mfcDlg 对话框




CGarage_mfcDlg::CGarage_mfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGarage_mfcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGarage_mfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGarage_mfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGarage_mfcDlg::OnBnClickedOk)
	ON_BN_CLICKED(BTN_Start, &CGarage_mfcDlg::OnBnClickedStart)
	ON_BN_CLICKED(BTN_STOP, &CGarage_mfcDlg::OnBnClickedStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(BTN_Switch, &CGarage_mfcDlg::OnBnClickedSwitch)
END_MESSAGE_MAP()


// CGrarage_mfcDlg 消息处理程序

BOOL CGarage_mfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 使程序弹出console，可以使用printf("ok!!");
	AllocConsole();
	freopen("CONOUT$","a+",stdout);

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGarage_mfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGarage_mfcDlg::OnPaint()
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
HCURSOR CGarage_mfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/** 关闭 */
void CGarage_mfcDlg::OnBnClickedOk()
{
	// 结束仿真
	OnBnClickedStop();

	CDialogEx::OnOK();
}

/** 开始仿真 */
void CGarage_mfcDlg::OnBnClickedStart()
{
	GarageStartup();
	// 启动定时器,100ms间隔
	SetTimer(ID_CLOCK_TIMER,100,NULL);
}

/** 结束仿真 */
void CGarage_mfcDlg::OnBnClickedStop()
{
	// 关闭定时器
	KillTimer(ID_CLOCK_TIMER);
	GarageShutdown();
}


void CGarage_mfcDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int n = 0;
	switch(nIDEvent)
	{
	case ID_CLOCK_TIMER:
		n++;
		printf("定时器到，n=%d\n",n);
		main_control();
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 车库门开关
void CGarage_mfcDlg::OnBnClickedSwitch()
{
	ButtonPressed = !ButtonPressed;
}
