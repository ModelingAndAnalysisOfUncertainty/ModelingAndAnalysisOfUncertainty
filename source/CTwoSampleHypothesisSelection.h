#pragma once
#include "afxdialogex.h"


// CTwoSampleHypothesisSelection dialog

class CTwoSampleHypothesisSelection : public CDialogEx
{
	DECLARE_DYNAMIC(CTwoSampleHypothesisSelection)

public:
public:
	CTwoSampleHypothesisSelection(CWnd* pParent = nullptr, CStringArray* Items = nullptr);   // standard constructor
	virtual ~CTwoSampleHypothesisSelection();
	

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TWOSAMPLE_HYPOTHESIS };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int n_Var;
	CStringArray VarNames;
	DECLARE_MESSAGE_MAP()
public:
	
	int SelectMeanVariance;
	int SelectUnknownKnown;
	int SelectedClass_1;
	int SelectedClass_2;
	int SelectedVariable;
	int TwoSided;

	

	CButton ctrl_SelectMean;
	CButton ctrl_SelectVariance;
	CButton ctrl_SelectKnown;
	CButton ctrl_SelectUnknown;

	CStatic ctrl_ShowKnownUnknown;
	CStatic ctrl_ShowSpecifyVariance;
	CStatic ctrl_ShowVarianceSpecify;

	CComboBox VariableSet;

	double Value_0;
	double VarianceEnter;
	double Significance;
	double EnterVariance;
	
	
	CEdit ctrl_EnterVariance;
	CEdit ctrl_VarianceEnter;

	
	
	
	afx_msg void OnBnClickedRadioSelectMean();
	afx_msg void OnBnClickedRadioSelectVariance();
	afx_msg void OnBnClickedRadioSelectKnown();
	afx_msg void OnBnClickedRadioSelectUnknown();
	afx_msg void OnCbnSelchangeComboVariableSet();
	afx_msg void LoadComboBoxEntries();
	
	BOOL OnInitDialog();
	
};
