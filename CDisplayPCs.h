#pragma once
#include "afxdialogex.h"


// CDisplayPCs dialog

class CDisplayPCs : public CDialog{
	DECLARE_DYNAMIC(CDisplayPCs)

public:
	CDisplayPCs(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDisplayPCs();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPLAY_PCS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int PC_1;
	int PC_2;
	int PC_3;
	int PC_4;
	int SelectOption;
};
