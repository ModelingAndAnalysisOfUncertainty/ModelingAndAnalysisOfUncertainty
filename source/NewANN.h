#pragma once
#include "afxdialogex.h"


// NewANN dialog

class NewANN : public CDialogEx
{
	DECLARE_DYNAMIC(NewANN)

public:
	NewANN(CWnd* pParent = nullptr);   // standard constructor
	virtual ~NewANN();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NewANN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
