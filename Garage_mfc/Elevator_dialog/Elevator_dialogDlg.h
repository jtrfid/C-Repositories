
// Elevator_dialogDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "ElevatorLib.h"
#include "LightBitmapButton.h"

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
   // 电梯箱体状态, 初始状态: Idle
   int m_state;
   // 电梯状态动画仿真
   void elevatorState(int state);
   // 电梯箱体下边沿距离地面的相对高度,初始值0
   int m_CurrentCarPosition;
   // 电梯箱体下边沿距离地面的最大相对高度，即到达最顶层时，电梯箱体下边沿距离地面的相对高度
   int m_MaxCarPosition; 

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
	// 电梯背景
	CStatic m_PicElevatorBackground;
	// 电梯箱体
	CStatic m_PicCar;
	int m_Car_x,m_Car_y; // 电梯箱体的左上角初始坐标
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 确定按钮
	CButton m_BtnOK;
	afx_msg void OnBnClickedOk();
	// 取消按钮
	CButton m_BtnCancel;
	afx_msg void OnBnClickedCancel();

    // 电梯外，1、2、3楼Up/Down按钮（Call Light）
	CLightBitmapButton m_UpLight[Lib_FloorNum];
	CLightBitmapButton m_DownLight[Lib_FloorNum];
	// 设置状态,如果当前Light is Off, 置为on;否则保持现状。
	afx_msg void OnBnClickedBtnup1();  
	afx_msg void OnDoubleclickedBtnup1();

	// 电梯内,1,2,3数字按钮（PanelFloorLight）
	CLightBitmapButton m_FloorNum[Lib_FloorNum];
	// 设置状态,如果当前Light is Off, 置为on;否则保持现状。
	afx_msg void OnClickedBtnnum1();
	afx_msg void OnDoubleclickedBtnnum1();

	// 电梯内开关门
	CLightBitmapButton m_Open;
	CLightBitmapButton m_Close;
	
	// 接收消息，更新电梯内外按钮灯状态
	afx_msg LRESULT OnLightMessage(WPARAM wParam,LPARAM lParam);

	// 状态文本
	CStatic m_TxtStatus;

	// 接收消息，更新状态文本
    afx_msg LRESULT OnViewStatusMessage(WPARAM wParam,LPARAM lParam);

	// 接收消息，开关门
	afx_msg LRESULT OnOpenCloseDoorMessage(WPARAM wParam,LPARAM lParam);

	// 主窗口句柄，在对话框OnInitDialog中被赋值，用于向其发送消息
	static HWND MAIN_WIN;
};
