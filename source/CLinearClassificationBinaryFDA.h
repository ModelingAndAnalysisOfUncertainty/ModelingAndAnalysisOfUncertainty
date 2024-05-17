#pragma once
#include "afxdialogex.h"


// CLinearClassificationSetUp dialog

class CLinearClassificationBinaryFDA : public CDialogEx
{
	DECLARE_DYNAMIC(CLinearClassificationBinaryFDA)

public:
	CLinearClassificationBinaryFDA(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLinearClassificationBinaryFDA();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FDA_BINARY};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int NumberClasses;
	BOOL Method;

	DECLARE_MESSAGE_MAP()
public:
	double FractionTraining;
	int NumberRuns;
	int ValidationMethod;
	afx_msg void OnBnClickedRadioTrainValidationClassificationLinear();
	afx_msg void OnBnClickedRadio5FoldCrossvalidationClassificationLinear();
	afx_msg void OnBnClickedRadio10FoldCrossvalidationClassificationLinear();
	afx_msg void OnBnClickedRadioMonteCarloClassificationLinear();
	CStatic FractionText;
	CEdit FractionInput;
	CStatic NumberRunsText;
	CEdit NumberRunsInput;
};
