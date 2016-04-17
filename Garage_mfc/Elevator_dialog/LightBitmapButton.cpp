// LightBitmapButton.cpp : 实现文件
//

#include "stdafx.h"
#include "Elevator_dialog.h"
#include "LightBitmapButton.h"

// CLightBitmapButton

IMPLEMENT_DYNAMIC(CLightBitmapButton, CBitmapButton)

CLightBitmapButton::CLightBitmapButton()
{
	m_LightOn = false;
}

CLightBitmapButton::~CLightBitmapButton()
{
}

BEGIN_MESSAGE_MAP(CLightBitmapButton, CBitmapButton)
	ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CLightBitmapButton 消息处理程序

// 左键按下，如果当前Light is Off, 置为on;否则保持现状。
void CLightBitmapButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 消息执行顺序：控件的消息执行在前，对话框的按钮命令执行在后。OnLButtonDblClk 消息会额外引起一次OnLButtonDown及其对话框按钮单击命令的发生。
	// (1) CLightBitmapButton::OnLButtonDown
	// (2) 对话框点击按钮命令BN_CLICKED，如，CElevator_dialogDlg::OnBnClickedBtnup1()
	if(!m_LightOn) m_LightOn = true;
	Invalidate(FALSE);
	CBitmapButton::OnLButtonDown(nFlags, point);
}

// 双击左键，Light is off
void CLightBitmapButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 消息执行顺序： 控件的消息执行在前，对话框的按钮命令执行在后。OnLButtonDblClk 消息会额外引起一次OnLButtonDown及其对话框按钮单击命令的发生。
	// (1) CLightBitmapButton::OnLButtonDown
	// (2) 对话框点击按钮命令，如，CElevator_dialogDlg::OnBnClickedBtnup1()
	// (3) CLightBitmapButton::OnLButtonDblClk()
	// (4) 对话框双击按钮命令BN_DOUBLECLICKED，如，CElevator_dialogDlg::OnDoubleclickedBtnup1()
	m_LightOn = false;
	// Invalidate( BOOL bErase = TRUE );
	// Invalidates the entire client area of CWnd
	// bErase: Specifies whether the background within the update region is to be erased.
	Invalidate(FALSE);  
	CBitmapButton::OnLButtonDblClk(nFlags, point);
}

// 自绘, 灯亮LightOn=true(按钮显示第二幅图),否则LightOn=false(按钮显示第一幅图),
// 控件失效，即未使能，显示第三幅图片
void CLightBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	ASSERT(lpDrawItemStruct != NULL); 
	// must have at least the first bitmap loaded before calling DrawItem 
	ASSERT(m_bitmap.m_hObject != NULL);     // 至少有一个正常显示的CBitmap对象 

	CBitmap* pBitmap = &m_bitmap; // 第一个图片正常显示(Light off)
	UINT state = lpDrawItemStruct->itemState; 

	if ((state & ODS_DISABLED) && m_bitmapFocus.m_hObject != NULL) 
		pBitmap = &m_bitmapFocus;   // 第三个图片用于未使能按钮(disable)
	else if(m_LightOn && m_bitmapSel.m_hObject != NULL)
		pBitmap = &m_bitmapSel; // 第二个图片，左键按下后显示(Light on)

	// draw the whole button 
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC); 
	CDC memDC; 
	memDC.CreateCompatibleDC(pDC); 
	CBitmap* pOld = memDC.SelectObject(pBitmap); 
	if (pOld == NULL) 
		return;     // destructors will clean up 

	CRect rect; 
	rect.CopyRect(&lpDrawItemStruct->rcItem); 
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
		&memDC, 0, 0, SRCCOPY); 
	memDC.SelectObject(pOld); 
}


// 被关联到窗口前被调用
void CLightBitmapButton::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0, BS_OWNERDRAW); // 自绘
	CBitmapButton::PreSubclassWindow();
}
