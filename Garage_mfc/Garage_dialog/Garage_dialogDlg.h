
// Garage_dialogDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CGarage_dialogDlg 对话框
class CGarage_dialogDlg : public CDialogEx
{
// 构造
public:
	CGarage_dialogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GARAGE_DIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	int doorHeight;     // 门高度,OnInitDialog()中，根据图片大小，计算获得的像素值
	// 当前门的相对位置(像素值),门的下边沿距离地面的相对高度。
	// 门关闭时=0, 打开时=doorHeight
	// Lib_CurrentDoorPosition = (Lib_DoorHeight/doorHeight）* currentDoorPosition;
	int currentDoorPosition; 

	// 动画步长(像素值), 缺省为10个像素值 
	int step; 
	// 动画间隔（周期，ms)，缺省值100ms
	int interval;
	void DoorUpDown(int state);  // 门开关锁动画,currentDoorPosition在0和dorrHeight之间变化

	// 开门状态，特定时间内，无动作，自动关门 ，缺省值10000ms(10s)
	int OpenInterval;
	bool StartTimerClosing; // ID_Timer_Closing定时器启动否？

	// 状态,初值为DoorClosed
	int State;

	// cat按钮在门外位置(初始位置)
	int catOutDoor_top,catOutDoor_left;
	// Cat按钮在门下的位置
	int catInDoor_top,catInDoor_left;

	// 打印当前状态
	void printfState(int state);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 车库门开关
	CButton m_btnSwitch;
	afx_msg void OnBnClickedbtnSwitch();
	// 确定
	CButton m_BTN_OK;
	afx_msg void OnBnClickedOk();
	// 门
	CStatic m_PicDoor;
	// 有车的车库
	CStatic m_PicGarage;
	
	// 关闭(取消)按钮
	CButton m_BtnCancel;
	afx_msg void OnBnClickedCancel();

	// 开始仿真
	CButton m_BtnStart;
	afx_msg void OnBnClickedBtnStart();

	// 结束仿真
	CButton m_BtnEnd;
	afx_msg void OnBnClickedBtnEnd();
	
	// 定时器回调函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// Cat按钮
	CButton m_BtnCat;
	afx_msg void OnBnClickedBtnCat();

	// 即将显示或隐藏窗口时的处理程序
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
