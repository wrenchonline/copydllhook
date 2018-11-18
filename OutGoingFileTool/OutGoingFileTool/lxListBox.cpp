// lxListBox.cpp: 实现文件
//

#include "stdafx.h"
#include "OutGoingFileTool.h"
#include "lxListBox.h"
#include "afxdialogex.h"
char   cFilePathName[250];
char pBuffer[250];
// lxListBox 对话框

IMPLEMENT_DYNAMIC(lxListBox, CListCtrl)

lxListBox::lxListBox(CWnd* pParent /*=nullptr*/)
	: CListCtrl()
{

}

lxListBox::~lxListBox()
{
}

void lxListBox::DoDataExchange(CDataExchange* pDX)
{
	CListCtrl::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(lxListBox, CListCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// lxListBox 消息处理程序
VOID lxListBox::OnDropFiles(HDROP hDropInfo){


	UINT  nNumOfFiles = DragQueryFileA(hDropInfo, 0xFFFFFFFF, NULL, 0); //文件的个数
	//LPCTSTR  fileNamE;
	
	for (UINT nIndex = 0; nIndex < nNumOfFiles; ++nIndex)
	{
		DragQueryFileA(hDropInfo, nIndex, cFilePathName, _MAX_PATH);  //得到文件名
		LPARAM lParam = (LPARAM)cFilePathName;
		USES_CONVERSION;
		_splitpath_s(cFilePathName, NULL, 0, NULL, 0, pBuffer, _MAX_FNAME, NULL, 0);// 得到文件名 
		InsertItem(0, (LPCTSTR)pBuffer);// 在第一列插入文件名
		SetItemText(0, 1, (LPCTSTR)cFilePathName);// 在第二列插入文件路径
	}
	DragFinish(hDropInfo);
}
