
// Left4Dead2PluginsDlg.cpp : 实现文件
//

#include "stdafx.h"
#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeft4Dead2PluginsDlg 对话框



CLeft4Dead2PluginsDlg::CLeft4Dead2PluginsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LEFT4DEAD2PLUGINS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CLeft4Dead2PluginsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit_GamePath);
	DDX_Control(pDX, IDC_LIST, List_PluginItems);
	DDX_Control(pDX, IDC_PROGRESS2, Progress);
	DDX_Control(pDX, IDC_EDIT2, Edit_ADDOP);
	DDX_Control(pDX, IDC_COMBO1, Combo_OP);
}

BEGIN_MESSAGE_MAP(CLeft4Dead2PluginsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CLeft4Dead2PluginsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CLeft4Dead2PluginsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CLeft4Dead2PluginsDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CLeft4Dead2PluginsDlg::OnNMRClickList)
	ON_COMMAND(ID_MENU_32771, &CLeft4Dead2PluginsDlg::OnMenuPluginActivate)
	ON_COMMAND(ID_MENU_32772, &CLeft4Dead2PluginsDlg::OnMenuPluginClose)
	ON_COMMAND(ID_MENU_32774, &CLeft4Dead2PluginsDlg::OnMenuPluginDelete)
	ON_COMMAND(ID_MENU_32775, &CLeft4Dead2PluginsDlg::OnMenuPluginConfig)
	ON_COMMAND(ID_MENU_32776, &CLeft4Dead2PluginsDlg::OnMenuRefrsh)
	ON_COMMAND(ID_Menu32779, &CLeft4Dead2PluginsDlg::OnMenuOpenConfigPath)
	ON_EN_SETFOCUS(IDC_EDIT2, &CLeft4Dead2PluginsDlg::OnEnSetfocusEdit2)
END_MESSAGE_MAP()


