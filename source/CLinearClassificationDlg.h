#pragma once
#include "afxdialogex.h"

// CLinearClassificationDlg dialog

class CLinearClassificationDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinearClassificationDlg)

public:
	CLinearClassificationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLinearClassificationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LINEAR_CLASSIFICATION};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
