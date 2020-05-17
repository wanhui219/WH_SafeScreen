
// WH_SafeScreenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WH_SafeScreen.h"
#include "WH_SafeScreenDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWH_SafeScreenDlg 对话框
//全局窗口
#define PWD "Why20160308"

CWH_SafeScreenDlg *pDlg = NULL;

HHOOK g_hKeyBoard = NULL;
//底层键盘系统调用，过滤alt + tab和win键
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
                pDlg->m_sta_hint.SetWindowTextA("！！！【大写锁定】！！！");
            }
            else
            {
                pDlg->m_sta_hint.SetWindowTextA("远程防误操作，输入密码亦可退出");
            }        

            break;
        }
    }

    return ::CallNextHookEx( g_hKeyBoard, code, wParam, lParam );
}
//注册全局hook
void SetHook()
{
    g_hKeyBoard = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardProc, GetModuleHandle("Hook"), 0);
}
//注销全局hook
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


// CWH_SafeScreenDlg 消息处理程序

BOOL CWH_SafeScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    pDlg = this;
    //全屏并禁止移动
    FullScreen();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWH_SafeScreenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

    //设置全屏
    CRect rcDlg;     
    rcDlg.TopLeft() = CPoint(0, 0);
    rcDlg.BottomRight() = CPoint(cx, cy); 
    this->MoveWindow(&rcDlg);

    //设置控件大小  
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
        exit(0);//直接退出
    }
}

BOOL CWH_SafeScreenDlg::PreTranslateMessage(MSG* pMsg)
{
    return FALSE;//按键无效
}