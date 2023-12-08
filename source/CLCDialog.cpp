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
    if (m_EditCtrl.GetSafeHwnd())
        m_EditCtrl.DestroyWindow();
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
    m_NodeCounts.resize(numLayers);
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
        int layerIndex = pItem->iItem;
        ASSERT(layerIndex >= 0 && layerIndex < m_NodeCounts.size()); // Ensure the index is within bounds
        if (layerIndex >= 0 && layerIndex < m_NodeCounts.size()) {
            int nodeCount = _ttoi(pItem->pszText);
            m_NodeCounts[layerIndex] = nodeCount;
        }
    }

    *pResult = 0;
}

void CLCDialog::OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int nItem = pNMItemActivate->iItem;
    int nSubItem = pNMItemActivate->iSubItem;

    // Only proceed if the second column is clicked and item is valid
    if (nSubItem == 1 && nItem != -1)
    {
        CRect rect;
        m_LayerNodeList.GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect);

        // Create or use an existing CEdit control to receive the input
        if(!::IsWindow(m_EditCtrl.GetSafeHwnd()))
        {
            m_EditCtrl.Create(WS_BORDER | WS_CHILD | ES_AUTOHSCROLL | ES_NUMBER, rect, this, IDC_EDIT1);
        }

        m_EditCtrl.SetWindowText(m_LayerNodeList.GetItemText(nItem, nSubItem));
        m_EditCtrl.MoveWindow(rect);
        m_EditCtrl.SetSel(0, -1); // Select all text
        m_EditCtrl.ShowWindow(SW_SHOW);
        m_EditCtrl.SetFocus(); // Set focus to the edit box
    }

    *pResult = 0;
}

void CLCDialog::OnEnKillfocusEdit()
{
    CString strValue;
    m_EditCtrl.GetWindowText(strValue); // Get the value from the edit control

    int nItem = m_LayerNodeList.GetSelectionMark(); // Get the selected item index
    if (nItem != -1) // Check if an item is actually selected
    {
        int nSubItem = 1; // We know it's the second column for "Nodes"

        m_LayerNodeList.SetItemText(nItem, nSubItem, strValue); // Update the list control item text

        // Now update the vector with the new value, if it's within range
        if (nItem >= 0 && nItem < m_NodeCounts.size())
        {
            m_NodeCounts[nItem] = _ttoi(strValue); // Convert and update the node count
        }
        else
        {
            // Handle the error case where the index is out of bounds
            // This could be an ASSERT or some error handling code
            ASSERT(FALSE);
        }
    }

    m_EditCtrl.ShowWindow(SW_HIDE); // Hide the edit control
}


std::vector<int> CLCDialog::GetNodeCounts() const
{
    return m_NodeCounts;
}



BEGIN_MESSAGE_MAP(CLCDialog, CDialogEx)
    ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, &CLCDialog::OnLvnEndlabeleditList)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CLCDialog::OnNMDblclkList)
    ON_EN_KILLFOCUS(IDC_EDIT1, &CLCDialog::OnEnKillfocusEdit)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CLCDialog::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CLCDialog message handlers


void CLCDialog::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // This can be used if additional actions are needed when the selection changes.
    *pResult = 0;
}
