#pragma once
#include "afxdialogex.h"


// CANNForm dialog

class CANNForm : public CDialogEx
{
	DECLARE_DYNAMIC(CANNForm)

public:
	CANNForm(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CANNForm();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANN_TRAINSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double learning_rate;
	int total_epoch;
	int batch_size;
	int m_selectedTask;
	CButton task1;
	CButton task2;
	CButton task3;
	CButton task4;
	CButton task5;
};
