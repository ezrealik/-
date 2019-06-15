
// Left4Dead2PluginsDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "stdafx.h"
#include "afxcmn.h"

// CLeft4Dead2PluginsDlg 对话框
class CLeft4Dead2PluginsDlg : public CDialogEx
{
// 构造
public:
	CLeft4Dead2PluginsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEFT4DEAD2PLUGINS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuPluginActivate();
	afx_msg void OnMenuPluginClose();
	afx_msg void OnMenuPluginDelete();
	afx_msg void OnMenuRefrsh();
	afx_msg void OnMenuPluginConfig();
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnMenuOpenConfigPath();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
protected:
	CProgressCtrl Progress;
	CEdit Edit_GamePath;
	CTreeCtrl List_PluginItems;
	WCHAR ConfigPath[1024] = { 0 };
	WCHAR SteamPath[1024] = { 0 };
	CImageList Imagelist;
	BOOL Close_Bol = FALSE;
	CButton CheckBox_Model;
	CEdit Edit_ADDOP;
	CComboBox Combo_OP;
	void InitEditInfo();
	BOOL GetFileRunPath(WCHAR *PathBuffer, DWORD BufLen);
	void InitListInfo();
	void InitComboBox();
	BOOL IsProcessExist(WCHAR *ProcessName);
	void ToWUpperStr(WCHAR *des, WCHAR*source);
};
