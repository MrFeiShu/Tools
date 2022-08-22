
// WinPackToolDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CWinPackToolDlg 对话框
class CWinPackToolDlg : public CDialogEx
{
// 构造
public:
	CWinPackToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WINPACKTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	BOOL ParseTask();

	BOOL TestTree();

private:
	char* m_szConf;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeTask;
	CListBox m_listLog;
};
