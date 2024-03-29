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
public:
	CButton five_fold;
	CButton ten_fold;
	CButton LOO;
	int linearClassificationSelection = 0; //   0: Training Validation, 1: 5 fold, 2: 10fold, 3: LOO

	CButton training_validation;
	CButton cross_validation;
	afx_msg void OnBnClickedCross();
	afx_msg void OnBnClickedTraining();
	
	CStatic group_cross_validation;
	afx_msg void OnBnClickedOk();
};
