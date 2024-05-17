#pragma once
#include "afxdialogex.h"


// CLinearClassificationMulticlassFDA dialog

class CLinearClassificationMulticlassFDA : public CDialogEx
{
	DECLARE_DYNAMIC(CLinearClassificationMulticlassFDA)

public:
	CLinearClassificationMulticlassFDA(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLinearClassificationMulticlassFDA();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FDA_MULTICLASS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double FractionTraining;
	int NumberRuns;
	int ValidationMethod;
	int ClassificationMethod;
	afx_msg void OnBnClickedRadioTrainValidationFdaMulticlass();
	afx_msg void OnBnClickedRadio5FoldCrossvalidationFdaMulticlass();
	afx_msg void OnBnClickedRadio10FoldCrossvalidationFdaMulticlass();
	afx_msg void OnBnClickedRadioMonteCarloFdaMulticlass();
	CStatic FractionText;
	CStatic NumberRunsText;
	CEdit FractionInput;
	CEdit NumberRunsInput;
};
