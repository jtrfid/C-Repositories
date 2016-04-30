#pragma once


// CLightBitmapButton
/******************************************************************************************************
 * CLightBitmapButton派生自CBitmapButton
 * CBitmapButton的4个bitmap：正常显示（Up），鼠标按下（Down），获得焦点（Focused），不可用（Disabled）。
 * 本扩展类使用前三个：
 * 鼠标按下左键后，如果原来是第一幅图，显示第二个图片(Light On)。双击左键，正常显示第一个图片(Light Off)
 * 控件失效，即未使能，显示第三幅图片。
 * 
 * 使用：
 * 左键按下，如果当前Light is Off, 置为on(按钮显示第二幅图); 否则保持现状(按钮显示第一幅图)。
 * 双击左键:Light Off(按钮显示第一幅图)，控件失效，即未使能，显示第三幅图片
 * 在对话框窗体拖动一个Button，它的ID是IDC_BUTTON1
 * 导入bitmap图片资源: IDB_bmp1,IDB_bmp2,IDB_bmp3分别代表Light On/Off和Disable
 * 对话框.h文件，定义一个成员变量，CMyBitmapButton m_MyBitmapButton; 
 * 对话框.cpp文件,OnInitDialog()中 
 *  m_MyBitmapButton.SubclassDlgItem(IDC_BUTTON1,this);       // 关联m_MyBitmapButton与IDC_BUTTON1 
 * 	m_MyBitmapButton.LoadBitmaps(IDB_bmp1,IDB_bmp2,IDB_bmp3);  
	m_MyBitmapButton.SizeToContent();   // 使按钮于图形同大小 
 ******************************************************************************************************/ 
class CLightBitmapButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CLightBitmapButton)
private:
	// 左键按下，如果当前Light is Off, 置为on(按钮显示第二幅图); 否则保持现状(按钮显示第一幅图)。
    // 双击左键:Light Off(按钮显示第一幅图)
	bool m_LightOn;  // 灯的状态通过按下鼠标左键反映 
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
	// 获取灯的状态：On,返回true; Off 返回false
	bool getLight() { return m_LightOn; };
	// 设置灯亮LightOn=true(按钮显示第二幅图),否则LightOn=false(按钮显示第一幅图)
	void setLight(bool LightOn) 
	{
		m_LightOn = LightOn;
		Invalidate(FALSE);
	}
};


