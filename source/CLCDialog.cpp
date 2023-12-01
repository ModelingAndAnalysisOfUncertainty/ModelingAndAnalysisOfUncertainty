// CLCDialog.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CLCDialog.h"


// CLCDialog dialog

IMPLEMENT_DYNAMIC(CLCDialog, CDialogEx)

CLCDialog::CLCDialog(int numLayers, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CLC, pParent), m_numLayers(numLayers)
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
    m_LayerNodeList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EDITLABELS);
	m_LayerNodeList.InsertColumn(0, _T("Layer"), LVCFMT_LEFT, 50);
	m_LayerNodeList.InsertColumn(1, _T("Nodes"), LVCFMT_LEFT, 50);
    SetNumLayers(m_numLayers);
	return TRUE;
}

void CLCDialog::SetNumLayers(int numLayers)
{
    m_LayerNodeList.DeleteAllItems();
    for (int i = 0; i < numLayers; ++i)
    {
        CString layerStr;
        layerStr.Format(_T("Layer %d"), i + 1);
        int itemIndex = m_LayerNodeList.InsertItem(i, layerStr);
        m_LayerNodeList.SetItemText(itemIndex, 1, _T("0"));
    }
}


void CLCDialog::OnOK()
{
    m_NodeCounts.clear();
    int rowCount = m_LayerNodeList.GetItemCount();
    for (int i = 0; i < rowCount; ++i)
    {
        CString nodeCountStr = m_LayerNodeList.GetItemText(i, 1); // 1 for second column
        m_NodeCounts.push_back(_ttoi(nodeCountStr));
    }

    CDialogEx::OnOK(); // Call the base class
}

void CLCDialog::OnLvnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
    LV_ITEM* pItem = &pDispInfo->item;

    if (pItem->pszText != NULL)
    {
        // update the inside structure
        int layerIndex = pItem->iItem;
        int nodeCount = _ttoi(pItem->pszText);
        // assume m_NodeCounts already has enough space
        m_NodeCounts[layerIndex] = nodeCount;
    }

    *pResult = 0;
}

std::vector<int> CLCDialog::GetNodeCounts() const
{
    return m_NodeCounts;
}


BEGIN_MESSAGE_MAP(CLCDialog, CDialogEx) 
    ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, &CLCDialog::OnLvnEndlabeleditList)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CLCDialog::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CLCDialog message handlers


void CLCDialog::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}
