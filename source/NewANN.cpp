// NewANN.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "NewANN.h"


// NewANN dialog

IMPLEMENT_DYNAMIC(NewANN, CDialog)

NewANN::NewANN(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_NEWANN, pParent)
	, learning_rate(0)
	, total_epoch(0)
	, batch_size(0)
{

}

NewANN::~NewANN()
{
}

BOOL NewANN::OnInitDialog()
{
	
	CDialog::OnInitDialog();
	// Set the first radio button to be selected by default
	m_selectedlayer = 1;
	layer1.SetCheck(BST_CHECKED);

	return TRUE;  
}

void NewANN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_learning_rate, learning_rate);
	DDX_Text(pDX, IDC_total_epoch, total_epoch);
	DDX_Text(pDX, IDC_batch_size, batch_size);

	DDX_Control(pDX, IDC_RADIO1, layer1);
	DDX_Control(pDX, IDC_RADIO2, layer2);
	DDX_Control(pDX, IDC_RADIO3, layer3);
	DDX_Control(pDX, IDC_RADIO4, layer4);
	DDX_Control(pDX, IDC_RADIO5, layer5);
	if (layer1.GetCheck() == BST_CHECKED) {
		m_selectedlayer = 1;
	}
	else if (layer2.GetCheck() == BST_CHECKED) {
		m_selectedlayer = 2;
	}
	else if (layer3.GetCheck() == BST_CHECKED) {
		m_selectedlayer = 3;
	}
	else if (layer4.GetCheck() == BST_CHECKED) {
		m_selectedlayer = 4;
	}
	else if (layer5.GetCheck() == BST_CHECKED) {
		m_selectedlayer = 5;
	}
}


BEGIN_MESSAGE_MAP(NewANN, CDialog)

END_MESSAGE_MAP()



