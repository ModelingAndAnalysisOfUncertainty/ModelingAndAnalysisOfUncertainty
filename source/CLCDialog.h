#pragma once
#include "afxdialogex.h"
#include <vector>


// CLCDialog dialog

class CLCDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLCDialog)

public:
	CLCDialog(CWnd* pParent = nullptr);   // standard constructor
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

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_LayerNodeList;
};