// CLeft4Dead2PluginsDlg 消息处理程序
//初始化组合框列表
void CLeft4Dead2PluginsDlg::InitComboBox() {
	FILE *file;
	CString GamePath;
	CHAR Temp[256] = { 0 };
	WCHAR Data[1024] = { 0 };
	Combo_OP.ResetContent();//清空项目
	Edit_GamePath.GetWindowTextW(GamePath);
	GamePath.Append(L"\\left4dead2\\addons\\sourcemod\\configs\\admins_simple.ini");
	WideCharToMultiByte(CP_ACP, 0, GamePath, -1, Temp, 256, NULL, 0);
	file = fopen(Temp, "rb+");
	if (file==NULL) {
		Combo_OP.AddString(L"插件未安装或无法读取配置文件");
		Combo_OP.SetCurSel(0);
		return;
	}
	fseek(file, 0, SEEK_END);
	int Flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	void *alocbuf = malloc(Flen + 1);
	if (!alocbuf) {
		MessageBox(L"内存申请失败！");
		return;
	}
	fread(alocbuf, sizeof(char), Flen - 1, file);
	char *pr = strtok((char*)alocbuf, "\r\n");
	char *prd;
	while (pr) {
		if (pr[0] == '"'&&pr[1] != '/') {
			if (strstr(pr, "99:z")) {
				pr[0] = { 0 };
				pr++;
				prd = strchr(pr, '"');
				if (prd) {
					prd[0] = { 0 };
				}
				//UTF8字符转换；
				int utf8len = MultiByteToWideChar(CP_UTF8, 0, pr, -1, NULL, 0);
				MultiByteToWideChar(CP_UTF8, 0, pr, -1, Data, utf8len);
				int wlen = WideCharToMultiByte(CP_ACP, 0, Data, -1, NULL, 0, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, Data, -1, pr, utf8len, NULL, NULL);
				// 将结果加入ComboBox列表OP
				Combo_OP.AddString(Data);
				
			}
		}
		pr = strtok(NULL, "\r\n");
	}
	Combo_OP.SetCurSel(0);
	fclose(file);
}
//初始化编辑框内容
void CLeft4Dead2PluginsDlg::InitEditInfo() {
	WCHAR GamePath[1024] = { 0 };
	Edit_ADDOP.SetWindowTextW(L"请输入管理员名字");
#pragma region 获取游戏目录
	GetPrivateProfileString(L"Path", L"GamePath", NULL, GamePath, sizeof(GamePath), ConfigPath);
	if (wcslen(GamePath)>10) {
		Edit_GamePath.SetWindowTextW(GamePath);
		int slen = Edit_GamePath.GetWindowTextLengthW();
		Edit_GamePath.SetSel(slen, slen, TRUE);
		GetDlgItem(IDC_LIST)->SetFocus();
	}
	else {
		//获取注册表
		WCHAR GamePathKey[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 550\\";
		//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 550
		HKEY hKey;
		int Rkey = RegOpenKeyEx(HKEY_LOCAL_MACHINE, GamePathKey, NULL, KEY_WOW64_64KEY | KEY_READ, &hKey);
		if (Rkey != ERROR_SUCCESS) {
			Edit_GamePath.SetWindowTextW(L"请选择游戏目录");
		}
		else {
			BYTE KeyData[1024] = { 0 };
			DWORD DataSize = 1024;
			int Rread = RegQueryValueEx(hKey, L"InstallLocation", NULL, NULL, KeyData, &DataSize);
			if (Rread != ERROR_SUCCESS) {
				Edit_GamePath.SetWindowTextW(L"请选择游戏目录");
			}
			else {
				Edit_GamePath.SetWindowTextW((LPCTSTR)KeyData);
				WritePrivateProfileString(L"Path", L"GamePath", (LPCTSTR)KeyData, ConfigPath);
				int slen = Edit_GamePath.GetWindowTextLengthW();
				Edit_GamePath.SetSel(slen, slen, TRUE);
				GetDlgItem(IDC_LIST)->SetFocus();
			}
		}
	}
#pragma endregion
#pragma region 获取Steam目录
	GetPrivateProfileString(L"Path", L"SteamPath", NULL, SteamPath, sizeof(SteamPath), ConfigPath);
	if (wcslen(SteamPath) < 5) {
		//获取Steam目录
		WCHAR *SteamPathKey = L"SOFTWARE\\Classes\\steam\\Shell\\Open\\Command";
		//HKEY_LOCAL_MACHINE\SOFTWARE\Classes\steam\Shell\Open\Command
		HKEY hKey;
		int Rkey = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SteamPathKey, NULL, KEY_WOW64_64KEY | KEY_READ, &hKey);
		WCHAR KeyData[1024] = { 0 }, *wp = nullptr;
		DWORD DataSize = 1024;
		int Rread = RegQueryValueEx(hKey, NULL, NULL, NULL, (BYTE*)KeyData, &DataSize);
		if (KeyData[0] == '\"') {
			wp = (WCHAR*)((char*)KeyData + (sizeof(WCHAR)));
			wp = wcschr(wp, '\"');
			ZeroMemory(wp, 1);
			//使用steam启动游戏
			wp = (WCHAR*)((char*)KeyData + (sizeof(WCHAR)));
			WritePrivateProfileString(L"Path", L"SteamPath", (LPCTSTR)wp, ConfigPath);
			CopyMemory(SteamPath, wp, (wcslen(wp)*(sizeof(WCHAR))));
		}
	}
#pragma endregion

}
// 获取程序启动目录
BOOL CLeft4Dead2PluginsDlg::GetFileRunPath(WCHAR *PathBuffer, DWORD BufLen) {
	WCHAR ModuleFile[1024] = { 0 }, *pc;
	GetModuleFileName(NULL, PathBuffer, BufLen);
	pc = wcsrchr(PathBuffer, L'\\');
	if (!pc) {
		return FALSE;
	}
	else {
		ZeroMemory(pc, wcslen(pc));
		return TRUE;
	}
}
//初始化列表框
void CLeft4Dead2PluginsDlg::InitListInfo() {
	WCHAR Temp[1024] = { 0 };
	if (!GetFileRunPath(Temp, 1024)) {
		return;
	}
	WIN32_FIND_DATA FindData;
	wcscat(Temp, L"\\Mods");
	CreateDirectory(Temp, NULL);
	wcscat(Temp, L"\\*.zip");
	HANDLE hFind = FindFirstFile(Temp, &FindData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}
	Imagelist.Create(25, 25, ILC_COLOR32, 2, 2);
	Imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_No));
	Imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_Yes));
	List_PluginItems.SetImageList(&Imagelist, LVSIL_NORMAL);
	do
	{
		if (wcscmp(FindData.cFileName, L".") != 0 && wcscmp(FindData.cFileName, L"..") != 0) {
			WCHAR Plugin[125] = { 0 };
			GetPrivateProfileString(L"Plugin", FindData.cFileName, NULL, Plugin, 125, ConfigPath);
			if (wcscmp(Plugin, L"true") == 0) {
				List_PluginItems.InsertItem(FindData.cFileName, 1, 1, NULL);
			}
			else {
				List_PluginItems.InsertItem(FindData.cFileName, 0, 0, NULL);
			}
		}
	} while (FindNextFile(hFind, &FindData));
}

