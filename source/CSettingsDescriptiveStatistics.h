#pragma once
#include "afxdialogex.h"


// CSettingsDescriptiveStatistics dialog

class CSettingsDescriptiveStatistics : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDescriptiveStatistics)

public:
	CSettingsDescriptiveStatistics(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSettingsDescriptiveStatistics();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTINGS_DESCRIPTIVE_STATISTICS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int Selected_Distribution;
	int n_Bins;
	CComboBox List_Distributions;
	BOOL OnInitDialog();
};
