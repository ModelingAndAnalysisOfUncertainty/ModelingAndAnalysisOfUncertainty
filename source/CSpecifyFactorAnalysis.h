#pragma once
#include "afxdialogex.h"


// CSpecifyFactorAnalysis dialog

class CSpecifyFactorAnalysis : public CDialogEx
{
	DECLARE_DYNAMIC(CSpecifyFactorAnalysis)

public:
	CSpecifyFactorAnalysis(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSpecifyFactorAnalysis();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FACTOR_ANALYSIS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int n_Factors;
	int OrthogonalRotation;
	int WhichOrthogonalRotation;
	CButton cntr_ShowVarimax;
	CButton cntr_ShowQuartimax;
	CButton cntr_ShowEquimax;
	CButton cntr_ShowParsimax;
	CStatic cntr_ShowOrthogonalRotation;
	afx_msg void OnBnClickedRadioObliqueFactorRotation();
	afx_msg void OnBnClickedRadioOrthogonalFactorRotation();
};
