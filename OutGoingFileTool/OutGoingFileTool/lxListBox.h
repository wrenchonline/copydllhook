#pragma once


// lxListBox 对话框

#include "FIlestruct.h"
#include <memory>
CString GetWorkDir();
class lxListBox : public CListCtrl
{

	DECLARE_DYNAMIC(lxListBox)

public:

	afx_msg VOID OnDropFiles(HDROP hDropInfo);
	lxListBox(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~lxListBox();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	std::shared_ptr<rjFileInfo> FileInfo;
	BOOL CheackTime(const char* str);
	DECLARE_MESSAGE_MAP()
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OUTGOINGFILETOOL_DIALOG };
#endif

protected:

};
