// CLCDialog.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CLCDialog.h"


// CLCDialog dialog

IMPLEMENT_DYNAMIC(CLCDialog, CDialogEx)

CLCDialog::CLCDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CLC, pParent)
{

}

CLCDialog::~CLCDialog()
{
}

void CLCDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LayerNodeList);
}

BOOL CLCDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_LayerNodeList.InsertColumn(0, _T("Layer"), LVCFMT_LEFT, 50);
	m_LayerNodeList.InsertColumn(1, _T("Nodes"), LVCFMT_LEFT, 50);

	int numLayers = 5; 
	for (int i = 0; i < numLayers; ++i)
	{
		CString layerStr;
		layerStr.Format(_T("Layer %d"), i + 1);

		int itemIndex = m_LayerNodeList.InsertItem(i, layerStr);
		m_LayerNodeList.SetItemText(itemIndex, 1, _T("0"));
	}
	return TRUE;
}


BEGIN_MESSAGE_MAP(CLCDialog, CDialogEx)
END_MESSAGE_MAP()


// CLCDialog message handlers
