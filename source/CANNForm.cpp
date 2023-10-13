// CANNForm.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CANNForm.h"


// CANNForm dialog

IMPLEMENT_DYNAMIC(CANNForm, CDialogEx)

CANNForm::CANNForm(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_ANN_TRAINSET, pParent)
    , learning_rate(0)
    , total_epoch(0)
    , batch_size(0)
{
    learning_rate = 0.01;
    total_epoch = 5;
    batch_size = 128;
}

CANNForm::~CANNForm()
{
}

void CANNForm::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_LEARNING_RATE, learning_rate);
    DDX_Text(pDX, IDC_EDIT_TOTAL_EPOCH, total_epoch);
    DDX_Text(pDX, IDC_EDIT_BATCH_SIZE, batch_size);
    DDX_Control(pDX, IDC_TASK1, task1);
    DDX_Control(pDX, IDC_TASK2, task2);
    DDX_Control(pDX, IDC_TASK3, task3);
    DDX_Control(pDX, IDC_TASK4, task4);
    DDX_Control(pDX, IDC_TASK5, task5);
    if (task1.GetCheck() == BST_CHECKED) {
        m_selectedTask = 1;
    }
    else if (task2.GetCheck() == BST_CHECKED) {
        m_selectedTask = 2;
    }
    else if (task3.GetCheck() == BST_CHECKED) {
        m_selectedTask = 3;
    }
    else if (task4.GetCheck() == BST_CHECKED) {
        m_selectedTask = 4;
    }
    else if (task5.GetCheck() == BST_CHECKED) {
        m_selectedTask = 5;
    }
}


BEGIN_MESSAGE_MAP(CANNForm, CDialogEx)

END_MESSAGE_MAP()