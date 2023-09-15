#pragma once
#include "afxdialogex.h"


// CSelectScatterDiagrams dialog

class CSelectScatterDiagrams : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectScatterDiagrams)

public:
	CSelectScatterDiagrams(CWnd* pParent, int nVar, bool Type);   // standard constructor
	virtual ~CSelectScatterDiagrams();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_SCATTER_PLOTS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int NumberVariables, n_Var;
	CArray <int> SelectedScoresLoadings;
	CStringArray OriginalList;
	CButton AddVariables;
	CButton RemoveVariables;
	CListBox AvailableScoreLoadingList;
	CListBox SelectedScoreLoadingList;
	bool type;
	BOOL OnInitDialog();

	afx_msg void LoadListBoxEntries();
	afx_msg void OnBnClickedButtonAddScoreLoading();
	afx_msg void OnBnClickedButtonRemoveScoreLoading();
	afx_msg void OnBnClickedOk();
};
