#pragma once
#include "afxdialogex.h"


// NewANN dialog

class NewANN : public CDialog
{
	DECLARE_DYNAMIC(NewANN)

public:
	NewANN(CWnd* pParent = nullptr);   // standard constructor
	BOOL OnInitDialog();
	virtual ~NewANN();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWANN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	double learning_rate;
	int total_epoch;
	int batch_size;
	int m_selectedlayer;

	CButton layer1;
	CButton layer2;
	CButton layer3;
	CButton layer4;
	CButton layer5;
};
