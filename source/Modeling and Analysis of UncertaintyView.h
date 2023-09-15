
// Modeling and Analysis of UncertaintyView.h : interface of the CModelingandAnalysisofUncertaintyView class
//

#pragma once

constexpr UINT IDC_DISPLAY_PCA{ 101 };
constexpr UINT IDC_NEXT_VARIABLE{ 102 };
constexpr UINT IDC_PREVIOUS_VARIABLE{ 103 };
constexpr UINT IDC_INDEPENDENT_ASSESSMENT_REGRESSION{ 104 };
constexpr UINT IDC_STANDARD_ASSESSMENT_REGRESSION{ 105 };
constexpr UINT IDC_SETTINGS_DESCRIPTIVE_STATISTICS{ 106 };
constexpr UINT IDC_LOADING_PLOTS{ 107 };
constexpr UINT IDC_SCORE_PLOTS{ 108 };
constexpr UINT IDC_SELECT_SCATTER_PLOTS{ 109 };
constexpr UINT IDC_DISPLAY_FA{ 110 };
constexpr UINT IDC_FACTOR_LOADINGS{ 111 };
constexpr UINT IDC_FACTOR_SCORES{ 112 };
constexpr UINT IDC_FACTOR_MATRICES{ 113 };

class CModelingandAnalysisofUncertaintyView : public CView{
protected: // create from serialization only
	CModelingandAnalysisofUncertaintyView() noexcept;
	DECLARE_DYNCREATE(CModelingandAnalysisofUncertaintyView)

// Attributes
public:
	CModelingandAnalysisofUncertaintyDoc* GetDocument() const;
private:
	void TwoSampleBoxPlot(CModelingandAnalysisofUncertaintyDoc*, CDC*, int, int, double, double, double, double, double, CArray<double>&, int, CPoint&, CPoint&, int, double, double);
	double GAM(double);
	// plots for two sample hypothesis testing
	void DisplayRectangleTwoTailed(CModelingandAnalysisofUncertaintyDoc*, CDC*, double, double, double, CString, double, double, char);
	void DisplayRectangleRightTailed(CModelingandAnalysisofUncertaintyDoc*, CDC*, double, double, double, CString, double, double, char);
	void DisplayRectangleLeftTailed(CModelingandAnalysisofUncertaintyDoc*, CDC*, double, double, double, CString, double, double, char);
	double Normal(double, double, double);
	void TwoSampleSequencePlot(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint, CPoint, CString, int, int);
	


// Operations
public:
	int nHt, nWt;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CModelingandAnalysisofUncertaintyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void On_Display_PCA();
	afx_msg void On_Next_Variable();
	afx_msg void On_Previous_Variable();
	afx_msg void On_Independent_Assessment_Regression();
	afx_msg void On_Standard_Assessment_Regression();
	afx_msg void On_Settings_Descriptive_Statistics();
	afx_msg void On_Display_Loadings();
	afx_msg void On_Display_Scores();
	afx_msg void On_Select_Scatter_Plots();
	afx_msg void On_Display_FA();
	afx_msg void On_Display_Factor_Loadings();
	afx_msg void On_Display_Factor_Scores();
	afx_msg void On_Display_Factor_Matrices();
	DECLARE_MESSAGE_MAP()
public:
	void PlotLossCurve();
	void PlotAccuraciesCurve();
private:
	int counter = 0, bins = 2, Distribution = 0, n = 1, n_Grid = 1;
	bool Type = false, Flag_Select = true;
	CArray <int> SelectedVariables;
	// Generating Ticks and Ticklabels for diagrams
	void GetTicks(double, double, CArray <double>&, CArray <double>&);
	void RemoveTicks(CArray <double>&);
	void AddTicks(CArray <double>&, double, double);
	int FormatTickLabel(CString&, double, int);
	int FormatValue(CString&, double, int);
	void ShowProbabilityPlot(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint, CPoint, CString);
	void GetDistributionParameters(CModelingandAnalysisofUncertaintyDoc*, CArray <double>&, CArray <double>&, double&, double&);
	void GetMinimum(CArray <double>&, int&, double&);
	void GetMaximum(CArray <double>&, int&, double&);
	void DisplayFileAndDataSetInformation(CModelingandAnalysisofUncertaintyDoc*, CDC*, bool);
	// Plots for descriptive statistics
	void DisplayDescriptiveStatistics(CDC*, CModelingandAnalysisofUncertaintyDoc*);
	void DisplayBoxPlot(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint, CPoint, int, CString);
	void DisplayHistogram(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint, CPoint, int, CString);
	void SequencePlot(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint , CPoint, CString);
	double GetPDF(double, CArray <double>&, double);
	void DisplayPDF(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint, CPoint);
	// Plots for PCA/FA models
	int DisplayCorrelationMatrix(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint);
	int DisplayCovarianceMatrices(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint, CString);
	void DisplayMatrix(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CArray <int>&, CPoint, CString);
	void DisplayEigenvaluesAndCumVariance(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint, CPoint, bool);
	void DisplayStoppingRules(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint, CPoint);
	void DisplayLoadings(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint, CPoint, CString);
	void DisplayScores(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint, CPoint, CString);
	void DisplayUnivariateStatistics(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint, CPoint);
	int DisplayPCAModelResults(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint);
	void DisplayScatterDiagrams(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint, CPoint, bool, bool, CString);
	void DisplayScatterPlot(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CArray <double>&, CPoint, CPoint, int, int, CString, CString, bool, bool, bool, double);
	void DisplayFactorAnalysisModel(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint);
	// Plots for Fisher discriminant analysis
	// Plots for regression analsyis
	void DisplayStandardRegressionAnalysis(CModelingandAnalysisofUncertaintyDoc*, CDC*);
	void ShowHypothesisTextingResults(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint&, CPoint&);
	void ShowRegressionCoefficients(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint&, CPoint&, int);
	void ShowModelPrediction(CModelingandAnalysisofUncertaintyDoc*, CDC*, CPoint&, CPoint&);
	void ShowPredictionError(CModelingandAnalysisofUncertaintyDoc*, CDC*, CArray <double>&, CPoint&, CPoint&);
public:
	CButton Next_Variable;
	CButton Previous_Variable;
	CButton Ind_Ass_Regression;
	CButton Sta_Ass_Regression;
	CButton Settings_Descriptive_Statistics;
	// CButtons for PCA
	CButton DisplayPCA;
	CButton DisplayLoadingPlots;
	CButton DisplayScorePlots;
	CButton SelectScatterPlots;
	// CButtons for FA
	CButton DisplayFA;
	CButton DisplayFactorLoadings;
	CButton DisplayFactorScores;
	CButton DisplayFactorMatrices;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// Thread Messenger for ANN
	afx_msg LRESULT OnTrainingComplete(WPARAM wParam, LPARAM lParam);
};
#ifndef _DEBUG  // debug version in Modeling and Analysis of UncertaintyView.cpp
inline CModelingandAnalysisofUncertaintyDoc* CModelingandAnalysisofUncertaintyView::GetDocument() const
   { return reinterpret_cast<CModelingandAnalysisofUncertaintyDoc*>(m_pDocument); }
#endif