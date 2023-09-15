// COneSampleHypothesisTestingMean.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "COneSampleHypothesisTestingMean.h"


// COneSampleHypothesisTestingMean dialog

IMPLEMENT_DYNAMIC(COneSampleHypothesisTestingMean, CDialog)

COneSampleHypothesisTestingMean::COneSampleHypothesisTestingMean(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_HYPOTHESIS_MEAN, pParent)
{

}

COneSampleHypothesisTestingMean::~COneSampleHypothesisTestingMean(){
}

void COneSampleHypothesisTestingMean::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COneSampleHypothesisTestingMean, CDialog)
END_MESSAGE_MAP()


// COneSampleHypothesisTestingMean message handlers


//void COneSampleHypothesisTestingMean::OnSelchangeComboVariableSelection()
//{
//	// TODO: Add your control notification handler code here
//}
