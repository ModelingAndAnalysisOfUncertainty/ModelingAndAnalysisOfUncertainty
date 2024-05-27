 // Modeling and Analysis of UncertaintyDoc.h : interface of the CModelingandAnalysisofUncertaintyDoc class
#include <fstream>
#include <ctime>
#include "ProbabilityDistributions.h"
#include <vector>
#include <omp.h>
#include <stdlib.h>
//#define USE_LIBTORCH
#ifdef USE_LIBTORCH
#include <torch/torch.h>
#include <torch/script.h>
#endif

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
	//number of x
	int n_Var = 0;
	//number of y data
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
	CArray <double> Class_Y, Label_Y, avg_ClassY, ROC_Y, ROC_X;
	CArray <int> Class_count, Confusion_Label;
	int TP, FP, TN, FN;// True_Label, False_Label;
	double sensitivity, specificity, mcc_test, ppv_test, F1_test, acc_test, AUC_Total;
	std::vector<double>accuracies,sensitivities,specificities,ppvs,f1_scores,mccs, auc_totals;
	std::vector<double> tpr, fpr;
	std::vector<std::vector<double>> tprList, fprList;  // save tpr,fpr for the k-fold
	CArray<double> y_pred_class, y_true_class;
	// FLAGS FOR LINEAR MULTIPLE REGRESSION ANALYSIS
	bool RegressionAnalysis = false;
	bool RegressionAnalysis_Independent = false;
	bool ShowStatisticalAnalysis = false;
	// FLAGS FOR LINEAR CLASSIFICATION
	bool Linear_Classification = false;
	
	// FLAGS FOR ARTIFICIAL NEURAL NETWORKS
	bool ANN_Training = false;
	std::vector<double> Loss_Ann;
	int ann_batch_size;
	// OPERATIONS NEEDED FOR VIEW CLASS
	int GetPosition(int, int, CArray<int>&);
	void SaveMatrix(std::string, CArray <double>&, CArray <int>&);
	void SaveVector(std::string, CArray <double>&);
	double DMAX(double a, double b) { if (a > b) return a; else return b; };
	double DMIN(double a, double b) { if (a < b) return a; else return b; };
	void QuickSort(CArray <double>&, int, int);
	void GetColumn(CArray <double>&, CArray <int>&, CArray <double>&, int);
	void GetLargestElement(CArray <double>&, CArray <int>&, double&);
	// Variables for Neural Network Model
	double Model_MCC;
	double Model_F1_Micro;
	double Model_F1_Macro;
	double Model_F1_Weighted;
	double Model_Accuracy;
	double Model_ARI;
	double Model_RI;
	double Model_FMI;
	double Model_JI;
	double Model_F1;
	double Model_SEN;
	double Model_SPE;



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
	double ScalarProduct(CArray <double>&, CArray <double>&);
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
	void getTrainTestData(std::vector<std::vector<double> >& X,
						  std::vector<std::vector<double> >& Xtrain, std::vector<std::vector<double> >& Xtest,
						  std::vector<std::vector<int>>& Y, std::vector<std::vector<int>>& Ytrain,
						  std::vector<std::vector<int>>& Ytest, double trainFraction, const int M, const int C);
	void UpdateBiases(int c, int n_weights, const int M, const int H, const int train,
					  std::vector<double>& yhat, std::vector<double>& private_w, const int eta, std::vector<std::vector<double>>& F,
					  std::vector<std::vector<int>>& Ytrain, int n_biases, std::vector<std::vector<double> >& Xslice, std::vector<double>& private_b,
					  int slice_index, int Ntrain);
	void CModelingandAnalysisofUncertaintyDoc::EvaluateModel(std::vector<int>& yass0, std::vector<int>& ytrue);
	int CModelingandAnalysisofUncertaintyDoc::n_choose_k(int n, int k);
	int CModelingandAnalysisofUncertaintyDoc::factorial(int m);
	void CModelingandAnalysisofUncertaintyDoc::GetConfusionMatrix(CArray<int>& ConfusionMatrix,
																  std::vector<int>& yass0, std::vector<int>& ytrue);
	#ifdef USE_LIBTORCH
	// ********************
	// *** LibTorch ANN ***
	// ********************
	// Model configs
	struct ModelConfig {
		ModelConfig(const std::string& configPath) : configPath(configPath) {}
		bool loadConfig();
		std::string configPath;
		float splitRatio;
		bool isMulti;
		int fromPretrained;
		std::string checkpointPath;
		int numLayers;
		std::vector<int> layerNodes;
		double learningRate;
		int iterations;
		int batchSize;
		torch::Device device = torch::cuda::is_available() ? torch::kCUDA : torch::kCPU;
	};
	// Data preprocessing
	struct DataPreprocessor {
		DataPreprocessor(const std::string& filePath) :filePath(filePath) {}

		bool prepareData(const ModelConfig& config);

		// Helper functions
		int getNumFeatures() const { return numFeatures; }
		int getNumLabels() const { return numLabels; }
		int getDatasetSize() const { return data.size(); }
		torch::Tensor getTrainData() const { return trainData; }
		torch::Tensor getTrainLabels() const { return trainLabels; }
		torch::Tensor getTestData() const { return testData; }
		torch::Tensor getTestLabels() const { return testLabels; }

		std::string filePath;
		float splitRatio;
		int numFeatures;
		int numLabels;
		std::vector<std::vector<float>> data;
		std::vector<std::vector<float>> dataTrain;
		std::vector<std::vector<float>> dataTest;
		std::vector<float> flattenedDataTrain;
		std::vector<float> flattenedDataTest;
		std::vector<float> labels;
		std::vector<float> labelsTrain;
		std::vector<float> labelsTest;
		torch::Tensor trainData;
		torch::Tensor trainLabels;
		torch::Tensor testData;
		torch::Tensor testLabels;
	};
	// Libtorch custom dataset module
	struct CustomDataset : torch::data::Dataset<CustomDataset> {
		torch::Tensor data;
		torch::Tensor labels;

		explicit CustomDataset(const torch::Tensor data, const torch::Tensor labels)
			: data(std::move(data)), labels(std::move(labels)) {}

		torch::data::Example<> get(size_t index) override {
			return { data[index], labels[index] };
		}

		torch::optional<size_t> size() const override {
			return data.size(0);
		}
	};
	// LibTorch neural network module
	struct NeuralNetworkImpl : torch::nn::Module {
		std::vector<torch::nn::Linear> layers;
		bool isMulti;
		NeuralNetworkImpl(const ModelConfig& config, int numFeatures, int numLabels) {
			isMulti = config.isMulti;
			layers.push_back(register_module("Input_Layer", torch::nn::Linear(numFeatures, config.layerNodes[0])));
			for (size_t i = 0; i < config.numLayers - 1; ++i) {
				layers.push_back(register_module("Layer_" + std::to_string(i), torch::nn::Linear(config.layerNodes[i], config.layerNodes[i + 1])));
			}
			layers.push_back(register_module("Output_Layer", torch::nn::Linear(config.layerNodes.back(), numLabels)));
		};

		torch::Tensor forward(torch::Tensor x) {
			for (size_t i = 0; i < layers.size() - 1; ++i) {
				// linear layer with TanH activation
				x = torch::tanh(layers[i]->forward(x));
			}
			x = layers.back()->forward(x);
			// if regression problem
			if (!isMulti) { return x; }
			// MultiClass problem with softmax, dim=1 normalizes across the class scores
			else { return torch::log_softmax(x, /*dim*/1); }
		};
	};
	TORCH_MODULE(NeuralNetwork);
	// Training utils
	template <typename DataLoader>
	std::pair<double, double> trainModel(
		NeuralNetwork& net,
		DataLoader& trainLoader,
		torch::optim::Optimizer& optimizer,
		torch::Device device
	);
	template <typename DataLoader>
	std::pair<double, double> validateModel(
		NeuralNetwork& net,
		DataLoader& testLoader,
		torch::Device device
	);
	#endif
	// Support Vector Machine Functions (SMO Algorithms)
	// SMO Model Structures
	struct SMOModel {
		CArray<double> alphas;
		double b;
		CArray<int> supportVectorIndices;
		// Default constructor
		SMOModel() : b(0) {}

		// Custom copy constructor
		SMOModel(const SMOModel& other) : b(other.b) {
			// Manually copy elements of alphas
			for (int i = 0; i < other.alphas.GetSize(); ++i) {
				alphas.Add(other.alphas[i]);
			}
			// Manually copy elements of supportVectorIndices
			for (int i = 0; i < other.supportVectorIndices.GetSize(); ++i) {
				supportVectorIndices.Add(other.supportVectorIndices[i]);
			}
		}
		// Custom copy assignment operator
		SMOModel& operator=(const SMOModel& other) {
			if (this != &other) { // self-assignment check
				b = other.b;
				alphas.RemoveAll();
				supportVectorIndices.RemoveAll();
				// Copy elements
				for (int i = 0; i < other.alphas.GetSize(); ++i) {
					alphas.Add(other.alphas[i]);
				}
				for (int i = 0; i < other.supportVectorIndices.GetSize(); ++i) {
					supportVectorIndices.Add(other.supportVectorIndices[i]);
				}
			}
			return *this;
		}
	};
	// Linear Kernal Function, dot product of two vectors
	double linearKernal(CArray <double>&, CArray <double>&);
	// Calculate the SVM output for a given input vector
	double svmOutput(CArray<double>&, CArray<double>&, CArray<double>&, CArray<int>&, CArray<double>&, double);
	// Select the alpha pair for optimization
	bool selectAlphaPair(int&, int&, CArray<double>&, CArray<double>&, CArray<int>&, CArray<double>&, double&, double&, double);
	// Optimize a pair of Lagrange multipliers
	bool optimizeAlphaPair(int, int, CArray<double>&, CArray<double>&, CArray<double>&, CArray<int>&, double&, double);
	// train SVM with SMO
	SMOModel trainSMO(CArray<double>&, CArray<int>&, CArray<double>&, double, double, int);
	// Linear Classification
	void CModelingandAnalysisofUncertaintyDoc::SplitDataForKFold(int currentFold, int foldSize, int numFolds, int numRows, int numCols, CArray<double>& data, CArray<double>& label,
		CArray<double>& trainData, CArray<int>& trainData_spec, CArray<double>& trainLabel, CArray<double>& testData, CArray<int>& testData_spec, CArray<double>& testLabel);
	void CModelingandAnalysisofUncertaintyDoc::EvaluateModel(CArray<double>& testData, CArray<int>& testData_spec, CArray<double>& testLabel, int numFeatures,
		std::vector<double>& accuracies,std::vector<double>& sensitivities,std::vector<double>& specificities,std::vector<double>& ppvs,std::vector<double>& f1_scores,
		std::vector<double>& mccs,std::vector<double>& auc_totals, std::vector<std::vector <double>>& tprList, std::vector<std::vector <double>>& fprList, int iteration) ;
	void CModelingandAnalysisofUncertaintyDoc::CalculateClassificationMetrics(const CArray<double>& y_pred, const CArray<double>& y_true, double threshold);
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
	afx_msg void OnQPSolver();
	afx_msg void OnANN_MFC();
	#ifdef USE_LIBTORCH
	afx_msg void OnANN_LIBTORCH();
	#endif


	afx_msg void OnANN_MFC_layer1(double, int, int,HANDLE hEvent);
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
	afx_msg void OnUpdateMachinelearningArtificialneuralnetworkwithaccuracy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMachinelearningArtificialneuralnetworkLIBTORCH(CCmdUI* pCmdUI);

};