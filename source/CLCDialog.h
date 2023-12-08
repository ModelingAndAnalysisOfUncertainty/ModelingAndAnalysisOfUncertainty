#pragma once
#include "afxdialogex.h"
#include <vector>
#include <iostream>
using namespace std;


// CLCDialog dialog

class CLCDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLCDialog)

public:
	CLCDialog(int numLayers, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLCDialog();
	void SetNumLayers(int numLayers);
	std::vector<int> GetNodeCounts() const;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	std::vector<int> m_NodeCounts;
	virtual void OnOK();
	afx_msg void OnLvnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkList(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnEnKillfocusEdit();

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_LayerNodeList;
	CEdit m_EditCtrl;

private:
	int m_numLayers;
public:
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
};
