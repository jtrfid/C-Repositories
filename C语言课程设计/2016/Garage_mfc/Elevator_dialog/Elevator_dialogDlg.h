
// Elevator_dialogDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "ElevatorLib.h"
#include "LightBitmapButton.h"

// CElevator_dialogDlg �Ի���
class CElevator_dialogDlg : public CDialogEx
{
// ����
public:
	CElevator_dialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ELEVATOR_DIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
   // ����ʱ������(ms,Ĭ��100ms)
   int m_Interval;
   // ���ź���һ����ʱ��ģ����˿�Ԥ�����µ���ʱ��(ms,Ĭ��2000ms,2s)
   int m_DoorInterval;
   // ���������ſ����Ŷ���������2s�����ţ�(�ſ��/20000)*m_Interval + 1, ��1�����ص���������֤������ʱ��������䶯��
   int m_DoorStep; 
   // ����,����ֵ��Ĭ��10
   int m_step;
   // ��������״̬, ��ʼ״̬: Idle
   int m_state;
   // ����״̬��������
   void elevatorState(int state);
   // ���������ſ����ŷ���
   void elevatorDoor(int state);
   // ���������±��ؾ���������Ը߶�,��ʼֵ0
   int m_CurrentCarPosition;
   // ���������±��ؾ������������Ը߶ȣ����������ʱ�����������±��ؾ���������Ը߶�
   int m_MaxCarPosition; 

   // ��ӡ��ǰ״̬
   void printfState(int state);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ���ݱ���
	CStatic m_PicElevatorBackground;
	// ��������
	CStatic m_PicCar;
	int m_Car_x,m_Car_y; // ������������Ͻǳ�ʼ����

	// �����������ż����Ͻǳ�ʼ����,��С
	CStatic m_PicDoorLeft;
	int m_DoorLeft_x, m_DoorLeft_y; 
	// �����������ż����Ͻǳ�ʼ���꣬��С
	CStatic m_PicDoorRight;
	int m_DoorRight_x, m_DoorRight_y;
	RECT m_DoorRect; // �����Ŵ�С��һ����
	int m_DoorCx; // �ŵĿ�ȣ����ڶ���������: m_DoorRect.right-->0��������m_DoorStep; ���ŷ�֮.
	
	// ��ʱ���ص�����
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	// ȷ����ť
	CButton m_BtnOK;
	afx_msg void OnBnClickedOk();
	// ȡ����ť
	CButton m_BtnCancel;
	afx_msg void OnBnClickedCancel();

    // �����⣬1��2��3¥Up/Down��ť��Call Light��
	CLightBitmapButton m_UpLight[Lib_FloorNum];
	CLightBitmapButton m_DownLight[Lib_FloorNum];
	// ����״̬,�����ǰLight is Off, ��Ϊon;���򱣳���״��
	afx_msg void OnBnClickedBtnup1();  
	afx_msg void OnDoubleclickedBtnup1();

	// ������,1,2,3���ְ�ť��PanelFloorLight��
	CLightBitmapButton m_FloorNum[Lib_FloorNum];
	// ����״̬,�����ǰLight is Off, ��Ϊon;���򱣳���״��
	afx_msg void OnClickedBtnnum1();
	afx_msg void OnDoubleclickedBtnnum1();

	// �����ڿ�����
	CLightBitmapButton m_Open;
	CLightBitmapButton m_Close;
	// ����״̬�������ǰLight is Off, ��Ϊon;���򱣳���״��
	afx_msg void OnBnClickedBtnOpenCloseDoor();
	
	// ������Ϣ�����µ������ⰴť��״̬
	afx_msg LRESULT OnLightMessage(WPARAM wParam,LPARAM lParam);

	// ״̬�ı�
	CStatic m_TxtStatus;

	// ������Ϣ������״̬�ı�
    afx_msg LRESULT OnViewStatusMessage(WPARAM wParam,LPARAM lParam);

	// ������Ϣ��������
	afx_msg LRESULT OnOpenCloseDoorMessage(WPARAM wParam,LPARAM lParam);

	// �����ھ�����ڶԻ���OnInitDialog�б���ֵ���������䷢����Ϣ
	static HWND MAIN_WIN;
	
};
