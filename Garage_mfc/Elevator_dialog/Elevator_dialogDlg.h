
// Elevator_dialogDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CElevator_dialogDlg 对话框
class CElevator_dialogDlg : public CDialogEx
{
// 构造
public:
	CElevator_dialogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ELEVATOR_DIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
   // 仿真时钟周期(ms,默认100ms)
   int m_Interval;
   // 步长,像素值，默认10
   int m_step;

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
	// 电梯背景
	CStatic m_PicElevatorBackground;
	// 电梯箱体
	CStatic m_PicCar;
	int m_Car_x,m_Car_y; // 电梯箱体的左上角初始坐标
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
