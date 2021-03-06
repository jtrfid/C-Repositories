
// Elevator_dialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Elevator_dialog.h"
#include "Elevator_dialogDlg.h"
#include "afxdialogex.h"
#include <math.h>

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

HWND CElevator_dialogDlg::MAIN_WIN = NULL; // 静态变量这样初始化

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
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_TxtStatus, m_TxtStatus);
	DDX_Control(pDX, IDC_DoorLeft, m_PicDoorLeft);
	DDX_Control(pDX, IDC_DoorRight, m_PicDoorRight);
}

BEGIN_MESSAGE_MAP(CElevator_dialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CElevator_dialogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CElevator_dialogDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BtnUp1, &CElevator_dialogDlg::OnBnClickedBtnup1)
	ON_BN_CLICKED(IDC_BtnUp2, &CElevator_dialogDlg::OnBnClickedBtnup1)
	ON_BN_CLICKED(IDC_BtnUp3, &CElevator_dialogDlg::OnBnClickedBtnup1)
	ON_BN_CLICKED(IDC_BtnDown1, &CElevator_dialogDlg::OnBnClickedBtnup1)
	ON_BN_CLICKED(IDC_BtnDown2, &CElevator_dialogDlg::OnBnClickedBtnup1)
	ON_BN_CLICKED(IDC_BtnDown3, &CElevator_dialogDlg::OnBnClickedBtnup1)
	ON_BN_DOUBLECLICKED(IDC_BtnUp1, &CElevator_dialogDlg::OnDoubleclickedBtnup1)
	ON_BN_DOUBLECLICKED(IDC_BtnUp2, &CElevator_dialogDlg::OnDoubleclickedBtnup1)
	ON_BN_DOUBLECLICKED(IDC_BtnUp3, &CElevator_dialogDlg::OnDoubleclickedBtnup1)
	ON_BN_DOUBLECLICKED(IDC_BtnDown1, &CElevator_dialogDlg::OnDoubleclickedBtnup1)
	ON_BN_DOUBLECLICKED(IDC_BtnDown2, &CElevator_dialogDlg::OnDoubleclickedBtnup1)
	ON_BN_DOUBLECLICKED(IDC_BtnDown3, &CElevator_dialogDlg::OnDoubleclickedBtnup1)
	ON_BN_CLICKED(IDC_BtnNum1, &CElevator_dialogDlg::OnClickedBtnnum1)
	ON_BN_CLICKED(IDC_BtnNum2, &CElevator_dialogDlg::OnClickedBtnnum1)
	ON_BN_CLICKED(IDC_BtnNum3, &CElevator_dialogDlg::OnClickedBtnnum1)
	ON_BN_DOUBLECLICKED(IDC_BtnNum1, &CElevator_dialogDlg::OnDoubleclickedBtnnum1)
	ON_BN_DOUBLECLICKED(IDC_BtnNum2, &CElevator_dialogDlg::OnDoubleclickedBtnnum1)
	ON_BN_DOUBLECLICKED(IDC_BtnNum3, &CElevator_dialogDlg::OnDoubleclickedBtnnum1)
	ON_MESSAGE(WM_LIGHT_MESSAGE,&CElevator_dialogDlg::OnLightMessage)
    ON_MESSAGE(WM_Status_MESSAGE,&CElevator_dialogDlg::OnViewStatusMessage)
	ON_MESSAGE(WM_Door_MESSAGE,&CElevator_dialogDlg::OnOpenCloseDoorMessage)
	ON_BN_CLICKED(IDC_BtnOpen, &CElevator_dialogDlg::OnBnClickedBtnOpenCloseDoor)
	ON_BN_CLICKED(IDC_BtnClose, &CElevator_dialogDlg::OnBnClickedBtnOpenCloseDoor)
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
	//printf("elevatorBackground=%d,%d,%d,%d\n",back.top,back.left,back.bottom,back.right); // 0,0,664,429
	m_PicCar.GetClientRect(&car); 
	//printf("Car=%d,%d,%d,%d\n",car.top,car.left,car.bottom,car.right); // Car=0,0,166,83
	m_PicElevatorBackground.SetWindowPos(0,0,0,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_Car_x = 178 - car.right/2;
	m_Car_y = (back.bottom-car.bottom);
	m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// SWP_NOCOPYBITS：清除客户区的所有内容。如果未设置该标志，客户区的有效内容被保存并且在窗口尺寸更新和重定位后拷贝回客户区。
	// 设置此项使电梯箱体门可以随着电梯箱体一起随动。
	// m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);

	// 电梯箱体左右门
	m_PicDoorLeft.GetClientRect(&m_DoorRect); // 0,0,40,166
	m_DoorLeft_x = m_Car_x + 1;
	m_DoorLeft_y = m_Car_y;
	//m_DoorRight_x = m_DoorLeft_x + 1 + m_DoorRect.right;  // 左右门中间留1个像素的空隙
	m_DoorRight_x = m_DoorLeft_x + m_DoorRect.right;  // 左右门紧贴，没有留空隙
	m_DoorRight_y = m_DoorLeft_y;
	m_PicDoorLeft.SetWindowPos(0,m_DoorLeft_x,m_DoorLeft_y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_PicDoorRight.SetWindowPos(0,m_DoorRight_x,m_DoorRight_y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// 放置ok，cancel
	RECT rectOk;
	int x,y;
	m_BtnOK.GetClientRect(&rectOk);
	x = back.right+10;
	y = back.bottom-10-rectOk.bottom;
	m_BtnCancel.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y = y - 10 - rectOk.bottom;
	m_BtnOK.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// 状态文本
	m_TxtStatus.SetWindowPos(0,x,5,rectOk.right,rectOk.bottom*2,SWP_SHOWWINDOW | SWP_NOZORDER);

	// 放置主窗口
	int cx,cy; // 包含窗口边框的窗口大小
	cx = 5 + back.right + 10 + rectOk.right + 10 + 5; // 左右边框各5个像素
	cy = 40 + back.bottom + 10 + 5; // 标题40，下边框 5
	this->SetWindowPos(0,100,10,cx,cy,SWP_SHOWWINDOW | SWP_NOZORDER);

	// 1、2、3楼向上/下按钮
	m_UpLight[0].SubclassDlgItem(IDC_BtnUp1,this);
	m_DownLight[0].SubclassDlgItem(IDC_BtnDown1,this);
	m_UpLight[1].SubclassDlgItem(IDC_BtnUp2,this);
	m_DownLight[1].SubclassDlgItem(IDC_BtnDown2,this);
	m_UpLight[2].SubclassDlgItem(IDC_BtnUp3,this);
	m_DownLight[2].SubclassDlgItem(IDC_BtnDown3,this);
	for(int i = 0; i < Lib_FloorNum; i++) {
		m_UpLight[i].LoadBitmaps(IDB_Up1,IDB_Up2,IDB_Up3);
		m_UpLight[i].SizeToContent();
		m_DownLight[i].LoadBitmaps(IDB_Down1,IDB_Down2,IDB_Down3);
		m_DownLight[i].SizeToContent();
	}
	// 1楼向下按钮失效，3楼向上按钮失效
	m_DownLight[0].EnableWindow(FALSE);
	m_UpLight[2].EnableWindow(FALSE);

	RECT rectLight; // 灯的大小
	m_UpLight[0].GetClientRect(&rectLight);
	// 3楼
	x = back.right - rectLight.right/2 - 50;
	// y = (back.bottom/3 - (rectLight.bottom + 2 + rectLight.bottom))/2;
	y = 20;
    m_UpLight[2].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
    m_DownLight[2].SetWindowPos(0,x,y+rectLight.bottom+2,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// 2楼
	y += back.bottom/3;
	m_UpLight[1].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_DownLight[1].SetWindowPos(0,x,y+rectLight.bottom+2,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// 1楼
	y += back.bottom/3;
	m_UpLight[0].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_DownLight[0].SetWindowPos(0,x,y+rectLight.bottom+2,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// 电梯内,1,2,3数字按钮
	m_FloorNum[0].SubclassDlgItem(IDC_BtnNum1,this);
	m_FloorNum[0].LoadBitmaps(IDB_13,IDB_11,IDB_12);
	m_FloorNum[0].SizeToContent();
	m_FloorNum[1].SubclassDlgItem(IDC_BtnNum2,this);
	m_FloorNum[1].LoadBitmaps(IDB_23,IDB_21,IDB_22);
	m_FloorNum[1].SizeToContent();
	m_FloorNum[2].SubclassDlgItem(IDC_BtnNum3,this);
	m_FloorNum[2].LoadBitmaps(IDB_33,IDB_31,IDB_32);
	m_FloorNum[2].SizeToContent();

	RECT rectNum;
	m_FloorNum[0].GetClientRect(&rectNum);
	x = 40; // 数字1的基： 40
	y = back.bottom - 200; // 数字1的基：back.bottom - 200
	m_FloorNum[0].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y -= rectNum.bottom - 2;
	m_FloorNum[1].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y -= rectNum.bottom - 2;
	m_FloorNum[2].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// 电梯内，开关门按钮
	m_Open.SubclassDlgItem(IDC_BtnOpen,this);
	m_Open.LoadBitmaps(IDB_Open2,IDB_Open1,IDB_Open3);
	m_Open.SizeToContent();
	m_Close.SubclassDlgItem(IDC_BtnClose,this);
	m_Close.LoadBitmaps(IDB_Close2,IDB_Close1,IDB_Close3);
	m_Close.SizeToContent();
	
	RECT rectOpen;
	m_Open.GetClientRect(&rectOpen);
	x = 40 + rectNum.right/2 - (rectOpen.right + 10);  // 数字1的基： 40
	y  = back.bottom - 200 + rectNum.bottom + 2; // 数字1的基：back.bottom - 200
	m_Open.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	x = 40 + rectNum.right/2 + 10;
	m_Close.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER); 

	// 对话框窗体位置,合并在前面的语句
	// this->SetWindowPos(0,100,10,0,0,SWP_NOSIZE | SWP_NOZORDER);

	
	// 启动仿真时钟
	m_Interval = 100;
	SetTimer(ID_ClOCK_TIMER,m_Interval,NULL);

	// 初始值
	m_step = 10;
	m_state = Idle;
	m_CurrentCarPosition = 0;
	m_MaxCarPosition = back.bottom - car.bottom;
	// 开门后有一个延时，模拟给乘客预留上下电梯时间(ms,默认2000ms,2s)
	m_DoorInterval = 2000;
	// 电梯箱体门开关门动画步长，2s开关门，(门宽度/2000)*m_Interval + 1, 留1个像素的余量，保证开关门时长内完成其动作
	m_DoorStep = (m_DoorRect.right*m_Interval)/2000 + 1; 
	// 门的宽度，用于动画，开门: m_DoorRect.right-->0，步长：m_DoorStep; 关门反之.
	m_DoorCx = m_DoorRect.right;

	// 本窗口句柄，本类静态变量
	MAIN_WIN = this->GetSafeHwnd();

	// 电梯楼层标志线
	CStatic *line1 = (CStatic *)GetDlgItem(IDC_STATIC_LINE1); // 3层
	int floor_lineX, floor_lineY;
	floor_lineX = back.right - 100;
	floor_lineY = back.bottom - m_MaxCarPosition;  // 第3层
	line1->SetWindowPos(0, floor_lineX, floor_lineY, 100, 1, SWP_SHOWWINDOW | SWP_NOZORDER);
	//line1->SetWindowPos(0, 0, floor_lineY, back.right, 1, SWP_SHOWWINDOW | SWP_NOZORDER);
	CStatic *line2 = (CStatic *)GetDlgItem(IDC_STATIC_LINE2); // 2层
	floor_lineY = back.bottom - m_MaxCarPosition/2;  // 第3层
	line2->SetWindowPos(0, floor_lineX, floor_lineY, 100, 1, SWP_SHOWWINDOW | SWP_NOZORDER);

	// 开始仿真
	printf("Elevator Startup\n");
	ElevatorStartup();
	m_BtnOK.SetWindowText(L"运行...");
	m_BtnCancel.SetWindowText(L"关闭");

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

void CElevator_dialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// 如果仿真，退出
	if(!IsElevatorRunning()) return;

	CString str("");
	switch(nIDEvent) {
	case ID_ClOCK_TIMER: // 动画仿真时钟
		main_control(&m_state); // 电梯状态机
		elevatorState(m_state); // 电梯状态动画仿真
		elevatorDoor(m_state); // 电梯门动画仿真
		break;
	case ID_Door_TIMER: // 开关门计时器, 开门后有一个延时，模拟给乘客预留上下电梯时间
		Lib_DoorTimerStarted = false;
		Lib_DoorOpened = true;
		KillTimer(ID_Door_TIMER);

		ASSERT(m_state == DoorOpen);  // 断言一定是DoorOpen
		str.Format(_T("[%d]楼\n开门结束"),GetNearestFloor());
		printf("[%d]楼开门结束\n",GetNearestFloor());
		break; 
	case ID_AUTO_TIMER: // 一定时间后，自动到一楼
		Lib_AutoTimerStarted = false;
		To1Floor(&m_state); // 自动下降到1楼
		KillTimer(ID_AUTO_TIMER);
		break;
	default:
		ASSERT(FALSE); // 不可能到此
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

// 电梯状态动画仿真
void CElevator_dialogDlg::elevatorState(int state)
{
	//printfState(state);
	switch(state)
	{
	case Idle:
	case DoorOpen:
	case DoorClosing:
		return;
	case MovingUp:   // 上升运动状态
	case MovingDown: // 下降运动状态
		break;
	default:
		printf("没有这种状态!!!\n");  
	}
	
	int step = (int) (Lib_Power*m_step);
	m_CurrentCarPosition += step;
	Lib_CurrentCarPosition = (Lib_MaxCarPosition/m_MaxCarPosition)*m_CurrentCarPosition;

	// 暂时未用
	Lib_CurrentCarVelocity = (Lib_MaxCarPosition/m_MaxCarPosition)*step/(m_Interval*1000);

	//printf("Velocity=%f\n",Lib_CurrentCarVelocity);
	//printf("%f,%d\n",GetFloor(),GetNearestFloor());

	if(step == 0) return;

	// 电梯箱体
	m_Car_y -= step;  
	m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);  // 清除客户区
	//m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// 电梯箱体左右门
	m_DoorLeft_y -= step;
	m_DoorRight_y = m_DoorLeft_y;
	//m_PicDoorLeft.SetWindowPos(0,m_DoorLeft_x,m_DoorLeft_y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);    // 清除客户区
	//m_PicDoorRight.SetWindowPos(0,m_DoorRight_x,m_DoorRight_y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS); // 清除客户区
	m_PicDoorLeft.SetWindowPos(0,m_DoorLeft_x,m_DoorLeft_y,0,0,SWP_NOSIZE | SWP_NOZORDER);  
	m_PicDoorRight.SetWindowPos(0,m_DoorRight_x,m_DoorRight_y,0,0,SWP_NOSIZE | SWP_NOZORDER); 
}

// 电梯箱体门开关门动画仿真
void CElevator_dialogDlg::elevatorDoor(int state)
{
	switch(state)
	{
	case Idle:
	case MovingUp:
	case MovingDown:
		return;
	case DoorOpen:
	case DoorClosing:
		break;
	default:
		printf("没有这种状态!!!\n");  
	}

	// 开门结束,延时开始，开门后有一个延时，模拟给乘客预留上下电梯时间
	if (m_DoorCx < 0) {
		m_DoorCx = 0;
		if (!Lib_DoorTimerStarted) {
			Lib_DoorTimerStarted = true;
			SetTimer(ID_Door_TIMER, m_DoorInterval, NULL); // 2000ms,2s
			printf("开门延时,预留乘客上下电梯时间[%ds]...\n", m_DoorInterval / 1000);
		}
	}
	else if (m_DoorCx > m_DoorRect.right) { // 关门结束
		//printf("m_DoorCx=%d,m_DoorRect.right=%d,m_DoorStep=%d\n", m_DoorCx, m_DoorRect.right, m_DoorStep);
		m_DoorCx = m_DoorRect.right;
		// ASSERT(m_state == DoorClosing);  // 断言一定是DoorClosing, 不一定呀!!!
		if (m_state == DoorClosing) {
			Lib_DoorClosed = true;
			CString str("");
			str.Format(_T("[%d]楼\n关门结束"), GetNearestFloor());
			printf("[%d]楼关门结束\n", GetNearestFloor());
			m_TxtStatus.SetWindowText(str);
	    }
	}

	// 电梯箱体左右门
	m_PicDoorLeft.SetWindowPos(0,0,0,m_DoorCx,m_DoorRect.bottom,SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);  
	int rightX = m_DoorRight_x + (m_DoorRect.right - m_DoorCx);
	m_PicDoorRight.SetWindowPos(0,rightX,m_DoorRight_y,m_DoorCx,m_DoorRect.bottom,SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOCOPYBITS);
	//m_PicDoorLeft.SetWindowPos(0,0,0,m_DoorCx,m_DoorRect.bottom,SWP_NOMOVE | SWP_NOZORDER);  
	//m_PicDoorRight.SetWindowPos(0,0,0,m_DoorCx,m_DoorRect.bottom,SWP_NOMOVE | SWP_NOZORDER);

	if(state == DoorOpen) m_DoorCx -= m_DoorStep; // 开门
	else m_DoorCx += m_DoorStep;  // 关门	

}

// 打印当前状态
void CElevator_dialogDlg::printfState(int state)
{
	switch(state)
	{
	case Idle:
		printf("Current State is Idle\n");
		break;
	case MovingUp:
		printf("Current State is MovingUp\n");
		break;
	case MovingDown:
		printf("Current State is MovingDown\n");
		break;
	case DoorOpen:
		printf("Current State is DoorOpen\n");
		break;
	case DoorClosing:
		printf("Current State is DoorClosing\n");
		break;
	default:
		printf("不存在的状态!!!\n");
		break;
	}
}


void CElevator_dialogDlg::OnBnClickedOk()
{
	/*** 测试
	static bool b = false;
	if (!b) SetMotorPower(0);
	else SetMotorPower(1);
	b = !b;
	****/
	Lib_Running = !Lib_Running;
	if (Lib_Running) {
		m_BtnOK.SetWindowText(L"运行...");  
		printf("系统正在运行...\n");
	}
	else {
		m_BtnOK.SetWindowText(L"暂停...");
		printf("系统暂停...\n");
	}
	//CDialogEx::OnOK();
}


void CElevator_dialogDlg::OnBnClickedCancel()
{
	ElevatorShutdown();
	CDialogEx::OnCancel();
}

// 设置电梯外，1、2、3楼Up/Down按钮（Call Light）的状态，如果当前Light is Off, 置为on;否则保持现状。
void CElevator_dialogDlg::OnBnClickedBtnup1()
{
	// 消息执行顺序：
	// (1) CLightBitmapButton::OnLButtonDown
	// (2) 对话框点击按钮命令，如，CElevator_dialogDlg::OnBnClickedBtnup1()
	for(int floor = 1; floor <= Lib_FloorNum; floor++){
		SetCallLight(floor,true,m_UpLight[floor-1].getLight());
		SetCallLight(floor,false,m_DownLight[floor-1].getLight());
	}
}

// 设置电梯外，1、2、3楼Up/Down按钮（Call Light）的状态, Light Off
void CElevator_dialogDlg::OnDoubleclickedBtnup1()
{
	// 消息执行顺序：
	// (1) CLightBitmapButton::OnLButtonDown
	// (2) 对话框点击按钮命令，如，CElevator_dialogDlg::OnBnClickedBtnup1()
	// (3) CLightBitmapButton::OnLButtonDblClk()
	// (4) 对话框双击按钮命令BN_DOUBLECLICKED，如，CElevator_dialogDlg::OnDoubleclickedBtnup1()
	for(int floor = 1; floor <= Lib_FloorNum; floor++){
		SetCallLight(floor,true,m_UpLight[floor-1].getLight());
		SetCallLight(floor,false,m_DownLight[floor-1].getLight());
	}
}

// 设置电梯内,1,2,3数字按钮（PanelFloorLight）的状态，如果当前Light is Off, 置为on;否则保持现状。
void CElevator_dialogDlg::OnClickedBtnnum1()
{
	// 有效性检查，非运动状态，按电梯内本楼层按钮无效
	if(m_state != MovingUp && m_state != MovingDown) {
		int CurrentFloor = GetNearestFloor();
		if (m_FloorNum[CurrentFloor - 1].getLight()) {
			m_FloorNum[CurrentFloor - 1].setLight(false);
			printf("已经在[%d]楼，按此楼层按钮无效。\n", CurrentFloor);
		}
	}
	for(int floor = 1; floor <= Lib_FloorNum; floor++){
		SetPanelFloorLight(floor,m_FloorNum[floor-1].getLight());
	}
}

// 设置电梯内,1,2,3数字按钮（PanelFloorLight）的状态, Light Off
void CElevator_dialogDlg::OnDoubleclickedBtnnum1()
{
	for(int floor = 1; floor <= Lib_FloorNum; floor++){
		SetPanelFloorLight(floor,m_FloorNum[floor-1].getLight());
	}
}

// 设置电梯内开关门按钮状态，如果当前Light is Off, 置为on;否则保持现状。
void CElevator_dialogDlg::OnBnClickedBtnOpenCloseDoor()
{
	/**** 移至状态函数MovingUp/MovingDown
	// 安全设置，运动状态，开关门失效
	if(m_state == MovingUp || m_state == MovingDown) {
		printf("运动状态，开关门失效!!!\n");
		// 恢复原值
		m_Open.setLight(GetOpenDoorLight());
		m_Close.setLight(GetCloseDoorLight());
		return;
	}
	*****/
	// 设置电梯内开关门按钮状态
	SetOpenDoorLight(m_Open.getLight());
	SetCloseDoorLight(m_Close.getLight());
}

// 接收消息，更新电梯内外按钮灯状态
LRESULT CElevator_dialogDlg::OnLightMessage(WPARAM wParam,LPARAM lParam)
{
	Light_Msg *msg = (Light_Msg*)lParam;
	// printf("msg:%d,%d,%d,%d\n",msg->type,msg->floor,msg->up,msg->LightOn);

	if(msg->type == 1) // 电梯内楼层数字按钮灯（Panel Floor Light）
	{
		m_FloorNum[msg->floor-1].setLight(msg->LightOn);
	}
	else if(msg->type == 2) // 电梯内开关门按钮灯
	{
		m_Open.setLight(msg->LightOn);
		m_Close.setLight(msg->LightOn);
	}
	else if(msg->type == 3) // 表示电梯外Up/Down按钮灯（Call Light）
	{
		if(msg->up)
			m_UpLight[msg->floor-1].setLight(msg->LightOn);
		else
			m_DownLight[msg->floor-1].setLight(msg->LightOn);
	}
	else
		ASSERT(FALSE); // 不可能到此

	delete msg;
	return 0;
}


// 接收消息，更新状态文本
LRESULT CElevator_dialogDlg::OnViewStatusMessage(WPARAM wParam,LPARAM lParam)
{
	CString *msg = (CString *)lParam;
	m_TxtStatus.SetWindowText(*msg);

	delete msg;
	return 0;
}

// 接收消息开关门，开门后有一个延时，模拟给乘客预留上下电梯时间
LRESULT CElevator_dialogDlg::OnOpenCloseDoorMessage(WPARAM wParam,LPARAM lParam)
{
	int floor = (int)wParam;  // 楼层
	bool open = (bool)lParam; // 开关门
	CString str;

	// ASSERT(!Lib_DoorTimerStarted); // 断言一定是Lib_DoorTimerStarted = false; 即断言参数为真，否则中断在此

	if(open) { // 开门
		str.Format(_T("[%d]楼\n开门..."),floor);	
		printf("[%d]楼开门...\n", floor);
		Lib_DoorOpened = false;
	}
	else { // 关门		
		str.Format(_T("[%d]楼\n关门..."),floor);	
		printf("[%d]楼关门...\n", floor);
		Lib_DoorClosed = false;
	}

	m_TxtStatus.SetWindowText(str);
	
	// 如果开启,关闭开门后的延时
	if (Lib_DoorTimerStarted) {
		KillTimer(ID_Door_TIMER);
		Lib_DoorTimerStarted = false;
	}

	return 0;
}
