
// Left4Dead2PluginsDlg.h : ͷ�ļ�
//

#pragma once


// CLeft4Dead2PluginsDlg �Ի���
class CLeft4Dead2PluginsDlg : public CDialogEx
{
// ����
public:
	CLeft4Dead2PluginsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEFT4DEAD2PLUGINS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
