
// gobotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "gobot.h"
#include "gobotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CgobotDlg �Ի���




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


// CgobotDlg ��Ϣ�������

BOOL CgobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	this->SetTimer(0, 300, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CgobotDlg::OnPaint()
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

	this->ResetUI();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CgobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CgobotDlg::ResetUI()
{
	CRect rc;
	GetDlgItem(IDC_PIC)->GetClientRect(&rc);//��ȡ�ؼ�����

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
	CClientDC *dc = (CClientDC*)GetDlgItem(IDC_PIC)->GetDC();//��ȡ�ؼ�DC
	this->DrawBoard(dc);
	this->DrawLine(dc);
	this->DrawChess(dc);
}

void CgobotDlg::DrawBoard(CClientDC *dc)
{

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));//��������Ϊ��ɫ
	CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH));
	CRect rc;

	GetDlgItem(IDC_PIC)->GetClientRect(&rc);//��ȡ�ؼ�����
	dc->SelectObject(&pen);//��dc�Ϸ��û���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	BOOL isOpen = FALSE;		//�Ƿ��(����Ϊ����)
	CString defaultDir = cfgPath;	//Ĭ�ϴ򿪵��ļ�·��
	CString fileName = "default.cfg";			//Ĭ�ϴ򿪵��ļ���
	CString filter = "�ļ� (*.cfg)|*.cfg||";	//�ļ����˵�����
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if(result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}

	//����chessArray��cfg�ļ�
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

	BOOL isOpen = TRUE;		//�Ƿ��(����Ϊ����)
	CString defaultDir = cfgPath;	//Ĭ�ϴ򿪵��ļ�·��
	CString fileName = "default.cfg";			//Ĭ�ϴ򿪵��ļ���
	CString filter = "�ļ� (*.cfg)|*.cfg||";	//�ļ����˵�����
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\default.cfg";
	if(result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}
	
	//��cfg�ļ���ȡ���ݵ�chessArray
	CFile file;
	file.Open(filePath,CFile::modeRead,NULL); 
	file.Read(this->chessArray, sizeof(int)*LINE_NUM*LINE_NUM); 
	file.Close();
}


void CgobotDlg::OnBnClickedBtnReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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