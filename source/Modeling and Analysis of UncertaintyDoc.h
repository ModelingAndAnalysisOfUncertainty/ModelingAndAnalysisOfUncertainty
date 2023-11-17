// Modeling and Analysis of UncertaintyDoc.h : interface of the CModelingandAnalysisofUncertaintyDoc class
//
#include <fstream>
#include <ctime>
#include "ProbabilityDistributions.h"
#include <vector>
#include <omp.h>

#pragma once

class CModelingandAnalysisofUncertaintyDoc : public CDocument {
protected: // create from serialization only
	CModelingandAnalysisofUncertaintyDoc() noexcept;
	DECLARE_DYNCREATE(CModelingandAnalysisofUncertaintyDoc)
	// Attributes
public:
	double xbar1 = 0.0, xbar2 = 0.0;
	int show_nObs1;
	int show_nObs2;
	int show_class1;
	int show_class2;
	double show_mean1;
	double show_mean2;
	double show_std1;
	double show_std2;
	double show_max2;
	double show_max1;
	double show_min1;
	double show_min2;
	double max_box;
	double min_box;
	double MAX_BOX;
	double MIN_BOX;
	double up1;
	double up2;
	double low1;
	double low2;
	double med1;
	double med2;
	CArray <double> OutlierArray1;
	CArray <double> OutlierArray2;
	CArray <double> Sample1;
	CArray <double> Sample2;
	CArray <double> OverallArray;
	int ol1;
	int ol2;
	CString show_var_known_unknown;
	CString show_var_equal_unequal;
	CString show_tail;
	CString show_decision;
	double show_z_statistic;
	double show_zcrit;
	double show_p_value;
	double show_F;
	double show_Fu;
	double show_Fl;
	double show_T;
	double show_SP;
	double show_tcrit;
	double show_delta_x;
	double show_critical_U;
	double show_critical_L;
	double show_confidence_U;
	double show_confidence_L;
	double show_mu;
	int number;
	int show_freedom;
	double minimum_sample1;
	double minimum_sample2;



private:
	CString my_text;

public:
	// VARIABLES FOR DATA INFO
	int n_Var = 0;
	int n_Obs = 0;
	CString PathAndFileName;
	bool FileOpen = false;
	CArray <double> Data;
	CArray <double> Data_ordered;
	CArray <int> Data_spec;
	CString Type;
	CString Problem;
	CString DateFirst;
	CString TimeFirst;
	CString DateLast;
	CString TimeLast;
	CString Application;
	CStringArray Tag;
	CStringArray Unit;
	CStringArray Name;
	// VARIABLES FOR DESCRIPTIVE STATISTICS
	CArray <double> MIN;
	CArray <double> MAX;
	CArray <double> Q1;
	CArray <double> Q2;
	CArray <double> Q3;
	CArray <double> RANGE;
	CArray <double> IQR;
	CArray <double> MEAN;
	CArray <double> STD;
	CArray <double> SE;
	CArray <double> SKEW;
	CArray <double> KURT;
	CArray <double> h;
	CArray <double*, double*> OUTLIERS;
	CArray <int> n_OUTLIERS;
	CArray <double> UB;
	CArray <double> LB;
	// *** VARIABLES FOR HYPOTHESIS TESTING
	int VariableSelected;
	// *** VARIABLES FOR PCA
	CArray <double> R, P, lambda, T, T_2, SPE, vre, vpc, CumVar;
	CArray <int> R_spec, T_spec, P_spec, PCs;
	double T95 = 0, Q95 = 0, alpha = 0.05;
	int n = 1, n_kg = 0, n_vpc = 0, n_vre = 0;
	bool OnlyX = true;
	// VARIABLES FOR FACTOR ANALYSIS
	CArray <double> RT, A, Sf, Se, F;
	CArray <int> RT_spec, A_spec, S_spec, F_spec;
	// VARIABLES FOR CLASSIFICATION
	int n_classes = 1;
	// VARIABLES FOR REGRESSION ANALYSIS
	CArray <double> Metrics_Raw;
	CArray <double> Metrics;
	CArray <int> Metrics_spec;
	CArray <double> w_raw;
	CArray <double> w;
	CArray <double> t_values;
	CArray <double> ConfIntervals;
	CArray <double> ypred;
	CArray <int> SelectedPredictors;
	// FLAG TO DISPLAY INITIAL WINDOW
	bool StartWindow = true;
	// FLAGS FOR BASIC STATISTICS
	double t_alpha;
	bool OnOpenedFile = false;
	bool DescriptiveStatistics = false;
	bool HypothesisTesting_OneSample = false;
	bool HypothesisTesting_TwoSample = false;
	bool ShapiroWilkTest = false;
	bool AndersonDarlingTest = false;
	bool ANOVA = false;
	// FLAGS FOR PRINCIPAL COMPONENT ANALYSIS
	bool PCA_Select_PCs = false;
	bool PCA_Display_PCs_Standard = false;
	bool PCA_Display_Loadings = false;
	bool PCA_Display_Scores = false;
	// FLAGS FOR FACTOR ANALYSIS
	bool FA_Display_Standard = false;
	bool FA_Display_Loadings = false;
	bool FA_Display_Scores = false;
	bool FA_Display_Matrices = false;
	// FLAGS FOR FISHER DISCRIMINANT ANALYSIS
	bool FDA = false;
	// FLAGS FOR LINEAR MULTIPLE REGRESSION ANALYSIS
	bool RegressionAnalysis = false;
	bool RegressionAnalysis_Independent = false;
	bool ShowStatisticalAnalysis = false;
	// FLAGS FOR ARTIFICIAL NEURAL NETWORKS
	bool ANN_Training = false;
	// OPERATIONS NEEDED FOR VIEW CLASS
	int GetPosition(int, int, CArray<int>&);
	void SaveMatrix(std::string, CArray <double>&, CArray <int>&);
	void SaveVector(std::string, CArray <double>&);
	double DMAX(double a, double b) { if (a > b) return a; else return b; };
	double DMIN(double a, double b) { if (a < b) return a; else return b; };
	void QuickSort(CArray <double>&, int, int);
	void GetColumn(CArray <double>&, CArray <int>&, CArray <double>&, int);
	void GetLargestElement(CArray <double>&, CArray <int>&, double&);
public:

