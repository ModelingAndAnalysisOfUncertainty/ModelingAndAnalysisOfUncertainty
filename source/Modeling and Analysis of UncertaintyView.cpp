#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "Modeling and Analysis of Uncertainty.h"
#endif

#include "Modeling and Analysis of UncertaintyDoc.h"
#include "Modeling and Analysis of UncertaintyView.h"
#include "ProbabilityDistributions.h"
#include "CSelectNumberPCs.h"
#include "CDisplayPCs.h"
#include "CSettingsDescriptiveStatistics.h"
#include "CSelectScatterDiagrams.h"
#include "NewANN.h"

#include <vector>
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>

// CModelingandAnalysisofUncertaintyView

IMPLEMENT_DYNCREATE(CModelingandAnalysisofUncertaintyView, CView)

BEGIN_MESSAGE_MAP(CModelingandAnalysisofUncertaintyView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CModelingandAnalysisofUncertaintyView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DISPLAY_PCA, On_Display_PCA)
	ON_BN_CLICKED(IDC_NEXT_VARIABLE, On_Next_Variable)
	ON_BN_CLICKED(IDC_PREVIOUS_VARIABLE, On_Previous_Variable)
	ON_BN_CLICKED(IDC_INDEPENDENT_ASSESSMENT_REGRESSION, On_Independent_Assessment_Regression)
	ON_BN_CLICKED(IDC_STANDARD_ASSESSMENT_REGRESSION, On_Standard_Assessment_Regression)
	ON_BN_CLICKED(IDC_SETTINGS_DESCRIPTIVE_STATISTICS, On_Settings_Descriptive_Statistics)
	ON_BN_CLICKED(IDC_LOADING_PLOTS, On_Display_Loadings)
	ON_BN_CLICKED(IDC_SCORE_PLOTS, On_Display_Scores)
	ON_BN_CLICKED(IDC_SELECT_SCATTER_PLOTS, On_Select_Scatter_Plots)
	ON_BN_CLICKED(IDC_DISPLAY_FA, On_Display_FA)
	ON_BN_CLICKED(IDC_FACTOR_LOADINGS, On_Display_Factor_Loadings)
	ON_BN_CLICKED(IDC_FACTOR_SCORES, On_Display_Factor_Scores)
	ON_BN_CLICKED(IDC_FACTOR_MATRICES, On_Display_Factor_Matrices)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_MACHINELEARNING_ARTIFICIALNEURALNETWORKWITHACCURACY, &CModelingandAnalysisofUncertaintyView::OnMachinelearningArtificialneuralnetworkwithaccuracy)
END_MESSAGE_MAP()

// CModelingandAnalysisofUncertaintyView construction/destruction

CModelingandAnalysisofUncertaintyView::CModelingandAnalysisofUncertaintyView() noexcept{
}

CModelingandAnalysisofUncertaintyView::~CModelingandAnalysisofUncertaintyView(){

}

BOOL CModelingandAnalysisofUncertaintyView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CModelingandAnalysisofUncertaintyView drawing

