
// gobotDlg.h : ͷ�ļ�
//

#pragma once
#include "DataDef.h"


// CgobotDlg �Ի���
class CgobotDlg : public CDialogEx
{
// ����
public:
	CgobotDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GOBOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	int chessArray[LINE_NUM*LINE_NUM];  //��ֵ0��1��-1��0�����λ��Ϊ�գ�1������壬-1�������
	int lineNum;  //���̴�С
	int lineStep;  //������
	int chessRadius; //���Ӵ�С
	CPoint ptLeftTop;  //������ʼλ��
	CPoint ptRightBottom;  //���̽���λ��

private:
	void DrawUI();
	void ResetUI();
	void DrawLine(CClientDC *dc);
	void DrawBoard(CClientDC *dc);
	void DrawChess(CClientDC *dc);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnReset();
public:
	void SendData(CommData* data);
	void ReceiveData(CommData* data);
};
