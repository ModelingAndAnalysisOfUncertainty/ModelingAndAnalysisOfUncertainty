// CSpecifyFactorAnalysis.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CSpecifyFactorAnalysis.h"


// CSpecifyFactorAnalysis dialog

IMPLEMENT_DYNAMIC(CSpecifyFactorAnalysis, CDialogEx)

CSpecifyFactorAnalysis::CSpecifyFactorAnalysis(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FACTOR_ANALYSIS, pParent)
	, n_Factors(1)
	, OrthogonalRotation(0)
	, WhichOrthogonalRotation(0)
{

}

CSpecifyFactorAnalysis::~CSpecifyFactorAnalysis(){
}

void CSpecifyFactorAnalysis::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUMBER_FACTORS, n_Factors);
	DDV_MinMaxInt(pDX, n_Factors, 1, INT_MAX);
	DDX_Radio(pDX, IDC_RADIO_ORTHOGONAL_FACTOR_ROTATION, OrthogonalRotation);
	DDX_Radio(pDX, IDC_RADIO_ORTHOGONAL_FACTOR_ROTATION_VARIMAX, WhichOrthogonalRotation);
	DDX_Control(pDX, IDC_RADIO_ORTHOGONAL_FACTOR_ROTATION_VARIMAX, cntr_ShowVarimax);
	DDX_Control(pDX, IDC_RADIO_ORTHOGONAL_FACTOR_ROTATION_QUARTIMAX, cntr_ShowQuartimax);
	DDX_Control(pDX, IDC_RADIO_ORTHOGONAL_FACTOR_ROTATION_EQUIMAX, cntr_ShowEquimax);
	DDX_Control(pDX, IDC_RADIO_ORTHOGONAL_FACTOR_ROTATION_PARSIMAX, cntr_ShowParsimax);
	DDX_Control(pDX, IDC_STATIC_SELECT_ORTHOGONAL_ROTATION, cntr_ShowOrthogonalRotation);
}

BEGIN_MESSAGE_MAP(CSpecifyFactorAnalysis, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_OBLIQUE_FACTOR_ROTATION, &CSpecifyFactorAnalysis::OnBnClickedRadioObliqueFactorRotation)
	ON_BN_CLICKED(IDC_RADIO_ORTHOGONAL_FACTOR_ROTATION, &CSpecifyFactorAnalysis::OnBnClickedRadioOrthogonalFactorRotation)
END_MESSAGE_MAP()

void CSpecifyFactorAnalysis::OnBnClickedRadioObliqueFactorRotation(){
	cntr_ShowVarimax.ShowWindow(SW_HIDE);
	cntr_ShowQuartimax.ShowWindow(SW_HIDE);
	cntr_ShowEquimax.ShowWindow(SW_HIDE);
	cntr_ShowParsimax.ShowWindow(SW_HIDE);
	cntr_ShowOrthogonalRotation.ShowWindow(SW_HIDE);
}

void CSpecifyFactorAnalysis::OnBnClickedRadioOrthogonalFactorRotation(){
	cntr_ShowVarimax.ShowWindow(SW_SHOW);
	cntr_ShowQuartimax.ShowWindow(SW_SHOW);
	cntr_ShowEquimax.ShowWindow(SW_SHOW);
	cntr_ShowParsimax.ShowWindow(SW_SHOW);
	cntr_ShowOrthogonalRotation.ShowWindow(SW_SHOW);
}