void CModelingandAnalysisofUncertaintyView::OnDraw(CDC* pDC){
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	CWnd* pwndParent = this->GetParent();
	CRect rc;
	pwndParent->GetWindowRect(&rc);
	int nHt = rc.Height(), nWt = (int)(0.989*rc.Width()), Offset = (int)(0.135 * rc.Height());
	CPoint TopLeftCorner, BottomRightCorner;
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	n = pDoc->n;
	if (pDoc->StartWindow) {
		pDC->TextOutW(20, 20, L"Welcome to Modeling and Analysis of Uncertainty");
	}
	else if (pDoc->OnOpenedFile) {
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		DisplayPCA.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);
		DisplayLoadingPlots.ShowWindow(SW_HIDE);
		DisplayScorePlots.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_HIDE);
		DisplayFactorLoadings.ShowWindow(SW_HIDE);
		DisplayFactorScores.ShowWindow(SW_HIDE);
		DisplayFactorMatrices.ShowWindow(SW_HIDE);
		SelectScatterPlots.ShowWindow(SW_HIDE);
		DisplayFileAndDataSetInformation(pDoc, pDC, true);
	}
	else if (pDoc->DescriptiveStatistics) {
		if (counter > pDoc->n_Var - 1) counter = 0;
		DisplayPCA.ShowWindow(SW_HIDE);
		DisplayLoadingPlots.ShowWindow(SW_HIDE);
		DisplayScorePlots.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_HIDE);
		DisplayFactorLoadings.ShowWindow(SW_HIDE);
		DisplayFactorScores.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_SHOW);
		Previous_Variable.ShowWindow(SW_SHOW);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_SHOW);
		SelectScatterPlots.ShowWindow(SW_HIDE);
		pDC->TextOutW(118, 20, _T("Previous"));
		pDC->TextOutW(201, 20, _T("Next"));
		int nVar, nObs = pDoc->n_Obs;
		if (pDoc->Problem == L"Regression problem") nVar = pDoc->n_Var;
		else nVar = pDoc->n_Var - 1;
		if (counter == nVar) counter = 0;
		if (counter == -1) counter = nVar - 1;
		DisplayDescriptiveStatistics(pDC, pDoc);
		CString Text;
		Text.Empty(), Text.Append(pDoc->Name.GetAt(counter));
		if (pDoc->Unit.GetAt(counter) != "") Text.Append(L" in [" + pDoc->Unit.GetAt(counter) + L"]");
		CArray <double> Sample;
		Sample.RemoveAll();
		TopLeftCorner.x = 20, TopLeftCorner.y = 460;
		BottomRightCorner.x = 500, BottomRightCorner.y = nHt - Offset;
		DisplayBoxPlot(pDoc, pDC, Sample, TopLeftCorner, BottomRightCorner, counter, Text);
		Sample.SetSize(nObs);
		for (int i = 0; i < nObs; i++) Sample.SetAt(i, pDoc->Data.GetAt(pDoc->GetPosition(i, counter, pDoc->Data_spec)));
		TopLeftCorner.x = 525, TopLeftCorner.y = (int)((double)(nHt - Offset) / (double)2);
		BottomRightCorner.x = 525 + (nWt - 525) / 2, BottomRightCorner.y = nHt - Offset;
		int bins_old = (int)(sqrt(nObs));
		if ((bins != 2) && (bins != bins_old)) bins_old = bins;
		if (bins == 2) bins = bins_old;
		DisplayHistogram(pDoc, pDC, Sample, TopLeftCorner, BottomRightCorner, bins_old, Text);
		TopLeftCorner.x = BottomRightCorner.x + 20, BottomRightCorner.x = nWt - 20;
		ShowProbabilityPlot(pDoc, pDC, Sample, TopLeftCorner, BottomRightCorner, Text);
		TopLeftCorner.x = 525, TopLeftCorner.y = 20;
		BottomRightCorner.x = 525 + (int)(0.75 * (double)(nWt - 525) ), BottomRightCorner.y = (int)((double)(nHt - Offset) / (double)2);
		SequencePlot(pDoc,pDC,Sample,TopLeftCorner,BottomRightCorner, Text);
		TopLeftCorner.x = 525 + (int)(0.75 * (double)(nWt - 525)), TopLeftCorner.y = 20;
		BottomRightCorner.x = nWt - 20, BottomRightCorner.y = (int)((double)(nHt - Offset) / (double)2);
		DisplayPDF(pDoc, pDC, Sample, TopLeftCorner,BottomRightCorner);
	}
	else if (pDoc->HypothesisTesting_OneSample) {
		DisplayPCA.ShowWindow(SW_HIDE);
		DisplayLoadingPlots.ShowWindow(SW_HIDE);
		DisplayScorePlots.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_HIDE);
		DisplayFactorLoadings.ShowWindow(SW_HIDE);
		DisplayFactorScores.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		SelectScatterPlots.ShowWindow(SW_HIDE);
		int VariableSelected = pDoc->VariableSelected;
		CArray <double> Sample;
		// copy the values for the hypothesis test (evidence) into the array Sample
		//DisplayVariableStatistics(pDC, Sample);
	}
	else if (pDoc->HypothesisTesting_TwoSample == true) {
		DisplayPCA.ShowWindow(HIDE_WINDOW);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);

		//Margaret's implementation on boxplots


		// Margaret's implementation on numerical value display 
		CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();

		CString Text;
		int number = 0;
		pDC->Rectangle(0, 0, 2200, 2200);
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
		pDC->TextOutW(160, 30, _T("Descriptive Statistics"));
		pDC->MoveTo(10, 50), pDC->LineTo(500, 50), Text.Format(L"%d", pDoc->n_Obs); 



		pDC->TextOutW(20, 60, L"Variable"), pDC->TextOutW(248, 60, L":"), pDC->TextOutW(270, 60, pDoc->Tag.GetAt(pDoc->number));
		pDC->TextOutW(20, 80, L"Name"), pDC->TextOutW(248, 80, L":"), pDC->TextOutW(270, 80, pDoc->Name.GetAt(pDoc->number));
		if (pDoc->Unit.GetAt(pDoc->number) == "") {
			pDC->TextOutW(20, 100, L"Unit"), pDC->TextOutW(248, 100, L":"), pDC->TextOutW(270, 100, L"N/A");
		}
		else {
			pDC->TextOutW(20, 100, L"Unit"), pDC->TextOutW(248, 100, L":"), pDC->TextOutW(270, 100, pDoc->Unit.GetAt(pDoc->number));
		}

		pDC->MoveTo(10, 120), pDC->LineTo(500, 120), Text.Format(L"%d", pDoc->n_Obs);

		CString sth1, sth2;
		//accessing the data
		//L"%.2f"

		int temp1 = pDoc->show_class1;
		int temp2 = pDoc->show_class2;
		sth1.Format(L"%d", temp1);
		sth2.Format(L"%d", temp2);
		pDC->TextOutW(20, 130, L"Class"), pDC->TextOutW(65, 130, sth1), pDC->TextOutW(270, 130, L"Class"), pDC->TextOutW(315, 130, sth2);

		pDC->MoveTo(250, 120), pDC->LineTo(250, 310), Text.Format(L"%d", pDoc->n_Obs);
		pDC->MoveTo(10, 50), pDC->LineTo(10, 310), Text.Format(L"%d", pDoc->n_Obs);
		pDC->MoveTo(500, 50), pDC->LineTo(500, 310), Text.Format(L"%d", pDoc->n_Obs);
		pDC->MoveTo(10, 150), pDC->LineTo(500, 150), Text.Format(L"%d", pDoc->n_Obs);

		temp1 = pDoc->show_nObs1;
		temp2 = pDoc->show_nObs2;
		sth1.Format(L"%d", temp1);
		sth2.Format(L"%d", temp2);
		pDC->TextOutW(20, 170, L"#Obs:"), pDC->TextOutW(170, 170, sth1), pDC->TextOutW(270, 170, L"#Obs:"), pDC->TextOutW(420, 170, sth2);

		double Temp1 = pDoc->show_mean1;
		double Temp2 = pDoc->show_mean2;
		sth1.Format(L"%.2f", Temp1);
		sth2.Format(L"%.2f", Temp2);
		pDC->TextOutW(20, 190, L"Mean:"), pDC->TextOutW(170, 190, sth1), pDC->TextOutW(270, 190, L"Mean:"), pDC->TextOutW(420, 190, sth2);

		Temp1 = pDoc->show_std1;
		Temp2 = pDoc->show_std2;
		sth1.Format(L"%.2f", Temp1);
		sth2.Format(L"%.2f", Temp2);
		pDC->TextOutW(20, 210, L"std:"), pDC->TextOutW(170, 210, sth1), pDC->TextOutW(270, 210, L"std:"), pDC->TextOutW(420, 210, sth2);


		Temp1 = pDoc->show_max1 - pDoc->minimum_sample1;
		Temp2 = pDoc->show_max2 - pDoc->minimum_sample2;
		sth1.Format(L"%.2f", Temp1);
		sth2.Format(L"%.2f", Temp2);
		pDC->TextOutW(20, 270, L"range:"), pDC->TextOutW(170, 270, sth1), pDC->TextOutW(270, 270, L"range:"), pDC->TextOutW(420, 270, sth2);

		Temp1 = pDoc->show_std1 / sqrt(pDoc->show_nObs1);
		Temp2 = pDoc->show_std2 / sqrt(pDoc->show_nObs2);
		sth1.Format(L"%.2f", Temp1);
		sth2.Format(L"%.2f", Temp2);
		pDC->TextOutW(20, 290, L"SE:"), pDC->TextOutW(170, 290, sth1), pDC->TextOutW(270, 290, L"SE:"), pDC->TextOutW(420, 290, sth2);

		pDC->MoveTo(10, 310), pDC->LineTo(500, 310), Text.Format(L"%d", pDoc->n_Obs);




		pDC->TextOutW(140, 360, _T("Results of Hypothesis Test"));
		pDC->MoveTo(10, 380), pDC->LineTo(500, 380), Text.Format(L"%d", pDoc->n_Obs);

		
		pDC->TextOutW(20, 390, _T("Known or Unknown Variance")), pDC->TextOutW(250, 390, L":"), pDC->TextOutW(330, 390, pDoc->show_var_known_unknown);
		if (pDoc->show_var_known_unknown == "Known") {
			if (pDoc->show_tail == "Two-Tailed Test") {
				pDC->TextOutW(20, 410, _T("Type")), pDC->TextOutW(250, 410, L":"), pDC->TextOutW(330, 410, L"Z-Test / Two-Tailed");
			}
			else if (pDoc->show_tail == "Right-Tailed Test") {
				pDC->TextOutW(20, 410, _T("Type")), pDC->TextOutW(250, 410, L":"), pDC->TextOutW(330, 410, L"Z-Test / Right-Tailed");
			}
			else {
				pDC->TextOutW(20, 410, _T("Type")), pDC->TextOutW(250, 410, L":"), pDC->TextOutW(330, 410, L"Z -Test/ Left-Tailed");
			}


			Temp1 = pDoc->show_z_statistic;
			sth1.Format(L"%f", Temp1);
			pDC->TextOutW(20, 430, _T("z-Statistic")), pDC->TextOutW(250, 430, L":"), pDC->TextOutW(330, 430, sth1);


			Temp1 = pDoc->show_zcrit;
			sth1.Format(L"%f", Temp1);
			pDC->TextOutW(20, 450, _T("zcrit")), pDC->TextOutW(250, 450, L":"), pDC->TextOutW(330, 450, sth1);


			Temp1 = pDoc->show_p_value;
			sth1.Format(L"%f", Temp1);
			pDC->TextOutW(20, 470, _T("p_value")), pDC->TextOutW(250, 470, L":"), pDC->TextOutW(330, 470, sth1);

			Temp1 = pDoc->show_delta_x;
			sth1.Format(L"%f", Temp1);
			pDC->TextOutW(20, 490, _T("delta_x")), pDC->TextOutW(250, 479, L":"), pDC->TextOutW(330, 490, sth1);

			if (pDoc->show_tail == "Right-Tailed Test") {
				Temp2 = pDoc->show_critical_U;


				sth1.Format(L"%.2f", Temp2);

				pDC->TextOutW(20, 510, _T("Z Critical Region")), pDC->TextOutW(250, 510, L":"), pDC->TextOutW(330, 510, L"xbar >"), pDC->TextOutW(380, 510, sth1);
				DisplayRectangleRightTailed(pDoc, pDC, nWt, Temp2, 0.0, L"critical", pDoc->show_delta_x, nHt, 'Z');


				Temp1 = pDoc->show_confidence_L;
				sth1.Format(L"%.2f", Temp1);
				Temp2 = pDoc->show_delta_x + (pDoc->show_delta_x - Temp1);
				pDC->TextOutW(20, 530, _T("Z Confidence Interval")), pDC->TextOutW(250, 530, L":"), pDC->TextOutW(330, 530, L"mu >"), pDC->TextOutW(380, 530, sth1);
				DisplayRectangleRightTailed(pDoc, pDC, nWt, 0.0, Temp1, L"confidence", pDoc->show_delta_x, nHt, 'Z');
			}

			else if (pDoc->show_tail == "Left-Tailed Test") {
				Temp1 = pDoc->show_critical_L;
				sth1.Format(L"%.2f", Temp1);
				pDC->TextOutW(20, 510, _T("Z Critical Region")), pDC->TextOutW(250, 510, L":"), pDC->TextOutW(330, 510, L"xbar <"), pDC->TextOutW(380, 510, sth1);

				DisplayRectangleLeftTailed(pDoc, pDC, nWt, 0.0, Temp1, L"critical", pDoc->show_delta_x, nHt, 'Z');


				Temp1 = pDoc->show_confidence_U;
				sth1.Format(L"%.2f", Temp1);
				pDC->TextOutW(20, 530, _T("Z Confidence Interval")), pDC->TextOutW(250, 530, L":"), pDC->TextOutW(330, 530, L"mu <"), pDC->TextOutW(370, 530, sth1);
				DisplayRectangleLeftTailed(pDoc, pDC, nWt, Temp1, 0.0, L"confidence", pDoc->show_delta_x, nHt, 'Z');

			}

			else if (pDoc->show_tail == "Two-Tailed Test") {
				Temp1 = pDoc->show_critical_L;
				Temp2 = pDoc->show_critical_U;

				sth1.Format(L"%.2f", Temp1);
				sth2.Format(L"%.2f", Temp2);

				pDC->TextOutW(20, 510, _T("Z Critical Region")), pDC->TextOutW(250, 510, L":"), pDC->TextOutW(330, 510, L"xbar"), pDC->TextOutW(370, 510, L"<"), pDC->TextOutW(390, 510, sth1), pDC->TextOutW(450, 510, L"or xbar >"), pDC->TextOutW(530, 510, sth2);
				DisplayRectangleTwoTailed(pDoc, pDC, nWt, Temp2, Temp1, L"critical", pDoc->show_delta_x, nHt, 'Z');

				Temp1 = pDoc->show_confidence_L;
				Temp2 = pDoc->show_confidence_U;
				sth1.Format(L"%.2f", Temp1);
				sth2.Format(L"%.2f", Temp2);
				pDC->TextOutW(20, 530, _T("Z Confidence Interval")), pDC->TextOutW(250, 530, L":"), pDC->TextOutW(330, 530, sth1), pDC->TextOutW(390, 530, L"< mu <"), pDC->TextOutW(450, 530, sth2);
				DisplayRectangleTwoTailed(pDoc, pDC, nWt, Temp2, Temp1, L"confidence", pDoc->show_delta_x, nHt, 'Z');
			}
			pDC->TextOutW(20, 550, _T("Decision")), pDC->TextOutW(250, 550, L":"), pDC->TextOutW(330, 550, pDoc->show_decision);


		}




		else {
			pDC->TextOutW(20, 410, _T("Equal or Uneuqal Variance")), pDC->TextOutW(250, 410, L":"), pDC->TextOutW(330, 410, pDoc->show_var_equal_unequal);
			if (pDoc->show_tail == "Two-Tailed Test") {
				pDC->TextOutW(20, 430, _T("Type")), pDC->TextOutW(250, 430, L":"), pDC->TextOutW(330, 430, L"T-Test / Two-Tailed");
			}
			else if (pDoc->show_tail == "Right-Tailed Test") {
				pDC->TextOutW(20, 430, _T("Type")), pDC->TextOutW(250, 430, L":"), pDC->TextOutW(330, 430, L"T-Test / Right-Tailed");
			}
			else {
				pDC->TextOutW(20, 430, _T("Type")), pDC->TextOutW(250, 430, L":"), pDC->TextOutW(330, 430, L"T-Test / Left-Tailed");
			}

			Temp1 = pDoc->show_F;
			sth1.Format(L"%f", Temp1);
			pDC->TextOutW(20, 450, _T("F")), pDC->TextOutW(250, 450, L":"), pDC->TextOutW(330, 450, sth1);

			if (pDoc->show_tail == "Two-Tailed Test") {
				Temp1 = pDoc->show_Fu;
				Temp2 = pDoc->show_Fl;

				sth1.Format(L"%.2f", Temp1);
				sth2.Format(L"%.2f", Temp2);

				pDC->TextOutW(20, 470, _T("F Critical Region")), pDC->TextOutW(250, 470, L":"), pDC->TextOutW(330, 470, L"f <"), pDC->TextOutW(360, 470, sth2), pDC->TextOutW(400, 470, L" or f >"), pDC->TextOutW(450, 470, sth1);

			}
			else if (pDoc->show_tail == "Right-Tailed Test") {
				Temp1 = pDoc->show_Fu;
				sth1.Format(L"%.2f", Temp1);
				pDC->TextOutW(20, 470, _T("F Critical Region")), pDC->TextOutW(250, 470, L":"), pDC->TextOutW(330, 470, L"f > "), pDC->TextOutW(360, 470, sth1);
			}
			else {
				Temp1 = pDoc->show_Fl;
				sth1.Format(L"%.2f", Temp1);
				pDC->TextOutW(20, 470, _T("F Critical Region")), pDC->TextOutW(250, 470, L":"), pDC->TextOutW(330, 470, L"f < "), pDC->TextOutW(360, 470, sth1);
			}

			if (pDoc->show_var_equal_unequal == "Equal") {
				Temp1 = pDoc->show_T;
				sth1.Format(L"%f", Temp1);
				Temp2 = pDoc->show_SP;
				sth2.Format(L"%f", Temp2);
				pDC->TextOutW(20, 490, _T("SP")), pDC->TextOutW(250, 490, L":"), pDC->TextOutW(330, 490, sth2);
				pDC->TextOutW(20, 510, _T("T")), pDC->TextOutW(250, 510, L":"), pDC->TextOutW(330, 510, sth1);

				Temp1 = pDoc->show_p_value;
				sth1.Format(L"%f", Temp1);
				Temp2 = pDoc->show_tcrit;
				sth2.Format(L"%f", Temp2);
				pDC->TextOutW(20, 530, _T("tcrit")), pDC->TextOutW(250, 530, L":"), pDC->TextOutW(330, 530, sth2);
				pDC->TextOutW(20, 550, _T("p value")), pDC->TextOutW(250, 550, L":"), pDC->TextOutW(330, 550, sth1);


				if (pDoc->show_tail == "Two-Tailed Test") {
					Temp1 = pDoc->show_critical_L;
					Temp2 = pDoc->show_critical_U;

					sth1.Format(L"%.2f", Temp1);
					sth2.Format(L"%.2f", Temp2);
					pDC->TextOutW(20, 570, _T("T Critical Region")), pDC->TextOutW(250, 570, L":"), pDC->TextOutW(330, 570, L"xbar"), pDC->TextOutW(370, 570, L"<"), pDC->TextOutW(390, 570, sth2), pDC->TextOutW(450, 570, L" or xbar >"), pDC->TextOutW(530, 570, sth1);
					DisplayRectangleTwoTailed(pDoc, pDC, nWt, Temp2, Temp1, L"critical", pDoc->show_delta_x, nHt, 'T');

					Temp1 = pDoc->show_confidence_L;
					Temp2 = pDoc->show_confidence_U;

					sth1.Format(L"%.2f", Temp1);
					sth2.Format(L"%.2f", Temp2);
					pDC->TextOutW(20, 590, _T("T Confidence Interval")), pDC->TextOutW(250, 590, L":"), pDC->TextOutW(330, 590, sth1), pDC->TextOutW(390, 590, L"< mu <"), pDC->TextOutW(450, 590, sth2);
					DisplayRectangleTwoTailed(pDoc, pDC, nWt, Temp2, Temp1, L"confidence", pDoc->show_delta_x, nHt, 'T');


				}
				else if (pDoc->show_tail == "Right-Tailed Test") {
					Temp1 = pDoc->show_critical_U;
					sth1.Format(L"%.2f", Temp1);
					Temp2 = pDoc->show_mu - (Temp1 - pDoc->show_mu);
					pDC->TextOutW(20, 570, _T("T Critical Region")), pDC->TextOutW(250, 570, L":"), pDC->TextOutW(330, 570, L"xbar >"), pDC->TextOutW(380, 570, sth1);

					DisplayRectangleRightTailed(pDoc, pDC, nWt, Temp1, Temp2, L"critical", pDoc->show_delta_x, nHt, 'T');


					Temp1 = pDoc->show_confidence_L;
					sth1.Format(L"%.2f", Temp1);

					pDC->TextOutW(20, 590, _T("T Confidence Interval")), pDC->TextOutW(250, 590, L":"), pDC->TextOutW(330, 590, L"mu >"), pDC->TextOutW(380, 590, sth1);

					DisplayRectangleRightTailed(pDoc, pDC, nWt, 0.0, Temp1, L"confidence", pDoc->show_delta_x, nHt, 'T');



				}
				else {
					Temp1 = pDoc->show_critical_L;
					sth1.Format(L"%.2f", Temp1);
					pDC->TextOutW(20, 570, _T("T Critical Region")), pDC->TextOutW(250, 570, L":"), pDC->TextOutW(330, 570, L"xbar <"), pDC->TextOutW(380, 570, sth1);
					Temp2 = pDoc->show_mu + (pDoc->show_mu - Temp1);
					DisplayRectangleLeftTailed(pDoc, pDC, nWt, Temp2, Temp1, L"critical", pDoc->show_delta_x, nHt, 'T');

					Temp1 = pDoc->show_confidence_U;
					sth1.Format(L"%.2f", Temp1);
					pDC->TextOutW(20, 590, _T("T Confidence Interval")), pDC->TextOutW(250, 590, L":"), pDC->TextOutW(330, 590, L"mu <"), pDC->TextOutW(370, 590, sth1);
					DisplayRectangleLeftTailed(pDoc, pDC, nWt, Temp1, 0.0, L"confidence", pDoc->show_delta_x, nHt, 'T');



				}
				Temp1 = pDoc->show_delta_x;
				sth1.Format(L"%.2f", Temp1);
				pDC->TextOutW(20, 610, _T("delta x")), pDC->TextOutW(250, 610, L":"), pDC->TextOutW(330, 610, sth1);
				pDC->TextOutW(20, 630, _T("Decision")), pDC->TextOutW(250, 630, L":"), pDC->TextOutW(330, 630, pDoc->show_decision);
			}

			else {
				Temp1 = pDoc->show_T;
				sth1.Format(L"%f", Temp1);
				Temp2 = pDoc->show_SP;
				sth2.Format(L"%f", Temp2);
				pDC->TextOutW(20, 490, _T("V")), pDC->TextOutW(250, 490, L":"), pDC->TextOutW(330, 490, sth2);
				pDC->TextOutW(20, 510, _T("T")), pDC->TextOutW(250, 510, L":"), pDC->TextOutW(330, 510, sth1);

				Temp1 = pDoc->show_p_value;
				sth1.Format(L"%f", Temp1);
				Temp2 = pDoc->show_tcrit;
				sth2.Format(L"%f", Temp2);
				pDC->TextOutW(20, 530, _T("tcrit")), pDC->TextOutW(250, 530, L":"), pDC->TextOutW(330, 530, sth2);
				pDC->TextOutW(20, 550, _T("p value")), pDC->TextOutW(250, 550, L":"), pDC->TextOutW(330, 550, sth1);


				if (pDoc->show_tail == "Two-Tailed Test") {
					Temp1 = pDoc->show_critical_L;
					Temp2 = pDoc->show_critical_U;
					sth1.Format(L"%.2f", Temp1);
					sth2.Format(L"%.2f", Temp2);
					pDC->TextOutW(20, 570, _T("T Critical Region")), pDC->TextOutW(250, 570, L":"), pDC->TextOutW(330, 570, L"xbar"), pDC->TextOutW(370, 570, L"<"), pDC->TextOutW(390, 570, sth2), pDC->TextOutW(450, 570, L" or xbar >"), pDC->TextOutW(530, 570, sth1);




					DisplayRectangleTwoTailed(pDoc, pDC, nWt, Temp2, Temp1, L"critical", pDoc->show_delta_x, nHt, 'T');

					Temp1 = pDoc->show_confidence_L;
					Temp2 = pDoc->show_confidence_U;

					sth1.Format(L"%.2f", Temp1);
					sth2.Format(L"%.2f", Temp2);
					pDC->TextOutW(20, 590, _T("T Confidence Interval")), pDC->TextOutW(250, 590, L":"), pDC->TextOutW(330, 590, sth1), pDC->TextOutW(390, 590, L"< mu <"), pDC->TextOutW(450, 590, sth2);
					DisplayRectangleTwoTailed(pDoc, pDC, nWt, Temp2, Temp1, L"confidence", pDoc->show_delta_x, nHt, 'Z');

				}
				else if (pDoc->show_tail == "Right-Tailed Test") {
					Temp1 = pDoc->show_critical_U;
					Temp2 = pDoc->show_mu - (Temp1 - pDoc->show_mu);


					sth1.Format(L"%.2f", Temp1);
					pDC->TextOutW(20, 570, _T("T Critical Region")), pDC->TextOutW(250, 570, L":"), pDC->TextOutW(330, 570, L"xbar >"), pDC->TextOutW(380, 570, sth1);

					DisplayRectangleRightTailed(pDoc, pDC, nWt, Temp1, Temp2, L"critical", pDoc->show_delta_x, nHt, 'T');



					Temp1 = pDoc->show_confidence_L;
					sth1.Format(L"%.2f", Temp1);
					pDC->TextOutW(20, 590, _T("T Confidence Interval")), pDC->TextOutW(250, 590, L":"), pDC->TextOutW(330, 590, L"mu >"), pDC->TextOutW(380, 590, sth1);

					DisplayRectangleRightTailed(pDoc, pDC, nWt, 0.0, Temp1, L"confidence", pDoc->show_delta_x, nHt, 'T');


				}
				else {
					Temp1 = pDoc->show_critical_L;
					sth1.Format(L"%.2f", Temp1);
					Temp2 = pDoc->show_mu + (pDoc->show_mu - Temp1);
					DisplayRectangleLeftTailed(pDoc, pDC, nWt, Temp2, Temp1, L"critical", pDoc->show_delta_x, nHt, 'T');
					pDC->TextOutW(20, 570, _T("T Critical Region")), pDC->TextOutW(250, 570, L":"), pDC->TextOutW(330, 570, L"xbar <"), pDC->TextOutW(380, 570, sth1);
					Temp2 = pDoc->show_mu - Temp1;


					Temp1 = pDoc->show_confidence_U;
					sth1.Format(L"%.2f", Temp1);
					pDC->TextOutW(20, 590, _T("T Confidence Interval")), pDC->TextOutW(250, 590, L":"), pDC->TextOutW(330, 590, L"mu <"), pDC->TextOutW(380, 590, sth1);
					DisplayRectangleLeftTailed(pDoc, pDC, nWt, Temp1, 0.0, L"confidence", pDoc->show_delta_x, nHt, 'T');



				}
				Temp1 = pDoc->show_delta_x;
				sth1.Format(L"%.2f", Temp1);
				pDC->TextOutW(20, 610, _T("delta x")), pDC->TextOutW(250, 610, L":"), pDC->TextOutW(330, 610, sth1);
				pDC->TextOutW(20, 630, _T("Decision")), pDC->TextOutW(250, 630, L":"), pDC->TextOutW(330, 630, pDoc->show_decision);
			}
		}

		CPoint TopLeft, BottomRight;
		TopLeft.x = 550;
		TopLeft.y = 50;


		BottomRight.x = TopLeft.x + 330 * nWt / 1920;
		BottomRight.y = TopLeft.y + 330 * nHt / 1042;



		double top, bot; // top is the largest value in the two samples, while bot is the smallest value in the two samples
		double top1, top2, bot1, bot2; // top1 is the largest value in sample 1, and top2 is the largest value in sample 2.
		top1 = pDoc->show_max1;
		top2 = pDoc->show_max2;
		bot1 = pDoc->minimum_sample1;
		bot2 = pDoc->minimum_sample2;

		if (top1 > top2) {
			top = top1;
		}
		else {
			top = top2;
		}

		if (bot1 > bot2) {
			bot = bot2;
		}
		else {
			bot = bot1;
		}

		

		TwoSampleBoxPlot(pDoc, pDC, nWt, nHt, pDoc->MAX_BOX, pDoc->MIN_BOX, pDoc->up2, pDoc->low2, pDoc->med2, pDoc->OutlierArray2, pDoc->ol2, TopLeft, BottomRight, pDoc->show_class2, top, bot);


		TopLeft.x += 350 * nWt / 1920;
		BottomRight.x = TopLeft.x + 330 * nWt / 1920;
		TwoSampleBoxPlot(pDoc, pDC, nWt, nHt, pDoc->max_box, pDoc->min_box, pDoc->up1, pDoc->low1, pDoc->med1, pDoc->OutlierArray1, pDoc->ol1, TopLeft, BottomRight, pDoc->show_class1, top, bot);

		Temp1 = pDoc->show_max1;
		Temp2 = pDoc->show_max2;
		sth1.Format(L"%.2f", Temp1);
		sth2.Format(L"%.2f", Temp2);
		pDC->TextOutW(20, 230, L"max:"), pDC->TextOutW(170, 230, sth1), pDC->TextOutW(270, 230, L"max:"), pDC->TextOutW(420, 230, sth2);

		Temp1 = pDoc->minimum_sample1;
		Temp2 = pDoc->minimum_sample2;
		sth1.Format(L"%.2f", Temp1);
		sth2.Format(L"%.2f", Temp2);
		pDC->TextOutW(20, 250, L"min:"), pDC->TextOutW(170, 250, sth1), pDC->TextOutW(270, 250, L"min:"), pDC->TextOutW(420, 250, sth2);





		BottomRight.x = TopLeft.x + 300 * nWt / 1920;
		TopLeft.x = 570;
		TopLeft.y = 500;
		BottomRight.y = TopLeft.y + 330 * nHt / 1042;

		TwoSampleSequencePlot(pDoc, pDC, pDoc->OverallArray, TopLeft, BottomRight, Text, pDoc->show_class1, pDoc->show_class2);


	}

	else if (pDoc->ANOVA) {
		DisplayPCA.ShowWindow(SW_HIDE);
		DisplayLoadingPlots.ShowWindow(SW_HIDE);
		DisplayScorePlots.ShowWindow(SW_HIDE);
		SelectScatterPlots.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_HIDE);
		DisplayFactorLoadings.ShowWindow(SW_HIDE);
		DisplayFactorScores.ShowWindow(SW_HIDE);
	}
	else if (pDoc->PCA_Select_PCs) {
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		DisplayPCA.ShowWindow(SW_HIDE);
		SelectScatterPlots.ShowWindow(SW_HIDE);
		DisplayLoadingPlots.ShowWindow(SW_HIDE);
		DisplayScorePlots.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_HIDE);
		DisplayFactorLoadings.ShowWindow(SW_HIDE);
		DisplayFactorScores.ShowWindow(SW_HIDE);
		if ( (pDoc->Problem == _T("Regression problem")) || (pDoc->Problem == _T("Analysis problem"))) DisplayFileAndDataSetInformation(pDoc, pDC, true);
		else DisplayFileAndDataSetInformation(pDoc, pDC, false);
		TopLeftCorner.x = 650, TopLeftCorner.y = 50;
		int Top_Y = DisplayCorrelationMatrix(pDoc, pDC, TopLeftCorner);
		TopLeftCorner.x = 650, TopLeftCorner.y = Top_Y + 30;
		BottomRightCorner.x = 650 + (int)((nWt - 650) / 2) - 25, BottomRightCorner.y = nHt - Offset;
		DisplayEigenvaluesAndCumVariance(pDoc, pDC, TopLeftCorner, BottomRightCorner, true);
		TopLeftCorner.x = 700 + (int)((nWt - 650) / 2), BottomRightCorner.x = nWt - 50;
		DisplayStoppingRules(pDoc, pDC, TopLeftCorner, BottomRightCorner);
	}
	else if (pDoc->PCA_Display_PCs_Standard) {
		DisplayPCA.ShowWindow(SW_SHOW);
		DisplayLoadingPlots.ShowWindow(SW_SHOW);
		DisplayScorePlots.ShowWindow(SW_SHOW);
		if (pDoc->n > 3) {
			Next_Variable.ShowWindow(SW_SHOW);
			Previous_Variable.ShowWindow(SW_SHOW);
		}
		else {
			Next_Variable.ShowWindow(SW_HIDE);
			Previous_Variable.ShowWindow(SW_HIDE);
		}
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		SelectScatterPlots.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_HIDE);
		DisplayFactorLoadings.ShowWindow(SW_HIDE);
		DisplayFactorScores.ShowWindow(SW_HIDE);
		TopLeftCorner.x = 50;
		BottomRightCorner.x = (int)(0.3 * nWt);
		int k = 0;
		CRect rect(20, 20, 250, 58);
		pDC->Rectangle(rect);
		CBrush Brush_Header;
		Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
		pDC->FillRect(&rect, &Brush_Header);
		pDC->SetBkMode(TRANSPARENT);
		CFont Standard_Font;
		Standard_Font.CreateStockObject(SYSTEM_FONT);
		pDC->SelectObject(Standard_Font);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(30, 30, _T("Loading vectors"));
		CRect Method_Box_Outside(445, 15, 805, 63), Method_Box_Inside(450,20,800,58);
		pDC->Rectangle(Method_Box_Outside);
		CBrush Method_Frame, Method_Inside;
		Method_Frame.CreateSolidBrush(RGB(243, 136, 10));
		pDC->FillRect(&Method_Box_Outside, &Method_Frame);
		Method_Inside.CreateSolidBrush(RGB(243, 236, 10));
		pDC->Rectangle(Method_Box_Inside);
		pDC->FillRect(&Method_Box_Inside, &Method_Inside);
		pDC->SetTextAlign(TA_CENTER);
		pDC->TextOutW(625, 30, _T("Principal Component Analysis"));
		CString Text, Text_temp;
		CArray <double> Loadings;
		int max_plots = 3, nVar;
		if (pDoc->OnlyX) nVar = pDoc->n_Var - 1;
		else nVar = pDoc->n_Var;
		if (counter * 3 > pDoc->n) counter = (int)(pDoc->n / max_plots);
		if (counter < 0) counter = 0;
		if (pDoc->n < 3) max_plots = pDoc->n;
		int dY = (int)((nHt - Offset - 65) / max_plots);
		for (int i = counter * max_plots; i < (counter + 1) * max_plots; i++) {
			TopLeftCorner.y = 65 + k * dY;
			BottomRightCorner.y = 50 + (k + 1) * dY;
			if (i < pDoc->n) {
				Loadings.RemoveAll();
				for (int j = 0; j < nVar; j++) Loadings.Add(pDoc->P.GetAt(pDoc->GetPosition(j, i, pDoc->P_spec)));
				Text.Empty(), Text_temp.Empty(), Text.Append(_T("Loading vector ")), Text_temp.Format(L"%d", i + 1), Text.Append(Text_temp);
				DisplayLoadings(pDoc, pDC, Loadings, TopLeftCorner, BottomRightCorner, Text);
			}
			k++;
		}
		TopLeftCorner.x = (int)(0.3 * nWt) + 50, TopLeftCorner.y = 70;
		BottomRightCorner.x = (int)(0.6 * nWt), BottomRightCorner.y = nHt - Offset - 70;
		DisplayUnivariateStatistics(pDoc, pDC, TopLeftCorner, BottomRightCorner);
		TopLeftCorner.x = (int)(0.6 * nWt) + 20, TopLeftCorner.y = DisplayPCAModelResults(pDoc, pDC, TopLeftCorner) + 20;
		BottomRightCorner.x = nWt - 20, BottomRightCorner.y = nHt - Offset;
		DisplayEigenvaluesAndCumVariance(pDoc, pDC, TopLeftCorner, BottomRightCorner, false);
	}	
	else if (pDoc->PCA_Display_Loadings) {
		Type = false;
		if (pDoc->n > 3) {
			Next_Variable.ShowWindow(SW_SHOW);
			Previous_Variable.ShowWindow(SW_SHOW);
		}
		else {
			Next_Variable.ShowWindow(SW_HIDE);
			Previous_Variable.ShowWindow(SW_HIDE);
		}
		DisplayPCA.ShowWindow(SW_SHOW);
		DisplayLoadingPlots.ShowWindow(SW_SHOW);
		DisplayScorePlots.ShowWindow(SW_SHOW);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_HIDE);
		DisplayFactorLoadings.ShowWindow(SW_HIDE);
		DisplayFactorScores.ShowWindow(SW_HIDE);
		if (n >= 3) SelectScatterPlots.ShowWindow(SW_SHOW);
		else SelectScatterPlots.ShowWindow(SW_HIDE);
		TopLeftCorner.x = 50;
		BottomRightCorner.x = nWt - 20 - (nHt - Offset - 70);
		int k = 0;
		CRect rect(20, 20, 250, 58);
		pDC->Rectangle(rect);
		CBrush Brush_Header;
		Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
		pDC->FillRect(&rect, &Brush_Header);
		pDC->SetBkMode(TRANSPARENT);
		CFont Standard_Font;
		Standard_Font.CreateStockObject(SYSTEM_FONT);
		pDC->SelectObject(Standard_Font);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(30, 30, _T("Loading vectors"));
		CRect Method_Box_Outside(445, 15, 805, 63), Method_Box_Inside(450, 20, 800, 58);
		pDC->Rectangle(Method_Box_Outside);
		CBrush Method_Frame, Method_Inside;
		Method_Frame.CreateSolidBrush(RGB(243, 136, 10));
		pDC->FillRect(&Method_Box_Outside, &Method_Frame);
		Method_Inside.CreateSolidBrush(RGB(243, 236, 10));
		pDC->Rectangle(Method_Box_Inside);
		pDC->FillRect(&Method_Box_Inside, &Method_Inside);
		pDC->SetTextAlign(TA_CENTER);
		pDC->TextOutW(625, 30, _T("Principal Component Analysis"));
		CString Text, Text_temp;
		CArray <double> Loadings;
		int max_plots = 3;
		if (counter * 3 > pDoc->n) counter = (int)(pDoc->n / max_plots);
		if (counter < 0) counter = 0;
		if (pDoc->n < 3) max_plots = pDoc->n;
		int dY = (int)((nHt - Offset - 65) / max_plots);
		for (int i = counter * max_plots; i < (counter + 1) * max_plots; i++) {
			TopLeftCorner.y = 65 + k * dY;
			BottomRightCorner.y = 50 + (k + 1) * dY;
			if (i < pDoc->n) {
				Loadings.RemoveAll();
				if (pDoc->OnlyX == true) {
					for (int j = 0; j < pDoc->n_Var - 1; j++) Loadings.Add(pDoc->P.GetAt(pDoc->GetPosition(j, i, pDoc->P_spec)));
				}
				else {
					for (int j = 0; j < pDoc->n_Var; j++) Loadings.Add(pDoc->P.GetAt(pDoc->GetPosition(j, i, pDoc->P_spec)));
				}
				Text.Empty(), Text_temp.Empty(), Text.Append(_T("Loading vector ")), Text_temp.Format(L"%d", i + 1), Text.Append(Text_temp);
				DisplayLoadings(pDoc, pDC, Loadings, TopLeftCorner, BottomRightCorner, Text);
			}
			k++;
		}
		TopLeftCorner.x = nWt - 20 - (nHt - Offset - 70), TopLeftCorner.y = 70;
		BottomRightCorner.x = nWt - 20, BottomRightCorner.y = nHt - Offset;
		DisplayScatterDiagrams(pDoc, pDC, TopLeftCorner, BottomRightCorner, Type, true, _T("PCA"));
	}
	else if (pDoc->PCA_Display_Scores) {
		Type = true;
		if (pDoc->n > 3) {
			Next_Variable.ShowWindow(SW_SHOW);
			Previous_Variable.ShowWindow(SW_SHOW);
		}
		else {
			Next_Variable.ShowWindow(SW_HIDE);
			Previous_Variable.ShowWindow(SW_HIDE);
		}
		DisplayPCA.ShowWindow(SW_SHOW);
		DisplayLoadingPlots.ShowWindow(SW_SHOW);
		DisplayScorePlots.ShowWindow(SW_SHOW);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_HIDE);
		DisplayFactorLoadings.ShowWindow(SW_HIDE);
		DisplayFactorScores.ShowWindow(SW_HIDE);
		if (n >= 3) SelectScatterPlots.ShowWindow(SW_SHOW);
		else SelectScatterPlots.ShowWindow(SW_HIDE);
		TopLeftCorner.x = 50;
		BottomRightCorner.x = nWt - 20 - (nHt - Offset - 70);
		int k = 0;
		CRect rect(20, 20, 250, 58);
		pDC->Rectangle(rect);
		CBrush Brush_Header;
		Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
		pDC->FillRect(&rect, &Brush_Header);
		pDC->SetBkMode(TRANSPARENT);
		CFont Standard_Font;
		Standard_Font.CreateStockObject(SYSTEM_FONT);
		pDC->SelectObject(Standard_Font);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(30, 30, _T("Score variables"));
		CRect Method_Box_Outside(445, 15, 805, 63), Method_Box_Inside(450, 20, 800, 58);
		pDC->Rectangle(Method_Box_Outside);
		CBrush Method_Frame, Method_Inside;
		Method_Frame.CreateSolidBrush(RGB(243, 136, 10));
		pDC->FillRect(&Method_Box_Outside, &Method_Frame);
		Method_Inside.CreateSolidBrush(RGB(243, 236, 10));
		pDC->Rectangle(Method_Box_Inside);
		pDC->FillRect(&Method_Box_Inside, &Method_Inside);
		pDC->SetTextAlign(TA_CENTER);
		pDC->TextOutW(625, 30, _T("Principal Component Analysis"));
		CString Text, Text_temp;
		CArray <double> Scores;
		int max_plots = 3;
		if (counter * 3 > pDoc->n) counter = (int)(pDoc->n / max_plots);
		if (counter < 0) counter = 0;
		if (pDoc->n < 3) max_plots = pDoc->n;
		int dY = (int)((nHt - Offset - 65) / max_plots);
		for (int i = counter * max_plots; i < (counter + 1) * max_plots; i++) {
			TopLeftCorner.y = 65 + k * dY;
			BottomRightCorner.y = 50 + (k + 1) * dY;
			if (i < pDoc->n) {
				Scores.RemoveAll();
				for (int j = 0; j < pDoc->n_Obs; j++) Scores.Add(pDoc->T.GetAt(pDoc->GetPosition(j, i, pDoc->T_spec)));
				Text.Empty(), Text_temp.Empty(), Text.Append(_T("Score variable ")), Text_temp.Format(L"%d", i + 1), Text.Append(Text_temp);
				DisplayScores(pDoc, pDC, Scores, TopLeftCorner, BottomRightCorner, Text);
			}
			k++;
		}
		TopLeftCorner.x = nWt - 20 - (nHt - Offset - 70), TopLeftCorner.y = 70;
		BottomRightCorner.x = nWt - 20, BottomRightCorner.y = nHt - Offset;
		DisplayScatterDiagrams(pDoc, pDC, TopLeftCorner, BottomRightCorner, Type, true, _T("PCA"));
	}
	else if (pDoc->FA_Display_Standard) {
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
		if (pDoc->n > 3) {
			Next_Variable.ShowWindow(SW_SHOW);
			Previous_Variable.ShowWindow(SW_SHOW);
		}
		else {
			Next_Variable.ShowWindow(SW_HIDE);
			Previous_Variable.ShowWindow(SW_HIDE);
		}
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		SelectScatterPlots.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_SHOW);
		DisplayFactorLoadings.ShowWindow(SW_SHOW);
		DisplayFactorScores.ShowWindow(SW_SHOW);
		DisplayFactorMatrices.ShowWindow(SW_SHOW);
		if (pDoc->n > 3) {
			Next_Variable.ShowWindow(SW_SHOW);
			Previous_Variable.ShowWindow(SW_SHOW);
		}
		else {
			Next_Variable.ShowWindow(SW_HIDE);
			Previous_Variable.ShowWindow(SW_HIDE);
		}
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		SelectScatterPlots.ShowWindow(SW_HIDE);
		TopLeftCorner.x = 50;
		BottomRightCorner.x = (int)(0.3 * nWt);
		int k = 0;
		CRect rect(20, 20, 250, 58);
		pDC->Rectangle(rect);
		CBrush Brush_Header;
		Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
		pDC->FillRect(&rect, &Brush_Header);
		pDC->SetBkMode(TRANSPARENT);
		CFont Standard_Font;
		Standard_Font.CreateStockObject(SYSTEM_FONT);
		pDC->SelectObject(Standard_Font);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(30, 30, _T("Factor loadings"));
		CRect Method_Box_Outside(445, 15, 805, 63), Method_Box_Inside(450, 20, 800, 58);
		pDC->Rectangle(Method_Box_Outside);
		CBrush Method_Frame, Method_Inside;
		Method_Frame.CreateSolidBrush(RGB(243, 136, 10));
		pDC->FillRect(&Method_Box_Outside, &Method_Frame);
		Method_Inside.CreateSolidBrush(RGB(243, 236, 10));
		pDC->Rectangle(Method_Box_Inside);
		pDC->FillRect(&Method_Box_Inside, &Method_Inside);
		pDC->SetTextAlign(TA_CENTER);
		pDC->TextOutW(625, 30, _T("Factor Analysis"));
		CString Text, Text_temp;
		CArray <double> Loadings;
		int max_plots = 3;
		if (counter * 3 > pDoc->n) counter = (int)(pDoc->n / max_plots);
		if (counter < 0) counter = 0;
		if (pDoc->n < 3) max_plots = pDoc->n;
		int dY = (int)((nHt - Offset - 65) / max_plots), nVar;
		for (int i = counter * max_plots; i < (counter + 1) * max_plots; i++) {
			TopLeftCorner.y = 65 + k * dY;
			BottomRightCorner.y = 50 + (k + 1) * dY;
			if (i < pDoc->n) {
				Loadings.RemoveAll();
				if (pDoc->OnlyX == true) nVar = pDoc->n_Var - 1;
				else nVar = pDoc->n_Var;
				for (int j = 0; j < nVar; j++) Loadings.Add(pDoc->A.GetAt(pDoc->GetPosition(j, i, pDoc->A_spec)));
				Text.Empty(), Text_temp.Empty(), Text.Append(_T("Loading vector ")), Text_temp.Format(L"%d", i + 1), Text.Append(Text_temp);
				DisplayLoadings(pDoc, pDC, Loadings, TopLeftCorner, BottomRightCorner, Text);
			}
			k++;
		}
		TopLeftCorner.x = BottomRightCorner.x + 20, TopLeftCorner.y = 70;
		TopLeftCorner.x = DisplayCovarianceMatrices(pDoc, pDC, TopLeftCorner, _T("Se"));
		DisplayFactorAnalysisModel(pDoc, pDC, TopLeftCorner);
	}
	else if (pDoc->FA_Display_Loadings) {
		Type = false;
		if (pDoc->n > 3) {
			Next_Variable.ShowWindow(SW_SHOW);
			Previous_Variable.ShowWindow(SW_SHOW);
		}
		else {
			Next_Variable.ShowWindow(SW_HIDE);
			Previous_Variable.ShowWindow(SW_HIDE);
		}
		DisplayPCA.ShowWindow(SW_HIDE);
		DisplayLoadingPlots.ShowWindow(SW_HIDE);
		DisplayScorePlots.ShowWindow(SW_HIDE);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_SHOW);
		DisplayFactorLoadings.ShowWindow(SW_SHOW);
		DisplayFactorScores.ShowWindow(SW_SHOW);
		DisplayFactorMatrices.ShowWindow(SW_HIDE);
		if (n >= 3) SelectScatterPlots.ShowWindow(SW_SHOW);
		else SelectScatterPlots.ShowWindow(SW_HIDE);
		TopLeftCorner.x = 50;
		BottomRightCorner.x = nWt - 20 - (nHt - Offset - 70);
		int k = 0;
		CRect rect(20, 20, 250, 58);
		pDC->Rectangle(rect);
		CBrush Brush_Header;
		Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
		pDC->FillRect(&rect, &Brush_Header);
		pDC->SetBkMode(TRANSPARENT);
		CFont Standard_Font;
		Standard_Font.CreateStockObject(SYSTEM_FONT);
		pDC->SelectObject(Standard_Font);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(30, 30, _T("Factor loadings"));
		CRect Method_Box_Outside(445, 15, 805, 63), Method_Box_Inside(450, 20, 800, 58);
		pDC->Rectangle(Method_Box_Outside);
		CBrush Method_Frame, Method_Inside;
		Method_Frame.CreateSolidBrush(RGB(243, 136, 10));
		pDC->FillRect(&Method_Box_Outside, &Method_Frame);
		Method_Inside.CreateSolidBrush(RGB(243, 236, 10));
		pDC->Rectangle(Method_Box_Inside);
		pDC->FillRect(&Method_Box_Inside, &Method_Inside);
		pDC->SetTextAlign(TA_CENTER);
		pDC->TextOutW(625, 30, _T("Factor Analysis"));
		CString Text, Text_temp;
		CArray <double> Loadings;
		int max_plots = 3;
		if (counter * 3 > pDoc->n) counter = (int)(pDoc->n / max_plots);
		if (counter < 0) counter = 0;
		if (pDoc->n < 3) max_plots = pDoc->n;
		int dY = (int)((nHt - Offset - 65) / max_plots);
		for (int i = counter * max_plots; i < (counter + 1) * max_plots; i++) {
			TopLeftCorner.y = 65 + k * dY;
			BottomRightCorner.y = 50 + (k + 1) * dY;
			if (i < pDoc->n) {
				Loadings.RemoveAll();
				if (pDoc->OnlyX == true) {
					for (int j = 0; j < pDoc->n_Var - 1; j++) Loadings.Add(pDoc->A.GetAt(pDoc->GetPosition(j, i, pDoc->A_spec)));
				}
				else {
					for (int j = 0; j < pDoc->n_Var; j++) Loadings.Add(pDoc->A.GetAt(pDoc->GetPosition(j, i, pDoc->A_spec)));
				}
				Text.Empty(), Text_temp.Empty(), Text.Append(_T("Loading vector ")), Text_temp.Format(L"%d", i + 1), Text.Append(Text_temp);
				DisplayLoadings(pDoc, pDC, Loadings, TopLeftCorner, BottomRightCorner, Text);
			}
			k++;
		}
		TopLeftCorner.x = nWt - 20 - (nHt - Offset - 70), TopLeftCorner.y = 70;
		BottomRightCorner.x = nWt - 20, BottomRightCorner.y = nHt - Offset;
		DisplayScatterDiagrams(pDoc, pDC, TopLeftCorner, BottomRightCorner, Type, true, _T("FA"));
	}
	else if (pDoc->FA_Display_Scores) {
		Type = true;
		if (pDoc->n > 3) {
			Next_Variable.ShowWindow(SW_SHOW);
			Previous_Variable.ShowWindow(SW_SHOW);
		}
		else {
			Next_Variable.ShowWindow(SW_HIDE);
			Previous_Variable.ShowWindow(SW_HIDE);
		}
		DisplayPCA.ShowWindow(SW_HIDE);
		DisplayLoadingPlots.ShowWindow(SW_HIDE);
		DisplayScorePlots.ShowWindow(SW_HIDE);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_SHOW);
		DisplayFactorLoadings.ShowWindow(SW_SHOW);
		DisplayFactorScores.ShowWindow(SW_SHOW);
		DisplayFactorMatrices.ShowWindow(SW_HIDE);
		if (n >= 3) SelectScatterPlots.ShowWindow(SW_SHOW);
		else SelectScatterPlots.ShowWindow(SW_HIDE);
		TopLeftCorner.x = 50;
		BottomRightCorner.x = nWt - 20 - (nHt - Offset - 70);
		int k = 0;
		CRect rect(20, 20, 250, 58);
		pDC->Rectangle(rect);
		CBrush Brush_Header;
		Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
		pDC->FillRect(&rect, &Brush_Header);
		pDC->SetBkMode(TRANSPARENT);
		CFont Standard_Font;
		Standard_Font.CreateStockObject(SYSTEM_FONT);
		pDC->SelectObject(Standard_Font);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(30, 30, _T("Score variables"));
		CRect Method_Box_Outside(445, 15, 805, 63), Method_Box_Inside(450, 20, 800, 58);
		pDC->Rectangle(Method_Box_Outside);
		CBrush Method_Frame, Method_Inside;
		Method_Frame.CreateSolidBrush(RGB(243, 136, 10));
		pDC->FillRect(&Method_Box_Outside, &Method_Frame);
		Method_Inside.CreateSolidBrush(RGB(243, 236, 10));
		pDC->Rectangle(Method_Box_Inside);
		pDC->FillRect(&Method_Box_Inside, &Method_Inside);
		pDC->SetTextAlign(TA_CENTER);
		pDC->TextOutW(625, 30, _T("Factor Analysis"));
		CString Text, Text_temp;
		CArray <double> Scores;
		int max_plots = 3;
		if (counter * 3 > pDoc->n) counter = (int)(pDoc->n / max_plots);
		if (counter < 0) counter = 0;
		if (pDoc->n < 3) max_plots = pDoc->n;
		int dY = (int)((nHt - Offset - 65) / max_plots);
		for (int i = counter * max_plots; i < (counter + 1) * max_plots; i++) {
			TopLeftCorner.y = 65 + k * dY;
			BottomRightCorner.y = 50 + (k + 1) * dY;
			if (i < pDoc->n) {
				Scores.RemoveAll();
				for (int j = 0; j < pDoc->n_Obs; j++) Scores.Add(pDoc->F.GetAt(pDoc->GetPosition(j, i, pDoc->F_spec)));
				Text.Empty(), Text_temp.Empty(), Text.Append(_T("Score variable ")), Text_temp.Format(L"%d", i + 1), Text.Append(Text_temp);
				DisplayScores(pDoc, pDC, Scores, TopLeftCorner, BottomRightCorner, Text);
			}
			k++;
		}
		TopLeftCorner.x = nWt - 20 - (nHt - Offset - 70), TopLeftCorner.y = 70;
		BottomRightCorner.x = nWt - 20, BottomRightCorner.y = nHt - Offset;
		DisplayScatterDiagrams(pDoc, pDC, TopLeftCorner, BottomRightCorner, Type, true, _T("FA"));
	}
	else if (pDoc->FA_Display_Matrices) {
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		DisplayPCA.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);
		DisplayLoadingPlots.ShowWindow(SW_HIDE);
		DisplayScorePlots.ShowWindow(SW_HIDE);
		DisplayFA.ShowWindow(SW_SHOW);
		DisplayFactorLoadings.ShowWindow(SW_SHOW);
		DisplayFactorScores.ShowWindow(SW_SHOW);
		DisplayFactorMatrices.ShowWindow(SW_SHOW);
	    SelectScatterPlots.ShowWindow(SW_HIDE);
		CRect rect(20, 20, 250, 58);
		pDC->Rectangle(rect);
		CBrush Brush_Header;
		Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
		pDC->FillRect(&rect, &Brush_Header);
		pDC->SetBkMode(TRANSPARENT);
		CFont Standard_Font;
		Standard_Font.CreateStockObject(SYSTEM_FONT);
		pDC->SelectObject(Standard_Font);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(30, 30, _T("Matrices"));
		CRect Method_Box_Outside(445, 15, 805, 63), Method_Box_Inside(450, 20, 800, 58);
		pDC->Rectangle(Method_Box_Outside);
		CBrush Method_Frame, Method_Inside;
		Method_Frame.CreateSolidBrush(RGB(243, 136, 10));
		pDC->FillRect(&Method_Box_Outside, &Method_Frame);
		Method_Inside.CreateSolidBrush(RGB(243, 236, 10));
		pDC->Rectangle(Method_Box_Inside);
		pDC->FillRect(&Method_Box_Inside, &Method_Inside);
		pDC->SetTextAlign(TA_CENTER);
		pDC->TextOutW(625, 30, _T("Factor Analysis"));
		TopLeftCorner.x = 50, TopLeftCorner.y = 70;
		TopLeftCorner.x = DisplayCovarianceMatrices(pDoc, pDC, TopLeftCorner, _T("Sf")) + 20;
		TopLeftCorner.x = DisplayCovarianceMatrices(pDoc, pDC, TopLeftCorner, _T("Se")) + 20;
		CString Label(_T("Rotation matrix"));
		CArray <double> Matrix;
		CArray <int> Matrix_temp;
		Matrix.RemoveAll(), Matrix_temp.RemoveAll();
		Matrix_temp.SetSize(3), Matrix_temp.SetAt(0, pDoc->n), Matrix_temp.SetAt(1, pDoc->n), Matrix_temp.SetAt(2, 0);
		for (int i = 0; i < pDoc->n; i++) {
			for (int j = 0; j < pDoc->n; j++) Matrix.Add(pDoc->RT.GetAt(pDoc->GetPosition(j, i, pDoc->RT_spec)));
		}
		DisplayMatrix(pDoc, pDC, Matrix, Matrix_temp, TopLeftCorner, Label);

	}
	else if (pDoc->FDA) {
		DisplayPCA.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		TopLeftCorner.x = 20, TopLeftCorner.y = 20, BottomRightCorner.x = 100, BottomRightCorner.y = 50;
		pDC->TextOutW(100, 100, _T("Here we go"));
	}
	else if (pDoc->RegressionAnalysis) {
		DisplayPCA.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);
		Ind_Ass_Regression.ShowWindow(SW_SHOW);
		Sta_Ass_Regression.ShowWindow(SW_HIDE);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
		pDC->TextOutW(300, 20, _T("Independent assessment :"));
		TopLeftCorner.x = 500, TopLeftCorner.y = 155;
		BottomRightCorner.x = 500 + (int)((nWt - 500) / 2);
		BottomRightCorner.y = 155 + (int)((nHt - 155 - Offset) / 2 );
		DisplayStandardRegressionAnalysis(pDoc,pDC);
		if (pDoc->ShowStatisticalAnalysis) ShowHypothesisTextingResults(pDoc, pDC, TopLeftCorner, BottomRightCorner);
		TopLeftCorner.y = BottomRightCorner.y, BottomRightCorner.y = nHt - Offset;
		ShowRegressionCoefficients(pDoc, pDC, TopLeftCorner, BottomRightCorner, pDoc->ShowStatisticalAnalysis);
		TopLeftCorner.x = 500 + (int)((nWt - 500) / 2) + 20, TopLeftCorner.y = 10;
		BottomRightCorner.x = nWt, BottomRightCorner.y = (int)((nHt - Offset) / 3);
		ShowModelPrediction(pDoc, pDC, TopLeftCorner, BottomRightCorner);
		TopLeftCorner.x = 500 + (int)((nWt - 500) / 2) + 20, TopLeftCorner.y = (int)((nHt - Offset) / 3) + 10;
		BottomRightCorner.x = nWt, BottomRightCorner.y = (int)(2 * (nHt - Offset) / 3);
		CArray <double> e;
		ShowPredictionError(pDoc, pDC, e, TopLeftCorner, BottomRightCorner);
		TopLeftCorner.x = 500 + (int)((nWt - 500) / 2) + 20, TopLeftCorner.y = (int)(2 * (nHt - Offset) / 3) + 10;
		BottomRightCorner.x = nWt, BottomRightCorner.y = nHt - Offset;
		CString Text(_T("Residual"));
		ShowProbabilityPlot(pDoc, pDC, e, TopLeftCorner, BottomRightCorner, Text);
	}
	else if (pDoc->RegressionAnalysis_Independent) {
		DisplayPCA.ShowWindow(SW_HIDE);
		Next_Variable.ShowWindow(SW_HIDE);
		Previous_Variable.ShowWindow(SW_HIDE);
		Ind_Ass_Regression.ShowWindow(SW_HIDE);
		Sta_Ass_Regression.ShowWindow(SW_SHOW);
		Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
	}
	else if (pDoc->ANN_Training) {
		CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
		// plot the loss and accuracy curve
		DisplayFileAndDataSetInformation(pDoc, pDC, true);
		PlotLossCurve();
		//PlotAccuraciesCurve();
	}
	
}

// *************************************************************
// ***   GENERAL MEMBERS, NOT RELATED TO A SPECIFIC METHOD   ***
// *************************************************************

void CModelingandAnalysisofUncertaintyView::GetTicks(double min, double max, CArray <double>& ticks, CArray <double>& ticklabels) {
	ticks.RemoveAll(), ticklabels.RemoveAll();
	double range = max - min, delta;
	if ((range >= 0.00002) && (range < 0.0002)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = 0.00001;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += 0.00001;
			}
			delta = -0.00001;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.00001;
			}
		}
		else {
			delta = max * (double)100000;
			delta = (double)((int)delta);
			delta /= (double)100000;
			ticklabels.Add(delta);
			delta -= 0.00001;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.00001;
			}
		}
	}
	else if ((range >= 0.0002) && (range < 0.002)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = 0.0001;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += 0.0001;
			}
			delta = -0.0001;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.0001;
			}
		}
		else {
			delta = max * (double)10000;
			delta = (double)((int)delta);
			delta /= (double)10000;
			ticklabels.Add(delta);
			delta -= 0.0001;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.0001;
			}
		}
	}
	else if ((range >= 0.002) && (range < 0.02)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = 0.001;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += 0.001;
			}
			delta = -0.001;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.001;
			}
		}
		else {
			delta = max * (double)1000;
			delta = (double)((int)delta);
			delta /= (double)1000;
			ticklabels.Add(delta);
			delta -= 0.001;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.001;
			}
		}
	}
	else if ((range >= 0.02) && (range < 0.2)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = 0.01;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += 0.01;
			}
			delta = -0.01;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.01;
			}
		}
		else {
			delta = max * (double)100;
			delta = (double)((int)delta);
			delta /= (double)100;
			ticklabels.Add(delta);
			delta -= 0.01;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.01;
			}
		}
	}
	else if ((range >= 0.2) && (range < 2)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = 0.1;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += 0.1;
			}
			delta = -0.1;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.1;
			}
		}
		else {
			delta = max * (double)10;
			delta = (double)((int)delta);
			delta /= (double)10;
			ticklabels.Add(delta);
			delta -= 0.1;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= 0.1;
			}
		}
	}
	else if ((range >= 2) && (range < 20)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = (double)1;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += (double)1;
			}
			delta = (double)(-1);
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)1;
			}
		}
		else {
			delta = (double)((int)max);
			ticklabels.Add(delta);
			delta -= (double)1;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)1;
			}
		}
	}
	else if ((range >= 20) && (range < 200)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = (double)10;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += (double)10;
			}
			delta = (double)(-10);
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)10;
			}
		}
		else {
			delta = max / (double)10;
			delta = (double)((int)delta);
			delta *= (double)10;
			ticklabels.Add(delta);
			delta -= (double)10;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)10;
			}
		}
	}
	else if ((range >= 200) && (range < 2000)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = (double)100;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += (double)100;
			}
			delta = (double)(-100);
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)(100);
			}
		}
		else {
			delta = max / (double)100;
			delta = (double)((int)delta);
			delta *= (double)100;
			ticklabels.Add(delta);
			delta -= (double)100;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)100;
			}
		}
	}
	else if ((range >= 2000) && (range < 20000)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = (double)1000;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += (double)1000;
			}
			delta = (double)(-1000);
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)(1000);
			}
		}
		else {
			delta = max / (double)1000;
			delta = (double)((int)delta);
			delta *= (double)1000;
			ticklabels.Add(delta);
			delta -= (double)1000;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)1000;
			}
		}
	}
	else if ((range >= 20000) && (range < 200000)) {
		if ((max >= (double)0) && (min <= (double)0)) {
			ticklabels.Add((double)0);
			delta = (double)10000;
			while (delta <= max) {
				ticklabels.Add(delta);
				delta += (double)10000;
			}
			delta = (double)(-10000);
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)(10000);
			}
		}
		else {
			delta = max / (double)10000;
			delta = (double)((int)delta);
			delta *= (double)10000;
			ticklabels.Add(delta);
			delta -= (double)10000;
			while (delta >= min) {
				ticklabels.Add(delta);
				delta -= (double)10000;
			}
		}
	}
	CString Text;
	if (ticklabels.GetSize() > 10) RemoveTicks(ticklabels);
	if (ticklabels.GetSize() < 5) AddTicks(ticklabels, min, max);
	for (int i = 0; i < ticklabels.GetSize(); i++) {
		delta = ticklabels.GetAt(i);
		delta -= min;
		delta /= range;
		ticks.Add(delta);
	}
}

