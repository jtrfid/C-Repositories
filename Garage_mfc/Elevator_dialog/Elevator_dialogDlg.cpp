
// Elevator_dialogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Elevator_dialog.h"
#include "Elevator_dialogDlg.h"
#include "afxdialogex.h"

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
	//  DDX_Control(pDX, IDC_BtnUp1, m_BtnUp1);
}

BEGIN_MESSAGE_MAP(CElevator_dialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CElevator_dialogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CElevator_dialogDlg::OnBnClickedCancel)
//	ON_BN_CLICKED(IDC_BtnUp1, &CElevator_dialogDlg::OnBnClickedBtnup1)
//ON_BN_CLICKED(IDC_BtnNum3, &CElevator_dialogDlg::OnBnClickedBtnnum3)
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

	// ����ok��cancel
	RECT rectOk;
	int x,y;
	m_BtnOK.GetClientRect(&rectOk);
	x = back.right+10;
	y = back.bottom-10-rectOk.bottom;
	m_BtnCancel.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y = y - 10 - rectOk.bottom;
	m_BtnOK.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// ����������
	int cx,cy; // �������ڱ߿�Ĵ��ڴ�С
	cx = 5 + back.right + 10 + rectOk.right + 10 + 5; // ���ұ߿��5������
	cy = 40 + back.bottom + 10 + 5; // ����40���±߿� 5
	this->SetWindowPos(0,100,100,cx,cy,SWP_SHOWWINDOW | SWP_NOZORDER);

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
	m_FloorNum[0].LoadBitmaps(IDB_12,IDB_11,IDB_13);
	m_FloorNum[0].SizeToContent();
	m_FloorNum[1].SubclassDlgItem(IDC_BtnNum2,this);
	m_FloorNum[1].LoadBitmaps(IDB_22,IDB_21,IDB_23);
	m_FloorNum[1].SizeToContent();
	m_FloorNum[2].SubclassDlgItem(IDC_BtnNum3,this);
	m_FloorNum[2].LoadBitmaps(IDB_32,IDB_31,IDB_33);
	m_FloorNum[2].SizeToContent();

	RECT rectNum;
	m_FloorNum[0].GetClientRect(&rectNum);
	x = 40; // ����1�Ļ��� 50
	y = back.bottom - 200; // ����1�Ļ���back.bottom - 200
	m_FloorNum[0].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y -= rectNum.bottom - 2;
	m_FloorNum[1].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	y -= rectNum.bottom - 2;
	m_FloorNum[2].SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);

	// ������
	m_Open.SubclassDlgItem(IDC_BtnOpen,this);
	m_Open.LoadBitmaps(IDB_Open1,IDB_Open2,IDB_Open3);
	m_Open.SizeToContent();
	m_Close.SubclassDlgItem(IDC_BtnClose,this);
	m_Close.LoadBitmaps(IDB_Close1,IDB_Close2,IDB_Close3);
	m_Close.SizeToContent();
	
	RECT rectOpen;
	m_Open.GetClientRect(&rectOpen);
	x = 40 + rectNum.right/2 - (rectOpen.right + 10);  // ����1�Ļ��� 40
	y  = back.bottom - 200 + rectNum.bottom + 2; // ����1�Ļ���back.bottom - 200
	m_Open.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
	x = 40 + rectNum.right/2 + 10;
	m_Close.SetWindowPos(0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER); 
	
	// ��������ʱ��
	m_Interval = 100;
	SetTimer(ID_ClOCK_TIMER,m_Interval,NULL);

	// ��ʼֵ
	m_step = 10;
	m_state = Idle;
	m_CurrentCarPosition = 0;
	m_MaxCarPosition = back.bottom - car.bottom;

	// ��ʼ����
	printf("Elevator Startup\n");
	ElevatorStartup();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

CBitmapButton m_btnX1;
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
	if(!IsElevatorRunning()) return;

	main_control(&m_state); // ����״̬��
	elevatorState(m_state); // ����״̬��������
	CDialogEx::OnTimer(nIDEvent);
}

// ����״̬��������
void CElevator_dialogDlg::elevatorState(int state)
{
	printfState(state);
	switch(state)
	{
	case Idle:
		break;
	case MovingUp:
		break;
	case MovingDown:
		break;
	case DoorOpen:
		return;
	case DoorClosing:
		break;
	default:
		printf("û������״̬!!!\n");  
	}
	
	int step = Lib_Power*m_step;
	m_CurrentCarPosition += step;
	Lib_CurrentCarPosition = (Lib_MaxCarPosition/m_MaxCarPosition)*m_CurrentCarPosition;
	Lib_CurrentCarVelocity = step/m_Interval;
	
	if(step == 0) return;
	m_Car_y -= step;
	m_PicCar.SetWindowPos(0,m_Car_x,m_Car_y,0,0,SWP_NOSIZE | SWP_NOZORDER);

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static bool b = false;
	m_UpLight[0].EnableWindow(b);
	m_FloorNum[0].EnableWindow(b);
	m_UpLight[0].setLight(b);
	m_UpLight[0].setLight(b);
	b = !b;
	//CDialogEx::OnOK();
}


void CElevator_dialogDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


//void CElevator_dialogDlg::OnBnClickedBtnup1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//
//}


//void CElevator_dialogDlg::OnBnClickedBtnnum3()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}
