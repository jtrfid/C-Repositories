// LightBitmapButton.cpp : ʵ���ļ�
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

// CLightBitmapButton ��Ϣ�������

// ������£������ǰLight is Off, ��Ϊon;���򱣳���״��
void CLightBitmapButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ��Ϣִ��˳�򣺿ؼ�����Ϣִ����ǰ���Ի���İ�ť����ִ���ں�OnLButtonDblClk ��Ϣ���������һ��OnLButtonDown����Ի���ť��������ķ�����
	// (1) CLightBitmapButton::OnLButtonDown
	// (2) �Ի�������ť����BN_CLICKED���磬CElevator_dialogDlg::OnBnClickedBtnup1()
	if(!m_LightOn) m_LightOn = true;
	Invalidate(FALSE);
	CBitmapButton::OnLButtonDown(nFlags, point);
}

// ˫�������Light is off
void CLightBitmapButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ��Ϣִ��˳�� �ؼ�����Ϣִ����ǰ���Ի���İ�ť����ִ���ں�OnLButtonDblClk ��Ϣ���������һ��OnLButtonDown����Ի���ť��������ķ�����
	// (1) CLightBitmapButton::OnLButtonDown
	// (2) �Ի�������ť����磬CElevator_dialogDlg::OnBnClickedBtnup1()
	// (3) CLightBitmapButton::OnLButtonDblClk()
	// (4) �Ի���˫����ť����BN_DOUBLECLICKED���磬CElevator_dialogDlg::OnDoubleclickedBtnup1()
	m_LightOn = false;
	// Invalidate( BOOL bErase = TRUE );
	// Invalidates the entire client area of CWnd
	// bErase: Specifies whether the background within the update region is to be erased.
	Invalidate(FALSE);  
	CBitmapButton::OnLButtonDblClk(nFlags, point);
}

// �Ի�, ����LightOn=true(��ť��ʾ�ڶ���ͼ),����LightOn=false(��ť��ʾ��һ��ͼ),
// �ؼ�ʧЧ����δʹ�ܣ���ʾ������ͼƬ
void CLightBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  ������Ĵ����Ի���ָ����
	ASSERT(lpDrawItemStruct != NULL); 
	// must have at least the first bitmap loaded before calling DrawItem 
	ASSERT(m_bitmap.m_hObject != NULL);     // ������һ��������ʾ��CBitmap���� 

	CBitmap* pBitmap = &m_bitmap; // ��һ��ͼƬ������ʾ(Light off)
	UINT state = lpDrawItemStruct->itemState; 

	if ((state & ODS_DISABLED) && m_bitmapFocus.m_hObject != NULL) 
		pBitmap = &m_bitmapFocus;   // ������ͼƬ����δʹ�ܰ�ť(disable)
	else if(m_LightOn && m_bitmapSel.m_hObject != NULL)
		pBitmap = &m_bitmapSel; // �ڶ���ͼƬ��������º���ʾ(Light on)

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


// ������������ǰ������
void CLightBitmapButton::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	ModifyStyle(0, BS_OWNERDRAW); // �Ի�
	CBitmapButton::PreSubclassWindow();
}