void CModelingandAnalysisofUncertaintyView::RemoveTicks(CArray <double>& TickLabels) {
	CArray <double> Temp;
	int counter = 0;
	Temp.RemoveAll();
	if (TickLabels.GetAt(0) != (double)0) {
		Temp.Add(TickLabels.GetAt(counter));
		counter = counter + 2;
		while (counter < TickLabels.GetSize()) {
			Temp.Add(TickLabels.GetAt(counter));
			counter = counter + 2;
		}
		if (Temp.GetSize() > 10) RemoveTicks(Temp);
	}
	else {
		CArray <double> Negatives, Positives;
		Negatives.RemoveAll(), Positives.RemoveAll();
		for (int i = 1; i < TickLabels.GetSize(); i++) {
			if (TickLabels.GetAt(i) > 0) Positives.Add(TickLabels.GetAt(i));
			else Negatives.Add(TickLabels.GetAt(i));
		}
		Temp.Add((double)0);
		for (int i = 1; i < Positives.GetSize(); i = i + 2) Temp.Add(Positives.GetAt(i));
		for (int i = 1; i < Negatives.GetSize(); i = i + 2) Temp.Add(Negatives.GetAt(i));
	}
	TickLabels.RemoveAll();
	for (int i = 0; i < Temp.GetSize(); i++) TickLabels.Add(Temp.GetAt(i));
}

void CModelingandAnalysisofUncertaintyView::AddTicks(CArray <double>& TickLabels, double min, double max) {
	CArray <double> Temp;
	Temp.RemoveAll();
	double Delta = fabs(TickLabels.GetAt(0) - TickLabels.GetAt(1)) / (double)2, value;
	int pos;
	GetMinimum(TickLabels, pos, value);
	if (value - Delta > min) {
		value -= Delta;
		Temp.Add(value);
	}
	else Temp.Add(value);
	value += Delta;
	while (value < max) {
		Temp.Add(value);
		value += Delta;
	}
	TickLabels.RemoveAll();
	for (int i = 0; i < Temp.GetSize(); i++) TickLabels.Add(Temp.GetAt(i));
}

int CModelingandAnalysisofUncertaintyView::FormatTickLabel(CString& Text, double value, int digit) {
	Text.Empty();
	double temp = fabs(value);
	int n_digits = 0;
	if (digit == 0) {
		if (temp >= 1) {
			if ((temp >= 1) && (temp < 10)) {
				if (temp - (double)((int)temp) != (double)0) {
					Text.Format(L"%.1f", value);
					n_digits = 1;
				}
				else Text.Format(L"%.0f", value);
			}
			else Text.Format(L"%.0f", value);
		}
		else if ((temp >= 0.1) && (temp < 1)) {
			Text.Format(L"%.1f", value);
			n_digits = 1;
		}
		else if ((temp >= 0.01) && (temp < 0.1)) {
			Text.Format(L"%.2f", value);
			n_digits = 2;
		}
		else if ((temp >= 0.001) && (temp < 0.01)) {
			Text.Format(L"%.3f", value);
			n_digits = 3;
		}
		else if ((temp >= 0.0001) && (temp < 0.001)) {
			Text.Format(L"%.4f", value);
			n_digits = 4;
		}
		else if ((temp >= 0.00001) && (temp < 0.0001)) {
			Text.Format(L"%.5f", value);
			n_digits = 5;
		}
		else if ((temp >= 0.000001) && (temp < 0.00001)) {
			Text.Format(L"%.6f", value);
			n_digits = 6;
		}
		else if (temp == (double)0) Text.Format(L"%.0f", value);
	}
	else {
		Text.Empty();
		if (digit == 1) Text.Format(L"%.1f", value);
		else if (digit == 2) Text.Format(L"%.2f", value);
		else if (digit == 3) Text.Format(L"%.3f", value);
		else if (digit == 4) Text.Format(L"%.4f", value);
		else if (digit == 5) Text.Format(L"%.5f", value);
		else if (digit == 6) Text.Format(L"%.6f", value);
		else Text.Format(L"%.0f", value);
	}
	return n_digits;
}

int CModelingandAnalysisofUncertaintyView::FormatValue(CString& Text, double value, int digit) {
	int n_digits = 0;
	if (digit == 0) {
		if (fabs(value) >= 10000) {
			if (value >= (double)0) value += (double)5;
			else value -= (double)5;
			Text.Empty(), Text.Format(L"%.0f", value);
		}
		else if (fabs(value) >= 1000) {
			if (value >= (double)0) value += 0.5;
			else value -= 0.5;
			Text.Empty(), Text.Format(L"%.1f", value);
			n_digits = 1;
		}
		else if (fabs(value) >= 100) {
			if (value >= (double)0) value += 0.05;
			else value -= 0.05;
			Text.Empty(), Text.Format(L"%.2f", value);
			n_digits = 2;
		}
		else if (fabs(value) >= 10) {
			if (value >= (double)0) value += 0.005;
			else value -= 0.005;
			Text.Empty(), Text.Format(L"%.3f", value);
			n_digits = 3;
		}
		else {
			if (value >= (double)0) value += 0.0005;
			else value -= 0.0005;
			Text.Empty(), Text.Format(L"%.4f", value);
			n_digits = 4;
		}
	}
	else {
		Text.Empty();
		if (digit == 1) Text.Format(L"%.1f", value);
		else if (digit == 2) Text.Format(L"%.2f", value);
		else if (digit == 3) Text.Format(L"%.3f", value);
		else if (digit == 4) Text.Format(L"%.4f", value);
		else Text.Format(L"%.0f", value);
	}
	return n_digits;
}

void CModelingandAnalysisofUncertaintyView::ShowProbabilityPlot(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& Values, CPoint TopLeft, CPoint BottomRight, CString Label) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	//pDC->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 90, x0 = TopLeft.x, y0 = TopLeft.y + 50, nObs = pDoc->n_Obs;
	double min, max, value, range, x_relative, y_relative, param_1, param_2, a, b;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 250, TopLeft.y + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	ProbabilityDistributions distribution;
	CArray <double> Values_Copy, Ticks, TickLabels, z;
	GetDistributionParameters(pDoc, Values, Values_Copy, param_1, param_2);
	if (Distribution == 0) {
		pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Probability plot (normal)"));
		distribution.SetProbabilityDistribution('n', (double)0, (double)1);
	}
	else if (Distribution == 1) {
		pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Probability plot (log-normal)"));
		distribution.SetProbabilityDistribution('l', param_1, param_2);
	}
	else if (Distribution == 2) {
		pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Probability plot (Weibull)"));
		distribution.SetProbabilityDistribution('w', param_1, param_2);
	}
	if (Distribution == 0) min = Values_Copy.GetAt(0), max = Values_Copy.GetAt(static_cast <int64_t>(nObs - 1));
	else {
		min = Values.GetAt(0), max = Values.GetAt(static_cast <int64_t>(nObs - 1));
		for (int i = 1; i < nObs; i++) {
			value = Values.GetAt(i);
			if (value > max) max = value;
			if (value < min) min = value;
		}
	}
	range = max - min;
	min -= 0.05 * range, max += 0.05 * range, range *= 1.1;
	GetTicks(min, max, Ticks, TickLabels);
	z.RemoveAll(), z.SetSize(nObs);
	for (int i = 0; i < nObs; i++) {
		value = ((double)i + 0.5) / ((double)nObs);
		if (Distribution == 0) value = distribution.CriticalValue(value);
		else value = distribution.CriticalValue(1 - value);
		z.SetAt(i, value);
	}
	min = Values_Copy.GetAt(0);
	int pos = static_cast<int64_t>(nObs - 1);
	max = Values_Copy.GetAt(pos);
	range = max - min;
	min -= 0.05 * range, max += 0.05 * range, range *= 1.1;
	x_relative = 0.05, y_relative = 0.0;
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
	y_relative = 1.0, Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
	x_relative = 0.05, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	CString Text;
	int digit_old = 0, digit_new;
	for (int i = 0; i < Ticks.GetSize(); i++) {
		x_relative = 0.05 + 0.95 * Ticks.GetAt(i), y_relative = 1.0;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height) - 10, pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, TickLabels.GetAt(i), digit_old);
		if (digit_new > digit_new) digit_old = digit_new;
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	double min_th = z.GetAt(0), max_th = z.GetAt(static_cast <int64_t>(nObs-1)), range_th = max_th - min_th;
	CPen Obs;
	CBrush Fill;
	Fill.CreateSolidBrush(RGB(255, 224, 146));
	Obs.CreatePen(PS_SOLID, 1, RGB(243, 102, 25));
	for (int i = 0; i < nObs; i++) {
		value = Values_Copy.GetAt(i);
		value -= min;
		value /= range;
		x_relative = 0.05 + value * 0.95;
		value = z.GetAt(i);
		value -= min_th;
		value /= range_th;
		y_relative = 0.05 + 0.9 * (1 - value);
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height);
		pDC->SelectObject(&Fill);
		CRect rect(Point.x - 3, Point.y - 3, Point.x + 3, Point.y + 3);
		pDC->FillRect(&rect, &Fill);
		pDC->SelectObject(&Obs);
		pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
	}
	double q25_th, q75_th;
	if (Distribution == 0) q25_th = distribution.CriticalValue(0.25), q75_th = distribution.CriticalValue(0.75);
	else q25_th = distribution.CriticalValue(0.75), q75_th = distribution.CriticalValue(0.25);
	int n25 = (int)(nObs / 4), n75 = (int)(3 * nObs / 4);
	double q25 = Values_Copy.GetAt(n25), q75 = Values_Copy.GetAt(n75);
	q25_th -= min_th, q25_th /= range_th, q75_th -= min_th, q75_th /= range_th;
	q25 -= min, q25 /= range, q75 -= min, q75 /= range;
	a = (q75_th - q25_th) / (q75 - q25), b = q25_th - a * q25;
	CPoint Point_1, Point_2;
	if (b >= (double)0) {
		x_relative = (double)0, y_relative = 0.05 + 0.9 * ((double)1 - b);
		Point_1.x = x0 + (int)((0.05 + x_relative * 0.95) * Length), Point_1.y = y0 + (int)(y_relative * Height);
		x_relative = ((double)1 - b) / a, y_relative = (double)0.05;
		Point_2.x = x0 + (int)((0.05 + x_relative * 0.95) * Length), Point_2.y = y0 + (int)(y_relative * Height);
	}
	else {
		x_relative = -b / a, y_relative = (double)0.95;
		Point_1.x = x0 + (int)((0.05 + x_relative * 0.95) * Length), Point_1.y = y0 + (int)(y_relative * Height);
		x_relative = ((double)1 - b) / a, y_relative = (double)0.05;
		Point_2 = x0 + (int)((0.05 + x_relative * 0.95) * Length), Point_2.y = y0 + (int)(y_relative * Height);
	}
	CPen Line;
	Line.CreatePen(PS_DASH, 1, RGB(243, 102, 25));
	pDC->SelectObject(&Line);
	pDC->MoveTo(Point_1), pDC->LineTo(Point_2);
	CFont XLabel, YLabel;
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	if (Distribution == 0) {
		Text.Empty(), Text.Append(Label + _T(" (z-score)"));
		pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, Text);
	}
	else pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, Label);
	YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	Label.Append(L" (theor.)");
	pDC->TextOutW(x0, y0 + (int)(0.5 * Height), Label);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_CENTER);
}

void CModelingandAnalysisofUncertaintyView::GetDistributionParameters(CModelingandAnalysisofUncertaintyDoc* pDoc, CArray <double>& Values, CArray <double>& Values_Copy, double& a, double& b) {
	double value = Values.GetAt(0), max = value;
	int nObs = pDoc->n_Obs;
	Values_Copy.RemoveAll();
	Values_Copy.SetSize(nObs);
	for (int i = 1; i < nObs; i++) {
		value = fabs(Values.GetAt(i));
		if (value > max) max = value;
	}
	a = (double)0, b = (double)0;
	for (int i = 0; i < nObs; i++) {
		value = Values.GetAt(i);
		value /= max;
		a += value;
		b += value * value;
		Values_Copy.SetAt(i, value);
	}
	pDoc->QuickSort(Values_Copy, 0, nObs - 1);
	a /= (double)nObs;
	b -= (double)(nObs)*a * a;
	b /= (double)(nObs - 1);
	b = sqrt(b);
	if (Distribution == 0) {
		for (int i = 0; i < nObs; i++) {
			value = Values_Copy.GetAt(i);
			value -= a;
			value /= b;
			Values_Copy.SetAt(i, value);
		}
	}
	else {
		double bar_sq, var, k1, k2, k3, eps = 100, temp_1, temp_2, y1, y2, dy, dk;
		if (Distribution == 1) {
			bar_sq = a * a, var = b * b;
			b = sqrt(log(var / bar_sq + 1));
			a = log(sqrt((bar_sq * bar_sq)/(var + bar_sq)));
		}
		else if (Distribution == 2) {
			ProbabilityDistributions temp;
			CString Text;
			k1 = 1.0, k2 = 0.99, bar_sq = a * a,	var = b * b;
			while (eps > 1e-10) {
				temp_1 = (double)1 + (double)2 / k1;
				temp_1 = temp.GAMMALN(temp_1);
				temp_1 = exp(temp_1);
				temp_2 = (double)1 + (double)1 / k1;
				temp_2 = temp.GAMMALN(temp_2);
				temp_2 = exp(temp_2);
				temp_2 *= temp_2;
				y1 = bar_sq * temp_1 - (var + bar_sq) * temp_2;
				temp_1 = (double)1 + (double)2 / k2;
				temp_1 = temp.GAMMALN(temp_1);
				temp_1 = exp(temp_1);
				temp_2 = (double)1 + (double)1 / k2;
				temp_2 = temp.GAMMALN(temp_2);
				temp_2 = exp(temp_2);
				temp_2 *= temp_2;
				y2 = bar_sq * temp_1 - (var + bar_sq) * temp_2;
				dy = y2 - y1;
				dk = k2 - k1;
				k3 = k2 - y2 * dk / dy;
				eps = fabs(k3 - k2);
				k1 = k2;
				k2 = k3;
			}
			a = k3;
			b = sqrt(bar_sq / temp_2);
		}
	}
}

void CModelingandAnalysisofUncertaintyView::GetMinimum(CArray <double>& Values, int& pos, double& min) {
	min = Values.GetAt(0);
	pos = 0;
	if (min == (double)0) min = 1e10;
	for (int i = 1; i < Values.GetSize(); i++) {
		if (Values.GetAt(i) < min) {
			min = Values.GetAt(i);
			pos = i;
		}
	}
}

void CModelingandAnalysisofUncertaintyView::GetMaximum(CArray <double>& Values, int& pos, double& max) {
	max = Values.GetAt(0);
	pos = 0;
	for (int i = 1; i < Values.GetSize(); i++) {
		if (Values.GetAt(i) > max) {
			max = Values.GetAt(i);
			pos = i;
		}
	}
}

void CModelingandAnalysisofUncertaintyView::DisplayFileAndDataSetInformation(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, bool ShowLabel) {
	CString Text;
	Text.Empty(), Text.Append(_T("Path and filename"));
	pDC->TextOutW(20, 20, Text);
	Text.Empty(), Text.Append(_T(":"));
	pDC->TextOutW(250, 20, Text);
	pDC->TextOutW(300, 20, pDoc->PathAndFileName);
	Text.Empty(), Text.Append(_T("Number of observations"));
	pDC->TextOutW(20, 40, Text);
	Text.Empty(), Text.Append(_T(":"));
	pDC->TextOutW(250, 40, Text);
	Text.Empty(), Text.Format(L"%d", pDoc->n_Obs);
	pDC->TextOutW(300, 40, Text);
	Text.Empty(), Text.Append(_T("Type of problem"));
	pDC->TextOutW(20, 60, Text);
	Text.Empty(), Text.Append(_T(":"));
	pDC->TextOutW(250, 60, Text);
	pDC->TextOutW(300, 60, pDoc->Problem);
	Text.Empty(), Text.Append(_T("Number of variables"));
	pDC->TextOutW(20, 80, Text);
	Text.Empty(), Text.Append(_T(":"));
	pDC->TextOutW(250, 80, Text);
	if (pDoc->Problem == L"Analysis problem") {
		Text.Empty(), Text.Format(L"%d", pDoc->n_Var), Text.Append(_T(" features/attributes"));
		pDC->TextOutW(300, 80, Text);
	}
	else if (pDoc->Problem == L"Regression problem") {
		Text.Empty(), Text.Format(L"%d", pDoc->n_Var - 1), Text.Append(_T(" regressors"));
		pDC->TextOutW(300, 80, Text);
		pDC->TextOutW(300, 100, _T("1 regressand"));
	}
	else {
		Text.Empty(), Text.Format(L"%d", pDoc->n_Var - 1), Text.Append(_T(" features/attributes"));
		pDC->TextOutW(300, 80, Text);
		if (ShowLabel == true) pDC->TextOutW(300, 100, _T("1 label"));
	}
	pDC->TextOutW(20, 130, _T("Variable number"));
	pDC->TextOutW(180, 130, _T("Variable tag"));
	pDC->TextOutW(320, 130, _T("Variable name"));
	pDC->TextOutW(500, 130, _T("Variable unit"));
	pDC->MoveTo(0, 152), pDC->LineTo(600, 152);
	pDC->MoveTo(155, 130), pDC->LineTo(155, 160);
	pDC->MoveTo(295, 130), pDC->LineTo(295, 160);
	pDC->MoveTo(480, 130), pDC->LineTo(480, 160);
	for (int i = 0; i < pDoc->n_Var; i++) {
		if ((ShowLabel == true) || (pDoc->Tag.GetAt(i) != "y")) {
			Text.Empty(), Text.Format(L"%d", i + 1);
			pDC->SetTextAlign(TA_RIGHT), pDC->TextOutW(75, 160 + i * 25, Text);
			Text.Empty(), Text = pDoc->Tag.GetAt(i);
			pDC->SetTextAlign(TA_RIGHT), pDC->TextOutW(240, 160 + i * 25, pDoc->Tag.GetAt(i));
			pDC->SetTextAlign(TA_CENTER), pDC->TextOutW(390, 160 + i * 25, pDoc->Name.GetAt(i));
			Text.Empty(), pDC->SetTextAlign(TA_CENTER), Text.Append(pDoc->Unit.GetAt(i));
			if (Text.GetLength() == 0) Text.Append(_T("N/A"));
			pDC->TextOutW(545, 160 + i * 25, Text);
			pDC->MoveTo(155, 160 + i * 25), pDC->LineTo(155, 160 + (i + 1) * 25);
			pDC->MoveTo(295, 160 + i * 25), pDC->LineTo(295, 160 + (i + 1) * 25);
			pDC->MoveTo(480, 160 + i * 25), pDC->LineTo(480, 160 + (i + 1) * 25);
		}
	}
}

// *************************************************************
// ***  PARTICULAR  MEMBERS  RELATED  TO  SPECIFIC  METHODS  ***
// *************************************************************

// ************************************************
// ***    Members for descriptive statistics    ***
// ************************************************

void CModelingandAnalysisofUncertaintyView::DisplayDescriptiveStatistics(CDC* pDC, CModelingandAnalysisofUncertaintyDoc* pDoc) {
	CString Text;
	int number = 0;
	CRect rectA(20, 20, 270, 58);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(30, 30, _T("Descriptive statistics"));
	CFont TextFont;
	TextFont.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&TextFont);
	pDC->TextOutW(40, 70, L"Variable"), pDC->TextOutW(300, 70, L":"), pDC->TextOutW(340, 70, pDoc->Tag.GetAt(counter));
	pDC->TextOutW(40, 90, L"Name"), pDC->TextOutW(300, 90, L":"), pDC->TextOutW(340, 90, pDoc->Name.GetAt(counter));
	pDC->TextOutW(40, 110, L"Unit"), pDC->TextOutW(300, 110, L":"), pDC->TextOutW(340, 110, pDoc->Unit.GetAt(counter));
	pDC->MoveTo(20, 132), pDC->LineTo(500, 132);
	pDC->TextOutW(40, 150, L"# observations"), pDC->TextOutW(300, 150, L":"), Text.Format(L"%d", pDoc->n_Obs), pDC->TextOutW(340, 150, Text);
	pDC->TextOutW(40, 170, L"Mean"), pDC->TextOutW(300, 170, L":"), FormatValue(Text, pDoc->MEAN.GetAt(counter), 0), pDC->TextOutW(340, 170, Text);
	pDC->TextOutW(40, 190, L"Standard deviation"), pDC->TextOutW(300, 190, L":"), FormatValue(Text, pDoc->STD.GetAt(counter), 0), pDC->TextOutW(340, 190, Text);
	pDC->TextOutW(40, 210, L"Standard error"), pDC->TextOutW(300, 210, L":"), FormatValue(Text, pDoc->SE.GetAt(counter), 0), pDC->TextOutW(340, 210, Text);
	pDC->MoveTo(20, 232), pDC->LineTo(500, 232);
	pDC->TextOutW(40, 250, L"25th percentile (Q1)"), pDC->TextOutW(300, 250, L":"), FormatValue(Text, pDoc->Q1.GetAt(counter), 0), pDC->TextOutW(340, 250, Text);
	pDC->TextOutW(40, 270, L"50th percentile (Median)"), pDC->TextOutW(300, 270, L":"), FormatValue(Text, pDoc->Q2.GetAt(counter), 0), pDC->TextOutW(340, 270, Text);
	pDC->TextOutW(40, 290, L"75th percentile (Q3)"), pDC->TextOutW(300, 290, L":"), FormatValue(Text, pDoc->Q3.GetAt(counter), 0), pDC->TextOutW(340, 290, Text);
	pDC->MoveTo(20, 312), pDC->LineTo(500, 312);
	pDC->TextOutW(40, 330, L"Min"), pDC->TextOutW(300, 330, L":"), FormatValue(Text, pDoc->MIN.GetAt(counter), 0), pDC->TextOutW(340, 330, Text);
	pDC->TextOutW(40, 350, L"Max"), pDC->TextOutW(300, 350, L":"), FormatValue(Text, pDoc->MAX.GetAt(counter), 0), pDC->TextOutW(340, 350, Text);
	pDC->TextOutW(40, 370, L"Range"), pDC->TextOutW(300, 370, L":"), FormatValue(Text, pDoc->RANGE.GetAt(counter), 0), pDC->TextOutW(340, 370, Text);
	pDC->MoveTo(20, 392), pDC->LineTo(500, 392);
	pDC->TextOutW(40, 410, L"Skewness"), pDC->TextOutW(300, 410, L":"), FormatValue(Text, pDoc->SKEW.GetAt(counter), 0), pDC->TextOutW(340, 410, Text);
	pDC->TextOutW(40, 430, L"Kurtosis"), pDC->TextOutW(300, 430, L":"), FormatValue(Text, pDoc->KURT.GetAt(counter), 0), pDC->TextOutW(340, 430, Text);
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
}

void CModelingandAnalysisofUncertaintyView::DisplayBoxPlot(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& Values, CPoint TopLeft, CPoint BottomRight, int counter, CString Label){
	if (Values.IsEmpty()) {
		int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 50, x0 = TopLeft.x, y0 = TopLeft.y + 50, X1, Y1, X2, Y2;
		double x_relative = 0.5, y_relative, value;
		CPoint Point;
		CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 250, TopLeft.y + 38);
		pDC->Rectangle(rectA);
		CBrush Brush_Header;
		Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
		pDC->FillRect(&rectA, &Brush_Header);
		pDC->SetBkMode(TRANSPARENT);
		CFont Standard_Font;
		Standard_Font.CreateStockObject(SYSTEM_FONT);
		pDC->SelectObject(Standard_Font);
		pDC->SetTextAlign(TA_LEFT);
		pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Boxplot"));
		int nOutliers = pDoc->n_OUTLIERS.GetAt(counter);
		CPen outliers;
		outliers.CreatePen(PS_SOLID, 2, RGB(178, 34, 52));
		pDC->SelectObject(&outliers);
		CString Text;
		CPen Obs;
		CBrush Fill;
		Fill.CreateSolidBrush(RGB(255, 224, 146));
		Obs.CreatePen(PS_SOLID, 1, RGB(243, 102, 25));
		if (nOutliers > 0) {
			for (int i = 0; i < nOutliers; i++) {
				value = pDoc->OUTLIERS[counter][i] - pDoc->MIN.GetAt(counter);
				value /= pDoc->RANGE.GetAt(counter);
				y_relative = 0.05 + 0.9 * ( (double)1 - value );
				Point.x = x0 + (int)(x_relative * (double)Length);
				Point.y = y0 + (int)(y_relative * (double)Height);
				pDC->SelectObject(&Fill);
				CRect rect(Point.x - 3, Point.y - 3, Point.x + 3, Point.y + 3);
				pDC->FillRect(&rect, &Fill);
				pDC->SelectObject(&Obs);
				pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
			}
		}
		CPen BoxAndWhiskers;
		BoxAndWhiskers.CreatePen(PS_SOLID, 2, RGB(243, 102, 25));
		pDC->SelectObject(&BoxAndWhiskers);
		CString BoxPlotInfo;
		value = pDoc->UB.GetAt(counter) - pDoc->MIN.GetAt(counter);
		value /= pDoc->RANGE.GetAt(counter);
		x_relative = 0.4, y_relative = 0.05 + 0.9 * ( (double)1 - value );
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
		x_relative = 0.6, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
		FormatValue(Text, pDoc->UB.GetAt(counter), 0);
		pDC->SetTextAlign(TA_LEFT);
		pDC->SetTextColor(RGB(243, 102, 25));
		pDC->TextOutW(Point.x + 5, Point.y - 7, Text);
		x_relative = 0.5, Point.x = x0 + (int)(x_relative * Length), pDC->MoveTo(Point);
		value = pDoc->Q3.GetAt(counter) - pDoc->MIN.GetAt(counter);
		value /= pDoc->RANGE.GetAt(counter);
		y_relative = 0.05 + 0.9 * ( (double)1 - value );
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
		x_relative = 0.35, X1 = x0 + (int)(x_relative * Length), Y1 = Point.y;
		x_relative = 0.65, X2 = x0 + (int)(x_relative * Length);
		value = pDoc->Q1.GetAt(counter) - pDoc->MIN.GetAt(counter);
		value /= pDoc->RANGE.GetAt(counter);
		y_relative = 0.05 + 0.9 * ( (double)1 - value ); Y2 = y0 + (int)(y_relative * Height);
		CRect BoxFrame(X1, Y1, X2, Y2), InteriorFrame(X1 + 2, Y1 + 2, X2 - 2, Y2 - 2);
		pDC->Rectangle(BoxFrame);
		CBrush FrameColor, InteriorColor;
		FrameColor.CreateSolidBrush(RGB(243, 102, 25));
		pDC->FillRect(&BoxFrame, &FrameColor);
		InteriorColor.CreateSolidBrush(RGB(255, 224, 146));
		pDC->FillRect(&InteriorFrame, &InteriorColor);
		FormatValue(Text, pDoc->Q3.GetAt(counter), 0);
		BoxPlotInfo.Empty(), BoxPlotInfo.Append(L"Q3 = "), BoxPlotInfo.Append(Text);
		pDC->SetTextAlign(TA_BOTTOM);
		pDC->TextOutW(X2 + 5, Y1, BoxPlotInfo);
		pDC->SetTextAlign(TA_TOP);
		FormatValue(Text, pDoc->Q1.GetAt(counter), 0), 0;
		BoxPlotInfo.Empty(), BoxPlotInfo.Append(L"Q1 = "), BoxPlotInfo.Append(Text);
		pDC->TextOutW(X2 + 5, Y2, BoxPlotInfo);
		x_relative = 0.35, Point.x = x0 + (int)(x_relative * Length);
		value = pDoc->Q2.GetAt(counter) - pDoc->MIN.GetAt(counter);
		value /= pDoc->RANGE.GetAt(counter);
		y_relative = 0.05 + 0.9 * ( (double)1 - value );
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
		x_relative = 0.65, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
		FormatValue(Text, pDoc->Q2.GetAt(counter), 0);
		BoxPlotInfo.Empty(), BoxPlotInfo.Append(L"Median = "); BoxPlotInfo.Append(Text);
		pDC->TextOutW(Point.x + 5, Point.y - 7, BoxPlotInfo);
		x_relative = 0.5, Point.x = x0 + (int)(x_relative * Length), Point.y = Y2, pDC->MoveTo(Point);
		value = pDoc->LB.GetAt(counter) - pDoc->MIN.GetAt(counter);
		value /= pDoc->RANGE.GetAt(counter);
		y_relative = 0.05 + 0.9 * ( 1 - value ), Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
		x_relative = 0.4, Point.x = x0 + (int)(x_relative * Length), pDC->MoveTo(Point);
		x_relative = 0.6, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
		FormatValue(Text, pDoc->LB.GetAt(counter), 0);
		pDC->TextOutW(Point.x + 5, Point.y - 7, Text);
		CPen Standard;
		Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		pDC->SelectObject(&Standard);
		x_relative = 0.2, y_relative = 0.0;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
		y_relative = 1.0, Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
		pDC->SetTextColor(RGB(0, 0, 0));
		CArray <double> Ticks, TickLabels;
		double min = pDoc->MIN.GetAt(counter), max = pDoc->MAX.GetAt(counter), range = max - min;
		GetTicks(min - 0.05 * range, max + 0.05 * range, Ticks, TickLabels);
		pDC->SetTextAlign(TA_RIGHT);
		int digit_old = 0, digit_new, pos;
		GetMinimum(TickLabels, pos, min);
		digit_new = FormatTickLabel(Text, TickLabels.GetAt(pos), digit_old);
		if (digit_new > digit_old) digit_old = digit_new;
		for (int i = 0; i < Ticks.GetSize(); i++) {
			x_relative = 0.22, y_relative = 1 - Ticks.GetAt(i);
			Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
			x_relative = 0.18, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
			digit_new = FormatTickLabel(Text, TickLabels.GetAt(i), digit_old);
			pDC->TextOutW(Point.x, Point.y - 7, Text);
			Text.Empty(), Text.Format(L"%d \t %d", digit_old, digit_new);
		}
		CFont YLabel;
		YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
		pDC->SelectObject(&YLabel);
		pDC->SetTextAlign(TA_CENTER | TA_TOP);
		pDC->TextOutW(x0, y0 + (int)(0.5 * Height), Label);
		pDC->SelectObject(&Standard_Font);
		pDC->SelectObject(&Standard);
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
	}
}

