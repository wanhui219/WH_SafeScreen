
// WH_SafeScreenDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CWH_SafeScreenDlg �Ի���
class CWH_SafeScreenDlg : public CDialogEx
{
// ����
public:
	CWH_SafeScreenDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WH_SAFESCREEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
