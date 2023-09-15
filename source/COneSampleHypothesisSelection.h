#pragma once
#include "afxdialogex.h"
#include "Modeling and Analysis of UncertaintyDoc.h"

// COneSampleHypothesisSelectoin dialog

class COneSampleHypothesisSelection : public CDialogEx{
	DECLARE_DYNAMIC(COneSampleHypothesisSelection)

public:
	COneSampleHypothesisSelection(CWnd* pParent = nullptr, CStringArray* Items = nullptr);   // standard constructor
	virtual ~COneSampleHypothesisSelection();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ONESAMPLE_HYPOTHESIS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int n_Var;
	CStringArray VarNames;
	DECLARE_MESSAGE_MAP()
public:
	int Mean;
	int Selection_knownVariance;
	int SelectedVariable;
	int SelectedClass;
	int OneTwoSided;
	double Variance;
	double alpha;
	double Value_0;
	CComboBox VariableSet;
	BOOL OnInitDialog();
	CStatic cntr_Show_KnownUnknownVariance;
	CStatic cntr_Show_SpecifyVariance;
	CButton cntr_Show_KnownVariance;
	CButton cntr_Show_UnknownVariance;
	CButton cntr_Show_ShowVariance;
	afx_msg void LoadComboBoxEntries();
	afx_msg void OnBnClickedRadioOnesampleKnownMean();
	afx_msg void OnBnClickedRadioOnesampleUnknownMean();
	afx_msg void OnBnClickedRadioOnesamplehypothesisMean();
	afx_msg void OnBnClickedRadioOnesamplehypothesisVariance();
	afx_msg void OnCBnSelection();
};