void CModelingandAnalysisofUncertaintyView::DisplayHistogram(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& Values, CPoint TopLeft, CPoint BottomRight, int n_Bins, CString Label) {
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 95, x0 = TopLeft.x, y0 = TopLeft.y + 55, X1, Y1, X2, Y2, Yold = 0;
	double x_relative, y_relative, value, min, max, range, dX, dx;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 250, TopLeft.y + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Histogram"));
	CArray <double> XTicks, YTicks, XTickLabels, YTickLabels;
	min = pDoc->MIN.GetAt(counter), max = pDoc->MAX.GetAt(counter);
	range = max - min;
	dX = range / (double)n_Bins;
	GetTicks(min - 0.05*range, max + 0.05*range, XTicks, XTickLabels);
	CArray <int> n_Hits;
	n_Hits.RemoveAll(), n_Hits.SetSize(n_Bins);
	for (int i = 0; i < pDoc->n_Obs; i++) {
		value = pDoc->Data.GetAt(pDoc->GetPosition(i, counter, pDoc->Data_spec));
		for (int j = 0; j < n_Bins; j++) {
			if (j < n_Bins - 1) {
				if ((value >= min + (double)j * dX) && (value < min + (double)(j + 1) * dX)) {
					n_Hits.SetAt(j, n_Hits.GetAt(j) + 1);
					break;
				}
			}
			else n_Hits.SetAt(j, n_Hits.GetAt(j) + 1);
		}
	}
	int n_Max;
	n_Max = n_Hits.GetAt(0);
	dx = 0.8 / (double)n_Bins;
	for (int i = 1; i < n_Bins; i++) if (n_Hits.GetAt(i) > n_Max) n_Max = n_Hits.GetAt(i);
	CBrush Bar;
	Bar.CreateSolidBrush(RGB(255, 224, 146));
	CPen BarFrame;
	BarFrame.CreatePen(PS_SOLID, 2, RGB(243, 102, 25));
	pDC->SelectObject(&BarFrame);
	y_relative = 1, Y1 = y0 + (int)(y_relative * Height);
	CString Text;
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	for (int i = 0; i < n_Bins; i++) {
		x_relative = 0.15 + (double)i * dx, X1 = x0 + (int)(x_relative * Length);
		x_relative = 0.15 + (double)(i + 1) * dx, X2 = x0 + (int)(x_relative * Length);
		value = (double)n_Hits.GetAt(i) / (double)n_Max;
		y_relative = 1 - value, Y2 = y0 + (int)(y_relative * Height);
		CRect rect(X1, Y1, X2, Y2);
		pDC->Rectangle(rect);
		pDC->FillRect(&rect, &Bar);
		Text.Empty(), Text.Format(L"%d", n_Hits.GetAt(i));
		pDC->TextOutW(X1 + (int)((X2 - X1) / 2), Y2, Text);
		pDC->MoveTo(X1, Y1), pDC->LineTo(X1, Y2), pDC->LineTo(X2,Y2);
		if (i > 0) {
			if (Yold < Y2) {
				pDC->MoveTo(X1, Yold);
				pDC->LineTo(X1, Y2);
			}
		}
		if (i == n_Bins - 1) {
			pDC->MoveTo(X2, Y2), pDC->LineTo(X2, Y1);
		}
		Yold = Y2;
	}
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	// draw x-axis for histogram
	y_relative = 1.0, x_relative = 0.1, value;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	// draw labels for x-axis
	int digit_old = 0, digit_new, pos;
	GetMinimum(XTickLabels, pos, min);
	digit_new = FormatTickLabel(Text, XTickLabels.GetAt(pos), digit_old);
	if (digit_new > digit_old) digit_old = digit_new;
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = 0.15 + 0.8 * XTicks.GetAt(i), y_relative = 1.0;
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height) - 10, pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, XTickLabels.GetAt(i), digit_old);
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	GetTicks((double)0, (double)n_Max, YTicks, YTickLabels);
	x_relative = 0.1, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	y_relative = 0.0, Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		x_relative = 0.11, y_relative = 1 - YTicks.GetAt(i);
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		x_relative = 0.1, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		FormatTickLabel(Text, YTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x - 2, Point.y - 7, Text);
	}
	CFont XLabel, YLabel;
	YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel);
	Text.Empty(), Text.Append(L"Frequency");
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0, y0 + (int)(0.5 * Height), Text);
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, Label);
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pDC->SelectObject(&Standard);
	pDC->SelectObject(&Standard_Font);
	pDC->SetTextAlign(TA_LEFT | TA_TOP); }

void CModelingandAnalysisofUncertaintyView::SequencePlot(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& Values, CPoint TopLeft, CPoint BottomRight, CString Label) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	//pDC->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 100, x0 = TopLeft.x, y0 = TopLeft.y + 50, nObs = pDoc->n_Obs;
	double min, max, value, range, x_relative, y_relative;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 250, TopLeft.y + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Sequence plot"));
	value = Values.GetAt(0);
	min = value, max = value;
	for (int i = 1; i < nObs; i++) {
		value = Values.GetAt(i);
		if (value > max) max = value;
		if (value < min) min = value;
	}
	range = max - min;
	CString Text;
	x_relative = 0.05, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	// Ticks denoting sample index
	CArray <double> XTicks, YTicks, XTickLabels, YTickLabels;
	GetTicks((double)1, (double)nObs, XTicks, XTickLabels);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = 0.05 + 0.95 * XTicks.GetAt(i);
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height) - 10, pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
		FormatTickLabel(Text, XTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	// y-axis denoting Observations
	x_relative = 0.05, y_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length),Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	y_relative = 0.0, Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
	// Ticks on y-axis
	GetTicks(min - 0.05 * range, max + 0.05 * range, YTicks, YTickLabels);
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		y_relative = 0.05 + 0.9 * (1.0 - YTicks.GetAt(i));
		Point.x = x0 + (int)(x_relative * (double)Length) + 10, Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		FormatTickLabel(Text, YTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x-2, Point.y - 7, Text);
	}
	x_relative = 0.05;
	double dx = 0.95 / (double)Values.GetSize();
	CPen Obs;
	CBrush Fill;
	Fill.CreateSolidBrush(RGB(255, 224, 146));
	Obs.CreatePen(PS_SOLID, 1, RGB(243, 102, 25));
	for (int i = 0; i < nObs; i++) {
		x_relative += dx;
		value = Values.GetAt(i);
		value -= min;
		value /= range;
		y_relative = 0.05 + 0.9 * (1 - value);
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height);
		pDC->SelectObject(&Fill);
		CRect rect(Point.x - 3, Point.y - 3, Point.x + 3, Point.y + 3);
		pDC->FillRect(&rect, &Fill);
		pDC->SelectObject(&Obs);
		pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
	}
	CFont XLabel, YLabel;
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, _T("Observation"));
	YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0, y0 + (int)(0.5 * Height), Label);
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pDC->SelectObject(&Standard);
	pDC->SelectObject(&Standard_Font);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
}

double CModelingandAnalysisofUncertaintyView::GetPDF(double sigma, CArray <double>& Sample, double x) {
	double pdf = 0, arg, pi = 3.1415926535897931159979635;
	for (int i = 0; i < Sample.GetSize(); i++) {
		arg = x - Sample.GetAt(i);
		arg /= sigma;
		arg *= arg;
		arg *= -0.5;
		pdf += exp(arg);
	}
	pdf /= (sigma * (double)Sample.GetSize() * sqrt(2 * pi));
	return pdf;
}

void CModelingandAnalysisofUncertaintyView::DisplayPDF(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& Values, CPoint TopLeft, CPoint BottomRight) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	//pDC->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 100, x0 = TopLeft.x, y0 = TopLeft.y + 50, nObs = pDoc->n_Obs;
	double min, max, value, range, x_relative, y_relative;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 250, TopLeft.y + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Estimated density function"));
	min = pDoc->MIN.GetAt(counter), max = pDoc->MAX.GetAt(counter), range = max - min;
	double sigma = pDoc->h.GetAt(counter) * pDoc->STD.GetAt(counter);
	double x, dx;
	range = max - min;
	max += 0.05 * range, min -= 0.05 * range, range *= 1.1;
	dx = range / (double)500;
	x = min;
	CArray <double> pdf, XTicks, YTicks, XTickLabels, YTickLabels;
	pdf.RemoveAll(), pdf.SetSize(501);
	double fMax = 0;
	for (int i = 0; i <= 500; i++) {
		value = GetPDF(sigma, Values, x);
		if (value > fMax) fMax = value;
		pdf.SetAt(i, value);
		x += dx;
	}
	for (int i = 0; i <= 500; i++) {
		value = pdf.GetAt(i);
		value /= fMax;
		pdf.SetAt(i, value);
	}
	CString Text;
	GetTicks((double)0, fMax, XTicks, XTickLabels);
	GetTicks(min, max, YTicks, YTickLabels);
	// starting with ordinate 
	y_relative = 1.0, x_relative = 0.1, value = 0;
	Point.x = x0 + (int)(x_relative * (double)Length);
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->MoveTo(Point);
	Point.y = y0;
	pDC->LineTo(Point);
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		y_relative = 1 - YTicks.GetAt(i), x_relative = 0.1;
		Point.x = x0 + (int)(x_relative * (double)Length) + 10, Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		FormatTickLabel(Text, YTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x - 2, Point.y - 7, Text);
	}
	// x-axis
	x_relative = 0.1, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	y_relative = 1.0;
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	int digit_old = 0, digit_new, pos;
	double MIN;
	GetMinimum(XTickLabels, pos, MIN);
	digit_new = FormatTickLabel(Text, XTickLabels.GetAt(pos), digit_old);
	if (digit_new > digit_old) digit_old = digit_new;
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = 0.1 + 0.9 * XTicks.GetAt(i), Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height) - 10;
		pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
		pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, XTickLabels.GetAt(i), digit_old);
		if (digit_old > 2) {
			value = XTickLabels.GetAt(i);
			value *= pow((double)10, (double)digit_old);
			Text.Empty(), Text.Format(L"%.0f", value);
		}
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	if (digit_old > 2) {
		Text.Empty(), Text.Format(L"·1e-%d", digit_old);
		pDC->SetTextAlign(TA_RIGHT | TA_TOP), pDC->TextOutW(x0 + Length, y0 + Height + 14, Text);
	}
	CPen Obs;
	CBrush Fill;
	Fill.CreateSolidBrush(RGB(255, 224, 146));
	Obs.CreatePen(PS_SOLID, 1, RGB(243, 102, 25));
	x_relative = 0.1, Point.x = x0 + (int)(x_relative * (double)Length) + 10;
	for (int i = 0; i < Values.GetSize(); i++) {
		value = Values.GetAt(i);
		value -= min;
		value /= range;
		y_relative = 1 - value;
		Point.y = y0 + (int)( y_relative * (double)Height);
		pDC->SelectObject(&Fill);
		CRect rect(Point.x - 3, Point.y - 3, Point.x + 3, Point.y + 3);
		pDC->FillRect(&rect, &Fill);
		pDC->SelectObject(&Obs);
		pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
	}
	double dy = 1 / (double)500, y = 0;
	y_relative = 1.0;
	CPen PDF;
	PDF.CreatePen(PS_SOLID, 2, RGB(243, 102, 25));
	pDC->SelectObject(&PDF);
	for (int i = 0; i <= 500; i++) {
		x_relative = 0.1 + 0.9 * pdf.GetAt(i);
		Point.y = y0 + (int)(y_relative * (double)Height);
		Point.x = x0 + (int)(x_relative * (double)Length);
		if (i == 0) pDC->MoveTo(Point);
		else pDC->LineTo(Point);
		y_relative -= dy;
	}
	CFont XLabel;
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	Text.Empty(), Text.Append(_T("f(") + pDoc->Tag.GetAt(counter) + _T(") for h = "));
	CString bandwidth;
	bandwidth.Empty(), bandwidth.Format(L"%.4f", pDoc->h.GetAt(counter));
	Text.Append(bandwidth);
	pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, Text);
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pDC->SelectObject(&Standard_Font);
	pDC->SelectObject(&Standard);
	pDC->SetTextAlign(TA_LEFT | TA_TOP); 
}

// ***********************************************************
// *** Members for principal component and factor analysis ***
// ***********************************************************

int CModelingandAnalysisofUncertaintyView::DisplayCorrelationMatrix(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint TopLeft) {
	int nVar, number = 0, x0 = TopLeft.x, y0 = TopLeft.y, height;
	CString Text;
	CPoint Point;
	CRect rectA(x0, y0, x0 + 250, y0 + 38);
	double value;
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(x0 + 20, 60, _T("Correlation matrix"));
	if (pDoc->OnlyX == false) nVar = pDoc->n_Var;
	else nVar = pDoc->n_Var - 1;
	pDC->SetTextAlign(TA_RIGHT);
	CBrush Red, Orange, Yellow, Yellow_bright;
	Red.CreateSolidBrush(RGB(250,160,113));
	Orange.CreateSolidBrush(RGB(250, 216, 46));
	Yellow.CreateSolidBrush(RGB(239, 250, 37));
	Yellow_bright.CreateSolidBrush(RGB(245,249,195));
	for (int i = 1; i < nVar; i++) {
		Point.y = y0 + 50 + (i - 1) * 20;
		for (int j = 0; j < i; j++) {
			Text.Empty();
			pDC->TextOutW(x0, Point.y, pDoc->Tag.GetAt(i));
			value = pDoc->R.GetAt(pDoc->GetPosition(j, i, pDoc->R_spec));
			if ((nVar > 2) && (nVar <= 10)) {
				Text.Format(L"%.3f", value);
				Point.x = x0 + 55 + j * 60;
				if ((fabs(value) >= 0.8) && (fabs(value) <= 1.0)) {
					CRect rect(Point.x - 45, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Red);
				}
				else if ((fabs(value) >= 0.6 && (fabs(value) < 0.8))) {
					CRect rect(Point.x - 45, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Orange);
				}
				else if ((fabs(value) >= 0.4 && (fabs(value) < 0.6))) {
					CRect rect(Point.x - 45, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Yellow);
				}
				else if ((fabs(value) >= 0.2 && (fabs(value) < 0.4))) {
					CRect rect(Point.x - 45, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Yellow_bright);
				}
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOutW(Point.x, Point.y, Text);
			}
			else if ((nVar > 10) && (nVar < 25) ) {
				Text.Format(L"%.2f", value);
				Point.x = x0 + 55 + j * 40;
				if ((fabs(value) >= 0.8) && (fabs(value) <= 1.0)) {
					CRect rect(Point.x-35, Point.y, Point.x, Point.y+16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Red);
				}
				else if ((fabs(value) >= 0.6 && (fabs(value) < 0.8))) {
					CRect rect(Point.x - 35, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Orange);
				}
				else if ((fabs(value) >= 0.4 && (fabs(value) < 0.6))) {
					CRect rect(Point.x - 35, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Yellow);
				}
				else if ((fabs(value) >= 0.2 && (fabs(value) < 0.4))) {
					CRect rect(Point.x - 35, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Yellow_bright);
				}
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOutW(Point.x, Point.y, Text);
			}
			else if (nVar >= 25) {
				Text.Format(L"%.2f", value);
				Point.x = x0 + 55 + j * 40;
				if (i <= 24) pDC->TextOutW(Point.x, Point.y, Text);
			}
		}
	}
	Point.y = y0 + 60 + (nVar - 1) * 20;
	if ((nVar > 2) && (nVar <= 10) ) {
		for (int i = 0; i < nVar; i++) {
			Point.x = x0 + (int)((i + 0.5) * 60);
			pDC->TextOutW(Point.x, Point.y, pDoc->Tag.GetAt(i));
		}
	}
	else if ((nVar > 10) && (nVar < 25)) {
		for (int i = 0; i < nVar  - 1; i++) {
			Point.x = x0 + 55 + (int)(i * 40);
			pDC->TextOutW(Point.x, Point.y, pDoc->Tag.GetAt(i));
		}
	}
	height = y0 + 60 + (nVar - 1) * 20;
	pDC->SelectObject(&Standard_Font);
	pDC->SelectObject(&Standard);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	return height;
}

int CModelingandAnalysisofUncertaintyView::DisplayCovarianceMatrices(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint TopLeft, CString S) {
	int nVar, number = 0, x0 = TopLeft.x, y0 = TopLeft.y + 20, length = 0;
	CString Text;
	CPoint Point;
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	CRect rectA(x0, y0, x0 + 250, y0 + 38);
	double value, max = 0.0;
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	Text.Empty(), Text.Append(_T("Covariance matrix (") + S + _T(")"));
	pDC->TextOutW(x0 + 20, y0 + 10, Text);
	if (pDoc->OnlyX == false) nVar = pDoc->n_Var;
	else nVar = pDoc->n_Var - 1;
	pDC->SetTextAlign(TA_RIGHT);
	if (S == "Sf") pDoc->GetLargestElement(pDoc->Sf, pDoc->S_spec, max);
	else pDoc->GetLargestElement(pDoc->Se, pDoc->S_spec, max);
	CBrush Red, Orange, Yellow, Yellow_bright;
	Red.CreateSolidBrush(RGB(250, 160, 113));
	Orange.CreateSolidBrush(RGB(250, 216, 46));
	Yellow.CreateSolidBrush(RGB(239, 250, 37));
	Yellow_bright.CreateSolidBrush(RGB(245, 249, 195));
	for (int i = 0; i < nVar; i++) {
		Point.y = y0 + 50 + i * 20;
		for (int j = 0; j <= i; j++) {
			Text.Empty();
			pDC->TextOutW(x0, Point.y, pDoc->Tag.GetAt(i));
			if (S == "Sf") value = pDoc->Sf.GetAt(pDoc->GetPosition(i, j, pDoc->S_spec));
			else value = pDoc->Se.GetAt(pDoc->GetPosition(i, j, pDoc->S_spec));
			if ((nVar > 2) && (nVar <= 10)) {
				Text.Format(L"%.3f", value);
				Point.x = x0 + 55 + j * 60;
				if ((fabs(value) / max >= 0.8) && (fabs(value) / max <= 1.0)) {
					CRect rect(Point.x - 45, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Red);
				}
				else if ((fabs(value) / max >= 0.6 && (fabs(value) / max < 0.8))) {
					CRect rect(Point.x - 45, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Orange);
				}
				else if ((fabs(value) / max >= 0.4 && (fabs(value) / max < 0.6))) {
					CRect rect(Point.x - 45, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Yellow);
				}
				else if ((fabs(value) / max >= 0.2 && (fabs(value) / max < 0.4))) {
					CRect rect(Point.x - 45, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Yellow_bright);
				}
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOutW(Point.x, Point.y, Text);
			}
			else if ((nVar > 10) && (nVar < 25)) {
				Text.Format(L"%.2f", value);
				Point.x = x0 + 55 + j * 40;
				if ((fabs(value) / max >= 0.8) && (fabs(value) / max <= 1.0)) {
					CRect rect(Point.x - 35, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Red);
				}
				else if ((fabs(value) / max >= 0.6 && (fabs(value) / max < 0.8))) {
					CRect rect(Point.x - 35, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Orange);
				}
				else if ((fabs(value) / max >= 0.4 && (fabs(value) / max < 0.6))) {
					CRect rect(Point.x - 35, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Yellow);
				}
				else if ((fabs(value) / max >= 0.2 && (fabs(value) / max < 0.4))) {
					CRect rect(Point.x - 35, Point.y, Point.x, Point.y + 16);
					pDC->Rectangle(rect);
					pDC->FillRect(&rect, &Yellow_bright);
				}
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOutW(Point.x, Point.y, Text);
			}
			else if (nVar >= 25) {
				Text.Format(L"%.2f", value);
				Point.x = x0 + 55 + j * 40;
				if (i <= 24) pDC->TextOutW(Point.x, Point.y, Text);
			}
		}
	}
	Point.y = y0 + 60 + nVar * 20;
	if ((nVar > 2) && (nVar <= 10)) {
		for (int i = 0; i < nVar; i++) {
			Point.x = x0 + (int)((i + 0.5) * 60);
			pDC->TextOutW(Point.x, Point.y, pDoc->Tag.GetAt(i));
		}
		length = x0 + 55 + nVar * 60;
	}
	else if ((nVar > 10) && (nVar < 25)) {
		for (int i = 0; i < nVar; i++) {
			Point.x = x0 + 55 + (int)(i * 40);
			pDC->TextOutW(Point.x, Point.y, pDoc->Tag.GetAt(i));
		}
		length = x0 + 55 + nVar * 40;
	}
	pDC->SelectObject(&Standard_Font);
	pDC->SelectObject(&Standard);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	return length;
}

void CModelingandAnalysisofUncertaintyView::DisplayMatrix(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& X, CArray <int>& X_spec, CPoint TopLeft, CString Label) {
	int row = X_spec.GetAt(0), col = X_spec.GetAt(1), number = 0, x0 = TopLeft.x, y0 = TopLeft.y + 20, length = 0;
	CString Text;
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	CRect rectA(x0, y0, x0 + 250, y0 + 38);
	double value;
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(x0 + 20, y0 + 10, Label);
	int Delta_x = 0, Delta_y = 20, y_abs;
	if (col < 10) Delta_x = 50;
	else if ((col >= 10) && (col < 20)) Delta_x = 30;
	else Delta_x = 20;
	y0 += 50;
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < row; i++) {
		y_abs = y0 + i * Delta_y;
		for (int j = 0; j < col; j++) {
			value = X.GetAt(pDoc->GetPosition(i, j, X_spec));
			if (col < 10) Text.Format(L"%.3f", value);
			else if ( (col>=10) && (col<20) ) Text.Format(L"%.2f", value);
			pDC->TextOutW(x0 + (j + 1) * Delta_x, y_abs, Text);
		}
	}
	pDC->SelectObject(&Standard_Font);
	pDC->SelectObject(&Standard);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
}

void CModelingandAnalysisofUncertaintyView::DisplayEigenvaluesAndCumVariance(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint TopLeft, CPoint BottomRight, bool flag) {
	int nVar, Length = BottomRight.x - TopLeft.x - 50, Height = BottomRight.y - TopLeft.y - 75;
	int x0 = TopLeft.x +25, y0 = TopLeft.y, X1, Y1, X2, Y2, KG = 0;
	double x_relative, y_relative, value, max;
	CPoint Point;
	CString Text;
	CRect rectA(x0, y0, x0 + 250, y0 + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(x0 + 20, y0 + 10, _T("Eigenvalues/cumulative variance"));
	y0 += 50;
	if (pDoc->OnlyX == false) nVar = pDoc->n_Var;
	else nVar = pDoc->n_Var - 1;
	CPen pen_red_thin(PS_SOLID, 1, RGB(178, 34, 52));
	CPen* pOldPen = pDC->SelectObject(&pen_red_thin);
	pDC->SetTextColor(RGB(178, 34, 52));
	// starting with the display of the eigenvalues
	CArray <double> lambda, cum_var, XTicks, YTicks, XTickLabels, YTickLabels;
	lambda.RemoveAll();
	max = pDoc->lambda.GetAt(0);
	for (int i = 0; i < pDoc->lambda.GetSize(); i++) {
		value = pDoc->lambda.GetAt(i);
		if (value >= 1) KG = i;
		value /= max;
		lambda.Add(value);
	}
	GetTicks((double)0, max, YTicks, YTickLabels);
	double dx = 1.0 / (double)(lambda.GetSize() + 1), factor = 1.0 - 2 * dx;
	// drawing left y - axis
	x_relative = 0.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length);
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->MoveTo(Point);
	y_relative = 0.0;
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->LineTo(Point);
	// drawing ticks on left y-axis
	pDC->SetTextAlign(TA_RIGHT);
	int digit_old = 0, digit_new, pos;
	double min;
	GetMinimum(YTickLabels, pos, min);
	digit_new = FormatTickLabel(Text, YTickLabels.GetAt(pos), digit_old);
	if (digit_new > digit_old) digit_old = digit_new;
	for (int i = 0; i < YTicks.GetSize(); i++) {
		x_relative = 0.0, y_relative = (1 - YTicks.GetAt(i));
		Point.x = x0 + (int)(x_relative * (double)Length) + 10, Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, YTickLabels.GetAt(i), digit_old), pDC->TextOutW(Point.x - 2, Point.y - 7, Text);
	}
	CPen KG_Threshold;
	KG_Threshold.CreatePen(PS_DASHDOT, 1, RGB(178, 34, 52));
	pDC->SelectObject(&KG_Threshold);
	x_relative = 0.0, y_relative = 1.0 -  1.0 / max;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	x_relative = 1.0 - dx, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	CPen pen_black_thin(PS_SOLID, 1, RGB(0, 0, 0));
	pOldPen = pDC->SelectObject(&pen_black_thin);
	pDC->SetTextColor(RGB(0, 0, 0));
	// drawing x-axis
	x_relative = 0.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length);
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->MoveTo(Point);
	x_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length);
	pDC->LineTo(Point);
	// Ticks on x-axis
	GetTicks((double)1, (double)pDoc->lambda.GetSize(), XTicks, XTickLabels);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = dx +  XTicks.GetAt(i) * factor, y_relative = 1.0;
		Point.x = x0 + (int)(x_relative * (double)Length);
		Point.y = y0 + (int)(y_relative * (double)Height) - 10;
		pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * (double)Height);
		pDC->LineTo(Point);
		FormatTickLabel(Text, XTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	// drawing right y-axis
	CPen pen_blue_thin(PS_SOLID, 1, RGB(60, 59, 110));
	pOldPen = pDC->SelectObject(&pen_blue_thin);
	pDC->SetTextColor(RGB(60, 59, 110));
	x_relative = 1.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative *(double)Length);
	Point.y = y0 + (int)(y_relative *(double)Height);
	pDC->MoveTo(Point);
	y_relative = 0.0;
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->LineTo(Point);
	// Ticks on right y-axis
	pDC->SetTextAlign(TA_LEFT);
	for (int i = 0; i <= 10; i = i + 2) {
		value = (double)i / 10;
		x_relative = 1.0, y_relative = (1 - value) * 1.0;
		Point.x = x0 + (int)(x_relative * (double)Length) - 10, Point.y = y0 + (int)(y_relative *(double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		Text.Format(L"%.1f", value), pDC->TextOutW(Point.x + 2, Point.y - 7, Text);
	}
	// drawing bars for eigenvalues and cumulative variance
	x_relative = 0.0;
	for (int i = 0; i < lambda.GetSize(); i++) {
		x_relative += dx;
		y_relative = 1 - lambda.GetAt(i);
		X1 = x0 + (int)((x_relative - 0.35 * dx) * (double)Length);
		Y1 = y0 + (int)(y_relative * (double)Height);
		X2 = x0 + (int)((x_relative - 0.1 * dx) * (double)Length);
		Y2 = y0 + Height;
		CRect rectEig(X1, Y1, X2, Y2);
		pDC->Rectangle(rectEig);
		CBrush brushEig;
		brushEig.CreateSolidBrush(RGB(178, 34, 52));
		pDC->FillRect(&rectEig, &brushEig);
		y_relative = 1 - pDoc->CumVar.GetAt(i);
		X1 = x0 + (int)((x_relative + 0.1 * dx) * (double)Length);
		Y1 = y0 + (int)(y_relative * (double)Height);
		X2 = x0 + (int)((x_relative + 0.35 * dx) * (double)Length);
		CRect rectCumVar(X1, Y1, X2, Y2);
		pDC->Rectangle(rectCumVar);
		CBrush brushCumVar;
		brushCumVar.CreateSolidBrush(RGB(60, 59, 110));
		pDC->FillRect(&rectCumVar, &brushCumVar);
	}
	CFont XLabel, YLeftLabel, YRightLabel;
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, _T("Principal component"));
	YLeftLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLeftLabel), pDC->SetTextAlign(TA_CENTER | TA_BOTTOM), pDC->SetTextColor(RGB(178, 34, 52));
	pDC->TextOutW(x0 - 30, y0 + (int)(0.5 * Height), _T("Eigenvalue"));
	YRightLabel.CreateFont(20, 10, 2700, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(YRightLabel), pDC->SetTextAlign(TA_CENTER | TA_BOTTOM), pDC->SetTextColor(RGB(60, 59, 110));
	pDC->TextOutW(x0 + Length + 30, y0 + (int)(0.5 * Height), _T("Cumulative variance"));
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pDC->SelectObject(&Standard_Font);
	y0 -= 50;
	CRect SuggestedNumberPCs(x0 + 270, y0, x0 + 570, y0 + 38);
	pDC->Rectangle(SuggestedNumberPCs);
	CBrush Brush_NPCs;
	Brush_NPCs.CreateSolidBrush(RGB(199, 248, 7));
	pDC->FillRect(&SuggestedNumberPCs, &Brush_NPCs);
	pDC->SetBkMode(TRANSPARENT);
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	CString Temp;
	if (flag == true) {
		Text.Empty(), Text.Append(L"Suggested # of PCs : ");
		Temp.Empty(), Temp.Format(L"%d", KG + 1), Text.Append(Temp + L" (Guttman-Kaiser)");
	}
	else {
		Text.Empty(), Text.Append(L"Selected # of PCs : ");
		Temp.Empty(), Temp.Format(L"%d", pDoc->n), Text.Append(Temp);
	}
	pDC->TextOutW(x0 + 290, y0 + 10, Text);
}

void CModelingandAnalysisofUncertaintyView::DisplayStoppingRules(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint TopLeft, CPoint BottomRight){
	int nVar = 0, Length = BottomRight.x - TopLeft.x - 50, Height = BottomRight.y - TopLeft.y - 75;
	int x0 = TopLeft.x + 25, y0 = TopLeft.y, pos_vpc = 0, pos_vre = 0;
	double x_relative, y_relative, value, value_vpc, value_vre, vpc_min = pDoc->vpc.GetAt(0), vre_min = pDoc->vre.GetAt(0), vpc_max = 0.0, vre_max = 0.0;
	CPoint Point;
	CString Text;
	CRect rectA(x0, y0, x0 + 250, y0 + 38);
	CArray <double> XTicks, YTicks, XTickLabels, YTickLabels;
	CBrush Brush_Header;
	CFont Standard_Font;
	pDC->Rectangle(rectA);
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(x0 + 20, y0 + 10, _T("Stopping rules"));
	y0 += 50;
	GetMinimum(pDoc->vpc, pos_vpc, vpc_min);
	GetMinimum(pDoc->vre, pos_vre, vpc_min);
	nVar = max(pos_vpc, pos_vre) + 3;
	if (nVar > (int)pDoc->vpc.GetSize()) nVar = (int)pDoc->vpc.GetSize();
	for (int i = 0; i < nVar; i++) {
		value = pDoc->vpc.GetAt(i);
		if (value > vpc_max) vpc_max = value;
		value = pDoc->vre.GetAt(i);
		if (value > vre_max) vre_max = value;
	}
	GetTicks((double)1, (double)nVar, XTicks, XTickLabels);
	GetTicks((double)0, pDoc->DMAX(vpc_max,vre_max), YTicks, YTickLabels);
	CPen pen_red_thin(PS_SOLID, 1, RGB(178, 34, 52)), pen_blue_thin(PS_SOLID, 1, RGB(60, 59, 110));
	pDC->SelectObject(&pen_red_thin);
	// left y-axis referring to values of VPC stopping rule
	x_relative = 0.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->MoveTo(Point);
	y_relative = 0.0;
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->LineTo(Point);
	// ticks on left y-axis
	pDC->SetTextAlign(TA_RIGHT);
	pDC->SetTextColor(RGB(178, 34, 52));
	int digit_old = 0, digit_new, pos;
	double min = 0.0;
	GetMinimum(YTickLabels, pos, min);
	digit_new = FormatTickLabel(Text, YTickLabels.GetAt(pos), digit_old);
	if (digit_new > digit_old) digit_old = digit_new;
	for (int i = 0; i < YTicks.GetSize(); i++) {
		x_relative = 0.0, y_relative = 1 - YTicks.GetAt(i);
		Point.x = x0 + (int)(x_relative * (double)Length) + 10, Point.y = y0 + (int)(y_relative * (double)Height);
		pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, YTickLabels.GetAt(i), digit_old), pDC->TextOutW(Point.x - 4, Point.y - 7, Text);
	}
	// plotting x-axis
	CPen pen_black_thin;
	pen_black_thin.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&pen_black_thin), pDC->SetTextColor(RGB(0, 0, 0));
	x_relative = 0.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	// x-ticks
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = XTicks.GetAt(i), y_relative = 1.0;
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height) - 10, pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
		FormatTickLabel(Text, XTickLabels.GetAt(i), 0), pDC->TextOutW(Point.x, Point.y + 2, Text);
	}
	//GetTicks((double)0, pDoc->vre.GetAt(static_cast <int64_t>(nVar-1)), YTicks, YTickLabels);
	// right y-label referring to values of VRE stopping rule
	pDC->SelectObject(&pen_blue_thin);
	x_relative = 1.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	y_relative = 0.0, Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
	pDC->SetTextAlign(TA_LEFT), pDC->SetTextColor(RGB(60, 59, 110));
	GetMinimum(YTickLabels, pos, min);
	digit_new = FormatTickLabel(Text, YTickLabels.GetAt(pos), digit_old);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		x_relative = 1.0, y_relative = 1.0 - YTicks.GetAt(i);
		Point.x = x0 + (int)(x_relative * (double)Length) - 10, Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, YTickLabels.GetAt(i), digit_old), pDC->TextOutW(Point.x + 4, Point.y - 7, Text);
	}
	double dx = 1.0 / (double)(nVar-1);
	x_relative = 0.0;
	CPen MIN_VPC;
	MIN_VPC.CreatePen(PS_SOLID, 6, RGB(178, 34, 52));
	for (int i = 0; i < nVar; i++) {
		value_vpc = 1.0 - pDoc->vpc.GetAt(i) / vpc_max;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(value_vpc * Height);
		if (i == 0) pDC->MoveTo(Point);
		else {
			pDC->SelectObject(&pen_red_thin);
			Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(value_vpc * Height), pDC->LineTo(Point);
		}
		
		if (i == pos_vpc) {
			pDC->SelectObject(&MIN_VPC), pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
		}
		else {
			pDC->SelectObject(&pen_blue_thin), pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
		}
		x_relative += dx;
	}
	x_relative = 0.0;
	CPen MIN_VRE;
	MIN_VRE.CreatePen(PS_SOLID, 6, RGB(60, 59, 110));
	for (int i = 0; i < nVar; i++) {
		value_vre = 1.0 - pDoc->vre.GetAt(i) / vre_max;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(value_vre * Height);
		if (i == 0) pDC->MoveTo(Point);
		else {
			pDC->SelectObject(&pen_blue_thin);
			Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(value_vre * Height), pDC->LineTo(Point);
		}
		if (i == pos_vre) pDC->SelectObject(&MIN_VRE);
		else pDC->SelectObject(&pen_blue_thin);
		pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
		x_relative += dx;
	}
	CFont XLabel, YLeftLabel, YRightLabel;
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, _T("# Principal components retained"));
	YLeftLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLeftLabel), pDC->SetTextAlign(TA_CENTER | TA_BOTTOM), pDC->SetTextColor(RGB(178, 34, 52));
	pDC->TextOutW(x0 - 40, y0 + (int)(0.5 * Height), _T("VPC score"));
	YRightLabel.CreateFont(20, 10, 2700, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(YRightLabel), pDC->SetTextAlign(TA_CENTER | TA_BOTTOM), pDC->SetTextColor(RGB(60, 59, 110));
	pDC->TextOutW(x0 + Length + 40, y0 + (int)(0.5 * Height), _T("VRE score"));
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	pDC->SelectObject(&Standard_Font);
	Text.Empty(), Text.Append(L"Suggested # of PCs : ");
	CString Temp;
	Temp.Empty(), Temp.Format(L"%d", pos_vpc + 1), Text.Append(Temp + L" (VPC) and ");
	Temp.Empty(), Temp.Format(L"%d", pos_vre + 1), Text.Append(Temp + L" (VRE)");
	y0 -= 50;
	CRect SuggestedNumberPCs(x0 + 270, y0, x0 + 570, y0 + 38);
	pDC->Rectangle(SuggestedNumberPCs);
	CBrush Brush_NPCs;
	Brush_NPCs.CreateSolidBrush(RGB(199, 248, 7));
	pDC->FillRect(&SuggestedNumberPCs, &Brush_NPCs);
	pDC->SetBkMode(TRANSPARENT);
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->TextOutW(x0 + 290, y0 + 10, Text);
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pDC->SelectObject(&Standard_Font); 
}

