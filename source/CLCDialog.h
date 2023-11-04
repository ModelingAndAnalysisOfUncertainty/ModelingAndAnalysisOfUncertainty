#pragma once
#include "afxdialogex.h"


// CLCDialog dialog

class CLCDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLCDialog)

public:
	CLCDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLCDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_LayerNodeList;
};
