#pragma once
#include "afxdialogex.h"


// CSelectNumberPCs dialog

class CSelectNumberPCs : public CDialog{
	DECLARE_DYNAMIC(CSelectNumberPCs)

public:
	CSelectNumberPCs(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSelectNumberPCs();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_SELECT_PCS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int n_PCs;
};