void CModelingandAnalysisofUncertaintyView::DisplayLoadings(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& loadings, CPoint TopLeft, CPoint BottomRight, CString Label) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	int Length = BottomRight.x - TopLeft.x - 25, Height = BottomRight.y - TopLeft.y - 25, x0 = TopLeft.x, y0 = TopLeft.y + 25, nObs = pDoc->n_Obs;
	double min, max, value, range, x_relative, y_relative;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 125, TopLeft.y + 20);
	CString Text;
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(&Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(TopLeft.x + 3, TopLeft.y + 3, Label);
	x_relative = 0.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length);
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->MoveTo(Point);
	y_relative = 0.0;
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->LineTo(Point);
	int i = 0;
	CArray <double> YTicks, YTickLabels;
	int pos;
	GetMaximum(loadings, pos, max), GetMinimum(loadings, pos, min);
	if (min > 0.0) min = 0.0;
	if (max < 0.0) max = 0.0;
	range = max - min;
	y_relative = 1.0 + min / range;
	GetTicks(min, max, YTicks, YTickLabels);
	double abscissa = y_relative;
	x_relative = 0.0;
	Point.x = x0 + (int)(x_relative * (double)Length);
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->MoveTo(Point);
	x_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length);
	pDC->LineTo(Point);
	int digit_old = 0, digit_new;
	double temp;
	GetMinimum(YTickLabels, pos, temp);
	digit_new = FormatTickLabel(Text, YTickLabels.GetAt(pos), digit_old);
	if (digit_new > digit_old) digit_old = digit_new;
	pDC->SetTextAlign(TA_RIGHT);
	for (i = 0; i < YTicks.GetSize(); i++) {
		x_relative = 0.0, y_relative = 1.0 - YTicks.GetAt(i);
		Point.x = x0 + (int)(x_relative * (double)Length) + 10, Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, YTickLabels.GetAt(i), digit_old), pDC->TextOutW(Point.x - 2, Point.y - 7, Text);
	}
	double dx = 1.0 / (double)(loadings.GetSize() + 1);
	int x1, y1, x2, y2;
	x_relative = 0;
	y1 = y0 + (int)(abscissa * (double)Height);
	for (int i = 0; i < loadings.GetSize(); i++) {
		x_relative += dx;
		value = loadings.GetAt(i);
		value -= min;
		value /= range;
		y_relative = 1.0 - value;
		x1 = x0 + (int)((x_relative - 0.4 * dx) * (double)Length);
		x2 = x0 + (int)((x_relative + 0.4 * dx) * (double)Length);
		y2 = y0 + (int)(y_relative * (double)Height);
		if (y2 > y1) {
			CRect InteriorBar(x1 + 2, y1 + 2, x2 - 2, y2 - 2), Frame(x1, y1, x2, y2);
			pDC->Rectangle(Frame);
			CBrush FrameColor;
			FrameColor.CreateSolidBrush(RGB(243, 102, 25));
			pDC->FillRect(&Frame, &FrameColor);
			pDC->Rectangle(InteriorBar);
			CBrush InteriorColor;
			InteriorColor.CreateSolidBrush(RGB(255, 224, 146));
			pDC->FillRect(&InteriorBar, &InteriorColor);
		}
		else {
			CRect InteriorBar(x1 + 2, y1 - 2, x2 - 2, y2 + 2), Frame(x1, y1, x2, y2);
			pDC->Rectangle(Frame);
			CBrush FrameColor;
			FrameColor.CreateSolidBrush(RGB(243, 102, 25));
			pDC->FillRect(&Frame, &FrameColor);
			pDC->Rectangle(InteriorBar);
			CBrush InteriorColor;
			InteriorColor.CreateSolidBrush(RGB(255, 224, 146));
			pDC->FillRect(&InteriorBar, &InteriorColor);
		}
		if (y_relative <= abscissa) pDC->SetTextAlign(TA_CENTER | TA_TOP);
		else pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pDC->TextOutW(x0 + (int)(x_relative * (double)Length), y1, pDoc->Tag.GetAt(i));
	}
	pDC->SelectObject(&Standard_Font);
	pDC->SelectObject(&Standard);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
}

void CModelingandAnalysisofUncertaintyView::DisplayScores(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& Scores, CPoint TopLeft, CPoint BottomRight, CString Label) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	int Length = BottomRight.x - TopLeft.x - 25, Height = BottomRight.y - TopLeft.y - 25, x0 = TopLeft.x, y0 = TopLeft.y + 25, nObs = pDoc->n_Obs, pos;
	double min, max, value, range, x_relative, y_relative;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 125, TopLeft.y + 20);
	CString Text;
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(&Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(TopLeft.x + 3, TopLeft.y + 3, Label);
	x_relative = 0.0, y_relative = 0.0;
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
	y_relative = 1.0, Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
	GetMaximum(Scores, pos, max), GetMinimum(Scores, pos, min);
	if (min > 0.0) min = 0.0;
	if (max < 0.0) max = 0.0;
	CArray <double> XTicks, YTicks, XTickLabels, YTickLabels;
	GetTicks(min, max, YTicks, YTickLabels);
	x_relative = 0.0, Point.x = x0 + (int)(x_relative * Length);
	int digit_old = 0, digit_new;
	double temp, abscissa;
	range = max - min, abscissa = 1 + min / range;
	GetMinimum(YTickLabels, pos, temp);
	digit_new = FormatTickLabel(Text, YTickLabels.GetAt(pos), digit_old);
	if (digit_new > digit_old) digit_old = digit_new;
	pDC->SetTextAlign(TA_RIGHT | TA_TOP);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		y_relative = 0.05 + 0.9 * (1 - YTicks.GetAt(i)), Point.y = y0 + (int)(y_relative * Height);
		pDC->MoveTo(Point.x + 10, Point.y), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, YTickLabels.GetAt(i), digit_old);
		pDC->TextOutW(Point.x - 2, Point.y - 7, Text);
	}
	x_relative = 0.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
	GetTicks((double)1, (double)pDoc->n_Obs, XTicks, XTickLabels);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = XTicks.GetAt(i), Point.x = x0 + (int)(x_relative * Length);
		pDC->MoveTo(Point.x, Point.y - 10), pDC->LineTo(Point);
		FormatTickLabel(Text, XTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	double dx = 1 / (double)Scores.GetSize();
	x_relative = 0.0;
	int x1, x2, y1, y2;
	for (int i = 0; i < Scores.GetSize(); i++) {
		x_relative += dx;
		value = Scores.GetAt(i);
		value -= min;
		value /= range;
		y_relative = 0.05 + 0.9 * (1 - value);
		x1 = x0 + (int)(x_relative * Length) - 5, x2 = x1 + 10, y1 = y0 + (int)(y_relative * Height) - 5, y2 = y1 + 10;
		CPen Marks;
		Marks.CreatePen(PS_SOLID, 2, RGB(243, 102, 25));
		pDC->SelectObject(&Marks);
		pDC->Ellipse(x1, y1, x2, y2);
		CRect rect(x1 + 2, y1 + 2, x2 - 2, y2 - 2);
		pDC->Rectangle(rect);
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 224, 146));
		pDC->FillRect(&rect, &brush);
	}
	pDC->SelectObject(&Standard_Font);
	pDC->SelectObject(&Standard);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
}

void CModelingandAnalysisofUncertaintyView::DisplayUnivariateStatistics(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint TopLeft, CPoint BottomRight) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	int Length = BottomRight.x - TopLeft.x - 25, Height = BottomRight.y - TopLeft.y - 25, x0 = TopLeft.x, y0 = TopLeft.y, nObs = pDoc->n_Obs;
	double value, x_relative, y_relative, value_t = 0.0, value_q = 0.0;
	CPoint Point;
	CRect rectA(x0, y0, x0 + 250, y0 + 38);
	CString Text;
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(&Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(x0 + 10, y0 + 10, _T("Univariate statistics"));
	y0 += 50;
	// *** Displaying T squared statistic 
	CArray <double> t_2, spe, XTicks, YTicks, XTickLabels, YTickLabels;
	t_2.RemoveAll(), spe.RemoveAll();
	for (int i = 0; i < pDoc->n_Obs; i++) {
		value = pDoc->T_2.GetAt(i);
		t_2.Add(value);
		if (value > value_t) value_t = t_2.GetAt(i);
		value = pDoc->SPE.GetAt(i);
		spe.Add(value);
		if (value > value_q) value_q = spe.GetAt(i);
	}
	for (int i = 0; i < pDoc->n_Obs; i++) {
		value = t_2.GetAt(i);
		value /= value_t;
		t_2.SetAt(i, value);
		value = spe.GetAt(i);
		value /= value_q;
		spe.SetAt(i, value);
	}
	// Draw y-axis
	GetTicks((double)0, value_t, YTicks, YTickLabels);
	x_relative = 0.0, y_relative = 0.475;
	Point.x = x0 + (int)(x_relative * (double)Length);
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->MoveTo(Point);
	y_relative = 0.0;
	Point.y = y0 + (int)(y_relative * (double)Height);
	pDC->LineTo(Point);
	// Ticks on y-axis
	int digit_old = 0, digit_new, pos;
	double min;
	GetMinimum(YTickLabels, pos, min);
	digit_new = FormatTickLabel(Text, YTickLabels.GetAt(pos), digit_old);
	if (digit_new > digit_old) digit_old = digit_new;
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		x_relative = 0.0, y_relative = ( 1.0 - YTicks.GetAt(i) ) * 0.475;
		Point.x = x0 + (int)(x_relative * (double)Length) + 10, Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text,YTickLabels.GetAt(i), digit_old), pDC->TextOutW(Point.x - 2, Point.y - 7, Text);
	}
	// Draw x-axis for T-squared statistic
	GetTicks((double)1, (double)pDoc->n_Obs, XTicks, XTickLabels);
	x_relative = 0.0, y_relative = 0.475;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	// Ticks on x-axis
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = XTicks.GetAt(i), y_relative = 0.475;
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height) - 10, pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
		FormatTickLabel(Text, XTickLabels.GetAt(i), 0), pDC->TextOutW(Point.x, Point.y, Text);
	}
	// draw values of T^2 statistic
	double x = 0, dx = 1.0 / (double)t_2.GetSize();
	int x1, y1, x2, y2;
	CPen pen_black(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen_black);
	for (int i = 0; i < t_2.GetSize(); i++) {
		x += dx, x_relative = x, y_relative = (1 - t_2.GetAt(i)) * 0.475;
		x1 = x0 + (int)(x_relative * (double)Length) - 3;
		y1 = y0 + (int)(y_relative * (double)Height) - 3;
		x2 = x1 + 6;
		y2 = y1 + 6;
		pDC->Ellipse(x1, y1, x2, y2);
	}
	double t95 = pDoc->T95 / value_t, q95 = pDoc->Q95 / value_q;
	x_relative = 0.0, y_relative = (1 - t95) * 0.475;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	CPen pen_ocre(PS_SOLID, 3, RGB(243, 102, 25));
	pDC->SelectObject(&pen_ocre);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	// Add label to y-axis
	CFont YLabel_T;
	YLabel_T.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel_T), pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pDC->TextOutW(x0 - 40, y0 + (int)(0.2375 * Height), _T("T squared statistic"));
	pDC->SelectObject(&Standard_Font);
	// *** Displaying Q statistic
	// Draw y-axis
	CPen pen_thinblack(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&pen_thinblack);
	x_relative = 0.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	y_relative = 0.525, Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
	// Ticks on y-axis
	GetTicks((double)0, value_q, YTicks, YTickLabels);
	digit_old = 0;
	digit_new = FormatTickLabel(Text, YTickLabels.GetAt(pos), digit_old);
	if (digit_new > digit_old) digit_old = digit_new;
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		x_relative = 0.0, y_relative = 0.525 + (1 - YTicks.GetAt(i)) * 0.475;
		Point.x = x0 + (int)(x_relative * (double)Length) + 10, Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, YTickLabels.GetAt(i), digit_old);
		pDC->TextOutW(Point.x, Point.y - 7, Text);
	}
	// x-axis
	x_relative = 0.0, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	// Ticks on x-axis
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = XTicks.GetAt(i), y_relative = 1.0;
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height) - 10, pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
		FormatTickLabel(Text, XTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	// Draw Q statistic
	x = 0;
	pDC->SelectObject(&pen_black);
	for (int i = 0; i < spe.GetSize(); i++) {
		x += dx, x_relative = x, y_relative = 0.525 + (1 - spe.GetAt(i)) * 0.475;
		x1 = x0 + (int)(x_relative * (double)Length) - 3;
		y1 = y0 + (int)(y_relative * (double)Height) - 3;
		x2 = x1 + 6;
		y2 = y1 + 6;
		pDC->Ellipse(x1, y1, x2, y2);
	}
	x_relative = 0.0, y_relative = 0.525 + (1 - q95) * 0.475;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	pOldPen = pDC->SelectObject(&pen_ocre);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	// Add label to y-axis
	CFont YLabel_Q;
	YLabel_Q.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel_Q), pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pDC->TextOutW(x0 - 40, y0 + (int)(0.7375 * Height), _T("Q statistic"));
	pDC->SelectObject(&Standard_Font);
}

int CModelingandAnalysisofUncertaintyView::DisplayPCAModelResults(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint TopLeft) {
	CPen Standard, Separator;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	Separator.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	int x0 = TopLeft.x, y0 = TopLeft.y, Delta_y = 20, Delta_x_1 = 30, Delta_x_2 = 70, pos;
	CString Text, Temp;
	CRect rectA(x0, y0, x0 + 250, y0 + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(&Standard_Font);
	double value, min, max;
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(x0 + 10, y0 + 10, _T("PCA model"));
	y0 += 50;
	pDC->SetTextAlign(TA_CENTER);
	Text.Empty(), Text.Append(_T("PC")), pDC->TextOutW(x0 + (int)(Delta_x_1 / 2), y0, Text);
	pDC->MoveTo(x0 + Delta_x_1, y0), pDC->LineTo(x0 + Delta_x_1, y0 + Delta_y);
	pDC->MoveTo(x0, y0 + 18), pDC->LineTo(x0 + Delta_x_1, y0 + 18);
	Text.Empty(), Text.Append(_T("lambda")), pDC->TextOutW(x0 + Delta_x_1 + (int)(0.5 * Delta_x_2), y0, Text);
	pDC->MoveTo(x0 + Delta_x_1 + Delta_x_2, y0), pDC->LineTo(x0 + Delta_x_1 + Delta_x_2, y0 + Delta_y);
	pDC->MoveTo(x0 + Delta_x_1, y0 + 18), pDC->LineTo(x0 + Delta_x_1 + Delta_x_2, y0 + 18);
	Text.Empty(), Text.Append(_T("Variance")), pDC->TextOutW(x0 + Delta_x_1 + (int)(1.5 * Delta_x_2), y0, Text);
	pDC->MoveTo(x0 + Delta_x_1 + 2 * Delta_x_2, y0), pDC->LineTo(x0 + Delta_x_1 + 2 * Delta_x_2, y0 + Delta_y);
	pDC->MoveTo(x0 + Delta_x_1 + Delta_x_2, y0 + 18), pDC->LineTo(x0 + Delta_x_1 + 2 * Delta_x_2, y0 + 18);
	Text.Empty(), Text.Append(_T("Cumulat.")), pDC->TextOutW(x0 + Delta_x_1 + (int)(2.5 * Delta_x_2), y0, Text);
	pDC->MoveTo(x0 + Delta_x_1 + 2 * Delta_x_2, y0 + 18), pDC->LineTo(x0 + Delta_x_1 + 3 * Delta_x_2, y0 + 18);
	for (int i = 0; i < pDoc->n; i++) {
		Text.Empty(), Temp.Empty(), Temp.Format(L"%d", i + 1), Text.Append(L"PC " + Temp);
		pDC->TextOutW(x0 + Delta_x_1 + (int)((3.5 + i) * Delta_x_2), y0, Text);
		if (i == 0) pDC->SelectObject(&Separator);
		else pDC->SelectObject(&Standard);
		pDC->MoveTo(x0 + Delta_x_1 + (3 + i) * Delta_x_2, y0), pDC->LineTo(x0 + Delta_x_1 + (3 + i) * Delta_x_2, y0 + Delta_y);
		pDC->SelectObject(&Standard);
		pDC->MoveTo(x0 + Delta_x_1 + (3 + i) * Delta_x_2, y0 + 18), pDC->LineTo(x0 + Delta_x_1 + (4 + i) * Delta_x_2, y0 + 18);
	}
	Text.Empty(), Text.Append(L"Variable");
	pDC->TextOutW(x0 + Delta_x_1 + (int)((3.5 + pDoc->n) * Delta_x_2), y0, Text);
	pDC->MoveTo(x0 + Delta_x_1 + (3 + pDoc->n) * Delta_x_2, y0), pDC->LineTo(x0 + Delta_x_1 + (3 + pDoc->n) * Delta_x_2, y0 + Delta_y);
	pDC->MoveTo(x0 + Delta_x_1 + (3 + pDoc->n) * Delta_x_2, y0 + 18), pDC->LineTo(x0 + Delta_x_1 + (4 + pDoc->n) * Delta_x_2, y0 + 18);
	CArray <double> pMax, w;
	pMax.RemoveAll();
	for (int i = 0; i < pDoc->n; i++) {
		pDoc->GetColumn(pDoc->P, pDoc->P_spec, w, i);
		GetMaximum(w, pos, max), GetMinimum(w,pos,min);
		value = pDoc->DMAX(fabs(max), fabs(min));
		pMax.Add(value);
	}
	pDC->SetTextAlign(TA_RIGHT);
	CBrush Retained_PCs, Red, Orange, Yellow, Yellow_bright;
	Retained_PCs.CreateSolidBrush(RGB(199, 248, 7));
	Red.CreateSolidBrush(RGB(250, 160, 113));
	Orange.CreateSolidBrush(RGB(250, 216, 46));
	Yellow.CreateSolidBrush(RGB(239, 250, 37));
	Yellow_bright.CreateSolidBrush(RGB(245, 249, 195));
	pDC->SetBkMode(TRANSPARENT);
	for (int i = 0; i < pDoc->lambda.GetSize(); i++) {
		if (i < pDoc->n) {
			CRect rect(x0 + 6, y0 + (i+1) * Delta_y + 2, x0 + Delta_x_1 - 5 , y0 + (i+2) * Delta_y - 2);
			pDC->Rectangle(rect), pDC->FillRect(&rect, &Retained_PCs);
		}
		Text.Empty(), Text.Format(L"%d", i + 1), pDC->SetTextAlign(TA_RIGHT), pDC->TextOutW(x0 + Delta_x_1 - 7, y0 + (i + 1) * Delta_y, Text);
		if (i < pDoc->n) {
			CRect rect(x0 + Delta_x_1 + 11, y0 + (i + 1) * Delta_y + 2, x0 + Delta_x_1 + Delta_x_2 - 11, y0 + (i + 2) * Delta_y - 2);
			pDC->Rectangle(rect), pDC->FillRect(&rect, &Retained_PCs);
		}
		Text.Empty(), Text.Format(L"%.3f", pDoc->lambda.GetAt(i)), pDC->TextOutW(x0 + Delta_x_1 + Delta_x_2 - 12, y0 + (i + 1) * Delta_y, Text);
		pDC->MoveTo(x0 + Delta_x_1 + Delta_x_2, y0 + (i + 1) * Delta_y), pDC->LineTo(x0 + Delta_x_1 + Delta_x_2, y0 + (i + 2) * Delta_y);
		pDC->MoveTo(x0 + Delta_x_1, y0 + (i + 1) * Delta_y), pDC->LineTo(x0 + Delta_x_1, y0 + (i + 2) * Delta_y);
		if (i < pDoc->n) {
			CRect rect(x0 + Delta_x_1 + Delta_x_2 + 14, y0 + (i + 1) * Delta_y + 2, x0 + Delta_x_1 + 2 * Delta_x_2 - 14, y0 + (i + 2) * Delta_y - 2);
			pDC->Rectangle(rect), pDC->FillRect(&rect, &Retained_PCs);
		}
		if (i == 0) value = pDoc->CumVar.GetAt(i);
		else value = pDoc->CumVar.GetAt(i) - pDoc->CumVar.GetAt(static_cast <int64_t>(i) - 1);
		Text.Empty(), Text.Format(L"%.3f", value), pDC->TextOutW(x0 + Delta_x_1 + 2 * Delta_x_2 - 15, y0 + (i + 1) * Delta_y, Text);
		pDC->MoveTo(x0 + Delta_x_1 + 2 * Delta_x_2, y0 + (i + 1) * Delta_y), pDC->LineTo(x0 + Delta_x_1 + 2 * Delta_x_2, y0 + (i + 2) * Delta_y);
		if (i < pDoc->n) {
			CRect rect(x0 + Delta_x_1 + 2 * Delta_x_2 + 14, y0 + (i + 1) * Delta_y + 2, x0 + Delta_x_1 + 3 * Delta_x_2 - 14, y0 + (i + 2) * Delta_y - 2);
			pDC->Rectangle(rect), pDC->FillRect(&rect, &Retained_PCs);
		}
		Text.Empty(), Text.Format(L"%.3f", pDoc->CumVar.GetAt(i)), pDC->TextOutW(x0 + Delta_x_1 + 3 * Delta_x_2 - 15, y0 + (i + 1) * Delta_y, Text);
		for (int j = 0; j < pDoc->n; j++) {
			value = pDoc->P.GetAt(pDoc->GetPosition(i, j, pDoc->P_spec));
			if ((fabs(value) >= 0.8 * pMax.GetAt(j)) && ((fabs(value) <= 1.0 * pMax.GetAt(j))) ) {
				CRect rect(x0 + Delta_x_1 + (3+j)*Delta_x_2 + 15, y0 + (i+1)*Delta_y + 2, x0 + Delta_x_1 + (4 + j) * Delta_x_2 - 15, y0 + (i + 2) * Delta_y - 2);
				pDC->Rectangle(rect), pDC->FillRect(&rect, &Red);
			}
			else if ((fabs(value) >= 0.6 * pMax.GetAt(j)) && ((fabs(value) < 0.8 * pMax.GetAt(j)))) {
				CRect rect(x0 + Delta_x_1 + (3 + j) * Delta_x_2 + 15, y0 + (i + 1) * Delta_y + 2, x0 + Delta_x_1 + (4 + j) * Delta_x_2 - 15, y0 + (i + 2) * Delta_y - 2);
				pDC->Rectangle(rect), pDC->FillRect(&rect, &Orange);
			}
			else if ((fabs(value) >= 0.4 * pMax.GetAt(j)) && ((fabs(value) < 0.6 * pMax.GetAt(j)))) {
				CRect rect(x0 + Delta_x_1 + (3 + j) * Delta_x_2 + 15, y0 + (i + 1) * Delta_y + 2, x0 + Delta_x_1 + (4 + j) * Delta_x_2 - 15, y0 + (i + 2) * Delta_y - 2);
				pDC->Rectangle(rect), pDC->FillRect(&rect, &Yellow);
			}
			else if ((fabs(value) >= 0.2 * pMax.GetAt(j)) && ((fabs(value) < 0.4 * pMax.GetAt(j)))) {
				CRect rect(x0 + Delta_x_1 + (3 + j) * Delta_x_2 + 15, y0 + (i + 1) * Delta_y + 2, x0 + Delta_x_1 + (4 + j) * Delta_x_2 - 15, y0 + (i + 2) * Delta_y - 2);
				pDC->Rectangle(rect), pDC->FillRect(&rect, &Yellow_bright);
			}
			Text.Empty(), Text.Format(L"%.3f", value), pDC->TextOutW(x0 + Delta_x_1 + (4 + j) * Delta_x_2 - 15, y0 + (i + 1) * Delta_y, Text);
			if (j == 0) pDC->SelectObject(&Separator);
			else pDC->SelectObject(&Standard);
			pDC->MoveTo(x0 + Delta_x_1 + (3 + j) * Delta_x_2, y0 + (i + 1) * Delta_y), pDC->LineTo(x0 + Delta_x_1 + (3 + j) * Delta_x_2, y0 + (i + 2) * Delta_y);
			pDC->SelectObject(&Standard);
		}
		pDC->SetTextAlign(TA_RIGHT);
		pDC->TextOutW(x0 + Delta_x_1 + (4 + pDoc->n) * Delta_x_2 - 23, y0 + (i + 1) * Delta_y, pDoc->Tag.GetAt(i));
		pDC->MoveTo(x0 + Delta_x_1 + (3 + pDoc->n) * Delta_x_2, y0 + (i + 1) * Delta_y);
		pDC->LineTo(x0 + Delta_x_1 + (3 + pDoc->n) * Delta_x_2, y0 + (i + 2) * Delta_y);
	}
	return y0 + ( (int)pDoc->lambda.GetSize() + 1 ) * Delta_y;
}

void CModelingandAnalysisofUncertaintyView::DisplayScatterDiagrams(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint TopLeft, CPoint BottomRight, bool Task, bool Display, CString Method) {
	int x0 = TopLeft.x, y0 = TopLeft.y, Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y;
	int x_Length, y_Length, nVar;
	double value;
	bool flag;
	CPoint topleft, bottomright;
	CString Label_x, Label_y, Text;
	CArray <double> Values_x, Values_y;
	if (Flag_Select) {
		if (pDoc->n == 2) n_Grid = 1;
		else if (pDoc->n == 3) n_Grid = 2;
		else if (pDoc->n > 3) n_Grid = 3;
		else n_Grid = 0;
		if (n_Grid == 0) return;
		SelectedVariables.RemoveAll();
		for (int i = 0; i < n_Grid + 1; i++) SelectedVariables.Add(i);
	}
	x_Length = (int)(Length / n_Grid), y_Length = (int)(Height / n_Grid);
	if (pDoc->OnlyX) nVar = pDoc->n_Var - 1;
	else nVar = pDoc->n_Var;
	for (int i = 0; i < n_Grid; i++) {
		for (int j = i + 1; j < n_Grid + 1; j++) {
			Values_x.RemoveAll(), Values_y.RemoveAll();
			topleft.x = x0 + (j - 1) * x_Length, topleft.y = y0 + i * y_Length;
			bottomright.x = x0 + j * x_Length, bottomright.y = y0 + (i + 1) * y_Length;
			Label_x.Empty(), Label_y.Empty();
			if (Task == true) {
				Text.Empty(), Text.Format(L"%d", SelectedVariables.GetAt(j) + 1), Label_x.Append(_T("Scores ") + Text);
				Text.Empty(), Text.Format(L"%d", SelectedVariables.GetAt(i) + 1), Label_y.Append(_T("Scores ") + Text);
				for (int k = 0; k < pDoc->n_Obs; k++) {
					if (Method == "PCA") value = pDoc->T.GetAt(pDoc->GetPosition(k, SelectedVariables.GetAt(j), pDoc->T_spec));
					else if (Method == "FA") value = pDoc->F.GetAt(pDoc->GetPosition(k, SelectedVariables.GetAt(j), pDoc->F_spec));
					Values_x.Add(value);
					if (Method == "PCA") value = pDoc->T.GetAt(pDoc->GetPosition(k, SelectedVariables.GetAt(i), pDoc->T_spec));
					else if (Method == "FA") value = pDoc->F.GetAt(pDoc->GetPosition(k, SelectedVariables.GetAt(i), pDoc->F_spec));
					Values_y.Add(value);
				}
			}
			else {
				Text.Empty(), Text.Format(L"%d", SelectedVariables.GetAt(j) + 1), Label_x.Append(_T("Loadings ") + Text);
				Text.Empty(), Text.Format(L"%d", SelectedVariables.GetAt(i) + 1), Label_y.Append(_T("Loadings ") + Text);
				for (int k = 0; k < nVar; k++) {
					if (Method == "PCA") value = pDoc->P.GetAt(pDoc->GetPosition(k, SelectedVariables.GetAt(j), pDoc->P_spec));
					else if (Method == "FA") value = pDoc->A.GetAt(pDoc->GetPosition(k, SelectedVariables.GetAt(j), pDoc->A_spec));
					Values_x.Add(value);
					if (Method == "PCA") value = pDoc->P.GetAt(pDoc->GetPosition(k, SelectedVariables.GetAt(i), pDoc->P_spec));
					else if (Method == "FA") value = pDoc->A.GetAt(pDoc->GetPosition(k, SelectedVariables.GetAt(i), pDoc->A_spec));
					Values_y.Add(value);
				}
			}
			if (i + 1 == j) flag = true;
			else flag = false;
			double nObs = (double)pDoc->n_Obs;
			ProbabilityDistributions F('F', (double)2, (double)(pDoc->n_Obs - 2));
			double f_crit = F.CriticalValue(pDoc->alpha);
			f_crit *= (2.0 * (nObs - 1.0 / nObs)) / (nObs - 2.0);
			DisplayScatterPlot(pDoc, pDC, Values_x, Values_y, topleft, bottomright, SelectedVariables.GetAt(j), SelectedVariables.GetAt(i), Label_x, Label_y, Task, Display, flag, f_crit);
		}
	}
}

void CModelingandAnalysisofUncertaintyView::DisplayScatterPlot(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& Values_x, CArray <double>& Values_y, CPoint TopLeft, CPoint BottomRight, int x, int y, CString Label_x, CString Label_y, bool Type, bool Display, bool Show, double f_crit) {
	int x0 = TopLeft.x + 20, y0 = TopLeft.y, Length = BottomRight.x - TopLeft.x - 20, Height = BottomRight.y - TopLeft.y - 20;
	double x_relative, y_relative, min_x, max_x, min_y, max_y, value, range_x, range_y;
	int pos;
	CArray <double> XTicks, YTicks, XTickLabels, YTickLabels;
	CPoint Point;
	CFont Label_x_text, Label_y_text;
	CString Text;
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	x_relative = 0.5, y_relative = 1.0;
	Label_x_text.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&Label_x_text);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height);
	if (Show == true) pDC->TextOutW(Point.x, Point.y, Label_x);
	x_relative = 0.0, y_relative = 0.5;
	Label_y_text.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	if (Show == true) pDC->SelectObject(&Label_y_text);
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height);
	if (Show == true) pDC->TextOutW(Point.x, Point.y, Label_y);
	x0 = x0 + 30, Length -= 30, Height -= 30;
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(&Standard_Font);
	double SemiMajor = 0.0, SemiMinor = 0.0;
	if ((Type == true) && (Display == true)) {
		SemiMinor = sqrt(pDoc->lambda.GetAt(x) * f_crit), SemiMajor = sqrt(pDoc->lambda.GetAt(y) * f_crit);
	}
	x_relative = 0.0, y_relative = 0.0;
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0+(int)(y_relative*Height),pDC->MoveTo(Point.x, Point.y);
	y_relative = 1.0;
	Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point.x, Point.y);
	x_relative = 1.0;
	Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point.x, Point.y);
	GetMinimum(Values_x, pos, min_x), GetMaximum(Values_x, pos, max_x);
	GetMinimum(Values_y, pos, min_y), GetMaximum(Values_y, pos, max_y);
	if (min_x > 0) min_x = 0.0;
	if (max_x < 0) max_x = 0.0;
	if (min_y > 0) min_y = 0.0;
	if (max_y < 0) max_y = 0.0;
	range_x = max_x - min_x;
	range_y = max_y - min_y;
	if ((Type == true) && (Display == true)) {
		if (SemiMinor > max_x) max_x = SemiMinor;
		if (-SemiMinor < min_x) min_x = -SemiMinor;
		if (SemiMajor > max_y) max_y = SemiMajor;
		if (-SemiMajor < min_y) min_y = -SemiMajor;
		range_x = max_x - min_x;
		range_y = max_y - min_y;
		double x1, x2, y1, y2, xbar = -min_x / range_x, ybar = 1 + min_y / range_y;
		x1 = 0.05 + 0.9 * (xbar - SemiMinor / range_x);
		x2 = 0.05 + 0.9 * (xbar + SemiMinor / range_x);
		y1 = 0.05 + 0.9 * (ybar - SemiMajor / range_y);
		y2 = 0.05 + 0.9 * (ybar + SemiMajor / range_y);
		int X1, X2, Y1, Y2;
		X1 = x0 + (int)(x1 * Length), X2 = x0 + (int)(x2 * Length);
		Y1 = y0 + (int)(y1 * Height), Y2 = y0 + (int)(y2 * Height);
		pDC->Ellipse(X1, Y1, X2, Y2);
	}
	GetTicks(min_x, max_x, XTicks, XTickLabels);
	GetTicks(min_y, max_y, YTicks, YTickLabels);
	y_relative = 1.0, Point.y = y0 + (int)(y_relative * Height);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = 0.05 + 0.9 * XTicks.GetAt(i), Point.x = x0 + (int)(x_relative * Length), pDC->MoveTo(Point);
		pDC->LineTo(Point.x, Point.y - 10);
		if (Show == true) {
			FormatTickLabel(Text, XTickLabels.GetAt(i), 0);
			pDC->TextOutW(Point.x, Point.y, Text);
		}
	}
	x_relative = 0.0, Point.x = x0 + (int)(x_relative * Length);
	pDC->SetTextAlign(TA_TOP | TA_RIGHT);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		y_relative = 0.05 + 0.9 * (1 - YTicks.GetAt(i)), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
		pDC->LineTo(Point.x + 10, Point.y);
		if (Show == true) {
			FormatTickLabel(Text, YTickLabels.GetAt(i), 0);
			pDC->TextOutW(Point.x - 2, Point.y - 7, Text);
		}
	}
	CPen Marks;
	Marks.CreatePen(PS_SOLID, 2, RGB(243, 102, 25));
	pDC->SelectObject(&Marks);
	CFont hFont;
	hFont.CreateFont(10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, _T("SYSTEM_FIXED_FONT"));
	pDC->SelectObject(hFont);
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	for (int i = 0; i < Values_x.GetSize(); i++) {
		value = Values_x.GetAt(i);
		value -= min_x;
		value /= range_x;
		x_relative = 0.05 + 0.9 * value;
		value = Values_y.GetAt(i);
		value -= min_y;
		value /= range_y;
		y_relative = 0.05 + 0.9 * (1 - value);
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height);
		pDC->Ellipse(Point.x - 3, Point.y - 3, Point.x + 3, Point.y + 3);
		if ( (Type == false) && (Display == true) ) pDC->TextOutW(Point.x, Point.y - 5, pDoc->Tag.GetAt(i));
	}
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	pDC->SelectObject(&Standard_Font);
}

