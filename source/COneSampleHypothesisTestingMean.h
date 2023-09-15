#pragma once
#include "afxdialogex.h"


// COneSampleHypothesisTestingMean dialog

class COneSampleHypothesisTestingMean : public CDialog
{
	DECLARE_DYNAMIC(COneSampleHypothesisTestingMean)

public:
	COneSampleHypothesisTestingMean(CWnd* pParent = nullptr);   // standard constructor
	virtual ~COneSampleHypothesisTestingMean();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HYPOTHESIS_MEAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
};
