
// gobotDlg.h : 头文件
//

#pragma once
#include "DataDef.h"


// CgobotDlg 对话框
class CgobotDlg : public CDialogEx
{
// 构造
public:
	CgobotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GOBOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	int chessArray[LINE_NUM*LINE_NUM];  //数值0、1、-1，0代表该位置为空，1代表白棋，-1代表黑棋
	int lineNum;  //棋盘大小
	int lineStep;  //网格间距
	int chessRadius; //棋子大小
	CPoint ptLeftTop;  //棋盘起始位置
	CPoint ptRightBottom;  //棋盘结束位置

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