void CModelingandAnalysisofUncertaintyView::DisplayFactorAnalysisModel(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint TopLeft) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	int x0 = TopLeft.x, y0 = TopLeft.y, Delta_y = 20, Delta_x_1 = 60, Delta_x_2 = 70, pos, nVar, y_pos;
	double value, min, max;
	CString Text, Temp;
	CRect rectA(x0, y0, x0 + 250, y0 + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(&Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(x0 + 20, y0 + 10, _T("Factor analysis model"));
	y0 += 50;
	pDC->SetTextAlign(TA_CENTER);
	Text.Empty(), Text.Append(_T("Variable")), pDC->TextOutW(x0 + (int)(Delta_x_1 / 2), y0, Text);
	pDC->MoveTo(x0, y0 + 18), pDC->LineTo(x0 + Delta_x_1, y0 + 18);
	CArray <double> pMax, w;
	pMax.RemoveAll();
	for (int i = 0; i < pDoc->n; i++) {
		pDoc->GetColumn(pDoc->A, pDoc->A_spec, w, i);
		GetMaximum(w, pos, max), GetMinimum(w, pos, min);
		value = pDoc->DMAX(fabs(max), fabs(min));
		pMax.Add(value);
	}
	CBrush Retained_PCs, Red, Orange, Yellow, Yellow_bright;
	Retained_PCs.CreateSolidBrush(RGB(199, 248, 7));
	Red.CreateSolidBrush(RGB(250, 160, 113));
	Orange.CreateSolidBrush(RGB(250, 216, 46));
	Yellow.CreateSolidBrush(RGB(239, 250, 37));
	Yellow_bright.CreateSolidBrush(RGB(245, 249, 195));
	pDC->SetBkMode(TRANSPARENT);
	if (pDoc->OnlyX) nVar = pDoc->n_Var - 1;
	else nVar = pDoc->n_Var;
	for (int i = 0; i < pDoc->n; i++) {
		pDoc->GetColumn(pDoc->A, pDoc->A_spec, w, i);
		GetMaximum(w, pos, max), GetMinimum(w, pos, min);
		value = pDoc->DMAX(fabs(max), fabs(min));
		pMax.Add(value);
		Text.Empty(), Temp.Empty(), Temp.Format(L"%d", i + 1), Text.Append(L"Factor " + Temp);
		pDC->TextOutW(x0 + Delta_x_1 + (int)((0.5 + i) * Delta_x_2), y0, Text);
		pDC->MoveTo(x0 + Delta_x_1 + i * Delta_x_2, y0), pDC->LineTo(x0 + Delta_x_1 + i * Delta_x_2, y0 + Delta_y);
		pDC->SelectObject(&Standard);
		pDC->MoveTo(x0 + Delta_x_1 + i * Delta_x_2, y0 + 18), pDC->LineTo(x0 + Delta_x_1 + (1 + i) * Delta_x_2, y0 + 18);
	}
	pDC->SetTextAlign(TA_RIGHT | TA_TOP);
	for (int i = 0; i < nVar; i++) {
		y_pos = y0 + (i + 1) * Delta_y;
		pDC->TextOutW(x0 + 44, y_pos, pDoc->Tag.GetAt(i));
		pDC->MoveTo(x0 + Delta_x_1, y_pos), pDC->LineTo(x0 + Delta_x_1, y_pos + Delta_y);
		for (int j = 0; j < pDoc->n; j++) {
			value = pDoc->A.GetAt(pDoc->GetPosition(i, j, pDoc->A_spec));
			if ((fabs(value) >= 0.8 * pMax.GetAt(j)) && ((fabs(value) <= 1.0 * pMax.GetAt(j)))) {
				CRect rect(x0 + Delta_x_1 + j * Delta_x_2 + 15, y_pos + 2, x0 + Delta_x_1 + (j + 1) * Delta_x_2 - 15, y_pos + Delta_y - 2);
				pDC->Rectangle(rect), pDC->FillRect(&rect, &Red);
			}
			else if ((fabs(value) >= 0.6 * pMax.GetAt(j)) && ((fabs(value) < 0.8 * pMax.GetAt(j)))) {
				CRect rect(x0 + Delta_x_1 + j * Delta_x_2 + 15, y_pos + 2, x0 + Delta_x_1 + (j + 1) * Delta_x_2 - 15, y_pos + Delta_y - 2);
				pDC->Rectangle(rect), pDC->FillRect(&rect, &Orange);
			}
			else if ((fabs(value) >= 0.4 * pMax.GetAt(j)) && ((fabs(value) < 0.6 * pMax.GetAt(j)))) {
				CRect rect(x0 + Delta_x_1 + j * Delta_x_2 + 15, y_pos + 2, x0 + Delta_x_1 + (j + 1) * Delta_x_2 - 15, y_pos + Delta_y - 2);
				pDC->Rectangle(rect), pDC->FillRect(&rect, &Yellow);
			}
			else if ((fabs(value) >= 0.2 * pMax.GetAt(j)) && ((fabs(value) < 0.4 * pMax.GetAt(j)))) {
				CRect rect(x0 + Delta_x_1 + j * Delta_x_2 + 15, y_pos + 2, x0 + Delta_x_1 + (j + 1) * Delta_x_2 - 15, y_pos + Delta_y - 2);
				pDC->Rectangle(rect), pDC->FillRect(&rect, &Yellow_bright);
			}
			Text.Empty(), Text.Format(L"%.3f", value), pDC->TextOutW(x0 + Delta_x_1 + (j + 1) * Delta_x_2 - 15, y_pos, Text);
			pDC->MoveTo(x0 + Delta_x_1 + j * Delta_x_2, y_pos), pDC->LineTo(x0 + Delta_x_1 + j * Delta_x_2, y_pos + Delta_y);
			pDC->SelectObject(&Standard);
		}
	}
	pDC->SelectObject(&Standard);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
}

// ************************************************
// ***    Members  for  regression  analysis    ***
// ************************************************

void CModelingandAnalysisofUncertaintyView::DisplayStandardRegressionAnalysis(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC) {
	CString Text;
	CRect rectA(10, 10, 250, 48);
	pDC->Rectangle(rectA);
	CBrush brush;
	brush.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &brush);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(20, 20, _T("Information about data"));
	pDC->TextOutW(20, 55, _T("Data file")), pDC->TextOutW(250, 55, _T(":")), pDC->TextOutW(300,55,pDoc->PathAndFileName);
	pDC->TextOutW(20, 75, _T("Application")), pDC->TextOutW(250, 75, _T(":")), pDC->TextOutW(300, 75, pDoc->Application);
	Text.Empty(), Text.Format(L"M = %d", pDoc->n_Var - 1);
	pDC->TextOutW(20, 95, _T("Number of regressors")), pDC->TextOutW(250, 95, _T(":")), pDC->TextOutW(300, 95, Text);
	Text.Empty();
	Text.Format(L"N = %d", pDoc->n_Obs);
	pDC->TextOutW(20, 115, _T("Number of observations")), pDC->TextOutW(250, 115, _T(":")), pDC->TextOutW(300, 115, Text);
	pDC->TextOutW(20, 135, _T("Regressand variable")), pDC->TextOutW(250, 135, _T(":")), 
		pDC->TextOutW(300, 135, pDoc->Name.GetAt(static_cast <int64_t>(pDoc->n_Var - 1)));
	pDC->TextOutW(40, 160, _T("Regressor")), pDC->TextOutW(180, 160, _T("Tag")), pDC->TextOutW(300, 160, _T("Name"));
	pDC->MoveTo(20, 180), pDC->LineTo(450, 180);
	pDC->MoveTo(140, 155), pDC->LineTo(140, 190);
	pDC->MoveTo(250, 155), pDC->LineTo(250, 190);
	int pos_Y = 190;
	for (int i = 0; i < pDoc->n_Var - 1; i++) {
		Text.Empty(), Text.Format(L"%d", i + 1);
		if (i < 9 ) pDC->TextOutW(72, pos_Y, Text);
		else pDC->TextOutW(65, pos_Y, Text);
		Text.Empty(), Text.Append(pDoc->Tag.GetAt(i));
		if (Text.GetLength() == 2) pDC->TextOutW(187, pos_Y, pDoc->Tag.GetAt(i));
		else if (Text.GetLength() == 3) pDC->TextOutW(180, pos_Y, pDoc->Tag.GetAt(i));
		pDC->TextOutW(300, pos_Y, pDoc->Name.GetAt(i));
		pDC->MoveTo(140, pos_Y), pDC->LineTo(140, pos_Y + 20);
		pDC->MoveTo(250, pos_Y), pDC->LineTo(250, pos_Y + 20);
		pos_Y += 20;
	}
	pos_Y += 10;
	CRect rectB(10, pos_Y, 250, pos_Y + 38);
	pDC->Rectangle(rectB);
	pDC->FillRect(&rectB, &brush);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(20,pos_Y+10,_T("Standard statistical analysis"));
	pos_Y += 50, pDC->TextOutW(20, pos_Y, _T("Metric")), pDC->TextOutW(250, pos_Y, _T(":")), pDC->TextOutW(300, pos_Y, _T("Value"));
	pDC->MoveTo(20, pos_Y + 20), pDC->LineTo(450, pos_Y + 20);
	pos_Y += 30, pDC->TextOutW(20, pos_Y, _T("Mean squared error")), pDC->TextOutW(250, pos_Y, _T(":"));
	Text.Empty(), Text.Format(L"%.4f", pDoc->Metrics_Raw.GetAt(0)), pDC->TextOutW(300, pos_Y, Text);
	pos_Y += 20, pDC->TextOutW(20, pos_Y, _T("Root mean squared error")), pDC->TextOutW(250, pos_Y, _T(":"));
	Text.Empty(), Text.Format(L"%.4f", pDoc->Metrics_Raw.GetAt(1)), pDC->TextOutW(300, pos_Y, Text);
	pos_Y += 20, pDC->TextOutW(20, pos_Y, _T("Mean absolute error")), pDC->TextOutW(250, pos_Y, _T(":"));
	Text.Empty(), Text.Format(L"%.4f", pDoc->Metrics_Raw.GetAt(2)), pDC->TextOutW(300, pos_Y, Text);
	pos_Y += 20, pDC->TextOutW(20, pos_Y, _T("Corr( y , yest )")), pDC->TextOutW(250, pos_Y, _T(":"));
	Text.Empty(), Text.Format(L"%.4f", pDoc->Metrics_Raw.GetAt(3)), pDC->TextOutW(300, pos_Y, Text);
	pos_Y += 20, pDC->TextOutW(20, pos_Y, _T("R squared")), pDC->TextOutW(250, pos_Y, _T(":"));
	Text.Empty(), Text.Format(L"%.4f", pDoc->Metrics_Raw.GetAt(4)), pDC->TextOutW(300, pos_Y, Text);
	pos_Y += 20, pDC->TextOutW(20, pos_Y, _T("Adjusted R squared")), pDC->TextOutW(250, pos_Y, _T(":"));
	Text.Empty(), Text.Format(L"%.4f", pDoc->Metrics_Raw.GetAt(5)), pDC->TextOutW(300, pos_Y, Text);
	pos_Y += 40;
	CRect rectC(10, pos_Y, 250, pos_Y + 38);
	pDC->Rectangle(rectC);
	pDC->FillRect(&rectC, &brush);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(20, pos_Y + 10, _T("Analysis of variance"));
	pos_Y += 50, pDC->TextOutW(20, pos_Y, _T("# Degrees of freedom")), pDC->TextOutW(250,pos_Y,_T(":")),
		pDC->TextOutW(300, pos_Y, _T("Sum of squares"));
	pDC->MoveTo(20, pos_Y + 20), pDC->LineTo(450, pos_Y + 20);
	pos_Y += 30, pDC->TextOutW(20, pos_Y, _T("N - 1")), pDC->TextOutW(75, pos_Y, _T("="));
	Text.Empty(), Text.Format(L"%d - 1", pDoc->n_Obs), pDC->TextOutW(90, pos_Y, Text), pDC->TextOutW(180,pos_Y,_T("="));
	Text.Empty(), Text.Format(L"%d", pDoc->n_Obs - 1), pDC->TextOutW(195, pos_Y, Text), pDC->TextOutW(250,pos_Y,_T(":"));
	int number = 0;
	pDC->TextOutW(300, pos_Y, _T("SSy ="));
	FormatValue(Text, pDoc->Metrics_Raw.GetAt(6), 0), pDC->TextOutW(339, pos_Y, Text);
	pos_Y += 20, pDC->TextOutW(20, pos_Y, _T("M")), pDC->TextOutW(180, pos_Y, _T("="));
	Text.Empty(), Text.Format(L"%d", pDoc->n_Var - 1), pDC->TextOutW(195, pos_Y, Text),pDC->TextOutW(250, pos_Y, _T(":"));
	pDC->TextOutW(300, pos_Y, _T("SSr ="));
	FormatValue(Text, pDoc->Metrics_Raw.GetAt(7), 0);
	pDC->TextOutW(339, pos_Y, Text);
	pos_Y += 20, pDC->TextOutW(20, pos_Y, _T("N - M - 1")), pDC->TextOutW(75, pos_Y, _T("="));
	Text.Empty(), Text.Format(L"%d - %d - 1", pDoc->n_Obs, pDoc->n_Var - 1), pDC->TextOutW(90, pos_Y, Text);
	pDC->TextOutW(180, pos_Y, _T("="));
	Text.Empty(), Text.Format(L"%d", pDoc->n_Obs - pDoc->n_Var), pDC->TextOutW(195, pos_Y, Text), pDC->TextOutW(250,pos_Y,_T(":"));
	pDC->TextOutW(300, pos_Y, _T("SSe ="));
	FormatValue(Text, pDoc->Metrics_Raw.GetAt(8), 0);
	pDC->TextOutW(339, pos_Y, Text);
}

void CModelingandAnalysisofUncertaintyView::ShowHypothesisTextingResults(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint& TopLeft, CPoint& BottomRight) {
	//pDC->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 75, pos;
	int x0 = TopLeft.x, y0 = TopLeft.y + 50, X1, Y1, X2, Y2, X_center;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 240, TopLeft.y + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(TopLeft.x+10, TopLeft.y+10, _T("Hypothesis testing"));
	double t_min, t_max, range, value, x_relative, y_relative, x1, y1, x2, y2, 
		dX = 0.925 / (double)pDoc->t_values.GetSize(), x_center, t_upper, t_lower;
	CPoint point;
	CArray <double> t;
	t.RemoveAll();
	GetMinimum(pDoc->t_values, pos, t_min), GetMaximum(pDoc->t_values, pos, t_max);
	if (t_max > (double)8) t_max = (double)8;
	if (t_min < (double)(-8)) t_min = (double)(-8);
	if (t_min > (double)0) t_min = (double)0;
	range = t_max - t_min;
	for (int i = 0; i < pDoc->t_values.GetSize(); i++) {
		value = pDoc->t_values.GetAt(i);
		value -= t_min;
		value /= range;
		if (value > (double)1) value = (double)1;
		if (value < (double)0) value = (double)0;
		t.Add(value);
	}
	t_lower = (-pDoc->t_alpha - t_min) / range;
	t_upper = (pDoc->t_alpha - t_min) / range;
	x_center = dX / (double)2;
	CBrush Brush_Bars;
	CRect Rect_Bar;
	y1 = (double)1 - fabs(t_min) / range, Y1 = y0 + (int)(y1 * Height);
	Brush_Bars.CreateSolidBrush(RGB(243, 213, 175));
	for (int i = 0; i < pDoc->t_values.GetSize(); i++) {
		x1 = 0.075 + x_center - 0.4 * dX, x2 = 0.075 + x_center + 0.4 * dX, y2 = 1 - t.GetAt(i);
		X1 = x0 + (int)(x1 * Length), X2 = x0 + (int)(x2 * Length), Y2 = y0 + (int)(y2 * Height);
		Rect_Bar.SetRect(X1, Y1, X2, Y2);
		pDC->Rectangle(Rect_Bar);
		pDC->FillRect(&Rect_Bar, &Brush_Bars);
		X_center = x0 + (int)((0.075 + x_center) * Length);
		pos = pDoc->SelectedPredictors.GetAt(i);
		if (pDoc->w_raw.GetAt(i) >= (double)0) {
			pDC->SetTextAlign(TA_TOP | TA_CENTER);
			pDC->TextOutW(X_center, Y1, pDoc->Tag.GetAt(pos));
		}
		else {
			pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
			pDC->TextOutW(X_center, Y1, pDoc->Tag.GetAt(pos));
		}
		x_center += dX;
	}
	y_relative = (double)1 - fabs(t_min) / range, x_relative = 0.075;
	y1 = y_relative, point.x = x0 + (int)(x_relative * Length), point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(point);
	x_relative = (double)1, point.x = x0 + (int)(x_relative * Length), pDC->LineTo(point);
	x_relative = 0.075, y_relative = (double)0;
	point.x = x0 + (int)(x_relative * Length), point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(point);
	y_relative = (double)1, point.y = y0 + (int)(y_relative * Height), pDC->LineTo(point);
	x1 = 0.075, y1 = 1 - t_lower, x2 = 1.0, y2 = y1;
	X1 = x0 + (int)(x1 * Length), X2 = x0 + (int)(x2 * Length), Y1 = y0 + (int)(y1 * Height), Y2 = Y1;
	CPen CriticalRegion;
	CriticalRegion.CreatePen(PS_DASH, 1, RGB(178, 34, 52));
	pDC->SelectObject(&CriticalRegion), pDC->MoveTo(X1, Y1), pDC->LineTo(X2, Y2);
	y1 = 1 - t_upper, Y1 = y0 + (int)(y1 * Height), Y2 = Y1;
	pDC->SelectObject(&CriticalRegion), pDC->MoveTo(X1, Y1), pDC->LineTo(X2, Y2);
	CArray <double> Ticks, TickLabels;
	GetTicks(t_min, t_max, Ticks, TickLabels);
	CPen YTicks;
	YTicks.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&YTicks);
	CString Text;
	pDC->SetTextAlign(TA_RIGHT);
	int digit_old = 0, digit_new;
	for (int i = 0; i < TickLabels.GetSize(); i++) {
		value = (double)TickLabels.GetAt(i);
		y_relative = 1 -  (value - t_min) / range, x_relative = 0.075;
		point.x = x0 + (int)(0.075 * Length), point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(point);
		x_relative = 0.085, point.x = x0 + (int)(x_relative * Length), pDC->LineTo(point);
		digit_new = FormatTickLabel(Text,TickLabels.GetAt(i), digit_old);
		if (digit_new > digit_old) digit_old = digit_new;
		pDC->TextOutW(x0 + (int)(0.07 * Length), point.y - 7, Text);
	}
	CFont XLabel, YLabel;
	YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	Text.Empty(), Text.Format(L"t-value (t_crit = %.4f)", pDoc->t_alpha);
	pDC->TextOutW(x0, y0 + (int)(0.5*Height), Text);
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	Text.Empty(), Text.Append(L"Regressor");
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height, Text);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_CENTER);
}

void CModelingandAnalysisofUncertaintyView::ShowRegressionCoefficients(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint& TopLeft, CPoint& BottomRight, int Show) {
	//pDC->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 75, pos;
	int x0 = TopLeft.x, y0 = TopLeft.y + 50, X1, Y1, X2, Y2, X_center;
	double w_min, w_max, value, range, x_relative, y_relative, x1, y1, x2, y2,
		dX = 0.925 / (double)pDoc->w_raw.GetSize(), x_center;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 240, TopLeft.y + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Regression coefficients"));
	if (Show) {
		for (int i = 0; i < pDoc->w_raw.GetSize(); i++) {
			if (i == 0) w_min = pDoc->ConfIntervals.GetAt(static_cast <int64_t>(2 * i)), w_max = pDoc->ConfIntervals.GetAt(static_cast <int64_t>(2 * i + 1));
			else {
				if (pDoc->ConfIntervals.GetAt(static_cast <int64_t>(2 * i)) < w_min) w_min = pDoc->ConfIntervals.GetAt(static_cast <int64_t>(2 * i));
				if (pDoc->ConfIntervals.GetAt(static_cast <int64_t>(2 * i + 1)) > w_max) w_max = pDoc->ConfIntervals.GetAt(static_cast <int64_t>(2 * i + 1));
			}
		}
		if (w_min > 0) w_min = (double)0;
		if (w_max < 0) w_max = (double)0;
		range = w_max - w_min;
	}
	else {
		w_min = pDoc->w.GetAt(0), w_max = pDoc->w_raw.GetAt(0);
		for (int i = 1; i < pDoc->w_raw.GetSize(); i++) {
			if (pDoc->w_raw.GetAt(i) < w_min) w_min = pDoc->w_raw.GetAt(i);
			if (pDoc->w_raw.GetAt(i) > w_max) w_max = pDoc->w_raw.GetAt(i);
		}
		if (w_min > 0) w_min = (double)0;
		if (w_max < 0) w_max = (double)0;
		range = w_max - w_min;
	}
	y1 = 1 + w_min / range;
	x_center = dX / (double)2;
	CBrush Brush_Bars;
	CRect Rect_Bar;
	Y1 = y0 + (int)(y1 * Height);
	Brush_Bars.CreateSolidBrush(RGB(243, 213, 175));
	CString Text;
	for (int i = 0; i < pDoc->w_raw.GetSize(); i++) {
		x1 = 0.075 + x_center - 0.4 * dX, x2 = 0.075 + x_center + 0.4 * dX;
		value = pDoc->w_raw.GetAt(i);
		value -= w_min;
		value /= range;
		y2 = 1 - value;
		X1 = x0 + (int)(x1 * Length), X2 = x0 + (int)(x2 * Length), Y2 = y0 + (int)(y2 * Height);
		Rect_Bar.SetRect(X1, Y1, X2, Y2);
		pDC->Rectangle(Rect_Bar);
		pDC->FillRect(&Rect_Bar, &Brush_Bars);
		X_center = x0 + (int)((0.075 + x_center) * Length);
		pos = pDoc->SelectedPredictors.GetAt(i);
		if (pDoc->w_raw.GetAt(i) >= (double)0) {
			pDC->SetTextAlign(TA_TOP | TA_CENTER);
			pDC->TextOutW(X_center, Y1, pDoc->Tag.GetAt(pos));
		}
		else {
			pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
			pDC->TextOutW(X_center, Y1, pDoc->Tag.GetAt(pos));
		}
		x_center += dX;
	}
	x_relative = 0.075, Point.x = x0 + (int)(x_relative * Length), Point.y = Y1;
	pDC->MoveTo(Point), x_relative = 1.0, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
	x_relative = 0.075, y_relative = (double)0;
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height);
	pDC->MoveTo(Point), y_relative = (double)1, Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
	CArray <double> Ticks, TickLabels;
	GetTicks(w_min, w_max, Ticks, TickLabels);
	pDC->SetTextAlign(TA_RIGHT);
	int digit_old = 0, digit_new;
	for (int i = 0; i < TickLabels.GetSize(); i++) {
		value = TickLabels.GetAt(i), value -= w_min;
		value /= range, y_relative = 1 - value, x_relative = 0.075;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height);
		pDC->MoveTo(Point), x_relative = 0.085, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text,TickLabels.GetAt(i), digit_old);
		if (digit_new > digit_old) digit_old = digit_new;
		pDC->TextOutW(x0 + (int)(0.07 * Length), Point.y - 7, Text);
	}
	CFont YLabel, XLabel;
	YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel);
	Text.Empty(), Text.Append(L"Regression coefficient");
	pDC->SetTextAlign(TA_CENTER|TA_TOP);
	pDC->TextOutW(x0, y0 + (int)(0.5 * Height), Text);
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	Text.Empty(), Text.Append(L"Regressor");
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height, Text);
	if (Show) {
		CPen pen(PS_SOLID, 4, RGB(243, 102, 25));
		pDC->SelectObject(&pen);
		x_center = dX / (double)2;
		double lower, upper;
		for (int i = 0; i < pDoc->w_raw.GetSize(); i++) {
			lower = pDoc->ConfIntervals.GetAt(static_cast <int64_t>(2 * i));
			lower -= w_min, lower /= range, y_relative = 1 - lower;
			Point.x = x0 + (int)((0.075 + x_center - 0.1 * dX) * Length), Point.y = y0 + (int)(y_relative * Height),pDC->MoveTo(Point);
			Point.x = x0 + (int)((0.075 + x_center + 0.1 * dX) * Length), pDC->LineTo(Point);
			upper = pDoc->ConfIntervals.GetAt(static_cast <int64_t>(2 * i + 1));
			upper -= w_min, upper /= range, y_relative = 1 - upper;
			Point.x = x0 + (int)((0.075 + x_center - 0.1 * dX) * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
			Point.x = x0 + (int)((0.075 + x_center + 0.1 * dX) * Length), pDC->LineTo(Point);
			Point.x = x0 + (int)((0.075 + x_center) * Length), pDC->MoveTo(Point);
			y_relative = 1 - lower, Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
			x_center += dX;
		}
	}
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_CENTER);
}

void CModelingandAnalysisofUncertaintyView::ShowModelPrediction(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CPoint& TopLeft, CPoint& BottomRight) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	//pDC->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 90;
	int x0 = TopLeft.x, y0 = TopLeft.y + 50;
	double y_min, y_max, value, range, x_relative, y_relative, dX;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 240, TopLeft.y + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Model prediction"));
	pDC->SetTextColor(RGB(60, 59, 110));
	pDC->TextOutW(TopLeft.x + 275, TopLeft.y + 10, _T("Measured value"));
	pDC->SetTextColor(RGB(204, 161, 61));
	pDC->TextOutW(TopLeft.x + 450, TopLeft.y + 10, _T("Predicted value"));
	pDC->SetTextColor(RGB(0, 0, 0));
	int pos = static_cast <int64_t>(pDoc->GetPosition(0, pDoc->n_Var - 1, pDoc->Data_spec));
	y_min = pDoc->DMIN(pDoc->Data.GetAt(pos),pDoc->ypred.GetAt(0));
	y_max = pDoc->DMAX(pDoc->Data.GetAt(pos), pDoc->ypred.GetAt(0));
	CArray <double> xTicks, xTickLabels, yTicks, yTickLabels;
	for (int i = 1; i < pDoc->n_Obs; i++) {
		pos = static_cast <int64_t>(pDoc->GetPosition(i, pDoc->n_Var - 1, pDoc->Data_spec));
		value = pDoc->DMAX(pDoc->Data.GetAt(pos), pDoc->ypred.GetAt(i));
		if (value > y_max) y_max = value;
		value = pDoc->DMIN(pDoc->Data.GetAt(pos), pDoc->ypred.GetAt(i));
		if (value < y_min) y_min = value;
	}
	range = y_max - y_min;
	x_relative = 0.075;
	y_relative = (double)1;
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
	x_relative = 0.075, y_relative = 0;
	Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
	y_relative = 1.0, Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
	dX = 0.925 / (double)pDoc->ypred.GetSize();
	CPen Predicted;
	Predicted.CreatePen(PS_SOLID, 1, RGB(204, 161, 61));
	pDC->SelectObject(&Predicted);
	for (int i = 0; i < pDoc->ypred.GetSize(); i++) {
		x_relative = 0.075 + (double)(i + 1) * dX;
		value = pDoc->ypred.GetAt(i);
		value -= y_min;
		value /= range;
		y_relative = 1 - value;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
	}
	pDC->Ellipse(TopLeft.x + 433, TopLeft.y + 15, TopLeft.x + 441, TopLeft.y + 23);
	CPen Measured;
	Measured.CreatePen(PS_SOLID, 3, RGB(60, 59, 110));
	pDC->SelectObject(&Measured);
	for (int i = 0; i < pDoc->ypred.GetSize(); i++) {
		x_relative = 0.075 + (double)(i + 1) * dX;
		pos = static_cast <int64_t>(pDoc->GetPosition(i, pDoc->n_Var - 1, pDoc->Data_spec));
		value = pDoc->Data.GetAt(pos);
		value -= y_min;
		value /= range;
		y_relative = (double)1 - value;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->Ellipse(Point.x - 2, Point.y-2, Point.x+2, Point.y+2);
	}
	pDC->Ellipse(TopLeft.x + 260, TopLeft.y + 17, TopLeft.x + 264, TopLeft.y + 21);
	GetTicks((double)1, (double)pDoc->n_Obs, xTicks, xTickLabels);
	GetTicks(y_min, y_max, yTicks, yTickLabels);
	CPen Normal;
	Normal.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Normal);
	pDC->SetTextAlign(TA_TOP | TA_CENTER);
	CString Text;
	int digit_old = 0, digit_new;
	for (int i = 0; i < xTicks.GetSize(); i++) {
		x_relative = 0.075 + 0.925 * xTicks.GetAt(i);
		y_relative = 0.97;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
		y_relative = 1;
		Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, xTickLabels.GetAt(i), digit_old);
		if (digit_new > digit_old) digit_old = digit_new;
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < yTicks.GetSize(); i++) {
		x_relative = 0.085, y_relative = 1 - yTicks.GetAt(i);
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
		x_relative = 0.075, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, yTickLabels.GetAt(i), digit_old);
		if (digit_new > digit_old) digit_old = digit_new;
		x_relative = 0.07, Point.x = x0 + (int)(x_relative * Length), pDC->TextOutW(Point.x, Point.y - 7, Text);
	}
	CFont XLabel, YLabel;
	YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel);
	Text.Empty(), Text.Append(L"Prediction");
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0, y0 + (int)(0.5 * Height), Text);
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	Text.Empty(), Text.Append(L"Observation");
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0 + (int)(0.5*Length), y0 + Height + 20, Text);
	pDC->SelectObject(&Standard);
	pDC->SelectObject(&Standard_Font);
	pDC->SetTextAlign(TA_CENTER);
}

