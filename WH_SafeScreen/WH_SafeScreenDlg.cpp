
// WH_SafeScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WH_SafeScreen.h"
#include "WH_SafeScreenDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWH_SafeScreenDlg �Ի���
//ȫ�ִ���
#define PWD "Why20160308"

CWH_SafeScreenDlg *pDlg = NULL;

HHOOK g_hKeyBoard = NULL;
//�ײ����ϵͳ���ã�����alt + tab��win��
LRESULT CALLBACK KeyBoardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION)
    {
        PKBDLLHOOKSTRUCT p;

        switch (wParam) 
        {
        case WM_KEYDOWN:  
        case WM_SYSKEYDOWN:
        case WM_KEYUP:    
        case WM_SYSKEYUP: 
            p = (PKBDLLHOOKSTRUCT)lParam;

            if(   (p->vkCode == VK_TAB    && (p->flags & LLKHF_ALTDOWN) != 0) //ALT+TAB
               || (p->vkCode == VK_F4     && (p->flags & LLKHF_ALTDOWN) != 0) //ALT+F4
               || (p->vkCode == VK_ESCAPE && (p->flags & LLKHF_ALTDOWN) != 0) //Alt+Esc
               || (p->vkCode == VK_ESCAPE && (GetKeyState(VK_CONTROL) & 0x8000) != 0) //Ctrl+Esc
               || (p->vkCode == VK_LWIN)
               || (p->vkCode == VK_RWIN)
               )
            {
                return 1;
            }
            if((GetKeyState(VK_CAPITAL) & 1) != 0)
            {
                pDlg->m_sta_hint.SetWindowTextA("����������д������������");
            }
            else
            {
                pDlg->m_sta_hint.SetWindowTextA("Զ�̷��������������������˳�");
            }        

            break;
        }
    }

    return ::CallNextHookEx( g_hKeyBoard, code, wParam, lParam );
}
//ע��ȫ��hook
void SetHook()
{
    g_hKeyBoard = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardProc, GetModuleHandle("Hook"), 0);
}
//ע��ȫ��hook
void UnSetHook()
{
    if(g_hKeyBoard)
        UnhookWindowsHookEx(g_hKeyBoard);
    g_hKeyBoard = NULL;
}

CWH_SafeScreenDlg::CWH_SafeScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWH_SafeScreenDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWH_SafeScreenDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDT_PWD, m_edt_pwd);
    m_edt_pwd.SetLimitText(64);
    DDX_Control(pDX, IDC_BTN_EXIT, m_btn_exit);
    DDX_Control(pDX, IDC_STA_HINT, m_sta_hint);
}

BEGIN_MESSAGE_MAP(CWH_SafeScreenDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_EXIT, &CWH_SafeScreenDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()


// CWH_SafeScreenDlg ��Ϣ�������

BOOL CWH_SafeScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    pDlg = this;
    //ȫ������ֹ�ƶ�
    FullScreen();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWH_SafeScreenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWH_SafeScreenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWH_SafeScreenDlg::FullScreen()
{
    SetHook();

    int cx,cy; 
    cx = GetSystemMetrics(SM_CXSCREEN); 
    cy = GetSystemMetrics(SM_CYSCREEN); 

    //����ȫ��
    CRect rcDlg;     
    rcDlg.TopLeft() = CPoint(0, 0);
    rcDlg.BottomRight() = CPoint(cx, cy); 
    this->MoveWindow(&rcDlg);

    //���ÿؼ���С  
    CRect rcPwd;
    rcPwd.left = 0;
    rcPwd.right = cx;
    rcPwd.top = cy / 2 - 15;
    rcPwd.bottom = cy / 2 + 15;
    m_edt_pwd.MoveWindow(&rcPwd);

    CRect rcHint;  
    rcHint.left = 0;
    rcHint.right = cx;
    rcHint.top = rcPwd.bottom - 100;
    rcHint.bottom = rcPwd.bottom - 50;
    m_sta_hint.MoveWindow(&rcHint);

    CRect rcBtn;  
    rcBtn.left = cx / 2 - 100;
    rcBtn.right = cx / 2 + 100;
    rcBtn.top = rcPwd.bottom + 50;
    rcBtn.bottom = rcPwd.bottom + 250;  
    m_btn_exit.MoveWindow(&rcBtn);
}

void CWH_SafeScreenDlg::OnBnClickedBtnExit()
{
    char pwd[128];
    ZeroMemory(pwd, sizeof(pwd));

    m_edt_pwd.GetWindowTextA(pwd, sizeof(pwd));
    if(! memcmp(pwd, PWD, strlen(PWD)))
    {
        UnSetHook();
        exit(0);//ֱ���˳�
    }
}

BOOL CWH_SafeScreenDlg::PreTranslateMessage(MSG* pMsg)
{
    return FALSE;//������Ч
}