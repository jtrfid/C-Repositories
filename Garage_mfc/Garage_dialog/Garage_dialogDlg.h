
// Garage_dialogDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CGarage_dialogDlg �Ի���
class CGarage_dialogDlg : public CDialogEx
{
// ����
public:
	CGarage_dialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GARAGE_DIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	int doorHeight;     // �Ÿ߶�,OnInitDialog()�У�����ͼƬ��С�������õ�����ֵ
	// ��ǰ�ŵ����λ��(����ֵ),�ŵ��±��ؾ���������Ը߶ȡ�
	// �Źر�ʱ=0, ��ʱ=doorHeight
	// Lib_CurrentDoorPosition = (Lib_DoorHeight/doorHeight��* currentDoorPosition;
	int currentDoorPosition; 

	// ��������(����ֵ), ȱʡΪ10������ֵ 
	int step; 
	// ������������ڣ�ms)��ȱʡֵ100ms
	int interval;
	void DoorUpDown(int state);  // �ſ���������,currentDoorPosition��0��dorrHeight֮��仯

	// ����ʱ��,���������������,ms����ȱʡֵ100ms
	int samplingInterval;

	// ״̬,��ֵΪDoorClosed
	int State;

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
	// �����ſ���
	CButton m_btnSwitch;
	afx_msg void OnBnClickedbtnSwitch();
	// ȷ��
	CButton m_BTN_OK;
	afx_msg void OnBnClickedOk();
	// ��
	CStatic m_PicDoor;
	// �г��ĳ���
	CStatic m_PicGarage;
	
	// �ر�(ȡ��)��ť
	CButton m_BtnCancel;
	afx_msg void OnBnClickedCancel();

	// ��ʼ����
	CButton m_BtnStart;
	afx_msg void OnBnClickedBtnStart();

	// ��������
	CButton m_BtnEnd;
	afx_msg void OnBnClickedBtnEnd();
	
	// ��ʱ���ص�����
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