BOOL CLeft4Dead2PluginsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//初始化
	GetFileRunPath(ConfigPath, sizeof(ConfigPath));
	wcscat(ConfigPath, L"\\Config.ini");
	//初始化编辑框内容
	InitEditInfo();
	InitListInfo();
	InitComboBox();
	


	// TODO: 在此添加额外的初始化代码

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLeft4Dead2PluginsDlg::OnPaint()
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
void CLeft4Dead2PluginsDlg::OnDestroy() {
	WCHAR ConfigPath[1024] = { 0 };
	if (!GetFileRunPath(ConfigPath, 1024)) {
		return;
	}
	wcscat(ConfigPath, L"\\Config.ini");
	WCHAR WriteKeyData[1024] = { 0 };
	Edit_GamePath.GetWindowTextW(WriteKeyData, 1024);
	WritePrivateProfileString(L"Path", L"GamePath", WriteKeyData, ConfigPath);

}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLeft4Dead2PluginsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//浏览游戏目录
void CLeft4Dead2PluginsDlg::OnBnClickedButton1()
{
	TCHAR szBuffer[1024] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("请选择游戏目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (!idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	Edit_GamePath.SetWindowTextW(szBuffer);
}

//右键菜单
void CLeft4Dead2PluginsDlg::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	CMenu Menu;
	Menu.LoadMenuW(IDR_MENU1);
	CMenu *pContNextMenu = Menu.GetSubMenu(0);
	CPoint mp;
	GetCursorPos(&mp);
	pContNextMenu->TrackPopupMenu(TPM_LEFTALIGN, mp.x, mp.y, AfxGetMainWnd());
}

