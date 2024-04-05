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
	int five_fold_value;
	CStatic five_fold;
	int ten_fold_value;
	CStatic ten_fold;
	int LOO_value;
	CStatic LOO;

	int linearClassificationSelection = 0; //   0: Training Validation, 1: 5 fold, 2: 10fold, 3: LOO

	int training_validation;
	int cross_validation;
	afx_msg void OnBnClickedCross();
	afx_msg void OnBnClickedTraining();
	
	CStatic group_cross_validation;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedTenFold();
};
