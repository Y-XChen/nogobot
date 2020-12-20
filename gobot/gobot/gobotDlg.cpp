
// gobotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "gobot.h"
#include "gobotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CgobotDlg 对话框




CgobotDlg::CgobotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CgobotDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CgobotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
//	ON_WM_NCLBUTTONDOWN()
ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDC_BTN_SAVE, &CgobotDlg::OnBnClickedBtnSave)
ON_BN_CLICKED(IDC_BTN_LOAD, &CgobotDlg::OnBnClickedBtnLoad)
ON_BN_CLICKED(IDC_BTN_RESET, &CgobotDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CgobotDlg 消息处理程序

BOOL CgobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->SetTimer(0, 300, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CgobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CgobotDlg::OnPaint()
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

	this->ResetUI();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CgobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CgobotDlg::ResetUI()
{
	CRect rc;
	GetDlgItem(IDC_PIC)->GetClientRect(&rc);//获取控件区域

	this->lineNum = LINE_NUM;
	this->ptLeftTop = rc.TopLeft();
	this->ptRightBottom = rc.BottomRight();
	this->lineStep = (int)(((double)(this->ptRightBottom.x - this->ptLeftTop.x)) / ((double)(this->lineNum - 1)));
	this->chessRadius = (this->lineStep / 2 ) - 4;

	memset(this->chessArray, 0x00, lineNum*lineNum*sizeof(int));
	for(int i = 0; i != this->lineNum; ++i)
	{
		for (int j = 0; j != this->lineNum; j++)
		{
			this->chessArray[i*lineNum+j] = 0;
		}
	}
}

void CgobotDlg::DrawUI()
{
	CClientDC *dc = (CClientDC*)GetDlgItem(IDC_PIC)->GetDC();//获取控件DC
	this->DrawBoard(dc);
	this->DrawLine(dc);
	this->DrawChess(dc);
}

void CgobotDlg::DrawBoard(CClientDC *dc)
{

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));//创建画笔为绿色
	CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH));
	CRect rc;

	GetDlgItem(IDC_PIC)->GetClientRect(&rc);//获取控件区域
	dc->SelectObject(&pen);//在dc上放置画笔
	dc->SelectObject(pbrush);
	dc->FillRect(rc, pbrush);

	
}

void CgobotDlg::DrawLine(CClientDC *dc)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	dc->SelectObject(&pen);

	for (int i = ptLeftTop.x; i <= ptRightBottom.x; i += lineStep)
	{
		dc->MoveTo(i, ptLeftTop.y);
		dc->LineTo(i, ptRightBottom.y);
	}             

	for (int j = ptLeftTop.y; j <= ptRightBottom.y; j += lineStep)
	{
		dc->MoveTo(ptLeftTop.x, j);
		dc->LineTo(ptRightBottom.x, j);
	}
}

void CgobotDlg::DrawChess(CClientDC *dc)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush *pbrush = NULL;
	dc->SelectObject(&pen);
	for (int i = 0; i < lineNum; i++)
	{
		for (int j = 0; j < lineNum; j++)
		{
			if (chessArray[i*lineNum + j] ==1)
			{
				pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH));
				dc->SelectObject(pbrush);
				dc->Ellipse(ptLeftTop.x+lineStep*i-chessRadius, ptLeftTop.y+lineStep *j- chessRadius, ptLeftTop.x + lineStep*i + chessRadius, ptLeftTop.y + lineStep *j + chessRadius);
			}
			if (chessArray[i*lineNum + j] == -1)
			{
				pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH));
				dc->SelectObject(pbrush);
				dc->Ellipse(ptLeftTop.x + lineStep*i - chessRadius, ptLeftTop.y + lineStep *j - chessRadius, ptLeftTop.x + lineStep*i + chessRadius, ptLeftTop.y + lineStep *j + chessRadius);
			}
		}
	}
}

void CgobotDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->DrawUI();
	CDialogEx::OnTimer(nIDEvent);
}


void CgobotDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	static int sum = 0;

	int posX,posY;
	posX=(int)(point.x-this->chessRadius/2)/lineStep ;
	posY=(int)(point.y-this->chessRadius/2)/lineStep ;
	if (posX > -1 && posX < lineNum && posY > -1 && posY < lineNum)
	{
		if(sum++ % 2 == 0)
			this->chessArray[posX * lineNum + posY] = 1;
		else
			this->chessArray[posX * lineNum + posY] = -1;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}



void CgobotDlg::OnBnClickedBtnSave()
{
	char pFileName[MAX_PATH] = {0x00}; 
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
	CString cfgPath(pFileName);

	BOOL isOpen = FALSE;		//是否打开(否则为保存)
	CString defaultDir = cfgPath;	//默认打开的文件路径
	CString fileName = "default.cfg";			//默认打开的文件名
	CString filter = "文件 (*.cfg)|*.cfg||";	//文件过滤的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if(result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}

	//保存chessArray到cfg文件
	CFile file;
	file.Open(filePath,CFile::modeCreate|CFile::modeWrite,NULL);
	file.Write((void*)this->chessArray, sizeof(this->chessArray));  
	file.Close();
}


void CgobotDlg::OnBnClickedBtnLoad()
{
	char pFileName[MAX_PATH] = {0x00}; 
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
	CString cfgPath(pFileName);

	BOOL isOpen = TRUE;		//是否打开(否则为保存)
	CString defaultDir = cfgPath;	//默认打开的文件路径
	CString fileName = "default.cfg";			//默认打开的文件名
	CString filter = "文件 (*.cfg)|*.cfg||";	//文件过滤的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\default.cfg";
	if(result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}
	
	//从cfg文件读取数据到chessArray
	CFile file;
	file.Open(filePath,CFile::modeRead,NULL); 
	file.Read(this->chessArray, sizeof(int)*LINE_NUM*LINE_NUM); 
	file.Close();
}


void CgobotDlg::OnBnClickedBtnReset()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ResetUI();
}

void CgobotDlg::SendData(CommData* data)
{
	memcpy(this->chessArray, data->points, sizeof(int)*LINE_NUM*LINE_NUM);
}

void CgobotDlg::ReceiveData(CommData* data)
{
	memcpy(data->points, this->chessArray, sizeof(int)*LINE_NUM*LINE_NUM);
}