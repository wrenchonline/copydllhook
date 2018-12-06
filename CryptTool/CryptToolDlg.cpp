
// CryptToolDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CryptTool.h"
#include "CryptToolDlg.h"
#include "afxdialogex.h"
#include "m_CEdit.h"
#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char comCmd[255] = { 0 };
char  Filepullpath[2048] = { 0 };
char  test[2048] = { 0 };
char  ptmpBuffer[250];
char   Ext[250];
char  pBuffer[250];
CString temp_desktop_path;
void OutputDebugStringEx(const char *strOutputString, ...)

{

	va_list vlArgs = NULL;

	va_start(vlArgs, strOutputString);

	size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;

	char *strBuffer = new char[nLen];

	_vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);

	va_end(vlArgs);

	OutputDebugStringA(strBuffer);

	delete[] strBuffer;

}

DWORD  ThreadProc(LPVOID pParam)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW;  // 指定wShowWindow成员有效
	si.wShowWindow = TRUE;          // 此成员设为TRUE的话则显示新建进程的主窗口，
						// 为FALSE的话则不显示
	BOOL bRet = CreateProcess(
		NULL,           // 不在此指定可执行文件的文件名
		(LPSTR)pParam,
		NULL,           // 默认进程安全性
		NULL,           // 默认线程安全性
		FALSE,          // 指定当前进程内的句柄不可以被子进程继承
		CREATE_NEW_CONSOLE, // 为新进程创建一个新的控制台窗口
		NULL,           // 使用本进程的环境变量
		NULL,           // 使用本进程的驱动器和目录
		&si,
		&pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCryptToolDlg 对话框



CCryptToolDlg::CCryptToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CRYPTTOOL_DIALOG, pParent)
	, nyear(0)
	, nMonth(0)
	, nDay(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCryptToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_data_for_end);
	DDX_Control(pDX, IDC_EDIT4, m_edit);
	DDX_Control(pDX, IDC_EDIT5, use_count);
}

