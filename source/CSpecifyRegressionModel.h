#pragma once
#include "afxdialogex.h"


// CSpecifyRegressionModel dialog

class CSpecifyRegressionModel : public CDialogEx
{
	DECLARE_DYNAMIC(CSpecifyRegressionModel)

public:
	CSpecifyRegressionModel(CWnd* pParent = nullptr, CStringArray* Items = nullptr);   // standard constructor
	virtual ~CSpecifyRegressionModel();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MULTIPLE_REGRESSION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int n_Var;
	CStringArray VariableList;
	DECLARE_MESSAGE_MAP()
public:
	int ValidationMethod;
	int FactorAnalysis;
	int NumberRuns = 100;
	double PercentageTestingData;
	double alpha;
	CArray <int> SelectedPredictors;
	CStatic cntr_ShowSignificanceText;
	CStatic cntr_ShowNumberRunsText;
	CStatic cntr_ShowPercentageTestDataText;
	CEdit cntr_ShowSignificanceSelection;
	CEdit cntr_ShowNumberRuns;
	CEdit cntr_ShowPercentageTestData;
	CListBox AvailableRegressorList;
	CListBox SelectedRegressorList;
	CButton AddVariables;
	CButton RemoveVariables;
	CButton AddAllVariables;
	CButton RemoveAllVariables;
	BOOL OnInitDialog();
	afx_msg void LoadListBoxEntries();
	afx_msg void OnBnClickedButtonSelectPredictors();
	afx_msg void OnBnClickedButtonRemovePredictors();
	afx_msg void OnBnClickedButtonSelectAllPredictors();
	afx_msg void OnBnClickedButtonRemoveAllPredictors();
	afx_msg void OnBnClickedRadioRegressionMonteCarlo();
	afx_msg void OnBnClickedRadioRegressionLooCvAssessment();
	afx_msg void OnBnClickedRadioRegression10FoldCv();
	afx_msg void OnBnClickedRadioRegression5FoldCv();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioFactorAnalysisYes();
	afx_msg void OnBnClickedRadioFactorAnalysisNo();
};