void CModelingandAnalysisofUncertaintyView::ShowPredictionError(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& e, CPoint& TopLeft, CPoint& BottomRight){
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	//pDC->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 90, pos;
	int x0 = TopLeft.x, y0 = TopLeft.y + 50;
	double y_min = (double)0, y_max = (double)0, value, range, x_relative, y_relative, dX;
	CPoint Point;
	CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 240, TopLeft.y + 38);
	pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Model residuals"));
	pDC->SetTextColor(RGB(178, 34, 52));
	pDC->TextOutW(TopLeft.x + 275, TopLeft.y + 10, _T("Error value"));
	pDC->SetTextColor(RGB(0, 0, 0));
	e.RemoveAll(), e.SetSize(pDoc->n_Obs);
	CString Text;
	for (int i = 0; i < pDoc->n_Obs; i++) {
		pos = static_cast <int64_t>(pDoc->GetPosition(i, pDoc->n_Var - 1, pDoc->Data_spec));
		value = pDoc->Data.GetAt(pos);
		value -= pDoc->ypred.GetAt(i);
		e.SetAt(i, value);
		if (i == 0) y_min = value, y_max = value;
		else {
			if (value > y_max) y_max = value;
			if (value < y_min) y_min = value;
		}
	}
	range = y_max - y_min;
	x_relative = 0.075, y_relative = 1.0, Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
	x_relative = 0.075, y_relative = 0.0, Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
	y_relative = 1.0, Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
	dX = 0.925 / (double)pDoc->n_Obs;
	x_relative = 0.075;
	CPen Error;
	Error.CreatePen(PS_SOLID, 1, RGB(178, 34, 52));
	pDC->SelectObject(&Error);
	for (int i = 0; i < pDoc->n_Obs; i++) {
		value = e.GetAt(i);
		value -= y_min;
		value /= range;
		y_relative = 1 - value;
		x_relative += dX;
		Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height);
		pDC->Ellipse(Point.x - 2, Point.y - 2, Point.x + 2, Point.y + 2);
	}
	pDC->Ellipse(TopLeft.x + 260, TopLeft.y + 17, TopLeft.x + 264, TopLeft.y + 21);
	CArray <double> XTicks, YTicks, XTickLabels, YTickLabels;
	GetTicks((double)1, (double)pDoc->n_Obs, XTicks, XTickLabels);
	GetTicks(y_min, y_max, YTicks, YTickLabels);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	int digit_old = 0, digit_new;
	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = XTicks.GetAt(i), y_relative = 0.97, Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
		y_relative = 1.0, Point.y = y0 + (int)(y_relative * Height), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, XTickLabels.GetAt(i), digit_old);
		if (digit_new > digit_old) digit_old = digit_new;
		pDC->TextOutW(Point.x, Point.y, Text);
	}
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		x_relative = 0.085, y_relative = 1 - YTicks.GetAt(i), Point.x = x0 + (int)(x_relative * Length), Point.y = y0 + (int)(y_relative * Height), pDC->MoveTo(Point);
		x_relative = 0.075, Point.x = x0 + (int)(x_relative * Length), pDC->LineTo(Point);
		digit_new = FormatTickLabel(Text, YTickLabels.GetAt(i), digit_old);
		if (digit_new > digit_old) digit_old = digit_new;
		x_relative = 0.07, Point.x = x0 + (int)(x_relative * Length);
		pDC->TextOutW(Point.x, Point.y - 7, Text);
	}
	CFont XLabel, YLabel;
	YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel);
	Text.Empty(), Text.Append(L"Prediction error");
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0, y0 + (int)(0.5 * Height), Text);
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	Text.Empty(), Text.Append(L"Observation");
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, Text);
	pDC->SelectObject(&Standard);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_CENTER);
}

// CModelingandAnalysisofUncertaintyView printing

void CModelingandAnalysisofUncertaintyView::OnFilePrintPreview(){
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CModelingandAnalysisofUncertaintyView::OnPreparePrinting(CPrintInfo* pInfo){
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CModelingandAnalysisofUncertaintyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo){
	// TODO: add extra initialization before printing
}

void CModelingandAnalysisofUncertaintyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo){
	// TODO: add cleanup after printing
}

void CModelingandAnalysisofUncertaintyView::OnRButtonDown(UINT nFlags, CPoint point){
}

void CModelingandAnalysisofUncertaintyView::OnLButtonDown(UINT nFlags, CPoint point) {
}

void CModelingandAnalysisofUncertaintyView::OnContextMenu(CWnd* pWnd, CPoint point){
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CModelingandAnalysisofUncertaintyView diagnostics

#ifdef _DEBUG
void CModelingandAnalysisofUncertaintyView::AssertValid() const{
	CView::AssertValid();
}

void CModelingandAnalysisofUncertaintyView::Dump(CDumpContext& dc) const{
	CView::Dump(dc);
}

CModelingandAnalysisofUncertaintyDoc* CModelingandAnalysisofUncertaintyView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModelingandAnalysisofUncertaintyDoc)));
	return (CModelingandAnalysisofUncertaintyDoc*)m_pDocument;
}
#endif //_DEBUG

// CModelingandAnalysisofUncertaintyView message handlers

int CModelingandAnalysisofUncertaintyView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1) return -1;
	CWnd* pwndParent = this->GetParent();
	CRect rc;
	pwndParent->GetWindowRect(&rc);
	int nHt = rc.Height(), nWt = (int)(0.989 * rc.Width()), Offset = (int)(0.135 * rc.Height());
	int x = (int)(2 * nWt / 3), y = (int)(2 * (nHt - Offset) / 3);
	Next_Variable.Create(_T("+"), BS_PUSHBUTTON, CRect(325, 20, 363, 58), this, IDC_NEXT_VARIABLE);
	Next_Variable.ShowWindow(SW_HIDE);
	Previous_Variable.Create(_T("-"), BS_PUSHBUTTON, CRect(275, 20, 313, 58), this, IDC_PREVIOUS_VARIABLE);
	Previous_Variable.ShowWindow(SW_HIDE);
	Settings_Descriptive_Statistics.Create(_T("Settings"), BS_PUSHBUTTON, CRect(375, 20, 500, 58), this, IDC_SETTINGS_DESCRIPTIVE_STATISTICS);
	Settings_Descriptive_Statistics.ShowWindow(SW_HIDE);
	// Buttons for principal component analysis

	int btnWidth = 150;
	int btnHeight = 38;
	int spacing = 20;
	int topMargin = 20;
	int rightMargin = 20;

	int firstButtonRight = nWt - rightMargin;
	DisplayPCA.Create(_T("Model overview"), BS_PUSHBUTTON, CRect(firstButtonRight - btnWidth, topMargin, firstButtonRight, topMargin + btnHeight), this, IDC_DISPLAY_PCA);
	DisplayPCA.ShowWindow(SW_HIDE);

	int secondButtonRight = firstButtonRight - btnWidth - spacing;
	DisplayLoadingPlots.Create(_T("Loading plots"), BS_PUSHBUTTON, CRect(secondButtonRight - btnWidth, topMargin, secondButtonRight, topMargin + btnHeight), this, IDC_LOADING_PLOTS);
	DisplayLoadingPlots.ShowWindow(SW_HIDE);

	int thirdButtonRight = secondButtonRight - btnWidth - spacing;
	DisplayScorePlots.Create(_T("Score plots"), BS_PUSHBUTTON, CRect(thirdButtonRight - btnWidth, topMargin, thirdButtonRight, topMargin + btnHeight), this, IDC_SCORE_PLOTS);
	DisplayScorePlots.ShowWindow(SW_HIDE);

	int fourthButtonRight = thirdButtonRight - btnWidth - spacing;
	SelectScatterPlots.Create(_T("Select plots"), BS_PUSHBUTTON, CRect(fourthButtonRight - btnWidth, topMargin, fourthButtonRight, topMargin + btnHeight), this, IDC_SELECT_SCATTER_PLOTS);
	SelectScatterPlots.ShowWindow(SW_HIDE);

	/* previous one, 
	DisplayPCA.Create(_T("Model overview"), BS_PUSHBUTTON, CRect((int)(nWt / 2 - 300), 20, (int)(nWt / 2 - 150), 58), this, IDC_DISPLAY_PCA);
	DisplayPCA.ShowWindow(SW_HIDE);
	DisplayLoadingPlots.Create(_T("Loading plots"), BS_PUSHBUTTON, CRect((int)(nWt / 2 - 75), 20, (int)(nWt / 2 + 75), 58), this, IDC_LOADING_PLOTS);
	DisplayLoadingPlots.ShowWindow(SW_HIDE);
	DisplayScorePlots.Create(_T("Score plots"), BS_PUSHBUTTON, CRect((int)(nWt / 2 + 150), 20, (int)(nWt / 2 + 300), 58), this, IDC_SCORE_PLOTS);
	DisplayScorePlots.ShowWindow(SW_HIDE);
	SelectScatterPlots.Create(_T("Select plots"), BS_PUSHBUTTON, CRect((int)(nWt / 2 + 375), 20, (int)(nWt / 2 + 525), 58), this, IDC_SELECT_SCATTER_PLOTS);
	SelectScatterPlots.ShowWindow(SW_HIDE);*/


	// Buttons for factor analysis
	DisplayFA.Create(_T("Model overview"), BS_PUSHBUTTON, CRect((int)(nWt / 2 - 300), 20, (int)(nWt / 2 -150), 58), this, IDC_DISPLAY_FA);
	DisplayFA.ShowWindow(SW_HIDE);
	DisplayFactorLoadings.Create(_T("Loading plots"), BS_PUSHBUTTON, CRect((int)(nWt / 2 - 75), 20, (int)(nWt / 2 + 75), 58), this, IDC_FACTOR_LOADINGS);
	DisplayFactorLoadings.ShowWindow(SW_HIDE);
	DisplayFactorScores.Create(_T("Score plots"), BS_PUSHBUTTON, CRect((int)(nWt / 2 + 150), 20, (int)(nWt / 2 + 300), 58), this, IDC_FACTOR_SCORES);
	DisplayFactorScores.ShowWindow(SW_HIDE);
	DisplayFactorMatrices.Create(_T("Matrices"), BS_PUSHBUTTON, CRect((int)(nWt / 2 + 375), 20, (int)(nWt / 2 + 525), 58), this, IDC_FACTOR_MATRICES);
	DisplayFactorMatrices.ShowWindow(SW_HIDE);
	// Buttons for regression analysis
	Ind_Ass_Regression.Create(_T("Press"), BS_PUSHBUTTON, CRect(500, 10, 600, 48), this, IDC_INDEPENDENT_ASSESSMENT_REGRESSION);
	Ind_Ass_Regression.ShowWindow(SW_HIDE);
	Sta_Ass_Regression.Create(_T("Press"), BS_PUSHBUTTON, CRect(500, 10, 600, 48), this, IDC_STANDARD_ASSESSMENT_REGRESSION);
	Sta_Ass_Regression.ShowWindow(SW_HIDE);

	return 0;
}

