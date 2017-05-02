#pragma once


// CLightBitmapButton
/******************************************************************************************************
 * CLightBitmapButton������CBitmapButton
 * CBitmapButton��4��bitmap��������ʾ��Up������갴�£�Down������ý��㣨Focused���������ã�Disabled����
 * ����չ��ʹ��ǰ������
 * ��갴����������ԭ���ǵ�һ��ͼ����ʾ�ڶ���ͼƬ(Light On)��˫�������������ʾ��һ��ͼƬ(Light Off)
 * �ؼ�ʧЧ����δʹ�ܣ���ʾ������ͼƬ��
 * 
 * ʹ�ã�
 * ������£������ǰLight is Off, ��Ϊon(��ť��ʾ�ڶ���ͼ); ���򱣳���״(��ť��ʾ��һ��ͼ)��
 * ˫�����:Light Off(��ť��ʾ��һ��ͼ)���ؼ�ʧЧ����δʹ�ܣ���ʾ������ͼƬ
 * �ڶԻ������϶�һ��Button������ID��IDC_BUTTON1
 * ����bitmapͼƬ��Դ: IDB_bmp1,IDB_bmp2,IDB_bmp3�ֱ����Light On/Off��Disable
 * �Ի���.h�ļ�������һ����Ա������CMyBitmapButton m_MyBitmapButton; 
 * �Ի���.cpp�ļ�,OnInitDialog()�� 
 *  m_MyBitmapButton.SubclassDlgItem(IDC_BUTTON1,this);       // ����m_MyBitmapButton��IDC_BUTTON1 
 * 	m_MyBitmapButton.LoadBitmaps(IDB_bmp1,IDB_bmp2,IDB_bmp3);  
	m_MyBitmapButton.SizeToContent();   // ʹ��ť��ͼ��ͬ��С 
 ******************************************************************************************************/ 
class CLightBitmapButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CLightBitmapButton)
private:
	// ������£������ǰLight is Off, ��Ϊon(��ť��ʾ�ڶ���ͼ); ���򱣳���״(��ť��ʾ��һ��ͼ)��
    // ˫�����:Light Off(��ť��ʾ��һ��ͼ)
	bool m_LightOn;  // �Ƶ�״̬ͨ��������������ӳ 
public:
	CLightBitmapButton();
	virtual ~CLightBitmapButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();

public:
	// ��ȡ�Ƶ�״̬��On,����true; Off ����false
	bool getLight() { return m_LightOn; };
	// ���õ���LightOn=true(��ť��ʾ�ڶ���ͼ),����LightOn=false(��ť��ʾ��һ��ͼ)
	void setLight(bool LightOn) 
	{
		m_LightOn = LightOn;
		Invalidate(FALSE);
	}
};


