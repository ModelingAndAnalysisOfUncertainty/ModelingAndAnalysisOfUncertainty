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

	
	CButton cross_validation;
	CButton training_validation;
	afx_msg void OnBnClickedRadioCrossValidation();
	afx_msg void OnBnClickedRadioTrainingValidation();
	CButton five_fold;
	CButton ten_fold;
	CButton LOO;
	CButton box_cross_validation;
};
