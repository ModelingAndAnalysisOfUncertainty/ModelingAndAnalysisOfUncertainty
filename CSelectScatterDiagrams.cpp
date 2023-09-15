// CSelectScatterDiagrams.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CSelectScatterDiagrams.h"


// CSelectScatterDiagrams dialog

IMPLEMENT_DYNAMIC(CSelectScatterDiagrams, CDialogEx)

CSelectScatterDiagrams::CSelectScatterDiagrams(CWnd* pParent, int nVar, bool Type)
	: CDialogEx(IDD_DIALOG_SELECT_SCATTER_PLOTS, pParent)
	, NumberVariables(0)
{
	n_Var = nVar;
	type = Type;
}

CSelectScatterDiagrams::~CSelectScatterDiagrams()
{
}

void CSelectScatterDiagrams::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADD_SCORE_LOADING, AddVariables);
	DDX_Control(pDX, IDC_LIST_SCORES_LOADINGS_AVAILABLE, AvailableScoreLoadingList);
	DDX_Control(pDX, IDC_LIST_SCORES_LOADINGS_SELECTED, SelectedScoreLoadingList);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_SCORE_LOADING, RemoveVariables);
	DDX_Radio(pDX, IDC_RADIO_SELECT_ONE_SCATTER_PLOT, NumberVariables);
	DDV_MinMaxInt(pDX, NumberVariables, 0, INT_MAX);
}


BEGIN_MESSAGE_MAP(CSelectScatterDiagrams, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_SCORE_LOADING, &CSelectScatterDiagrams::OnBnClickedButtonAddScoreLoading)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_SCORE_LOADING, &CSelectScatterDiagrams::OnBnClickedButtonRemoveScoreLoading)
	ON_BN_CLICKED(IDOK, &CSelectScatterDiagrams::OnBnClickedOk)
END_MESSAGE_MAP()

void CSelectScatterDiagrams::LoadListBoxEntries() {
	CString Text, Number;
	for (int i = 0; i < n_Var; i++) {
		Text.Empty();
		if (type == true) Text.Append(_T("Score variable "));
		else Text.Append(_T("Loading vector "));
		Number.Empty(), Number.Format(L"%d", i + 1), Text.Append(Number);
		AvailableScoreLoadingList.AddString(Text);
		OriginalList.Add(Text);
	}
}

BOOL CSelectScatterDiagrams::OnInitDialog() {
	CDialogEx::OnInitDialog();
	LoadListBoxEntries();
	return true;
}

// CSelectScatterDiagrams message handlers

void CSelectScatterDiagrams::OnBnClickedButtonAddScoreLoading() {
	int SelectedVariable = AvailableScoreLoadingList.GetCurSel();
	if (SelectedVariable != LB_ERR) {
		CString Text;
		AvailableScoreLoadingList.GetText(SelectedVariable, Text);
		SelectedScoreLoadingList.AddString(Text);
		AvailableScoreLoadingList.DeleteString(SelectedVariable);
	}
}

void CSelectScatterDiagrams::OnBnClickedButtonRemoveScoreLoading() {
	int SelectedVariable = SelectedScoreLoadingList.GetCurSel();
	if (SelectedVariable != LB_ERR) {
		CString Text;
		SelectedScoreLoadingList.GetText(SelectedVariable, Text);
		AvailableScoreLoadingList.AddString(Text);
		SelectedScoreLoadingList.DeleteString(SelectedVariable);
	}
}

void CSelectScatterDiagrams::OnBnClickedOk(){
	CDialogEx::OnOK();
	SelectedScoresLoadings.RemoveAll();
	CString Predictor, Item;
	int j = 0;
	Predictor.Empty();
	SelectedScoreLoadingList.GetText(j, Predictor);
	for (int i = 0; i < n_Var; i++) {
		Item.Empty();
		Item = OriginalList.GetAt(i);
		if (Predictor == Item) {
			j++;
			SelectedScoresLoadings.Add(i);
			if (j < SelectedScoreLoadingList.GetCount()) {
				Predictor.Empty();
				SelectedScoreLoadingList.GetText(j, Predictor);
			}
		}
	}
}