//激活插件
void CLeft4Dead2PluginsDlg::OnMenuPluginActivate()
{
	Progress.SetRange(0, 100);
	WCHAR ZipPath[1024] = { 0 }, UnZipPath[1024] = { 0 }, Temp[1024] = { 0 };
	UINT UnZipPathlen = 0;
	CString ItemText;
	ItemText = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
	if (wcsstr(ItemText, L"插件平台")) {
		GetPrivateProfileString(L"Plugin", L"Sourcemod", NULL, ZipPath, 1024, ConfigPath);
		if (!wcscmp(ZipPath, L"true")) {
			if (!Close_Bol) {
				MessageBox(L"插件平台已开启，请勿重复使用！");
				return;
			}
		}
	}
	GetFileRunPath(ZipPath, 1024);
	Edit_GamePath.GetWindowTextW(UnZipPath,1024);
	wcscat(ZipPath, L"\\Mods\\");
	CopyMemory(Temp, ZipPath, 1024);
	wcscat(ZipPath, ItemText);
	HZIP hzp = OpenZip(ZipPath, NULL);
	SetUnzipBaseDir(hzp, Temp);
	if (!hzp) {
		CString Tip;
		Tip.Format(L"文件:[%s]\r\n有密码或zip文件损害！", ZipPath);
		MessageBox(Tip);
		return;
	}
	ZIPENTRY etp = { 0 };
	GetZipItem(hzp, -1, &etp);
	int num = etp.index;
	if (num == 0) {
		CString Tip, ItemT;
		ItemT = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
		Tip.Format(L"插件:[%s]文件无封装文件，安装失败！", ItemT);
		MessageBox(Tip);
		return;
	}
	Progress.SetPos(10);
	UnZipPathlen = wcslen(UnZipPath);
	DWORD UnZipStyle = 0;
	WCHAR TempPath[1024] = { 0 }, *pchr;
	CopyMemory(TempPath, UnZipPath, sizeof(UnZipPath));
	//确认根目录文件是否为插件
	for (int i = 0; i < num; i++) {
		GetZipItem(hzp, i, &etp);
		if (!wcscmp(etp.name, L"left4dead2/") || !wcscmp(etp.name, L"LEFT4DEAD2/")) {
			SetUnzipBaseDir(hzp, TempPath);
			UnZipStyle = 0x1;
			break;
		}
		else if (!wcscmp(etp.name, L"addons/") || !wcscmp(etp.name, L"ADDONS/")) {
			wcscat(TempPath, L"\\left4dead2");
			SetUnzipBaseDir(hzp, TempPath);
			UnZipStyle = 0x2;
			break;
		}
		else if (!wcscmp(etp.name, L"scripts/") || !wcscmp(etp.name, L"SCRIPTS/")) {
			wcscat(TempPath, L"\\left4dead2");
			SetUnzipBaseDir(hzp, TempPath);
			UnZipStyle = 0x2E;
			break;
		}
		else {
			UnZipStyle = 0x9EB;
		}
	}
	Progress.SetPos(30);
	//确认文件是否为插件
	if (UnZipStyle == 0x9EB) {
		DWORD Flag = FALSE;
		for (int i = 0; i < num; i++) {
			GetZipItem(hzp, i, &etp);
			if (wcsstr(etp.name, L"left4dead2/") || wcsstr(etp.name, L"LEFT4DEAD2/")) {
				Flag = TRUE;
				break;
			}
			else if (wcsstr(etp.name, L"addons/") || wcsstr(etp.name, L"addons/")) {
				Flag = TRUE;
				break;
			}
			else if (wcsstr(etp.name, L"scripts/") || wcsstr(etp.name, L"SCRIPTS/")) {
				Flag = TRUE;
				break;
			}
			else if (wcsstr(etp.name, L".nut") || wcsstr(etp.name, L".NUT")) {
				Flag = TRUE;
				break;
			}
			else {
				Flag = FALSE;
			}
		}
		if (Flag == TRUE) {
			CString Tip, ItemT;
			ItemT = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
			Tip.Format(L"我在文件:[%s]找到与游戏插件相关的信息\r\n但我并不知如何去区分它,也许你应该把它整理一下？", ItemT);
			MessageBox(Tip);
			Progress.SetPos(0);
			return;
		}
	}
	Progress.SetPos(40);
	//寻找文件有无插件;
	if (UnZipStyle == 0x9EB) {
		for (int i = 0; i < num; i++) {
			GetZipItem(hzp, i, &etp);
			if (wcsstr(etp.name, L".smx") || wcsstr(etp.name, L".SMX")) {
				UnZipStyle = 0xFF;
				break;
			}
			else {
				UnZipStyle = 0x9EB;
			}
		}
	}
	if (UnZipStyle == 0x9EB) {
		MessageBox(L"未找到插件信息，请确认是否为插件包！");
		Progress.SetPos(0);
		return;
	}
	//激活插件；
	int Count = 0;
	for (int i = 0; i < num; i++) {
		float pos = ((float)i / (float)num) * 100;
		Progress.SetPos((int)pos);
		ZIPENTRY ze = { 0 };
		GetZipItem(hzp, i, &ze);
		OutputDebugString(ze.name);
		OutputDebugString(L"\n");
		//如果是不能直接复制的则解压插件文件；
		if (UnZipStyle == 0xFF) {
			//GetFileRunPath(Temp, 1024);
			GetTempPath(1024, Temp);
			wcscat(Temp, L"Mods\\Temp");
			SHCreateDirectoryEx(NULL,Temp, NULL);
			WCHAR NewF[1024] = { 0 }, TmpPath[1024] = { 0 };
			//CString NewF;
			SetUnzipBaseDir(hzp, Temp);
			CopyMemory(TmpPath, Temp, 1024);
			if (wcsstr(ze.name, L".smx") || wcsstr(ze.name, L".SMX")) {
				UnzipItem(hzp, i, ze.name);
				pchr = wcsrchr(ze.name, '/');
				if (pchr) {
					do {
						pchr[0] = '\\';
					} while (pchr = wcsrchr(ze.name, '/'));
				}
				wcscat(Temp, L"\\");
				wcscat(Temp, ze.name);
				wcscat(NewF, UnZipPath);
				wcscat(NewF, L"\\left4dead2\\addons\\sourcemod\\plugins");
				SHCreateDirectoryEx(NULL, NewF, NULL);
				pchr = wcsrchr(ze.name, '\\');
				if (pchr) {
					wcscat(NewF, pchr);
				}
				else
				{
					wcscat(NewF, L"\\");
					wcscat(NewF, ze.name);
				}
				CString Tip, ItemT, KeyBo;
				ItemT = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
				if (!CopyFile(Temp, NewF, FALSE)) {
					Tip.Format(L"插件:[%s]\r\n安装失败,请确认程序是否有权限操作文件！", ItemT);
					MessageBox(Tip);
					Progress.SetPos(0);
					return;
				}
				Count++;
				KeyBo.Format(L"%d", Count);
				WritePrivateProfileString(L"PluginPath", ItemT, KeyBo, ConfigPath);
				KeyBo.Format(L"%s_FDC_%d", ItemT, Count);
				WritePrivateProfileString(L"PluginPath", KeyBo, NewF, ConfigPath);
				DeleteFile(Temp);
			}
			else if(wcsstr(ze.name, L".cfg") || wcsstr(ze.name, L".cfg")) {
				UnzipItem(hzp, i, ze.name);
				pchr = wcsrchr(ze.name, '/');
				if (pchr) {
					do {
						pchr[0] = '\\';
					} while (pchr = wcsrchr(ze.name, '/'));
				}
				wcscat(Temp, L"\\");
				wcscat(Temp, ze.name);
				wcscat(NewF, UnZipPath);
				wcscat(NewF, L"\\left4dead2\\cfg\\sourcemod");
				SHCreateDirectoryEx(NULL, NewF, NULL);
				pchr = wcsrchr(ze.name, '\\');
				if (pchr) {
					wcscat(NewF, pchr);
				}
				else
				{
					wcscat(NewF, L"\\");
					wcscat(NewF, ze.name);
				}
				CString Tip, ItemT, KeyBo;
				ItemT = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
				if (!CopyFile(Temp, NewF, FALSE)) {
					Tip.Format(L"插件配置:[%s]\r\n安装失败,请确认程序是否有权限操作文件！", ItemT);
					MessageBox(Tip);
					Progress.SetPos(0);
					return;
				}
				Count++;
				KeyBo.Format(L"%d", Count);
				WritePrivateProfileString(L"PluginPath", ItemT, KeyBo, ConfigPath);
				KeyBo.Format(L"%s_FDC_%d", ItemT, Count);
				WritePrivateProfileString(L"PluginPath", KeyBo, NewF, ConfigPath);
				DeleteFile(Temp);
			}
		}
		else{
			CString ItemTr, KeyBo,Path;
			ItemTr = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
			Count++;
			UnzipItem(hzp, i, ze.name);
			pchr = wcsrchr(ze.name, '/');
			if (pchr) {
				do {
					pchr[0] = '\\';
				} while (pchr = wcsrchr(ze.name, '/'));
			}
			KeyBo.Format(L"%d", Count);
			WritePrivateProfileString(L"PluginPath", ItemTr, KeyBo, ConfigPath);
			KeyBo.Format(L"%s_Default_%d", ItemTr, Count);
			Path.Format(L"%s\\%s", TempPath, ze.name);
			WritePrivateProfileString(L"PluginPath", KeyBo, Path, ConfigPath);

		}
	}
	CloseZip(hzp);
	Progress.SetPos(100);
	CString Tip,ItemT;
	ItemT = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
	Tip.Format(L"插件:[%s]\r\n安装成功！", ItemT);
	WritePrivateProfileString(L"Plugin", ItemT, L"true", ConfigPath);
	if (wcsstr(ItemT, L"插件平台")) {
		InitComboBox();
		WritePrivateProfileString(L"Plugin", L"Sourcemod", L"true", ConfigPath);
	}
	List_PluginItems.SetItemImage(List_PluginItems.GetSelectedItem(), 1, 1);
	MessageBox(Tip);
	Progress.SetPos(0);
}
//关闭插件
void CLeft4Dead2PluginsDlg::OnMenuPluginClose()
{
	WCHAR Temp[1024] = { 0 }, TempText[256] = { 0 };
	CString Tip, ItemT,GamePath;
	ItemT = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
	GetPrivateProfileString(L"Plugin", ItemT, NULL, TempText, 1024, ConfigPath);
	if (wcscmp(TempText, L"true")) {
		if (!Close_Bol) {
			Tip.Format(L"插件:[%s]没有激活", ItemT);
			MessageBox(Tip);
			return;
		}
	}
	GetPrivateProfileString(L"PluginPath", ItemT, NULL, TempText, 1024, ConfigPath);
	int innt = 0;
	swscanf(TempText, L"%d", &innt);
	if (innt <= 0) {
		if (!Close_Bol) {
			Tip.Format(L"插件:[%s]没有找到对应的文件信息!", ItemT);
			MessageBox(Tip);
			return;
		}
		else{
			return;
		}
	}
	for (int i = 0; i < innt; i++) {
		Progress.SetPos((int)((float)i / (float)innt) * 100);
		//删除标准解压文件
		Tip.Format(L"%s_Default_%d", ItemT, (i + 1));
		GetPrivateProfileString(L"PluginPath", Tip, NULL, Temp, 1024, ConfigPath);
		DeleteFile(Temp);
		WritePrivateProfileString(L"PluginPath", Tip, NULL, ConfigPath);
		//删除文件计数
		WritePrivateProfileString(L"PluginPath", ItemT, NULL, ConfigPath);
		//删除自定义解压文件
		Tip.Format(L"%s_FDC_%d", ItemT, (i + 1));
		GetPrivateProfileString(L"PluginPath", Tip, NULL, Temp, 1024, ConfigPath);
		DeleteFile(Temp);
		WritePrivateProfileString(L"PluginPath", Tip, NULL, ConfigPath);
	}
	if (Close_Bol) {
		Close_Bol = FALSE;
		return;
	}
	Progress.SetPos(100);
	List_PluginItems.SetItemImage(List_PluginItems.GetSelectedItem(), 0, 0);
	WritePrivateProfileString(L"Plugin", ItemT, NULL, ConfigPath);
	WritePrivateProfileString(L"Plugin", L"Sourcemod", L"false", ConfigPath);
	InitComboBox();
	Tip.Format(L"插件:[%s]已关闭激活", ItemT);
	MessageBox(Tip);
	Progress.SetPos(0);
	return;
}
//删除插件文件
void CLeft4Dead2PluginsDlg::OnMenuPluginDelete()
{
	if (MessageBox(L"删除操作会将本体也删除，是否继续执行？", L"询问?", MB_ICONWARNING | MB_YESNO) != IDYES) {
		return;
	}
	Close_Bol = TRUE;
	OnMenuPluginClose();
	WCHAR Path[1024] = { 0 };
	GetFileRunPath(Path, 1024);
	wcscat(Path, L"\\Mods\\");
	CString ItemI,File;
	ItemI = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
	File.Format(L"%s%s", Path, ItemI);
	if (!DeleteFile(File)) {
		MessageBox(L"插件文件删除失败，请确认程序是否有权限！");
		return;
	}
	OnMenuRefrsh();
	MessageBox(L"插件文件删除成功！");
}
//打开插件配置文件
void CLeft4Dead2PluginsDlg::OnMenuPluginConfig()
{
	WCHAR Temp[1024] = { 0 }, Text[256] = { 0 };
	int Num = 0, Cou = 0;
	CString ItemI,Keyr;
	ItemI = List_PluginItems.GetItemText(List_PluginItems.GetSelectedItem());
	GetPrivateProfileString(L"Plugin", ItemI, NULL, Text, 1024, ConfigPath);
	if (wcscmp(Text, L"true")) {
		Keyr.Format(L"插件:[%s]没有激活", ItemI);
		MessageBox(Keyr);
		return;
	}
	GetPrivateProfileString(L"PluginPath", ItemI, NULL, Text, 256, ConfigPath);
	swscanf(Text, L"%d", &Num);
	BOOL IsOpenCfg = FALSE;
	for (int i = 0; i < Num; i++) {
		Keyr.Format(L"%s_FDC_%d", ItemI, (i + 1));
		GetPrivateProfileString(L"PluginPath", Keyr, NULL, Temp, 1024, ConfigPath);
		if (wcsstr(Temp, L".cfg") || wcsstr(Temp, L".CFG")) {
			Cou++;
			if (Cou > 2 && IsOpenCfg == FALSE) {
				IsOpenCfg = TRUE;
				if (MessageBox(L"该项插件配置文件很多是否确定打开后续配置文件？", NULL, MB_ICONWARNING | MB_YESNO) == IDNO) {
					break;
				}
			}
			ShellExecute(NULL, L"open", Temp, NULL, NULL, SW_SHOWNORMAL);
		}
		Keyr.Format(L"%s_Default_%d", ItemI, (i + 1));
		GetPrivateProfileString(L"PluginPath", Keyr, NULL, Temp, 1024, ConfigPath);
		if (wcsstr(Temp, L".cfg") || wcsstr(Temp, L".CFG")) {
			Cou++;
			if (Cou > 2 && IsOpenCfg == FALSE) {
				IsOpenCfg = TRUE;
				if (MessageBox(L"该项插件配置文件很多是否确定打开后续配置文件？", NULL, MB_ICONWARNING | MB_YESNO) == IDNO) {
					break;
				}
			}
			ShellExecute(NULL, L"open", Temp, NULL, NULL, SW_SHOWNORMAL);
		}
	}
}
//刷新插件列表
void CLeft4Dead2PluginsDlg::OnMenuRefrsh()
{
	WCHAR Temp[1024] = { 0 };
	if (!GetFileRunPath(Temp, 1024)) {
		return;
	}
	List_PluginItems.DeleteAllItems();
	WIN32_FIND_DATA FindData;
	wcscat(Temp, L"\\Mods");
	CreateDirectory(Temp, NULL);
	wcscat(Temp, L"\\*.zip");
	HANDLE hFind = FindFirstFile(Temp, &FindData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}
	do
	{
		if (wcscmp(FindData.cFileName, L".") != 0 && wcscmp(FindData.cFileName, L"..") != 0) {
			WCHAR Plugin[125] = { 0 };
			GetPrivateProfileString(L"Plugin", FindData.cFileName, NULL, Plugin, 125, ConfigPath);
			if (wcscmp(Plugin, L"true") == 0) {
				List_PluginItems.InsertItem(FindData.cFileName, 1, 1, NULL);
			}
			else {
				List_PluginItems.InsertItem(FindData.cFileName, 0, 0, NULL);
			}
		}
	} while (FindNextFile(hFind, &FindData));
}
//编辑框获取焦点,清除文本
void CLeft4Dead2PluginsDlg::OnEnSetfocusEdit2()
{
	WCHAR Text[256] = { 0 };
	Edit_ADDOP.GetWindowTextW(Text, 256);
	if (wcsstr(Text, L"请输入管理员名字")) {
		Edit_ADDOP.SetWindowTextW(NULL);
	}
}
//打开配置文件目录
void CLeft4Dead2PluginsDlg::OnMenuOpenConfigPath()
{
	CString Path;
	Edit_GamePath.GetWindowTextW(Path);
	Path.Append(L"\\left4dead2\\cfg\\sourcemod");
	ShellExecute(NULL, L"open", Path, NULL, NULL, SW_SHOWNORMAL);
}
//宽字符转大写
void CLeft4Dead2PluginsDlg::ToWUpperStr(WCHAR *des, WCHAR*source) {
	int slen = wcslen(source);
	DWORD Project;
	//修改内存地址为读写属性
	VirtualProtect(des, slen * sizeof(WCHAR), PAGE_READWRITE, &Project); 
	for (int i = 0; i < slen; i++)
	{
		des[i] = towupper(source[i]);//转换大写
	}
	//还原原始内存属性
	VirtualProtect(des, slen * sizeof(WCHAR), Project, &Project);
}
//判断进程是否存在
BOOL CLeft4Dead2PluginsDlg::IsProcessExist(WCHAR *ProcessName) {
	PROCESSENTRY32 prentry32 = { sizeof(PROCESSENTRY32) };
	HANDLE TlSnwp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	BOOL IsExist = FALSE;
	if (TlSnwp == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	if (!Process32First(TlSnwp, &prentry32)) {
		return FALSE;
	}
	do 
	{
		ToWUpperStr(ProcessName, ProcessName);
		ToWUpperStr(prentry32.szExeFile, prentry32.szExeFile);
		if (wcsstr(prentry32.szExeFile, ProcessName)) {
			IsExist = TRUE;
			break;
		}
	} while (Process32Next(TlSnwp,&prentry32));
	return IsExist;
}
//启动游戏
void CLeft4Dead2PluginsDlg::OnBnClickedButton2()
{
	CString Path;
	Edit_GamePath.GetWindowTextW(Path);
	Path.Append(L"\\left4dead2.exe");
	if (!PathFileExists(Path)) {
		MessageBox(L"目录中无法找到游戏文件[left4dead2.exe],请确认目录是否正确！");
		return;
	}
	WCHAR Keyr[256] = { 0 };
	GetPrivateProfileString(L"Plugin", L"RunStyle", NULL, Keyr, 256, ConfigPath);//获取是否正版启动方式
	//正版启动方式
	if (!wcscmp(Keyr, L"Steam")) {
		if (!IsProcessExist(L"steam.exe")) {
			if (!PathFileExists(SteamPath)) {
				MessageBox(L"[Steam]目录获取失败,请手动打开[Steam]后重试!");
				return;
			}
			ShellExecute(NULL, L"open", (LPCWCHAR)SteamPath, L"-applaunch 550 -insecure", NULL, SW_SHOWNORMAL);
			return;
		}
		ShellExecute(NULL, L"open", Path, L"-steam -insecure", NULL, SW_SHOWNORMAL);
	}
	//盗版启动方式
	else {
		if (!wcscmp(Keyr, L"Other")) {
			ShellExecute(NULL, L"open", Path, L"-insecure", NULL, SW_SHOWNORMAL);
		}
		else {
			//询问是否使用正版启动方式启动游戏
			if (MessageBox(L"你是希望使用正版启动方式，启动游戏吗？", L"Tip:", MB_ICONINFORMATION | MB_YESNO) == IDYES) {
				WritePrivateProfileString(L"Plugin", L"RunStyle", L"Steam", ConfigPath);
				if (!IsProcessExist(L"steam.exe")) {
					if (!PathFileExists(SteamPath)) {
						MessageBox(L"[Steam]目录获取失败,请手动打开[Steam]后重试!");
						return;
					}
					ShellExecute(NULL, L"open", (LPCWCHAR)SteamPath, L"-applaunch 550 -insecure", NULL, SW_SHOWNORMAL);
					return;
				}
				ShellExecute(NULL, L"open", Path, L"-steam -insecure", NULL, SW_SHOWNORMAL);
			}
			else {
				WritePrivateProfileString(L"Plugin", L"RunStyle", L"Other", ConfigPath);
				ShellExecute(NULL, L"open", Path, L"-insecure", NULL, SW_SHOWNORMAL);
			}
		}
	}
	
}

//添加管理员;
void CLeft4Dead2PluginsDlg::OnBnClickedButton3()
{

	FILE *file;
	CString GamePath;
	CHAR Temp[256] = { 0 },vTemp[512] = { 0 };
	WCHAR Data[1024] = { 0 };
	Edit_GamePath.GetWindowTextW(GamePath);
	GamePath.Append(L"\\left4dead2\\addons\\sourcemod\\configs\\admins_simple.ini");
	WideCharToMultiByte(CP_ACP, 0, GamePath, -1, Temp, 256, NULL, 0);
	file = fopen(Temp, "ab");
	if (!file) {
		Combo_OP.AddString(L"插件未安装或无法读取配置文件");
		Combo_OP.SetCurSel(0);
	}
	fseek(file, 0, SEEK_END);
	int Flen = ftell(file);
	Edit_ADDOP.GetWindowTextW(GamePath);
	//判断是否是SteamID,因为SteamID不需要密码
	if (wcsstr(GamePath, L"STEAM_") != NULL || wcsstr(GamePath, L"steam_") != NULL) {
		WideCharToMultiByte(CP_ACP, 0, GamePath, -1, Temp, 256, NULL, 0);
		sprintf(vTemp, "\r\n\"%s\" \"99:z\"", Temp);
	}
	else {
		WideCharToMultiByte(CP_ACP, 0, GamePath, -1, Temp, 256, NULL, 0);
		sprintf(vTemp, "\r\n\"%s\" \"99:z\" \"1234567890\"", Temp);
	}
	//标准字符转换UTF8字符
	int Len = strlen(vTemp);
	int iLen = MultiByteToWideChar(CP_ACP, 0, vTemp, Len, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, vTemp, Len, Data, iLen);
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, Data, iLen, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, Data, iLen, vTemp, utf8Len, NULL, NULL);
	//将结果写入管理员文件[admins_simple.ini]
	fwrite(vTemp, utf8Len, 1, file);
	fclose(file);
	Edit_GamePath.GetWindowTextW(GamePath);
	GamePath.Append(L"\\left4dead2\\cfg\\autoexec.cfg");
	WideCharToMultiByte(CP_ACP, 0, GamePath, -1, Temp, 256, NULL, 0);
	file = fopen(Temp, "wb");
	if (!file) {
		Combo_OP.AddString(L"平台未安装");
		Combo_OP.SetCurSel(0);
	}
	fseek(file, 0, SEEK_END);
	Flen = ftell(file);
	void *Rvbuff = LocalAlloc(LMEM_ZEROINIT, Flen);
	if (!Rvbuff) {
		MessageBox(L"分配内存失败!", NULL, 0);
		return;
	}
	fseek(file, 0, SEEK_SET);
	fread(Rvbuff, Flen, 1, file);
	Edit_ADDOP.GetWindowTextW(GamePath);
	WideCharToMultiByte(CP_ACP, 0, GamePath, -1, Temp, 256, NULL, 0);
	sprintf(vTemp, "setinfo _admin 1234567890\r\n");
	//标准字符转换UTF8字符
	Len = strlen(vTemp);
	iLen = MultiByteToWideChar(CP_ACP, 0, vTemp, Len, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, vTemp, Len, Data, iLen);
	utf8Len = WideCharToMultiByte(CP_UTF8, 0, Data, iLen, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, Data, iLen, vTemp, utf8Len, NULL, NULL);
	//将结果写入管理员文件[autoexec.cfg]
	fwrite(vTemp, utf8Len, 1, file);
	fclose(file);
	InitComboBox();
	MessageBox(L"添加管理员成功！");
}

