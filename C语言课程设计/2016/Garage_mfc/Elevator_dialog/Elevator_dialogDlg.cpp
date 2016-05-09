
// Elevator_dialogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Elevator_dialog.h"
#include "Elevator_dialogDlg.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CElevator_dialogDlg �Ի���

HWND CElevator_dialogDlg::MAIN_WIN = NULL; // ��̬����������ʼ��

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


// CElevator_dialogDlg ��Ϣ�������

BOOL CElevator_dialogDlg::OnInitDialog()
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

	// ���õ�������ͱ���
	RECT back,car;
	m_PicElevatorBackground.GetClientRect(&back); 
	//printf("elevatorBackground=%d,%d,%d,%d\n",back.top,back.left,back.bottom,back.right); // 0,0,664,429
	m_PicCar.GetClientRect(&car); 
	//printf("Car=%d,%d,%d,%d\n",car.top,car.left,car.bottom,car.right); // Car=0,0,166,83
	m_PicElevatorBackground.SetWindowPos(0,0,0,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_Car_x = 178 - car.right/2;
	m_Car_y = (back.bottom-car.bottom);
	m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// SWP_NOCOPYBITS������ͻ������������ݡ����δ���øñ�־���ͻ�������Ч���ݱ����沢���ڴ��ڳߴ���º��ض�λ�󿽱��ؿͻ�����
	// ���ô���ʹ���������ſ������ŵ�������һ���涯��
	// m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);

	// ��������������
	m_PicDoorLeft.GetClientRect(&m_DoorRect); // 0,0,40,166
	m_DoorLeft_x = m_Car_x + 1;
	m_DoorLeft_y = m_Car_y;
	//m_DoorRight_x = m_DoorLeft_x + 1 + m_DoorRect.right;  // �������м���1�����صĿ�϶
	m_DoorRight_x = m_DoorLeft_x + m_DoorRect.right;  // �����Ž�����û������϶
	m_DoorRight_y = m_DoorLeft_y;
	m_PicDoorLeft.SetWindowPos(0,m_DoorLeft_x,m_DoorLeft_y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_PicDoorRight.SetWindowPos(0,m_DoorRight_x,m_DoorRight_y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// ����ok��cancel
	RECT rectOk;
	int x,y;
	m_BtnOK.GetClientRect(&rectOk);
	x = back.right+10;
	y = back.bottom-10-rectOk.bottom;
	m_BtnCancel.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y = y - 10 - rectOk.bottom;
	m_BtnOK.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// ״̬�ı�
	m_TxtStatus.SetWindowPos(0,x,5,rectOk.right,rectOk.bottom*2,SWP_SHOWWINDOW | SWP_NOZORDER);

	// ����������
	int cx,cy; // �������ڱ߿�Ĵ��ڴ�С
	cx = 5 + back.right + 10 + rectOk.right + 10 + 5; // ���ұ߿��5������
	cy = 40 + back.bottom + 10 + 5; // ����40���±߿� 5
	this->SetWindowPos(0,100,10,cx,cy,SWP_SHOWWINDOW | SWP_NOZORDER);

	// 1��2��3¥����/�°�ť
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
	// 1¥���°�ťʧЧ��3¥���ϰ�ťʧЧ
	m_DownLight[0].EnableWindow(FALSE);
	m_UpLight[2].EnableWindow(FALSE);

	RECT rectLight; // �ƵĴ�С
	m_UpLight[0].GetClientRect(&rectLight);
	// 3¥
	x = back.right - rectLight.right/2 - 50;
	y = (back.bottom/3 - (rectLight.bottom + 2 + rectLight.bottom))/2;
    m_UpLight[2].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
    m_DownLight[2].SetWindowPos(0,x,y+rectLight.bottom+2,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// 2¥
	y += back.bottom/3;
	m_UpLight[1].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_DownLight[1].SetWindowPos(0,x,y+rectLight.bottom+2,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// 1¥
	y += back.bottom/3;
	m_UpLight[0].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	m_DownLight[0].SetWindowPos(0,x,y+rectLight.bottom+2,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// ������,1,2,3���ְ�ť
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
	x = 40; // ����1�Ļ��� 40
	y = back.bottom - 200; // ����1�Ļ���back.bottom - 200
	m_FloorNum[0].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y -= rectNum.bottom - 2;
	m_FloorNum[1].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y -= rectNum.bottom - 2;
	m_FloorNum[2].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// �����ڣ������Ű�ť
	m_Open.SubclassDlgItem(IDC_BtnOpen,this);
	m_Open.LoadBitmaps(IDB_Open2,IDB_Open1,IDB_Open3);
	m_Open.SizeToContent();
	m_Close.SubclassDlgItem(IDC_BtnClose,this);
	m_Close.LoadBitmaps(IDB_Close2,IDB_Close1,IDB_Close3);
	m_Close.SizeToContent();
	
	RECT rectOpen;
	m_Open.GetClientRect(&rectOpen);
	x = 40 + rectNum.right/2 - (rectOpen.right + 10);  // ����1�Ļ��� 40
	y  = back.bottom - 200 + rectNum.bottom + 2; // ����1�Ļ���back.bottom - 200
	m_Open.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	x = 40 + rectNum.right/2 + 10;
	m_Close.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER); 

	// �Ի�����λ��,�ϲ���ǰ������
	// this->SetWindowPos(0,100,10,0,0,SWP_NOSIZE | SWP_NOZORDER);
	
	// ��������ʱ��
	m_Interval = 100;
	SetTimer(ID_ClOCK_TIMER,m_Interval,NULL);

	// ��ʼֵ
	m_step = 10;
	m_state = Idle;
	m_CurrentCarPosition = 0;
	m_MaxCarPosition = back.bottom - car.bottom;
	// ���ź���һ����ʱ��ģ����˿�Ԥ�����µ���ʱ��(ms,Ĭ��2000ms,2s)
	m_DoorInterval = 2000;
	// ���������ſ����Ŷ���������2s�����ţ�(�ſ��/2000)*m_Interval + 1, ��1�����ص���������֤������ʱ��������䶯��
	m_DoorStep = (m_DoorRect.right*m_Interval)/2000 + 1; 
	// �ŵĿ�ȣ����ڶ���������: m_DoorRect.right-->0��������m_DoorStep; ���ŷ�֮.
	m_DoorCx = m_DoorRect.right;

	// �����ھ�������ྲ̬����
	MAIN_WIN = this->GetSafeHwnd();

	
	// ��ʼ����
	printf("Elevator Startup\n");
	ElevatorStartup();
	m_BtnOK.SetWindowText(L"����...");
	m_BtnCancel.SetWindowText(L"�ر�");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CElevator_dialogDlg::OnPaint()
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
HCURSOR CElevator_dialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CElevator_dialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// ������棬�˳�
	if(!IsElevatorRunning()) return;

	CString str("");
	switch(nIDEvent) {
	case ID_ClOCK_TIMER: // ��������ʱ��
		main_control(&m_state); // ����״̬��
		elevatorState(m_state); // ����״̬��������
		elevatorDoor(m_state); // �����Ŷ�������
		break;
	case ID_Door_TIMER: // �����ż�ʱ��, ���ź���һ����ʱ��ģ����˿�Ԥ�����µ���ʱ��
		Lib_DoorTimerStarted = false;
		Lib_DoorOpened = true;
		KillTimer(ID_Door_TIMER);

		ASSERT(m_state == DoorOpen);  // ����һ����DoorOpen
		str.Format(_T("[%d]¥\n���Ž���"),GetNearestFloor());
		printf("[%d]¥���Ž���\n",GetNearestFloor());
		break; 
	case ID_AUTO_TIMER: // һ��ʱ����Զ���һ¥
		Lib_AutoTimerStarted = false;
		To1Floor(&m_state); // �Զ��½���1¥
		KillTimer(ID_AUTO_TIMER);
		break;
	default:
		ASSERT(FALSE); // �����ܵ���
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

// ����״̬��������
void CElevator_dialogDlg::elevatorState(int state)
{
	//printfState(state);
	switch(state)
	{
	case Idle:
	case DoorOpen:
	case DoorClosing:
		return;
	case MovingUp:   // �����˶�״̬
	case MovingDown: // �½��˶�״̬
		break;
	default:
		printf("û������״̬!!!\n");  
	}
	
	int step = (int) (Lib_Power*m_step);
	m_CurrentCarPosition += step;
	Lib_CurrentCarPosition = (Lib_MaxCarPosition/m_MaxCarPosition)*m_CurrentCarPosition;
	Lib_CurrentCarVelocity = (Lib_MaxCarPosition/m_MaxCarPosition)*step/(m_Interval*1000);

	//printf("Velocity=%f\n",Lib_CurrentCarVelocity);
	//printf("%f,%d\n",GetFloor(),GetNearestFloor());

	if(step == 0) return;

	// ��������
	m_Car_y -= step;  
	m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);  // ����ͻ���
	//m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	// ��������������
	m_DoorLeft_y -= step;
	m_DoorRight_y = m_DoorLeft_y;
	//m_PicDoorLeft.SetWindowPos(0,m_DoorLeft_x,m_DoorLeft_y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS);    // ����ͻ���
	//m_PicDoorRight.SetWindowPos(0,m_DoorRight_x,m_DoorRight_y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOCOPYBITS); // ����ͻ���
	m_PicDoorLeft.SetWindowPos(0,m_DoorLeft_x,m_DoorLeft_y,0,0,SWP_NOSIZE | SWP_NOZORDER);  
	m_PicDoorRight.SetWindowPos(0,m_DoorRight_x,m_DoorRight_y,0,0,SWP_NOSIZE | SWP_NOZORDER); 
}

// ���������ſ����Ŷ�������
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
		printf("û������״̬!!!\n");  
	}

	// ���Ž���,��ʱ��ʼ�����ź���һ����ʱ��ģ����˿�Ԥ�����µ���ʱ��
	if (m_DoorCx < 0) {
		m_DoorCx = 0;
		if (!Lib_DoorTimerStarted) {
			Lib_DoorTimerStarted = true;
			SetTimer(ID_Door_TIMER, m_DoorInterval, NULL); // 2000ms,2s
			printf("������ʱ,Ԥ���˿����µ���ʱ��[%ds]...\n", m_DoorInterval / 1000);
		}
	}
	else if (m_DoorCx > m_DoorRect.right) { // ���Ž���
		//printf("m_DoorCx=%d,m_DoorRect.right=%d,m_DoorStep=%d\n", m_DoorCx, m_DoorRect.right, m_DoorStep);
		m_DoorCx = m_DoorRect.right;
		// ASSERT(m_state == DoorClosing);  // ����һ����DoorClosing, ��һ��ѽ!!!
		if (m_state == DoorClosing) {
			Lib_DoorClosed = true;
			CString str("");
			str.Format(_T("[%d]¥\n���Ž���"), GetNearestFloor());
			printf("[%d]¥���Ž���\n", GetNearestFloor());
			m_TxtStatus.SetWindowText(str);
	    }
	}

	// ��������������
	m_PicDoorLeft.SetWindowPos(0,0,0,m_DoorCx,m_DoorRect.bottom,SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);  
	int rightX = m_DoorRight_x + (m_DoorRect.right - m_DoorCx);
	m_PicDoorRight.SetWindowPos(0,rightX,m_DoorRight_y,m_DoorCx,m_DoorRect.bottom,SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOCOPYBITS);
	//m_PicDoorLeft.SetWindowPos(0,0,0,m_DoorCx,m_DoorRect.bottom,SWP_NOMOVE | SWP_NOZORDER);  
	//m_PicDoorRight.SetWindowPos(0,0,0,m_DoorCx,m_DoorRect.bottom,SWP_NOMOVE | SWP_NOZORDER);

	if(state == DoorOpen) m_DoorCx -= m_DoorStep; // ����
	else m_DoorCx += m_DoorStep;  // ����	

}

// ��ӡ��ǰ״̬
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
		printf("�����ڵ�״̬!!!\n");
		break;
	}
}


void CElevator_dialogDlg::OnBnClickedOk()
{
	/*** ����
	static bool b = false;
	if (!b) SetMotorPower(0);
	else SetMotorPower(1);
	b = !b;
	****/
	Lib_Running = !Lib_Running;
	if (Lib_Running) {
		m_BtnOK.SetWindowText(L"����...");  
		printf("ϵͳ��������...\n");
	}
	else {
		m_BtnOK.SetWindowText(L"��ͣ...");
		printf("ϵͳ��ͣ...\n");
	}
	//CDialogEx::OnOK();
}


void CElevator_dialogDlg::OnBnClickedCancel()
{
	ElevatorShutdown();
	CDialogEx::OnCancel();
}

// ���õ����⣬1��2��3¥Up/Down��ť��Call Light����״̬�������ǰLight is Off, ��Ϊon;���򱣳���״��
void CElevator_dialogDlg::OnBnClickedBtnup1()
{
	// ��Ϣִ��˳��
	// (1) CLightBitmapButton::OnLButtonDown
	// (2) �Ի�������ť����磬CElevator_dialogDlg::OnBnClickedBtnup1()
	for(int floor = 1; floor <= Lib_FloorNum; floor++){
		SetCallLight(floor,true,m_UpLight[floor-1].getLight());
		SetCallLight(floor,false,m_DownLight[floor-1].getLight());
	}
}

// ���õ����⣬1��2��3¥Up/Down��ť��Call Light����״̬, Light Off
void CElevator_dialogDlg::OnDoubleclickedBtnup1()
{
	// ��Ϣִ��˳��
	// (1) CLightBitmapButton::OnLButtonDown
	// (2) �Ի�������ť����磬CElevator_dialogDlg::OnBnClickedBtnup1()
	// (3) CLightBitmapButton::OnLButtonDblClk()
	// (4) �Ի���˫����ť����BN_DOUBLECLICKED���磬CElevator_dialogDlg::OnDoubleclickedBtnup1()
	for(int floor = 1; floor <= Lib_FloorNum; floor++){
		SetCallLight(floor,true,m_UpLight[floor-1].getLight());
		SetCallLight(floor,false,m_DownLight[floor-1].getLight());
	}
}

// ���õ�����,1,2,3���ְ�ť��PanelFloorLight����״̬�������ǰLight is Off, ��Ϊon;���򱣳���״��
void CElevator_dialogDlg::OnClickedBtnnum1()
{
	// ��Ч�Լ�飬���˶�״̬���������ڱ�¥�㰴ť��Ч
	if(m_state != MovingUp && m_state != MovingDown) {
		int CurrentFloor = GetNearestFloor();
		if (m_FloorNum[CurrentFloor - 1].getLight()) {
			m_FloorNum[CurrentFloor - 1].setLight(false);
			printf("�Ѿ���[%d]¥������¥�㰴ť��Ч��\n", CurrentFloor);
		}
	}
	for(int floor = 1; floor <= Lib_FloorNum; floor++){
		SetPanelFloorLight(floor,m_FloorNum[floor-1].getLight());
	}
}

// ���õ�����,1,2,3���ְ�ť��PanelFloorLight����״̬, Light Off
void CElevator_dialogDlg::OnDoubleclickedBtnnum1()
{
	for(int floor = 1; floor <= Lib_FloorNum; floor++){
		SetPanelFloorLight(floor,m_FloorNum[floor-1].getLight());
	}
}

// ���õ����ڿ����Ű�ť״̬�������ǰLight is Off, ��Ϊon;���򱣳���״��
void CElevator_dialogDlg::OnBnClickedBtnOpenCloseDoor()
{
	/**** ����״̬����MovingUp/MovingDown
	// ��ȫ���ã��˶�״̬��������ʧЧ
	if(m_state == MovingUp || m_state == MovingDown) {
		printf("�˶�״̬��������ʧЧ!!!\n");
		// �ָ�ԭֵ
		m_Open.setLight(GetOpenDoorLight());
		m_Close.setLight(GetCloseDoorLight());
		return;
	}
	*****/
	// ���õ����ڿ����Ű�ť״̬
	SetOpenDoorLight(m_Open.getLight());
	SetCloseDoorLight(m_Close.getLight());
}

// ������Ϣ�����µ������ⰴť��״̬
LRESULT CElevator_dialogDlg::OnLightMessage(WPARAM wParam,LPARAM lParam)
{
	Light_Msg *msg = (Light_Msg*)lParam;
	// printf("msg:%d,%d,%d,%d\n",msg->type,msg->floor,msg->up,msg->LightOn);

	if(msg->type == 1) // ������¥�����ְ�ť�ƣ�Panel Floor Light��
	{
		m_FloorNum[msg->floor-1].setLight(msg->LightOn);
	}
	else if(msg->type == 2) // �����ڿ����Ű�ť��
	{
		m_Open.setLight(msg->LightOn);
		m_Close.setLight(msg->LightOn);
	}
	else if(msg->type == 3) // ��ʾ������Up/Down��ť�ƣ�Call Light��
	{
		if(msg->up)
			m_UpLight[msg->floor-1].setLight(msg->LightOn);
		else
			m_DownLight[msg->floor-1].setLight(msg->LightOn);
	}
	else
		ASSERT(FALSE); // �����ܵ���

	delete msg;
	return 0;
}


// ������Ϣ������״̬�ı�
LRESULT CElevator_dialogDlg::OnViewStatusMessage(WPARAM wParam,LPARAM lParam)
{
	CString *msg = (CString *)lParam;
	m_TxtStatus.SetWindowText(*msg);

	delete msg;
	return 0;
}

// ������Ϣ�����ţ����ź���һ����ʱ��ģ����˿�Ԥ�����µ���ʱ��
LRESULT CElevator_dialogDlg::OnOpenCloseDoorMessage(WPARAM wParam,LPARAM lParam)
{
	int floor = (int)wParam;  // ¥��
	bool open = (bool)lParam; // ������
	CString str;

	// ASSERT(!Lib_DoorTimerStarted); // ����һ����Lib_DoorTimerStarted = false; �����Բ���Ϊ�棬�����ж��ڴ�

	if(open) { // ����
		str.Format(_T("[%d]¥\n����..."),floor);	
		printf("[%d]¥����...\n", floor);
		Lib_DoorOpened = false;
	}
	else { // ����		
		str.Format(_T("[%d]¥\n����..."),floor);	
		printf("[%d]¥����...\n", floor);
		Lib_DoorClosed = false;
	}

	m_TxtStatus.SetWindowText(str);
	
	// �������,�رտ��ź����ʱ
	if (Lib_DoorTimerStarted) {
		KillTimer(ID_Door_TIMER);
		Lib_DoorTimerStarted = false;
	}

	return 0;
}