	// Overrides
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);	// called when document is opened, used to read in data
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Implementation
public:
	virtual ~CModelingandAnalysisofUncertaintyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// *******************************************
	// ***      Auxilliary      Functions      ***
	// *******************************************
	double SIGN(double a, double b) { if (b >= (double)0) return fabs(a); else return -fabs(a); };
	int IMIN(int a, int b) { if (a > b) return b; else return a; }
	double SQR(double a) { if (a == (double)0) return (double)0; else return a * a; };
	double PYTHAG(double, double);
	void SwapElements(double&, double&);
	int Partition(CArray <double>&, int, int);
	double GetObjectiveFunctionKDE(CArray <double>&, double);
	// *******************************************
	// ***     Basic   Vector   Operations     ***
	// *******************************************
	void ScaleVector(CArray <double>&, double);
	void NormalizeVector(CArray <double>&);
	double GetLength(CArray <double>&);
	double GetSquaredLength(CArray <double>&);
	void CenterVector(CArray <double>&);
	// *******************************************
	// ***     Basic   Matrix   Operations     ***
	// *******************************************
	void GetRow(CArray <double>&, CArray <int>&, CArray <double>&, int);
	void InsertRow(CArray <double>&, CArray <int>&, CArray <double>&, int);
	void InsertColumn(CArray <double>&, CArray <int>&, CArray <double>&, int);
	double Determinant(CArray <double>&, CArray <int>&);
	void Transpose(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void TransposeParallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void Inverse(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void AddingMatrices(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void AddingMatricesParallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void SubtractingMatrices(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void SubtractingMatricesParallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void MatrixVectorProduct(CArray <double>&, CArray <int>&, CArray <double>&, CArray <double>&);
	void MatrixVectorProductParallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <double>&);
	void MatrixProduct(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void MatrixProductParallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void X_tr_X(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void X_tr_X_Parallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void X_tr_Y(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void X_tr_Y_Parallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void X_Y_tr(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void X_Y_tr_Parallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void X_X_tr(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void X_X_tr_Parallel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void SetUpPivot(CArray <double>&, CArray <int>&, int);
	void ManipulateRowForwardPath(CArray<double>&, CArray<int>&, int, int);
	void ManipulateRowForwardPathParallel(CArray <double>&, CArray <int>&, int, int);
	void ManipulateRowBackwardPath(CArray<double>&, CArray<int>&, int, int);
	void ManipulateRowBackwardPathParallel(CArray <double>&, CArray <int>&, int, int);
	void GaussJordanElimination(CArray<double>&, CArray<int>&, CArray<double>&, CArray<double>&);
	void GaussJordanEliminationParallel(CArray<double>&, CArray<int>&, CArray<double>&, CArray<double>&);
	void MatrixParallelTest();
	// *******************************************
	// ***      Matrix  Format  Conversion     ***
	// *******************************************
	std::vector<std::vector<double>> CarrayToVectorM(CArray <double>&, CArray <int>&);
	void VectorToCarrayM(std::vector<std::vector<double>>&, CArray <double>&, CArray <int>&);
	std::vector<double> CarrayToVectorV(CArray <double>&);
	void VectorToCarrayV(std::vector<double>&, CArray <double>&);
	// *******************************************
	// ***      Matrix     Decompositions      ***
	// *******************************************
	void QR(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, bool&);
	void CHOL(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, bool&);
	void SortSVD(CArray <double>&, CArray <int>&, CArray <double>&, CArray <double>&, CArray <int>&);
	void SVD(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <double>&, CArray <int>&, bool&);
	void LANCZOS(CArray<double>&, CArray <int>&, CArray <double>&, CArray <double>&, bool&);
	void QL(CArray<double>&, CArray <int>&, CArray <double>&, CArray <double>&, bool&);
	void SortEigendecomposition(CArray<double>&, CArray <int>&, CArray <double>&);
	void EVD(CArray<double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, bool&, bool&);
	void Transformation(CArray <double>& Temp, CArray <int>& Temp_spec, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&);
	void GEVD(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, CArray <double>&, int, bool&);
	// *******************************************
	// *** Multivariate Statistical Operations ***
	// *******************************************
	void StandardizeDataMatrix(CArray <double>&, CArray <double>&, CArray <double>&);
	void GetCorrelationMatrix(CArray <double>&, CArray <int>&, CArray <double>&, CArray <int>&, bool);
	void VPC(CArray <double>&, CArray <int>&, int&);
	void VRE(CArray <double>&, CArray <int>&, int&);
	// *** Additional functions
	double GetOptimalBandwidth(CArray <double>&);
	double NumericalIntegration(CArray <double>&, double);
	void SetUpFDAMatrices(CArray <double>&, CArray <double>&, CArray <int>&, CArray <double>&);
	// Regression analysis
	void GetStandardRegressionModel(CArray <double>&, CArray <int>&, CArray <double>&, CArray <double>&, CArray <double>&);
	void GetRegressionModel_LOOCV(CArray <double>&, CArray <int>&, CArray <double>&);
	void GetRegressionModel_5_Fold_CV(CArray <double>&, CArray <int>&, CArray <double>&);
	void GetRegressionModel_10_Fold_CV(CArray <double>&, CArray <int>&, CArray <double>&);
	void GetRegressionModel_MonteCarlo(CArray <double>&, CArray <int>&, CArray <double>&, int, double);
	void StandardizeRegressionData(CArray <double>&, CArray <int>&, CArray <double>&, CArray <double>&, CArray <double>&, double&, double&);
	void GetRegressionVector(CArray <double>&, CArray <int>&, CArray <double>&, CArray <double>&, bool);
	void GetStatisticalRegressorAnalysis(CArray <double>&);
	void GetRegressionMetrics(CArray <double>&, CArray <double>&, CArray <double>&);
	//Neural Network Functions
	std::vector<int> randsample(int n, int k);
	void GetNetworkPrediction(const std::vector<std::vector<double>>& X, const int H,
		                      const std::vector<double>& w, const std::vector<double>& b,
		                      std::vector<std::vector<double>>& F, std::vector<double>& yhat);
	void GetNetworkPredictionParallel(const std::vector<std::vector<double>>& X, const int H,
		                              const std::vector<double>& w, const std::vector<double>& b,
		                              std::vector<std::vector<double>>& F, std::vector<double>& yhat);
	std::vector<std::vector<double>> zscore(const std::vector<std::vector<double>>& data);
	std::vector<std::vector<double>> zscoreParallel(const std::vector<std::vector<double>>& data);
	double sum_squared_error(const std::vector<std::vector<double>>& Y1, const std::vector<std::vector<double>>& Y2);
	double sum_squared_error_parallel(const std::vector<std::vector<double>>& Y1, const std::vector<std::vector<double>>& Y2);
	void VecTranspose(std::vector<std::vector<double> >& b);
	void VecTransposeInt(std::vector<std::vector<int> >& b);

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnDescriptiveStatistics();
	afx_msg void OnOneSample();
	afx_msg void OnTwoSample();
	afx_msg void OnSWTest();
	afx_msg void OnADTest();
	afx_msg void OnANOVA();
	afx_msg void OnPCA_model();
	afx_msg void OnFA_Model();
	afx_msg void OnICA();
	afx_msg void OnCCA();
	afx_msg void OnFDA();
	afx_msg void OnLinearClassification();
	afx_msg void OnMultivariateRegression();
	afx_msg void OnPLS();
	afx_msg void OnKPCA();
	afx_msg void OnLR();
	afx_msg void OnSVM();
	afx_msg void OnKSVM();
	afx_msg void OnKFDA();
	afx_msg void OnL1_Regularization();
	afx_msg void OnL2_Regularization();
	afx_msg void OnKPLS();
	afx_msg void OnANN();
	afx_msg void OnANN_MFC();
	afx_msg void OnANN_MFC_Parallel();
	afx_msg void OnANN_batchParallel();
	afx_msg void OnUpdateDescriptiveStatistics(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOnesample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTwosample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSWTest(CCmdUI* pCmdUI);
	afx_msg void OnUpdateADTest(CCmdUI* pCmdUI);
	afx_msg void OnUpdateANOVA(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePCA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateICA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCCA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFDA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLinearClassification(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMultivariateRegression(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePLS(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKPCA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLR(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSVM(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKSVM(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKFDA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateL1_Regularization(CCmdUI* pCmdUI);
	afx_msg void OnUpdateL2_Regularization(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKPLS(CCmdUI* pCmdUI);
	afx_msg void OnUpdateANN(CCmdUI* pCmdUI);
};