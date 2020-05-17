
// WH_SafeScreenDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CWH_SafeScreenDlg 对话框
class CWH_SafeScreenDlg : public CDialogEx
{
// 构造
public:
	CWH_SafeScreenDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WH_SAFESCREEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
    CEdit m_edt_pwd;
    CButton m_btn_exit;
    afx_msg void OnBnClickedBtnExit();
    void FullScreen();
    CStatic m_sta_hint;
};