void CModelingandAnalysisofUncertaintyView::On_Display_PCA() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->ShowStatisticalAnalysis = false;
	pDoc->OnOpenedFile = false;
	pDoc->DescriptiveStatistics = false;
	pDoc->HypothesisTesting_OneSample = false;
	pDoc->HypothesisTesting_TwoSample = false;
	pDoc->ShapiroWilkTest = false;
	pDoc->AndersonDarlingTest = false;
	pDoc->ANOVA = false;
	pDoc->PCA_Select_PCs = false;
	pDoc->PCA_Display_PCs_Standard = true;
	pDoc->PCA_Display_Loadings = false;
	pDoc->PCA_Display_Scores = false;
	pDoc->FA_Display_Standard = false;
	pDoc->FDA = false;
	pDoc->RegressionAnalysis = false;
	pDoc->RegressionAnalysis_Independent = false;
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Display_Loadings() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->ShowStatisticalAnalysis = false;
	pDoc->OnOpenedFile = false;
	pDoc->DescriptiveStatistics = false;
	pDoc->HypothesisTesting_OneSample = false;
	pDoc->HypothesisTesting_TwoSample = false;
	pDoc->ShapiroWilkTest = false;
	pDoc->AndersonDarlingTest = false;
	pDoc->ANOVA = false;
	pDoc->PCA_Select_PCs = false;
	pDoc->PCA_Display_PCs_Standard = false;
	pDoc->PCA_Display_Loadings = true;
	pDoc->PCA_Display_Scores = false;
	pDoc->FA_Display_Standard = false;
	pDoc->FDA = false;
	pDoc->RegressionAnalysis = false;
	pDoc->RegressionAnalysis_Independent = false;
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Display_Scores() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->ShowStatisticalAnalysis = false;
	pDoc->OnOpenedFile = false;
	pDoc->DescriptiveStatistics = false;
	pDoc->HypothesisTesting_OneSample = false;
	pDoc->HypothesisTesting_TwoSample = false;
	pDoc->ShapiroWilkTest = false;
	pDoc->AndersonDarlingTest = false;
	pDoc->ANOVA = false;
	pDoc->PCA_Select_PCs = false;
	pDoc->PCA_Display_PCs_Standard = false;
	pDoc->PCA_Display_Loadings = false;
	pDoc->PCA_Display_Scores = true;
	pDoc->FA_Display_Standard = false;
	pDoc->FDA = false;
	pDoc->RegressionAnalysis = false;
	pDoc->RegressionAnalysis_Independent = false;
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Next_Variable() {
	Distribution = 0;
	counter++;
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Previous_Variable() {
	Distribution = 0;
	counter--;
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Independent_Assessment_Regression() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	pDoc->RegressionAnalysis = false;
	pDoc->RegressionAnalysis_Independent = true;
	Ind_Ass_Regression.ShowWindow(SW_HIDE);
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Standard_Assessment_Regression() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	pDoc->RegressionAnalysis = true;
	pDoc->RegressionAnalysis_Independent = false;
	Sta_Ass_Regression.ShowWindow(SW_HIDE);
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Settings_Descriptive_Statistics() {
	CSettingsDescriptiveStatistics Settings;
	Settings.n_Bins = bins;
	Settings.Selected_Distribution = Distribution;
	if (Settings.DoModal() == IDOK) {
		bins = Settings.n_Bins;
		Distribution = Settings.Selected_Distribution;
	}
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Select_Scatter_Plots() {
	CWnd* pParent = nullptr;
	CSelectScatterDiagrams SelectNumberPlots(pParent,n,Type);
	Flag_Select = false;
	int number;
	if (SelectNumberPlots.DoModal() == IDOK) {
		n_Grid = SelectNumberPlots.NumberVariables + 1;
		SelectedVariables.RemoveAll();
		for (int i = 0; i < n_Grid + 1; i++) {
			number = SelectNumberPlots.SelectedScoresLoadings.GetAt(i);
			SelectedVariables.Add(number);
		}
	}
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Display_FA() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->ShowStatisticalAnalysis = false;
	pDoc->OnOpenedFile = false;
	pDoc->DescriptiveStatistics = false;
	pDoc->HypothesisTesting_OneSample = false;
	pDoc->HypothesisTesting_TwoSample = false;
	pDoc->ShapiroWilkTest = false;
	pDoc->AndersonDarlingTest = false;
	pDoc->ANOVA = false;
	pDoc->PCA_Select_PCs = false;
	pDoc->PCA_Display_PCs_Standard = false;
	pDoc->PCA_Display_Loadings = false;
	pDoc->PCA_Display_Scores = false;
	pDoc->FA_Display_Standard = true;
	pDoc->FA_Display_Loadings = false;
	pDoc->FA_Display_Scores = false;
	pDoc->FDA = false;
	pDoc->RegressionAnalysis = false;
	pDoc->RegressionAnalysis_Independent = false;
	Invalidate();
	UpdateWindow();

}

void CModelingandAnalysisofUncertaintyView::On_Display_Factor_Loadings() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->ShowStatisticalAnalysis = false;
	pDoc->OnOpenedFile = false;
	pDoc->DescriptiveStatistics = false;
	pDoc->HypothesisTesting_OneSample = false;
	pDoc->HypothesisTesting_TwoSample = false;
	pDoc->ShapiroWilkTest = false;
	pDoc->AndersonDarlingTest = false;
	pDoc->ANOVA = false;
	pDoc->PCA_Select_PCs = false;
	pDoc->PCA_Display_PCs_Standard = false;
	pDoc->PCA_Display_Loadings = false;
	pDoc->PCA_Display_Scores = false;
	pDoc->FA_Display_Standard = false;
	pDoc->FA_Display_Loadings = true;
	pDoc->FA_Display_Scores = false;
	pDoc->FDA = false;
	pDoc->RegressionAnalysis = false;
	pDoc->RegressionAnalysis_Independent = false;
	Invalidate();
	UpdateWindow();

}

void CModelingandAnalysisofUncertaintyView::On_Display_Factor_Scores() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->ShowStatisticalAnalysis = false;
	pDoc->OnOpenedFile = false;
	pDoc->DescriptiveStatistics = false;
	pDoc->HypothesisTesting_OneSample = false;
	pDoc->HypothesisTesting_TwoSample = false;
	pDoc->ShapiroWilkTest = false;
	pDoc->AndersonDarlingTest = false;
	pDoc->ANOVA = false;
	pDoc->PCA_Select_PCs = false;
	pDoc->PCA_Display_PCs_Standard = false;
	pDoc->PCA_Display_Loadings = false;
	pDoc->PCA_Display_Scores = false;
	pDoc->FA_Display_Standard = false;
	pDoc->FA_Display_Loadings = false;
	pDoc->FA_Display_Scores = true;
	pDoc->FDA = false;
	pDoc->RegressionAnalysis = false;
	pDoc->RegressionAnalysis_Independent = false;
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::On_Display_Factor_Matrices() {
	CModelingandAnalysisofUncertaintyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;
	pDoc->ShowStatisticalAnalysis = false;
	pDoc->OnOpenedFile = false;
	pDoc->DescriptiveStatistics = false;
	pDoc->HypothesisTesting_OneSample = false;
	pDoc->HypothesisTesting_TwoSample = false;
	pDoc->ShapiroWilkTest = false;
	pDoc->AndersonDarlingTest = false;
	pDoc->ANOVA = false;
	pDoc->PCA_Select_PCs = false;
	pDoc->PCA_Display_PCs_Standard = false;
	pDoc->PCA_Display_Loadings = false;
	pDoc->PCA_Display_Scores = false;
	pDoc->FA_Display_Standard = false;
	pDoc->FA_Display_Loadings = false;
	pDoc->FA_Display_Scores = false;
	pDoc->FA_Display_Matrices = true;
	pDoc->FDA = false;
	pDoc->RegressionAnalysis = false;
	pDoc->RegressionAnalysis_Independent = false;
	Invalidate();
	UpdateWindow();
}

void CModelingandAnalysisofUncertaintyView::DisplayRectangleTwoTailed(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, double max_width, double upper, double lower, CString kind, double obs, double max_height, char z_t) {



	if (kind == "confidence") {
		// drawing the confidence interval
		CPoint top_left, bottom_right, tip_top_left, tip_bottom_right;
		top_left.x = max_width - 140 - 460;
		top_left.y = (max_height / 3) + 60 + 300;
		bottom_right.x = top_left.x + 500;
		bottom_right.y = (max_height / 3) + 60 + 260;
		int constant = bottom_right.x - top_left.x;
		int length = bottom_right.x - top_left.x;
		double offset = length * 0.1;

		double height = top_left.y - bottom_right.y;
		double ratio = height * 3;




		if (top_left.x > 1200 && bottom_right.x > 1200) {

			if (bottom_right.y <= 400) {
				bottom_right.y = 400;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 405;
					ratio = 15;
				}

				// drawing the cofidence interval
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);
				//vertical separation line 
				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x, bottom_right.y + ratio);
				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x, bottom_right.y + ratio);

				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x + offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x - offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(top_left.x, bottom_right.y + ratio), pDC->LineTo(top_left.x + offset / 1.5, bottom_right.y + ratio);
				pDC->MoveTo(bottom_right.x, bottom_right.y + ratio), pDC->LineTo(bottom_right.x - offset / 1.5, bottom_right.y + ratio);


				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);

				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");





				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					int CircleCenter = top_left.x + (((bottom_right.x - top_left.x) / (upper - lower)) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.8;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.8;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}



			}

			else {
				// drawing the cofidence interval
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);




				//drawing the vertical separation line 
				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x, bottom_right.y + ratio);
				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x, bottom_right.y + ratio);

				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x + offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x - offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(top_left.x, bottom_right.y + ratio), pDC->LineTo(top_left.x + offset / 1.5, bottom_right.y + ratio);
				pDC->MoveTo(bottom_right.x, bottom_right.y + ratio), pDC->LineTo(bottom_right.x - offset / 1.5, bottom_right.y + ratio);


				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);

				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");







				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					int CircleCenter = top_left.x + (((bottom_right.x - top_left.x) / (upper - lower)) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.8;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.8;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}
			}





		}


		if (top_left.x <= 1200 && bottom_right.x > 1200) {


			//just copy and paste 
			top_left.x = 1200;
			length = bottom_right.x - top_left.x;
			offset = length * 0.1;


			if (bottom_right.y <= 400) {
				bottom_right.y = 400;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 405;
					ratio = 15;
				}

				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);
				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x, bottom_right.y + ratio);
				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x, bottom_right.y + ratio);

				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x + offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x - offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(top_left.x, bottom_right.y + ratio), pDC->LineTo(top_left.x + offset / 1.5, bottom_right.y + ratio);
				pDC->MoveTo(bottom_right.x, bottom_right.y + ratio), pDC->LineTo(bottom_right.x - offset / 1.5, bottom_right.y + ratio);


				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");







				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (pDoc->show_mu > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}
			}

			else {
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);

				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x, bottom_right.y + ratio);
				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x, bottom_right.y + ratio);

				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x + offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x - offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(top_left.x, bottom_right.y + ratio), pDC->LineTo(top_left.x + offset / 1.5, bottom_right.y + ratio);
				pDC->MoveTo(bottom_right.x, bottom_right.y + ratio), pDC->LineTo(bottom_right.x - offset / 1.5, bottom_right.y + ratio);

				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");




				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);



				if (top_left.y - tip_bottom_right.y <= 50) {
					CRect rectA(tip_bottom_right.x, top_left.y + 50, tip_top_left.x, top_left.y + 70);
					pDC->Rectangle(rectA);
					CBrush BrushA;
					BrushA.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rectA, &BrushA);
					pDC->TextOutW(tip_bottom_right.x + 3, top_left.y + 50, L" Confidence Interval");
				}









				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (pDoc->show_mu > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}








			}












		}






	}


	if (kind == "critical") {

		// drawing the usable region (acceptance + rejection)
		double range = fabs(upper - lower);
		CPoint top_left, bottom_right, tip_top_left, tip_bottom_right;
		top_left.x = max_width - 140 - 470;
		top_left.y = (int)(max_height / 3) + 10;
		bottom_right.x = top_left.x + 500;
		bottom_right.y = (int)(max_height / 3) - 30;
		int length = bottom_right.x - top_left.x;
		double offset = length * 0.1;
		int constant = bottom_right.x - top_left.x;
		double height = top_left.y - bottom_right.y;
		double ratio = height * 3;


		if (top_left.x > 1200 && bottom_right.x > 1200) {


			if (bottom_right.y <= 180) {
				bottom_right.y = 180;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 185;
					ratio = 15;
				}
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);


				//lower bound
				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR1, &BrushR1);

				//upper bound
				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);



				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}

				// drawing the target 
				int center = (bottom_right.x - top_left.x) / 2;
				center += top_left.x;
				double spectrum = upper - lower;
				double value_center = (upper + lower) / 2;
				// when xObs is in the acceptance region
				if (obs <= upper && obs >= lower) {
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);

					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}
			else {
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);


				//lower bound
				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR1, &BrushR1);

				//upper bound
				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);



				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}


				// drawing the target 
				int center = (bottom_right.x - top_left.x) / 2;
				center += top_left.x;
				double spectrum = upper - lower;
				double value_center = (upper + lower) / 2;
				// when xObs is in the acceptance region
				if (obs <= upper && obs >= lower) {
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);

					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}

		}



		if (top_left.x <= 1200 && bottom_right.x > 1200) {

			if (bottom_right.y <= 180) {
				bottom_right.y = 180;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 185;
					ratio = 15;
				}

				top_left.x = 1200;
				length = bottom_right.x - top_left.x;
				offset = length * 0.1;


				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);

				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR1, &BrushR1);

				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);



				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");


				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}






				if (obs <= upper && obs >= lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}

			else {
				top_left.x = 1200;
				length = bottom_right.x - top_left.x;
				offset = length * 0.1;


				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);

				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR1, &BrushR1);

				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}







				if (obs <= upper && obs >= lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}


		}






	}

}
void CModelingandAnalysisofUncertaintyView::DisplayRectangleLeftTailed(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, double max_width, double upper, double lower, CString kind, double obs, double max_height, char z_t) {



	if (kind == "confidence") {
		lower = pDoc->show_delta_x - (upper - pDoc->show_delta_x);

		// drawing the confidence interval
		CPoint top_left, bottom_right, tip_top_left, tip_bottom_right;
		top_left.x = max_width - 140 - 460;
		top_left.y = (max_height / 3) + 60 + 300;
		bottom_right.x = top_left.x + 500;
		bottom_right.y = (max_height / 3) + 60 + 260;
		int constant = bottom_right.x - top_left.x;
		int length = bottom_right.x - top_left.x;
		double offset = length * 0.1;

		double height = top_left.y - bottom_right.y;
		double ratio = height * 3;




		if (top_left.x > 1200 && bottom_right.x > 1200) {

			if (bottom_right.y <= 400) {
				bottom_right.y = 400;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 405;
					ratio = 15;
				}

				// drawing the cofidence interval
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header, b1;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);



				//vertical separation line 

				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x, bottom_right.y + ratio);


				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x - offset / 1.5, top_left.y - ratio);

				pDC->MoveTo(bottom_right.x, bottom_right.y + ratio), pDC->LineTo(bottom_right.x - offset / 1.5, bottom_right.y + ratio);


				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");





				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);
					int CircleCenter = top_left.x + (((bottom_right.x - top_left.x) / (upper - lower)) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 100 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu > upper) {

					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);
					int CircleCenter = bottom_right.x + (int)offset * 0.8;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {

					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);
					int CircleCenter = top_left.x - (int)offset * 0.8;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}



			}

			else {
				// drawing the cofidence interval
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);




				//drawing the vertical separation line 

				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x, bottom_right.y + ratio);


				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x - offset / 1.5, top_left.y - ratio);

				pDC->MoveTo(bottom_right.x, bottom_right.y + ratio), pDC->LineTo(bottom_right.x - offset / 1.5, bottom_right.y + ratio);


				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");







				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {



					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					int CircleCenter = top_left.x + (((bottom_right.x - top_left.x) / (upper - lower)) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.8;


					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.8;


					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}
			}





		}


		if (top_left.x <= 1200 && bottom_right.x > 1200) {


			//just copy and paste 
			top_left.x = 1200;
			length = bottom_right.x - top_left.x;
			offset = length * 0.1;


			if (bottom_right.y <= 400) {
				bottom_right.y = 400;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 405;
					ratio = 15;
				}

				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);

				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x, bottom_right.y + ratio);


				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x - offset / 1.5, top_left.y - ratio);

				pDC->MoveTo(bottom_right.x, bottom_right.y + ratio), pDC->LineTo(bottom_right.x - offset / 1.5, bottom_right.y + ratio);


				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);

				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");







				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (pDoc->show_mu - lower));


					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (pDoc->show_mu > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;


					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;


					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}
			}

			else {
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);


				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x, bottom_right.y + ratio);


				pDC->MoveTo(bottom_right.x, top_left.y - ratio), pDC->LineTo(bottom_right.x - offset / 1.5, top_left.y - ratio);

				pDC->MoveTo(bottom_right.x, bottom_right.y + ratio), pDC->LineTo(bottom_right.x - offset / 1.5, bottom_right.y + ratio);

				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");




				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);

				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);



				if (top_left.y - tip_bottom_right.y <= 50) {
					CRect rectA(tip_bottom_right.x, top_left.y + 50, tip_top_left.x, top_left.y + 70);
					pDC->Rectangle(rectA);
					CBrush BrushA;
					BrushA.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rectA, &BrushA);
					pDC->TextOutW(tip_bottom_right.x + 3, top_left.y + 50, L" Confidence Interval");
				}









				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					int decrease = constant * 0.1 - offset;
					decrease /= 4;


					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (pDoc->show_mu > upper) {
					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;


					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {


					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;


					CRect rect_t(top_left.x - (int)offset - 10, top_left.y, top_left.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}








			}












		}






	}



	if (kind == "critical") {
		upper = pDoc->show_mu + (pDoc->show_mu - lower);
		// drawing the usable region (acceptance + rejection)
		double range = fabs(upper - lower);
		CPoint top_left, bottom_right, tip_top_left, tip_bottom_right;
		top_left.x = max_width - 140 - 470;
		top_left.y = (int)(max_height / 3) + 10;
		bottom_right.x = top_left.x + 500;
		bottom_right.y = (int)(max_height / 3) - 30;
		int length = bottom_right.x - top_left.x;
		double offset = length * 0.11;
		int constant = bottom_right.x - top_left.x;
		double height = top_left.y - bottom_right.y;
		double ratio = height * 3;


		if (top_left.x > 1200 && bottom_right.x > 1200) {


			if (bottom_right.y <= 180) {
				bottom_right.y = 180;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 185;
					ratio = 15;
				}
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);


				//lower bound
				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR1, &BrushR1);

				//upper bound
				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;

				sth2.Format(L"%.2f", lower);

				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);



				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}


				// drawing the target 
				int center = (bottom_right.x - top_left.x) / 2;
				center += top_left.x;
				double spectrum = upper - lower;
				double value_center = (upper + lower) / 2;
				// when xObs is in the acceptance region
				if (obs <= upper && obs >= lower) {
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);

					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}
			else {
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);


				//lower bound
				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR1, &BrushR1);

				//upper bound
				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;

				sth2.Format(L"%.2f", lower);

				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);




				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}


				// drawing the target 
				int center = (bottom_right.x - top_left.x) / 2;
				center += top_left.x;
				double spectrum = upper - lower;
				double value_center = (upper + lower) / 2;
				// when xObs is in the acceptance region
				if (obs <= upper && obs >= lower) {
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);

					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}

		}



		if (top_left.x <= 1200 && bottom_right.x > 1200) {

			if (bottom_right.y <= 180) {
				bottom_right.y = 180;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 185;
					ratio = 15;
				}

				top_left.x = 1200;
				length = bottom_right.x - top_left.x;
				offset = length * 0.1;


				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);

				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR1, &BrushR1);

				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;

				sth2.Format(L"%.2f", lower);

				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);



				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");


				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}






				if (obs <= upper && obs >= lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}

			else {
				top_left.x = 1200;
				length = bottom_right.x - top_left.x;
				offset = length * 0.1;


				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);

				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR1, &BrushR1);

				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;

				sth2.Format(L"%.2f", lower);

				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}







				if (obs <= upper && obs >= lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}


		}






	}

}
void CModelingandAnalysisofUncertaintyView::DisplayRectangleRightTailed(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, double max_width, double upper, double lower, CString kind, double obs, double max_height, char z_t) {

	if (kind == "confidence") {
		upper = pDoc->show_delta_x + (pDoc->show_delta_x - lower);

		// drawing the confidence interval
		CPoint top_left, bottom_right, tip_top_left, tip_bottom_right;
		top_left.x = max_width - 140 - 460;
		top_left.y = (max_height / 3) + 60 + 300;
		bottom_right.x = top_left.x + 500;
		bottom_right.y = (max_height / 3) + 60 + 260;
		int constant = bottom_right.x - top_left.x;
		int length = bottom_right.x - top_left.x;
		double offset = length * 0.1;
		double height = top_left.y - bottom_right.y;
		double ratio = height * 3;




		if (top_left.x > 1200 && bottom_right.x > 1200) {

			if (bottom_right.y <= 400) {
				bottom_right.y = 400;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 405;
					ratio = 15;
				}

				// drawing the cofidence interval
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header, b1;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);



				//vertical separation line 

				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x, bottom_right.y + ratio);
				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x + offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(top_left.x, bottom_right.y + ratio), pDC->LineTo(top_left.x + offset / 1.5, bottom_right.y + ratio);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");





				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					int CircleCenter = top_left.x + (((bottom_right.x - top_left.x) / (upper - lower)) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 100 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu > upper) {

					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);
					int CircleCenter = bottom_right.x + (int)offset * 0.8;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {

					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);
					int CircleCenter = top_left.x - (int)offset * 0.8;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}



			}

			else {
				// drawing the cofidence interval
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);




				//drawing the vertical separation line 

				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x, bottom_right.y + ratio);
				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x + offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(top_left.x, bottom_right.y + ratio), pDC->LineTo(top_left.x + offset / 1.5, bottom_right.y + ratio);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");







				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {



					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					int CircleCenter = top_left.x + (((bottom_right.x - top_left.x) / (upper - lower)) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.8;


					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.8;


					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}
			}





		}


		if (top_left.x <= 1200 && bottom_right.x > 1200) {


			//just copy and paste 
			top_left.x = 1200;
			length = bottom_right.x - top_left.x;
			offset = length * 0.1;


			if (bottom_right.y <= 400) {
				bottom_right.y = 400;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 405;
					ratio = 15;
				}

				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);

				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x, bottom_right.y + ratio);
				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x + offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(top_left.x, bottom_right.y + ratio), pDC->LineTo(top_left.x + offset / 1.5, bottom_right.y + ratio);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);


				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");







				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (pDoc->show_mu - lower));


					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (pDoc->show_mu > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;


					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);



					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;


					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}
			}

			else {
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rect, &Brush_Header);


				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x, bottom_right.y + ratio);
				pDC->MoveTo(top_left.x, top_left.y - ratio), pDC->LineTo(top_left.x + offset / 1.5, top_left.y - ratio);
				pDC->MoveTo(top_left.x, bottom_right.y + ratio), pDC->LineTo(top_left.x + offset / 1.5, bottom_right.y + ratio);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);
				sth2.Format(L"%.2f", lower);
				pDC->TextOutW(top_left.x - 10, 10 + top_left.y, sth2);

				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 120;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(177, 156, 217));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Confidence Interval");







				if (top_left.y - tip_bottom_right.y <= 50) {
					CRect rectA(tip_bottom_right.x, top_left.y + 50, tip_top_left.x, top_left.y + 70);
					pDC->Rectangle(rectA);
					CBrush BrushA;
					BrushA.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rectA, &BrushA);
					pDC->TextOutW(tip_bottom_right.x + 3, top_left.y + 50, L" Confidence Interval");
				}









				if (pDoc->show_mu <= upper && pDoc->show_mu >= lower) {

					int decrease = constant * 0.1 - offset;
					decrease /= 4;


					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (pDoc->show_mu - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (pDoc->show_mu > upper) {
					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;


					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (pDoc->show_mu < lower) {


					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;


					CRect rect_t(bottom_right.x + (int)offset + 10, top_left.y, bottom_right.x, bottom_right.y);
					pDC->Rectangle(rect_t);
					CBrush b1;
					b1.CreateSolidBrush(RGB(177, 156, 217));
					pDC->FillRect(&rect_t, &b1);

					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", pDoc->show_mu);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);


					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}








			}












		}






	}



	if (kind == "critical") {

		lower = pDoc->show_mu - (upper - pDoc->show_mu);
		// drawing the usable region (acceptance + rejection)
		double range = fabs(upper - lower);
		CPoint top_left, bottom_right, tip_top_left, tip_bottom_right;
		top_left.x = max_width - 140 - 470;
		top_left.y = (int)(max_height / 3) + 10;
		bottom_right.x = top_left.x + 500;
		bottom_right.y = (int)(max_height / 3) - 30;
		int length = bottom_right.x - top_left.x;
		double offset = length * 0.11;
		int constant = bottom_right.x - top_left.x;
		double height = top_left.y - bottom_right.y;
		double ratio = height * 3;


		if (top_left.x > 1200 && bottom_right.x > 1200) {


			if (bottom_right.y <= 180) {
				bottom_right.y = 180;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 185;
					ratio = 15;
				}
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);


				//lower bound
				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectR1, &BrushR1);

				//upper bound
				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);

				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);



				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}

				// drawing the target 
				int center = (bottom_right.x - top_left.x) / 2;
				center += top_left.x;
				double spectrum = upper - lower;
				double value_center = (upper + lower) / 2;
				// when xObs is in the acceptance region
				if (obs <= upper && obs >= lower) {
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);

					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}
			else {
				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);


				//lower bound
				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectR1, &BrushR1);

				//upper bound
				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);

				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);




				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}


				// drawing the target 
				int center = (bottom_right.x - top_left.x) / 2;
				center += top_left.x;
				double spectrum = upper - lower;
				double value_center = (upper + lower) / 2;
				// when xObs is in the acceptance region
				if (obs <= upper && obs >= lower) {
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);

					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {

					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 10), pDC->LineTo(CircleCenter, bottom_right.y - 10);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}

		}



		if (top_left.x <= 1200 && bottom_right.x > 1200) {

			if (bottom_right.y <= 180) {
				bottom_right.y = 180;
				height = top_left.y - bottom_right.y;
				ratio = height * 3;

				if (height <= 5) {
					top_left.y = 185;
					ratio = 15;
				}

				top_left.x = 1200;
				length = bottom_right.x - top_left.x;
				offset = length * 0.1;


				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);

				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectR1, &BrushR1);

				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);

				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);



				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");


				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}






				if (obs <= upper && obs >= lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}

			else {
				top_left.x = 1200;
				length = bottom_right.x - top_left.x;
				offset = length * 0.1;


				CRect rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
				pDC->Rectangle(rect);
				CBrush Brush_Header;
				Brush_Header.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rect, &Brush_Header);

				CRect rectR1(top_left.x, top_left.y, top_left.x - (int)offset, bottom_right.y);
				pDC->Rectangle(rectR1);
				CBrush BrushR1;
				BrushR1.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectR1, &BrushR1);

				CRect rectR2(bottom_right.x, top_left.y, bottom_right.x + (int)offset, bottom_right.y);
				pDC->Rectangle(rectR2);
				CBrush BrushR2;
				BrushR2.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectR2, &BrushR2);

				CString sth1, sth2;
				sth1.Format(L"%.2f", upper);

				pDC->TextOutW(bottom_right.x - 10, 10 + top_left.y, sth1);



				// drawing the tip
				CPoint tip_bottom_right, tip_top_left;
				tip_bottom_right.x = bottom_right.x - 200 + 70 + (int)offset;
				tip_top_left.x = bottom_right.x - 200 + 70;

				tip_top_left.y = bottom_right.y - 170;
				tip_bottom_right.y = tip_top_left.y + 10 + ratio * 0.1;

				CRect rectA(tip_bottom_right.x, tip_bottom_right.y, tip_top_left.x, tip_top_left.y);
				pDC->Rectangle(rectA);
				CBrush BrushA;
				BrushA.CreateSolidBrush(RGB(243, 102, 25));
				pDC->FillRect(&rectA, &BrushA);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y, L" Critical Region");

				CRect rectB(tip_bottom_right.x, tip_bottom_right.y + 30, tip_top_left.x, tip_top_left.y + 30);
				pDC->Rectangle(rectB);
				CBrush BrushB;
				BrushB.CreateSolidBrush(RGB(0, 225, 98));
				pDC->FillRect(&rectB, &BrushB);
				pDC->TextOutW(tip_bottom_right.x + 3, tip_top_left.y + 30, L" Acceptance Region");

				if (1 + 1 == 2) {
					// plotting the bell curve

					CPoint pixel_coor, value_coor;
					// find the range of y value 

					double f = (double)0;
					double PI = 3.141592653589793115997963468544185161590576171875;
					double mu = pDoc->show_mu;
					double var = double(0);
					if (z_t == 'Z') {
						double specialized = fabs(pDoc->show_zcrit);

						var = (lower - mu) / (-specialized * 15);
						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 20;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}
					}

					if (z_t == 'T') {
						double specialized = fabs(pDoc->show_tcrit);
						double rl = lower, ul = upper;
						if (upper <= 0.1) {
							upper *= 15;
							lower *= 15;
							var = (lower - mu) / (-specialized * 15);
						}
						else {
							var = (lower - mu) / (-specialized);
						}

						// y_upper is at x=mu
						double y_upper = exp(0);
						y_upper /= sqrt(2 * PI * var);

						//y_lower is at x=lower
						double y_lower = exp(-0.5 * (lower - mu) * (lower - mu) / var);
						y_lower /= sqrt(2 * PI * var);

						double y_abs_lower = exp(-0.5 * (lower - 0.1 - mu) * (lower - 0.1 - mu) / var);
						y_abs_lower /= sqrt(2 * PI * var);

						double range_value_y = y_upper - y_abs_lower;
						double range_value_x = upper - lower;

						int temporary = bottom_right.y - 30;

						int range_pixel_y = (temporary)-(bottom_right.y - 50 - 100);
						int range_pixel_x = (bottom_right.x - top_left.x);

						double ratio_x = (double)range_pixel_x / range_value_x;
						double ratio_y = (double)(range_pixel_y) / range_value_y;


						value_coor.x = lower;
						value_coor.y = y_lower;

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmany = ((double)(length / 10) / ratio_x) * 1000;

						for (int i = 1; i <= howmany; i++) {
							pixel_coor.x = top_left.x - (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower - 0.001 * i - mu) * (lower - 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						pixel_coor.x = top_left.x + (int)(ratio_x * (lower - lower));
						pixel_coor.y = (temporary)-(int)(ratio_y * (y_lower - y_lower));
						pDC->MoveTo(pixel_coor.x, pixel_coor.y);

						int howmuch = int((upper - lower) / 0.001);
						for (int i = 1; i <= howmuch + howmany; i++) {
							pixel_coor.x = top_left.x + (int)(ratio_x * (0.001 * i));
							f = exp(-0.5 * (lower + 0.001 * i - mu) * (lower + 0.001 * i - mu) / var);
							f /= sqrt(2 * PI * var);
							pixel_coor.y = (temporary)-(int)(ratio_y * (f - y_lower));
							pDC->LineTo(pixel_coor.x, pixel_coor.y);
						}

						if (rl != lower) {
							lower /= 15;
							upper /= 15;
						}
					}



				}







				if (obs <= upper && obs >= lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x + ((bottom_right.x - top_left.x) / (upper - lower) * (obs - lower));
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}

				else if (obs > upper) {

					int CircleCenter = bottom_right.x + (int)offset * 0.7;
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);

					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);
				}

				else if (obs < lower) {
					int decrease = constant * 0.1 - offset;
					decrease /= 4;
					int CircleCenter = top_left.x - (int)offset * 0.7;
					pDC->Ellipse(CircleCenter + decrease - (top_left.y - bottom_right.y) / 2, top_left.y, CircleCenter - decrease + (top_left.y - bottom_right.y) / 2, bottom_right.y);
					CString sth3;
					sth3.Format(L"%.2f", obs);
					pDC->TextOutW(CircleCenter - 23, 10 + top_left.y, sth3);
					pDC->MoveTo(CircleCenter, top_left.y + 6), pDC->LineTo(CircleCenter, bottom_right.y - 6);
					pDC->MoveTo(top_left.x - (int)offset - 20, (top_left.y + bottom_right.y) / 2), pDC->LineTo(bottom_right.x + (int)offset + 20, (top_left.y + bottom_right.y) / 2);

				}


			}


		}






	}
}
double CModelingandAnalysisofUncertaintyView::Normal(double para_mu, double para_var, double x) {
	double f = (double)0, PI = 3.141592653589793115997963468544185161590576171875;
	double mu = para_mu, var = para_var;
	f = exp(-0.5 * (x - mu) * (x - mu) / var);
	f /= sqrt(2 * PI * var);
	return f;
}
void CModelingandAnalysisofUncertaintyView::TwoSampleSequencePlot(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, CArray <double>& Values, CPoint TopLeft, CPoint BottomRight, CString Label, int sample1, int sample2) {
	CPen Standard;
	Standard.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(&Standard);
	//pDC->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
	int Length = BottomRight.x - TopLeft.x, Height = BottomRight.y - TopLeft.y - 100, x0 = TopLeft.x, y0 = TopLeft.y + 50, nObs = pDoc->n_Obs;
	double min, max, value, range, x_relative, y_relative;
	CPoint Point;
	//CRect rectA(TopLeft.x, TopLeft.y, TopLeft.x + 250, TopLeft.y + 38);
	//pDC->Rectangle(rectA);
	CBrush Brush_Header;
	Brush_Header.CreateSolidBrush(RGB(238, 228, 50));
	//pDC->FillRect(&rectA, &Brush_Header);
	pDC->SetBkMode(TRANSPARENT);
	CFont Standard_Font;
	Standard_Font.CreateStockObject(SYSTEM_FONT);
	pDC->SelectObject(Standard_Font);
	pDC->SetTextAlign(TA_LEFT);
	//pDC->TextOutW(TopLeft.x + 10, TopLeft.y + 10, _T("Sequence plot"));

	value = Values.GetAt(0);
	min = value, max = value;
	int NewL = BottomRight.x - TopLeft.x - 45;
	int t = (int)((double)pDoc->show_nObs2 / ((double)pDoc->show_nObs1 + (double)pDoc->show_nObs2) * (double)NewL + TopLeft.x + 45 * nWt / 1920);

	pDC->MoveTo(t, BottomRight.y), pDC->LineTo(t, TopLeft.y);

	CString c1, c2;
	c1.Format(L"%.d", sample1);
	c2.Format(L"%.d", sample2);

	pDC->TextOutW(TopLeft.x + 160 * nWt / 1920, BottomRight.y + 5, L"Class ");
	pDC->TextOutW(TopLeft.x + 210 * nWt / 1920, BottomRight.y + 5, c2);
	pDC->TextOutW(TopLeft.x + 500 * nWt / 1920, BottomRight.y + 5, L"Class ");
	pDC->TextOutW(TopLeft.x + 550 * nWt / 1920, BottomRight.y + 5, c1);



	for (int i = 1; i < pDoc->show_nObs1 + pDoc->show_nObs2; i++) {
		value = Values.GetAt(i);
		if (value > max) max = value;
		if (value < min) min = value;
	}

	range = max - min;
	CString Text;
	x_relative = 0.05, y_relative = 1.0;
	Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	x_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
	// Ticks denoting sample index
	CArray <double> XTicks, YTicks, XTickLabels, YTickLabels;
	GetTicks((double)1, (double)pDoc->show_nObs1 + pDoc->show_nObs2, XTicks, XTickLabels);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);


	for (int i = 0; i < XTicks.GetSize(); i++) {
		x_relative = 0.05 + 0.95 * XTicks.GetAt(i);
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height) - 10, pDC->MoveTo(Point);
		Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
		FormatTickLabel(Text, XTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x, Point.y, Text);
	}

	// y-axis denoting Observations
	x_relative = 0.05, y_relative = 1.0, Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
	y_relative = 0.0, Point.y = y0 + (int)(y_relative * (double)Height), pDC->LineTo(Point);
	// Ticks on y-axis
	GetTicks(min - 0.05 * range, max + 0.05 * range, YTicks, YTickLabels);
	pDC->SetTextAlign(TA_RIGHT);
	for (int i = 0; i < YTicks.GetSize(); i++) {
		y_relative = 0.05 + 0.9 * (1.0 - YTicks.GetAt(i));
		Point.x = x0 + (int)(x_relative * (double)Length) + 10, Point.y = y0 + (int)(y_relative * (double)Height), pDC->MoveTo(Point);
		Point.x = x0 + (int)(x_relative * (double)Length), pDC->LineTo(Point);
		FormatTickLabel(Text, YTickLabels.GetAt(i), 0);
		pDC->TextOutW(Point.x - 2, Point.y - 7, Text);
	}

	x_relative = 0.05;
	double dx = 0.95 / (double)Values.GetSize();
	CPen Obs;
	CBrush Fill;
	Fill.CreateSolidBrush(RGB(255, 224, 146));
	Obs.CreatePen(PS_SOLID, 1, RGB(243, 102, 25));

	for (int i = 0; i < pDoc->show_nObs1 + pDoc->show_nObs2; i++) {
		x_relative += dx;
		value = Values.GetAt(i);
		value -= min;
		value /= range;
		y_relative = 0.05 + 0.9 * (1 - value);
		Point.x = x0 + (int)(x_relative * (double)Length), Point.y = y0 + (int)(y_relative * (double)Height);
		pDC->SelectObject(&Fill);
		CRect rect(Point.x - 3, Point.y - 3, Point.x + 3, Point.y + 3);
		pDC->FillRect(&rect, &Fill);
		pDC->SelectObject(&Obs);
		pDC->Ellipse(Point.x - 4, Point.y - 4, Point.x + 4, Point.y + 4);
	}


	CFont XLabel, YLabel;
	XLabel.CreateFont(20, 10, 0, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&XLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	//pDC->TextOutW(x0 + (int)(0.5 * Length), y0 + Height + 20, _T("Observation"));
	YLabel.CreateFont(20, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Courier New"));
	pDC->SelectObject(&YLabel);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	//pDC->TextOutW(x0, y0 + (int)(0.5 * Height), Label);
	pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pDC->SelectObject(&Standard);
	pDC->SelectObject(&Standard_Font);
	pDC->SetTextAlign(TA_LEFT | TA_TOP);

}
double CModelingandAnalysisofUncertaintyView::GAM(double xx) {
	double x, y, tmp, ser;
	static double coef[6] = { 76.18009172947146,-86.50532032941677,24.01409824083091,-1.231739572450155,0.1208650973866179e-2,-0.5395239384953e-5 };
	int j;
	y = x = xx;
	tmp = x + 5.5;
	tmp -= (x + 0.5) * log(tmp);
	ser = 1.000000000190015;
	for (j = 0; j <= 5; j++) ser += coef[j] / ++y;
	return -tmp + log(2.5066282746310005 * ser / x);
}
void CModelingandAnalysisofUncertaintyView::TwoSampleBoxPlot(CModelingandAnalysisofUncertaintyDoc* pDoc, CDC* pDC, int max_width, int max_height, double MAX_PURE, double MIN_PURE, double UpperQ, double LowerQ, double MediumQ, CArray<double>& O2, int N2, CPoint& TopLeft, CPoint& BottomRight, int WhatClass, double rooftop, double floor) {
	
	CPoint TopPurePixel, BottomPurePixel;
	nWt = max_width;
	nHt = max_height;

	TopPurePixel.y = TopLeft.y + 30;
	BottomPurePixel.y = BottomRight.y - 30;



	// draw the axis
	pDC->MoveTo(TopLeft.x, TopLeft.y), pDC->LineTo(TopLeft.x, BottomRight.y);
	pDC->MoveTo(TopLeft.x, BottomRight.y), pDC->LineTo(BottomRight.x, BottomRight.y);



	double most_max, least_min;
	most_max = rooftop;
	least_min = floor;


	// deciding range for values including outliers (range = largest value within the two samples - smallest value within the two samples)

	int PureRangePixel = BottomPurePixel.y - TopPurePixel.y;
	double PureRangeValue = (most_max - least_min);
	double ratio = (double)(PureRangePixel) / PureRangeValue;



	// drawing sample 2
	// drawing the max and min borders
	
	

	CPoint max_pixel_sample2, min_pixel_sample2;
	max_pixel_sample2.y = BottomPurePixel.y - ratio * (MAX_PURE - least_min);
	min_pixel_sample2.y = BottomPurePixel.y - ratio * (MIN_PURE - least_min);

	
	pDC->MoveTo(TopLeft.x + 120 * nWt / 1920, max_pixel_sample2.y), pDC->LineTo(TopLeft.x + 180 * nWt / 1920, max_pixel_sample2.y);
	
	pDC->MoveTo(TopLeft.x + 120 * nWt / 1920, min_pixel_sample2.y), pDC->LineTo(TopLeft.x + 180 * nWt / 1920, min_pixel_sample2.y);



	// drawing the quartiles 
	CPoint q1_pixel_sample2, q2_pixel_sample2, q3_pixel_sample2;
	q1_pixel_sample2.y = BottomPurePixel.y - ratio * (LowerQ - least_min);
	q2_pixel_sample2.y = BottomPurePixel.y - ratio * (MediumQ - least_min);
	q3_pixel_sample2.y = BottomPurePixel.y - ratio * (UpperQ - least_min);
	
	CRect Rectangle3(TopLeft.x + 90 * nWt / 1920, q1_pixel_sample2.y, TopLeft.x + 220 * nWt / 1920, q2_pixel_sample2.y), Rectangle4(TopLeft.x + 90 * nWt / 1920, q2_pixel_sample2.y, TopLeft.x + 220 * nWt / 1920, q3_pixel_sample2.y);
	pDC->Rectangle(Rectangle3);
	pDC->Rectangle(Rectangle4);
	
	CBrush Brush3, Brush4;
	Brush3.CreateSolidBrush(RGB(174, 234, 255));
	pDC->FillRect(&Rectangle3, &Brush3);
	Brush4.CreateSolidBrush(RGB(174, 234, 255));
	pDC->FillRect(&Rectangle4, &Brush4);



	// drawing the border lines 
	pDC->MoveTo(TopLeft.x + 90 * nWt / 1920, q1_pixel_sample2.y), pDC->LineTo(TopLeft.x + 220 * nWt / 1920, q1_pixel_sample2.y);
	pDC->MoveTo(TopLeft.x + 90 * nWt / 1920, q2_pixel_sample2.y), pDC->LineTo(TopLeft.x + 220 * nWt / 1920, q2_pixel_sample2.y);
	pDC->MoveTo(TopLeft.x + 90 * nWt / 1920, q3_pixel_sample2.y), pDC->LineTo(TopLeft.x + 220 * nWt / 1920, q3_pixel_sample2.y);

	pDC->MoveTo(TopLeft.x + 90 * nWt / 1920, q1_pixel_sample2.y), pDC->LineTo(TopLeft.x + 90 * nWt / 1920, q3_pixel_sample2.y);
	pDC->MoveTo(TopLeft.x + 220 * nWt / 1920, q1_pixel_sample2.y), pDC->LineTo(TopLeft.x + 220 * nWt / 1920, q3_pixel_sample2.y);
	
	//drawing the tails
	pDC->MoveTo(TopLeft.x + 150 * nWt / 1920, max_pixel_sample2.y), pDC->LineTo(TopLeft.x + 150 * nWt / 1920, q3_pixel_sample2.y);
	pDC->MoveTo(TopLeft.x + 150 * nWt / 1920, min_pixel_sample2.y), pDC->LineTo(TopLeft.x + 150 * nWt / 1920, q1_pixel_sample2.y);
	pDC->TextOutW(TopLeft.x + 110 * nWt / 1920, BottomRight.y + 10, L"class ");
	CString sth2;
	sth2.Format(L"%.d", WhatClass);
	pDC->TextOutW(TopLeft.x + 152 * nWt / 1920, BottomRight.y + 10, sth2);


	if (!O2.IsEmpty()) {
		for (int i = 0; i < N2; i++) {


			CPoint center;
			center.y = BottomPurePixel.y - ratio * (O2.GetAt(i) - least_min);


			pDC->Ellipse(TopLeft.x + 143 * nWt / 1920, center.y + 6, TopLeft.x + 155 * nWt / 1920, center.y - 6);
		}
	}

	//drawing the ticks 
	CArray <double> a1, a2;
	GetTicks(least_min, most_max, a1, a2);
	int l1 = a1.GetSize(); // l1 denotes the length of 
	int l2 = a2.GetSize();



	CPoint TicksPixelYCoor;
	CString TempTickValue;
	for (int i = 0; i < l1; i++) {
		TempTickValue = "";
		TicksPixelYCoor.y = BottomRight.y - (int)((double)PureRangePixel * a1.GetAt(i));
		pDC->MoveTo(TopLeft.x - 10 * nWt / 1920, TicksPixelYCoor.y), pDC->LineTo(TopLeft.x + 10 * nWt / 1920, TicksPixelYCoor.y);
		TempTickValue.Format(L"%.2f", a2.GetAt(i));
		pDC->TextOutW(TopLeft.x + 20 * nWt / 1920, TicksPixelYCoor.y - 10, TempTickValue);
	}
	
}





// Helper function to draw grid
void DrawGrid(CDC& dc, int startX, int startY, int endX, int endY, double xTickInterval, double yTickInterval, int numXTicks, int numYTicks) {
	CPen gridPen(PS_DOT, 1, RGB(200, 200, 200));  // Light gray color for grid
	dc.SelectObject(&gridPen);

	// Draw vertical grid lines
	for (int i = 1; i <= numXTicks; ++i) {
		int x = startX + static_cast<int>(i * xTickInterval);
		dc.MoveTo(x, startY);
		dc.LineTo(x, endY);
	}

	// Draw horizontal grid lines
	for (int i = 1; i <= numYTicks; ++i) {
		int y = startY - static_cast<int>(i * yTickInterval);
		dc.MoveTo(startX, y);
		dc.LineTo(endX, y);
	}
}


// After ANN training call this view function to plot the loss curve
void CModelingandAnalysisofUncertaintyView::PlotLossCurve() {
	// read in data from txt log file
	std::vector<double> losses;
	std::ifstream file("ANN_Update/training_loss.txt");
	std::string line;
	while (std::getline(file, line)) {
		if (line.find("loss:") != std::string::npos) {
			size_t pos = line.find("loss:");
			double loss_value = std::stod(line.substr(pos + 6));
			losses.push_back(loss_value);
		}
	}

	CClientDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	int margin = 70;  // Increased margin for more space
	int spacingBetweenGraphs = 50;  // spacing between graphs
	int graphHeight = (rc.Height() - 3 * margin - spacingBetweenGraphs) / 2;
	int graphWidth = rc.Width() - 2 * margin;

	int startX = margin;
	int startY = (2 * graphHeight) + (2 * margin) + spacingBetweenGraphs;
	int endX = startX + graphWidth;
	int endY = startY - graphHeight;

	double scaleX = static_cast<double>(graphWidth) / losses.size();
	double scaleY = static_cast<double>(graphHeight);

	if (!losses.empty()) {
		double maxLoss = *std::max_element(losses.begin(), losses.end());
		double scaleX = static_cast<double>(graphWidth) / losses.size();
		double scaleY = static_cast<double>(graphHeight) / maxLoss;




		CPen penLine(PS_SOLID, 2, RGB(255, 0, 0));  // Red color for curve
		dc.SelectObject(&penLine);

		dc.MoveTo(startX, startY - static_cast<int>(losses[0] * scaleY));
		for (int i = 0; i < losses.size(); ++i) {
			int x = startX + static_cast<int>(i * scaleX);
			int y = startY - static_cast<int>(losses[i] * scaleY);
			dc.LineTo(x, y);
		}

		int numXTicks = 10;
		int numYTicks = 10;

		double xTickInterval = static_cast<double>(graphWidth) / numXTicks;
		double yTickInterval = static_cast<double>(graphHeight) / numYTicks;

		// Explicitly draw the X-axis for the loss curve
		CPen penAxis(PS_SOLID, 2, RGB(0, 0, 0));  // Black color for axis
		dc.SelectObject(&penAxis);
		dc.MoveTo(startX, startY);
		dc.LineTo(endX, startY);
		dc.MoveTo(startX, startY);
		dc.LineTo(startX, endY);

		// Draw X-axis ticks and labels
		for (int i = 1; i <= numXTicks; ++i) {
			int x = startX + static_cast<int>(i * xTickInterval);
			dc.MoveTo(x, startY);
			dc.LineTo(x, startY - 5);
			int labelValue = static_cast<int>(i * losses.size() / numXTicks);
			CString label;
			label.Format(_T("%d"), labelValue);
			dc.TextOutW(x - 10, startY + 10, label);
		}

		// Draw Y-axis ticks and labels
		for (int i = 1; i <= numYTicks; ++i) {
			int y = startY - static_cast<int>(i * yTickInterval);
			dc.MoveTo(startX, y);
			dc.LineTo(startX + 5, y);
			double labelValue = static_cast<double>(i * maxLoss / numYTicks);
			CString label;
			label.Format(_T("%.2f"), labelValue);
			dc.TextOutW(startX - 40, y - 5, label);
		}

		// Draw the X and Y axis labels at more appropriate locations
		dc.TextOutW(startX, startY + 10, L"Iterations");
		dc.TextOutW(startX - 40, endY - 25, L"Loss Value");


		DrawGrid(dc, startX, startY, endX, endY, xTickInterval, yTickInterval, numXTicks, numYTicks);
	}
}



// After ANN training call this view function to plot the accuracy curve
void CModelingandAnalysisofUncertaintyView::PlotAccuraciesCurve() {

	// read in txtx log file data
	std::vector<double> training_accuracies, testing_accuracies;
	std::ifstream file("ANN_Update/training_acc.txt");
	std::string line;
	while (std::getline(file, line)) {
		if (line.find("training accuracy:") != std::string::npos) {
			size_t pos = line.find("training accuracy:");
			double training_accuracy = std::stod(line.substr(pos + 18));
			training_accuracies.push_back(training_accuracy);
		}
		else if (line.find("testing accuracy:") != std::string::npos) {
			size_t pos = line.find("testing accuracy:");
			double testing_accuracy = std::stod(line.substr(pos + 17));
			testing_accuracies.push_back(testing_accuracy);
		}
	}

	CClientDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	int margin = 70;  // Increased margin for more space
	int spacingBetweenGraphs = 50;  // spacing between graphs
	int graphHeight = (rc.Height() - 3 * margin - spacingBetweenGraphs) / 2;
	int graphWidth = rc.Width() - 2 * margin;

	int startX = margin;
	int startY = (2 * graphHeight) + (2 * margin) + spacingBetweenGraphs;
	int endX = startX + graphWidth;
	int endY = startY - graphHeight;

	double scaleX = static_cast<double>(graphWidth) / training_accuracies.size();
	double scaleY = static_cast<double>(graphHeight);  // As accuracies range from 0 to 1

	// Plotting Training Accuracies
	CPen penTraining(PS_SOLID, 2, RGB(0, 0, 255));  // Blue color for training curve
	dc.SelectObject(&penTraining);
	if (!training_accuracies.empty()) {
		int firstX = startX;
		int firstY = startY - static_cast<int>(training_accuracies[0] * scaleY);
		dc.MoveTo(firstX, firstY);
		for (int i = 1; i < training_accuracies.size(); ++i) {
			int x = startX + static_cast<int>((i + 1) * scaleX);
			int y = startY - static_cast<int>(training_accuracies[i] * scaleY);
			dc.LineTo(x, y);
		}
	}

	// Plotting Testing Accuracies
	CPen penTesting(PS_SOLID, 2, RGB(0, 255, 0));  // Green color for testing curve
	dc.SelectObject(&penTesting);
	if (!testing_accuracies.empty()) {
		int firstX = startX;
		int firstY = startY - static_cast<int>(testing_accuracies[0] * scaleY);
		dc.MoveTo(firstX, firstY);
		for (int i = 1; i < testing_accuracies.size(); ++i) {
			int x = startX + static_cast<int>((i + 1) * scaleX);
			int y = startY - static_cast<int>(testing_accuracies[i] * scaleY);
			dc.LineTo(x, y);
		}
	}

	// Explicitly draw the X and Y axes for the accuracy curve
	CPen penAxis(PS_SOLID, 2, RGB(0, 0, 0));  // Black color for axis
	dc.SelectObject(&penAxis);
	dc.MoveTo(startX, startY);
	dc.LineTo(endX, startY);
	dc.MoveTo(startX, startY);
	dc.LineTo(startX, endY);
	// Drawing the X and Y axis labels
	dc.TextOutW(startX, startY + 10, L"Epoch");
	dc.TextOutW(startX - 40, endY - 25, L"Accuracy");


	int numXTicks = 10;
	if (training_accuracies.size() < 10) {
		numXTicks = training_accuracies.size() - 1;
	}
	int numYTicks = 10;

	// Draw X-axis ticks and labels
	double xTickInterval = static_cast<double>(graphWidth) / numXTicks;
	for (int i = 1; i <= numXTicks; ++i) {
		int x = startX + static_cast<int>(i * xTickInterval);
		dc.MoveTo(x, startY);
		dc.LineTo(x, startY - 5); // Draw ticks upwards, not downwards
		CString label;
		label.Format(_T("%d"), static_cast<int>(i + 1));
		dc.TextOutW(x - 10, startY + 10, label);
	}
	// Draw Y-axis ticks and labels
	double yTickInterval = static_cast<double>(graphHeight) / numYTicks;
	for (int i = 1; i <= numYTicks; ++i) {
		int y = startY - static_cast<int>(i * yTickInterval);
		dc.MoveTo(startX, y);
		dc.LineTo(startX + 5, y);
		double labelValue = static_cast<float>(1 / numYTicks);
		CString label;
		label.Format(_T("%.1f"), static_cast<float>(i) / numYTicks);
		dc.TextOutW(startX - 40, y - 5, label);
	}
	DrawGrid(dc, startX, startY, endX, endY, xTickInterval, yTickInterval, numXTicks, numYTicks);

	int legendStartX = endX - 150; // Position 150 pixels left from the right edge
	int legendStartY = startY - graphHeight - 20; // Start 20 pixels below the top of the accuracy graph 
	dc.SelectObject(&penTraining);
	dc.MoveTo(legendStartX, legendStartY);
	dc.LineTo(legendStartX + 40, legendStartY);
	dc.TextOutW(legendStartX + 50, legendStartY - 5, L"Training");

	// Testing data
	legendStartY += 20; // Position 20 pixels below the training label
	dc.SelectObject(&penTesting);
	dc.MoveTo(legendStartX, legendStartY);
	dc.LineTo(legendStartX + 40, legendStartY);
	dc.TextOutW(legendStartX + 50, legendStartY - 5, L"Testing");
}


 
void CModelingandAnalysisofUncertaintyView::OnMachinelearningArtificialneuralnetworkwithaccuracy()
{
	//NewANN New_ANN_Dlg; 
	//New_ANN_Dlg.DoModal(); 
}