BEGIN_MESSAGE_MAP(CCryptToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CCryptToolDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCryptToolDlg 消息处理程序

BOOL CCryptToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	//处理管理员权限的信息
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);
	m_dateCtrl.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	m_data_for_end.SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	use_count.SetText("10");//默认是10次
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCryptToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCryptToolDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCryptToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//加密打包
void CCryptToolDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//EnableWindow(FALSE);


	INT eof = -1;
	size_t iflag = 0;
	size_t len = sizeof(RjFileSrtuct);


	ZeroMemory(Filepullpath, 2048);
	GetTempPathA(2048, Filepullpath);

	CTime m_date;//获取外发时间
	CTime m_date_End;//获取到期时间
	CString Use_Count;//获取使用次数
	CString FilePath;//获取文件路径
	UpdateData(TRUE);
	m_dateCtrl.GetTime(m_date);
	CString date = m_date.Format("%Y-%m-%d %H:%M:%S");
	m_data_for_end.GetTime(m_date_End);
	CString date_end = m_date_End.Format("%Y-%m-%d %H:%M:%S");
	UpdateData(FALSE);
	use_count.GetWindowText(Use_Count);
	m_edit.GetWindowText(FilePath);
	if (FilePath.IsEmpty() || date.IsEmpty() || date_end.IsEmpty() || Use_Count.IsEmpty())
	{
		::MessageBox(NULL, "参数填写不全", "错误提示：", MB_YESNO | MB_ICONEXCLAMATION);
		return;
	}
	encryptInfo = std::shared_ptr<rjFileInfo>(new rjFileInfo());
	_splitpath_s(FilePath.GetBuffer(), NULL, 0, NULL, 0, pBuffer, _MAX_FNAME, Ext, _MAX_FNAME);// 得到文件名
	FILE * TEMP = fopen(FilePath.GetBuffer(), "rb+");
	memcpy(ptmpBuffer, pBuffer, 250);
	strcat(pBuffer, Ext); //文件名衔接个后缀名
	strcat(ptmpBuffer, ".rjs"); //文件名衔接个后缀名
	//拼装临时文件的路径。
	strcat(Filepullpath, ptmpBuffer);
	//临时文件，也就是加密后的文件，现在我们创建它。
	FILE * TEMP1 = fopen(Filepullpath, "wb+");
	//so，I make out struct encrypt  information of encrypt file.
	fseek(TEMP, 0, SEEK_END);   //指针：移动到文件尾部
	encryptInfo->encryptHead.length = ftell(TEMP); //获取原文件大小
	memcpy(encryptInfo->encryptHead.FileHeadName, FileName, sizeof(FileName));
	memcpy(encryptInfo->encryptHead.FileSrcName, pBuffer, 60);//填写原文件名
	encryptInfo->encryptHead.Count = _ttoi(Use_Count); //写入文件使用次数
	encryptInfo->encryptHead.onlyread = 1;
	encryptInfo->encryptHead.forbidensaveas = 1;
	memcpy(encryptInfo->encryptHead.Outgoingfiletime.startuserTime, date.GetBuffer(), 20);
	memcpy(encryptInfo->encryptHead.Outgoingfiletime.stopuserTime, date_end.GetBuffer(), 20);
	// Here we are encrypt file ,so i make encrypt Buffer. 

	//从结构体头开始复制，已经是1字节对齐了
	while (iflag <= len)
	{
		fwrite(encryptInfo->encryptHead.FileHeadName + iflag, 1, 1, TEMP1);//这个encryptInfo->encryptHead.FileHeadName指针写法不太规范,一般懂汇编和C的编程设计者容易理解，也没用原始指针。
		iflag++;
	}
	char*  buf = new char[encryptInfo->encryptHead.length];
	ZeroMemory(buf, (int)encryptInfo->encryptHead.length);
	fseek(TEMP, 0, SEEK_SET);//移动到头部
	while (fread(buf, 1, 1, TEMP)) {
		buf[0] ^= 'a';
		fseek(TEMP1, 0, SEEK_END);
		fwrite(buf, 1, 1, TEMP1);
		fflush(TEMP1);
	}
	fseek(TEMP1, 0, SEEK_END);
	fclose(TEMP);
	fclose(TEMP1);
	//Here we are tmpFile had encrypt complete. Deal contraction 
	memcpy(test, GetWorkDir().GetBuffer(), 2048);
	memcpy_s(Filebuffer, 250, ptmpBuffer, 250);
	strcat(test, ptmpBuffer);

	DeleteFileA(test);

	if (!MoveFile(Filepullpath, test)) {
		::MessageBox(NULL, "移动文件失败，请程序员检查目录是否存在", "错误提示", MB_ICONINFORMATION | MB_YESNO);
		DeleteFileA(Filepullpath);
		return;
	}
	//complete packet.
	strcat(Filebuffer, " copydllhook OutGoingFileTool");
	SHGetSpecialFolderPath(0, desktop_path, CSIDL_DESKTOPDIRECTORY, 0);
	temp_desktop_path = CString(desktop_path);
	temp_desktop_path.Replace("\\", "\\\\");
	temp_desktop_path += "\\\\a.rar";
	CompressFile(temp_desktop_path.GetBuffer(), Filebuffer); //这个Rar不支持中文。
	return;
}
int WINAPI CCryptToolDlg::CompressFile(const char* comSavewhere, const char* needCom)//压缩文件
{

	int ret = 0;
	sprintf_s(comCmd, 255, "Rar a -ep %s %s", comSavewhere, needCom);
	AfxBeginThread((AFX_THREADPROC)ThreadProc, comCmd, THREAD_PRIORITY_TIME_CRITICAL);
	return ret;
}
int  CCryptToolDlg::UncompreFile(const char* uncomTowhere, const char* needUncom)//解压文件
{
	char uncomCmd[2048] = { 0 };
	sprintf_s(uncomCmd, "UnRAR -y -p- -u x %s %s", needUncom, uncomTowhere);
	int ret = WinExec(uncomCmd, SW_HIDE);
	free((void *)needUncom);
	return ret;
}

//获取工作路径
CString CCryptToolDlg::GetWorkDir()
{
	char buf[MAX_PATH];
	_fullpath(buf, ".\\", MAX_PATH);
	CString csFullPath(buf);
	return csFullPath;
}