
// Modeling and Analysis of UncertaintyDoc.cpp : implementation of the CModelingandAnalysisofUncertaintyDoc class
//

#include "pch.h"
#include "framework.h"
#include <vector>
#include <algorithm>

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>        
#include <random>
#include <chrono>

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Modeling and Analysis of Uncertainty.h"
#endif

#include "Modeling and Analysis of UncertaintyDoc.h"
#include "Modeling and Analysis of UncertaintyView.h"
#include <propkey.h>
#include "CSelectNumberPCs.h"
#include "COneSampleHypothesisSelection.h"
#include "CTwoSampleHypothesisSelection.h"
#include "CSpecifyFactorAnalysis.h"
#include "CSpecifyRegressionModel.h"
#include "CANNForm.h"

//Dlib for Aritificial Neural Netowrk
#include <dlib/dnn.h>
#include <dlib/data_io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Thread Handler for ANN
#define WM_USER_THREAD_FINISHED WM_APP + 1

// CModelingandAnalysisofUncertaintyDoc

IMPLEMENT_DYNCREATE(CModelingandAnalysisofUncertaintyDoc, CDocument)

BEGIN_MESSAGE_MAP(CModelingandAnalysisofUncertaintyDoc, CDocument)
	ON_COMMAND(ID_BASICSTATISTICS_DESCRIPTIVESTATISTICS, &CModelingandAnalysisofUncertaintyDoc::OnDescriptiveStatistics)
	ON_COMMAND(ID_HYPOTHESISTESTING_ONESAMPLE, &CModelingandAnalysisofUncertaintyDoc::OnOneSample)
	ON_COMMAND(ID_HYPOTHESISTESTING_TWOSAMPLE, &CModelingandAnalysisofUncertaintyDoc::OnTwoSample)
	ON_COMMAND(ID_PROBABILITYDISTRIBUTION_SHAPIRO, &CModelingandAnalysisofUncertaintyDoc::OnSWTest)
	ON_COMMAND(ID_PROBABILITYDISTRIBUTION_ANDERSON, &CModelingandAnalysisofUncertaintyDoc::OnADTest)
	ON_COMMAND(ID_BASICSTATISTICS_ANOVA, &CModelingandAnalysisofUncertaintyDoc::OnANOVA)
	ON_COMMAND(ID_PRINCIPALCOMPONENTS_PRINCIPALCOMPONENTMODEL, &CModelingandAnalysisofUncertaintyDoc::OnPCA_model)
	ON_COMMAND(ID_PRINCIPALCOMPONENTANALYSIS_FACTORANALYSISMODEL, &CModelingandAnalysisofUncertaintyDoc::OnFA_Model)
	ON_COMMAND(ID_MULTIVARIATESTATISTICS_INDEPENDENTCOMPONANTANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnICA)
	ON_COMMAND(ID_MULTIVARIATESTATISTICS_CANONICALCORRELATIONANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnCCA)
	ON_COMMAND(ID_MULTIVARIATESTATISTICS_LINEARFISHERDISCRIMINANTANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnFDA)
	ON_COMMAND(ID_MULTIVARIATESTATISTICS_LINEARCLASSIFICATION, &CModelingandAnalysisofUncertaintyDoc::OnLinearClassification)
	ON_COMMAND(ID_MULTIVARIATESTATISTICS_MULTIVARIATEREGRESSION, &CModelingandAnalysisofUncertaintyDoc::OnMultivariateRegression)
	ON_COMMAND(ID_MULTIVARIATESTATISTICS_PARTIALLEASTSQUARES, &CModelingandAnalysisofUncertaintyDoc::OnPLS)
	ON_COMMAND(ID_MACHINELEARNING_KERNELPRINCIPALCOMPONENTANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnKPCA)
	ON_COMMAND(ID_MACHINELEARNING_LOGISTICREGRESSION, &CModelingandAnalysisofUncertaintyDoc::OnLR)
	ON_COMMAND(ID_SUPPORTVECTORMACHINES_LINEAR, &CModelingandAnalysisofUncertaintyDoc::OnSVM)
	ON_COMMAND(ID_SUPPORTVECTORMACHINES_KERNEL, &CModelingandAnalysisofUncertaintyDoc::OnKSVM)
	ON_COMMAND(ID_MACHINELEARNING_KERNELFISHERDISCRIMINANTANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnKFDA)
	ON_COMMAND(ID_REGULARIZATION_L1NORM, &CModelingandAnalysisofUncertaintyDoc::OnL1_Regularization)
	ON_COMMAND(ID_REGULARIZATION_L2NORM, &CModelingandAnalysisofUncertaintyDoc::OnL2_Regularization)
	ON_COMMAND(ID_MACHINELEARNING_KERNELPARTIALLEASTSQUARES, &CModelingandAnalysisofUncertaintyDoc::OnKPLS)
	ON_COMMAND(ID_MACHINELEARNING_ARTIFICIALNEURALNETWORK, &CModelingandAnalysisofUncertaintyDoc::OnANN_MFC)

	ON_UPDATE_COMMAND_UI(ID_BASICSTATISTICS_DESCRIPTIVESTATISTICS, &CModelingandAnalysisofUncertaintyDoc::OnUpdateDescriptiveStatistics)
	ON_UPDATE_COMMAND_UI(ID_HYPOTHESISTESTING_ONESAMPLE, &CModelingandAnalysisofUncertaintyDoc::OnUpdateOnesample)
	ON_UPDATE_COMMAND_UI(ID_HYPOTHESISTESTING_TWOSAMPLE, &CModelingandAnalysisofUncertaintyDoc::OnUpdateTwosample)
	ON_UPDATE_COMMAND_UI(ID_PROBABILITYDISTRIBUTION_SHAPIRO, &CModelingandAnalysisofUncertaintyDoc::OnUpdateSWTest)
	ON_UPDATE_COMMAND_UI(ID_PROBABILITYDISTRIBUTION_ANDERSON, &CModelingandAnalysisofUncertaintyDoc::OnUpdateADTest)
	ON_UPDATE_COMMAND_UI(ID_BASICSTATISTICS_ANOVA, &CModelingandAnalysisofUncertaintyDoc::OnUpdateANOVA)
	ON_UPDATE_COMMAND_UI(ID_PRINCIPALCOMPONENTS_PRINCIPALCOMPONENTMODEL, &CModelingandAnalysisofUncertaintyDoc::OnUpdatePCA)
	ON_UPDATE_COMMAND_UI(ID_PRINCIPALCOMPONENTANALYSIS_FACTORANALYSISMODEL, &CModelingandAnalysisofUncertaintyDoc::OnUpdateFA)
	ON_UPDATE_COMMAND_UI(ID_MULTIVARIATESTATISTICS_INDEPENDENTCOMPONANTANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnUpdateICA)
	ON_UPDATE_COMMAND_UI(ID_MULTIVARIATESTATISTICS_CANONICALCORRELATIONANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnUpdateCCA)
	ON_UPDATE_COMMAND_UI(ID_MULTIVARIATESTATISTICS_LINEARFISHERDISCRIMINANTANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnUpdateFDA)
	ON_UPDATE_COMMAND_UI(ID_MULTIVARIATESTATISTICS_LINEARCLASSIFICATION, &CModelingandAnalysisofUncertaintyDoc::OnUpdateLinearClassification)
	ON_UPDATE_COMMAND_UI(ID_MULTIVARIATESTATISTICS_MULTIVARIATEREGRESSION, &CModelingandAnalysisofUncertaintyDoc::OnUpdateMultivariateRegression)
	ON_UPDATE_COMMAND_UI(ID_MULTIVARIATESTATISTICS_PARTIALLEASTSQUARES, &CModelingandAnalysisofUncertaintyDoc::OnUpdatePLS)
	ON_UPDATE_COMMAND_UI(ID_MACHINELEARNING_KERNELPRINCIPALCOMPONENTANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnUpdateKPCA)
	ON_UPDATE_COMMAND_UI(ID_MACHINELEARNING_LOGISTICREGRESSION, &CModelingandAnalysisofUncertaintyDoc::OnUpdateLR)
	ON_UPDATE_COMMAND_UI(ID_SUPPORTVECTORMACHINES_LINEAR, &CModelingandAnalysisofUncertaintyDoc::OnUpdateSVM)
	ON_UPDATE_COMMAND_UI(ID_SUPPORTVECTORMACHINES_KERNEL, &CModelingandAnalysisofUncertaintyDoc::OnUpdateKSVM)
	ON_UPDATE_COMMAND_UI(ID_MACHINELEARNING_KERNELFISHERDISCRIMINANTANALYSIS, &CModelingandAnalysisofUncertaintyDoc::OnUpdateKSVM)
	ON_UPDATE_COMMAND_UI(ID_REGULARIZATION_L1NORM, &CModelingandAnalysisofUncertaintyDoc::OnUpdateL1_Regularization)
	ON_UPDATE_COMMAND_UI(ID_REGULARIZATION_L2NORM, &CModelingandAnalysisofUncertaintyDoc::OnUpdateL2_Regularization)
	ON_UPDATE_COMMAND_UI(ID_MACHINELEARNING_KERNELPARTIALLEASTSQUARES, &CModelingandAnalysisofUncertaintyDoc::OnUpdateKPLS)
	ON_UPDATE_COMMAND_UI(ID_MACHINELEARNING_ARTIFICIALNEURALNETWORK, &CModelingandAnalysisofUncertaintyDoc::OnUpdateKPLS)
END_MESSAGE_MAP()

// CModelingandAnalysisofUncertaintyDoc construction/destruction
CModelingandAnalysisofUncertaintyDoc::CModelingandAnalysisofUncertaintyDoc() noexcept {
	OutlierArray1.RemoveAll();
	OutlierArray2.RemoveAll();
	show_nObs1 = 0;
	show_nObs2 = 0;
	show_class1 = 0;
	show_class2 = 0;
	show_mean1 = 0.0;
	show_mean2 = 0.0;
	show_std1 = 0.0;
	show_std2 = 0.0;
	show_max2 = 0.0;
	show_max1 = 0.0;
	number = 0;

	show_var_equal_unequal = "";
	show_var_known_unknown = "";
	show_tail = "";
	show_decision = "";
	show_z_statistic = 0.0;
	show_zcrit = 0.0;
	show_p_value = 0.0;
	show_F = 0.0;
	show_Fu = 0.0;
	show_Fl = 0.0;
	show_T = 0.0;
	show_SP = 0.0;
	show_delta_x = 0.0;
	show_critical_U = 0.0;
	show_critical_L = 0.0;
	show_confidence_U = 0.0;
	show_confidence_L = 0.0;
	number = 0;
	show_freedom = 0;

	Data.RemoveAll();
	Data_ordered.RemoveAll();
	Data_spec.RemoveAll();
	DateFirst.Empty();
	TimeFirst.Empty();
	DateLast.Empty();
	TimeLast.Empty();
	Application.Empty();
	Tag.RemoveAll();
	Unit.RemoveAll();
	Name.RemoveAll();
	MIN.RemoveAll();
	MAX.RemoveAll();
	Q1.RemoveAll();
	Q2.RemoveAll();
	Q3.RemoveAll();
	IQR.RemoveAll();
	RANGE.RemoveAll();
	MEAN.RemoveAll();
	STD.RemoveAll();
	SE.RemoveAll();
	SKEW.RemoveAll();
	KURT.RemoveAll();
	h.RemoveAll();
	OUTLIERS.RemoveAll();
	n_OUTLIERS.RemoveAll();
	UB.RemoveAll();
	LB.RemoveAll();
	P.RemoveAll();
	T.RemoveAll();
	PCs.RemoveAll();
	PCs.SetSize(4);
	PCs.SetAt(0, 1);
	PCs.SetAt(1, 2);
	PCs.SetAt(2, 3);
	PCs.SetAt(3, 4);
	w.RemoveAll();
	w_raw.RemoveAll();
	t_alpha = 0;
}
CModelingandAnalysisofUncertaintyDoc::~CModelingandAnalysisofUncertaintyDoc() {
	OutlierArray1.RemoveAll();
	OutlierArray2.RemoveAll();
	show_nObs1 = 0;
	show_nObs2 = 0;
	show_class1 = 0;
	show_class2 = 0;
	show_mean1 = 0.0;
	show_mean2 = 0.0;
	show_std1 = 0.0;
	show_std2 = 0.0;
	show_max2 = 0.0;
	show_max1 = 0.0;
	show_F = 0.0;
	show_Fu = 0.0;
	show_Fl = 0.0;
	show_delta_x = 0.0;

	show_var_equal_unequal = "";
	show_var_known_unknown = "";
	show_tail = "";
	show_decision = "";
	show_z_statistic = 0.0;
	show_zcrit = 0.0;
	show_p_value = 0.0;
	show_T = 0.0;
	show_SP = 0.0;
	show_critical_U = 0.0;
	show_critical_L = 0.0;
	show_confidence_U = 0.0;
	show_confidence_L = 0.0;
	show_freedom = 0;

	Data.RemoveAll();
	Data_ordered.RemoveAll();
	Data_spec.RemoveAll();
	DateFirst.Empty();
	TimeFirst.Empty();
	DateLast.Empty();
	TimeLast.Empty();
	Application.Empty();
	Tag.RemoveAll();
	Unit.RemoveAll();
	Name.RemoveAll();
	MIN.RemoveAll();
	MAX.RemoveAll();
	Q1.RemoveAll();
	Q2.RemoveAll();
	Q3.RemoveAll();
	IQR.RemoveAll();
	RANGE.RemoveAll();
	MEAN.RemoveAll();
	STD.RemoveAll();
	SE.RemoveAll();
	SKEW.RemoveAll();
	KURT.RemoveAll();
	h.RemoveAll();
	OUTLIERS.RemoveAll();
	n_OUTLIERS.RemoveAll();
	UB.RemoveAll();
	LB.RemoveAll();
	P.RemoveAll();
	T.RemoveAll();
	w.RemoveAll();
	w_raw.RemoveAll();
}

//*****************************************************************
//***   BEGIN    ->    AUXILLIARY  FUNCTIONS  AND  OPERATIONS   ***
//*****************************************************************

// Computes sqrt ( a^2 + b^2 )
double CModelingandAnalysisofUncertaintyDoc::PYTHAG(double a, double b) {
	double absa = 0, absb = 0;
	absa = fabs(a);
	absb = fabs(b);
	if (absa > absb) return absa * sqrt((double)1 + SQR(absb / absa));
	else if (absb == 0) return (double)0;
	else return absb * sqrt((double)1 + SQR(absa / absb));
}

// Swaps two elements x -> y and y -> x
void CModelingandAnalysisofUncertaintyDoc::SwapElements(double& x, double& y) {
	double temp = x;
	x = y;
	y = temp;
}

// Rearranges the elements in an array -> needed for the quick sort algorithm
int CModelingandAnalysisofUncertaintyDoc::Partition(CArray <double>& a, int LowIndex, int HighIndex) {
	double PivotElement = a.GetAt(HighIndex), temp1 = 0, temp2 = 0;
	int i = (LowIndex - 1);
	for (int j = LowIndex; j < HighIndex; j++) {
		if (a.GetAt(j) <= PivotElement) {
			i++;
			temp1 = a.GetAt(i);
			temp2 = a.GetAt(j);
			SwapElements(temp1, temp2);
			a.SetAt(i, temp1);
			a.SetAt(j, temp2);
		}
	}
	temp1 = a.GetAt(static_cast<int64_t>(i) + 1);
	temp2 = a.GetAt(HighIndex);
	SwapElements(temp1, temp2);
	a.SetAt(static_cast<int64_t>(i) + 1, temp1);
	a.SetAt(HighIndex, temp2);
	return (i + 1);
}

// Quick sort algorithm to sort elements from largest to smallest
void CModelingandAnalysisofUncertaintyDoc::QuickSort(CArray <double>& a, int LowIndex, int HighIndex) {
	if (LowIndex < HighIndex) {
		int pivot = Partition(a, LowIndex, HighIndex);
		QuickSort(a, LowIndex, pivot - 1);
		QuickSort(a, pivot + 1, HighIndex);
	}
}

void CModelingandAnalysisofUncertaintyDoc::SaveMatrix(std::string filename, CArray <double>& A, CArray <int>& A_spec) {
	std::ofstream FILE;
	FILE.open(filename);
	if (FILE.is_open()) {
		if (A_spec.GetAt(2) == 2) {
			for (int i = 0; i < A_spec.GetAt(0); i++) {
				for (int j = 0; j <= i; j++) {
					FILE << A.GetAt(GetPosition(i, j, A_spec)) << "\t";
				}
				FILE << std::endl;
			}

		}
		else if (A_spec.GetAt(2) == 3) {
			for (int i = 0; i < A_spec.GetAt(0); i++) {
				for (int j = i; j < A_spec.GetAt(1); j++) {
					FILE << A.GetAt(GetPosition(i, j, A_spec)) << "\t";
				}
				FILE << std::endl;
			}
		}
		else {
			for (int i = 0; i < A_spec.GetAt(0); i++) {
				for (int j = 0; j < A_spec.GetAt(1); j++) {
					FILE << A.GetAt(GetPosition(i, j, A_spec)) << "\t";
				}
				FILE << std::endl;
			}
		}
	}
	FILE.close();
}

// Save a matrix to file 'filename'
void CModelingandAnalysisofUncertaintyDoc::SaveVector(std::string filename, CArray <double>& a) {
	std::ofstream FILE;
	FILE.open(filename);
	if (FILE.is_open()) {
		for (int i = 0; i < a.GetSize(); i++) FILE << a.GetAt(i) << std::endl;
	}
	FILE.close();
}

//*****************************************************************
//***   BEGIN   ->  STANDARD  MATRIX  AND  VECTOR  OPERATIONS   ***
//*****************************************************************

// Scales the vector : vector = vector / factor
void CModelingandAnalysisofUncertaintyDoc::ScaleVector(CArray <double>& vector, double factor) {
	for (int i = 0; i < vector.GetSize(); i++) vector.SetAt(i, vector.GetAt(i) / factor);
}

// Scales the vector 'vector' to unit length
void CModelingandAnalysisofUncertaintyDoc::NormalizeVector(CArray <double>& vector) {
	int dim = (int)vector.GetSize();
	double temp = GetSquaredLength(vector);
	for (int i = 0; i < dim; i++) vector.SetAt(i, vector.GetAt(i) / temp);
}

double CModelingandAnalysisofUncertaintyDoc::GetSquaredLength(CArray <double>& vector) {
	int dim = (int)vector.GetSize();
	double temp = (double)0;
	for (int i = 0; i < dim; i++) temp += SQR(vector.GetAt(i));
	return temp;
}

double CModelingandAnalysisofUncertaintyDoc::GetLength(CArray <double>& vector) {
	double temp = GetSquaredLength(vector);
	return sqrt(temp);
}

void CModelingandAnalysisofUncertaintyDoc::CenterVector(CArray <double>& vector) {
	double mean = (double)0;
	for (int i = 0; i < vector.GetSize(); i++) mean += vector.GetAt(i);
	mean /= (double)(vector.GetSize());
	for (int i = 0; i < vector.GetSize(); i++) vector.SetAt(i, vector.GetAt(i) - mean);
}

void CModelingandAnalysisofUncertaintyDoc::GetLargestElement(CArray <double>& A, CArray <int>& A_spec, double& max) {
	max = A.GetAt(0);
	int row = A_spec.GetAt(0), col = A_spec.GetAt(1);
	double value;
	if (A_spec.GetAt(2) == 0) {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				value = fabs(A.GetAt(GetPosition(i, j, A_spec)));
				if (value > max) max = value;
			}
		}
	}
	else if ((A_spec.GetAt(1)) || (A_spec.GetAt(2))) {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j <= i; j++) {
				value = fabs(A.GetAt(GetPosition(i, j, A_spec)));
				if (value > max) max = value;
			}
		}
	}
	else {
		for (int i = 0; i < row; i++) {
			for (int j = i; j < col; j++) {
				value = fabs(A.GetAt(GetPosition(i, j, A_spec)));
				if (value > max) max = value;
			}
		}
	}
}

// Determines where a particular element is stored in a matrix
int CModelingandAnalysisofUncertaintyDoc::GetPosition(int row, int col, CArray <int>& Spec) {
	// ************************************
	// *** Convention :                 ***
	// *** 0 -> not a specific matrix   ***
	// *** 1 -> symmetric matrix        ***
	// *** 2 -> lower triangular matrix ***
	// *** 3 -> upper triangular matrix ***
	// ************************************
	int Element = 0, n_rows = Spec.GetAt(0);
	if (Spec.GetAt(2) == 0)
		return col * Spec.GetAt(0) + row;
	else if (Spec.GetAt(2) == 1) {
		if (row >= col) {
			if (col > 0) Element = col * n_rows - (col * (col - 1) / 2 + 1) + row - col + 1;
			else Element = row;
		}
		else Element = row * n_rows - (row * (row - 1) / 2 + 1) + col - row + 1;
	}
	else if (Spec.GetAt(2) == 2) {
		if (row >= col) {
			if (col > 0) Element = col * n_rows - (col * (col - 1) / 2 + 1) + row - col + 1;
			else Element = row;
		}
	}
	else if (Spec.GetAt(2) == 3) {
		if (col >= row) {
			if (row > 0) Element = row * n_rows - (row * (row - 1) / 2 + 1) + col - row + 1;
			else Element = col;
		}
	}
	else return 0;
	return Element;
}

// Grab the row stored at 'row' from the matrix A
void CModelingandAnalysisofUncertaintyDoc::GetRow(CArray <double>& A, CArray <int>& A_spec, CArray <double>& a, int row) {
	if (row < A_spec.GetAt(0)) {
		int dim = A_spec.GetAt(1), pos;
		a.SetSize(dim);
		for (int i = 0; i < dim; i++) {
			pos = GetPosition(row, i, A_spec);
			a.SetAt(i, A.GetAt(pos));
		}
	}
}

// Grab the column stored at 'col' from the matrix 'A'
void CModelingandAnalysisofUncertaintyDoc::GetColumn(CArray <double>& A, CArray <int>& A_spec, CArray <double>& a, int col) {
	if (col < A_spec.GetAt(1)) {
		int dim = A_spec.GetAt(0), pos;
		a.SetSize(dim);
		for (int i = 0; i < dim; i++) {
			pos = GetPosition(i, col, A_spec);
			a.SetAt(i, A.GetAt(pos));
		}
	}
}

// Insert the row 'a' into the matrix 'A' at the row 'row'
void CModelingandAnalysisofUncertaintyDoc::InsertRow(CArray <double>& A, CArray <int>& A_spec, CArray <double>& a, int row) {
	if (A_spec.GetAt(1) == a.GetSize()) {
		int pos_A = 0;
		for (int i = 0; i < a.GetSize(); i++) {
			pos_A = GetPosition(row, i, A_spec);
			A.SetAt(pos_A, a.GetAt(i));
		}
	}
}

// Insert the column 'a' into the matrix 'A' at the column 'col'
void CModelingandAnalysisofUncertaintyDoc::InsertColumn(CArray <double>& A, CArray <int>& A_spec, CArray <double>& a, int col) {
	if (A_spec.GetAt(0) == a.GetSize()) {
		int pos;
		for (int i = 0; i < a.GetSize(); i++) {
			pos = GetPosition(i, col, A_spec);
			A.SetAt(pos, a.GetAt(i));
		}
	}
}

// Computing determinant using LU decomposition
double CModelingandAnalysisofUncertaintyDoc::Determinant(CArray <double>& A, CArray <int>& A_spec) {
	double det = (double)1, TINY = 1e-20;
	int row = A_spec.GetAt(0);
	if ((A_spec.GetAt(2) == 0) || (A_spec.GetAt(2) == 1)) {
		int pos_1, pos_2;
		CArray <double> Temp;
		CArray <int> Temp_spec;
		Temp_spec.SetSize(3);
		Temp_spec.SetAt(0, row);
		Temp_spec.SetAt(1, row);
		Temp_spec.SetAt(1, 0);
		if (A_spec.GetAt(2) == 0) Temp.SetSize(A.GetSize());
		else {
			int dim = row;
			int64_t space = static_cast<int64_t>(row) * dim;
			Temp.SetSize(space);
		}
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < row; j++) {
				pos_1 = GetPosition(i, j, A_spec);
				pos_2 = GetPosition(i, j, Temp_spec);
				Temp.SetAt(pos_2, A.GetAt(pos_1));
			}
		}
		CArray <double> vv;
		vv.SetSize(row);
		int i, imax = 0, j, k;
		double big, dum, sum, temp;
		for (i = 1; i <= row; i++) {
			big = 0.0;
			for (j = 1; j <= row; j++) {
				pos_1 = GetPosition(i - 1, j - 1, Temp_spec);
				if ((temp = fabs(Temp.GetAt(pos_1))) > big) big = temp;
			}
			if (big != 0.0) vv.SetAt(static_cast<int64_t>(i) - 1, (double)1 / big);
		}
		for (j = 1; j <= row; j++) {
			for (i = 1; i < j; i++) {
				pos_1 = GetPosition(i - 1, j - 1, Temp_spec);
				sum = Temp.GetAt(pos_1);
				for (k = 1; k < i; k++) {
					pos_1 = GetPosition(i - 1, k - 1, Temp_spec);
					pos_2 = GetPosition(k - 1, j - 1, Temp_spec);
					sum -= Temp.GetAt(pos_1) * Temp.GetAt(pos_2);
				}
				pos_1 = GetPosition(i - 1, j - 1, Temp_spec);
				Temp.SetAt(pos_1, sum);
			}
			big = 0.0;
			for (i = j; i <= row; i++) {
				pos_1 = GetPosition(i - 1, j - 1, Temp_spec);
				sum = Temp.GetAt(pos_1);
				for (k = 1; k < j; k++) {
					pos_1 = GetPosition(i - 1, k - 1, Temp_spec);
					pos_2 = GetPosition(k - 1, j - 1, Temp_spec);
					sum -= Temp.GetAt(pos_1) * Temp.GetAt(pos_2);
				}
				pos_1 = GetPosition(i - 1, j - 1, Temp_spec);
				Temp.SetAt(pos_1, sum);
				if ((dum = vv.GetAt(static_cast<int64_t>(i) - 1) * fabs(sum)) >= big) {
					big = dum;
					imax = i;
				}
			}
			if (j != imax) {
				for (k = 1; k <= row; k++) {
					pos_1 = GetPosition(imax - 1, k - 1, Temp_spec);
					pos_2 = GetPosition(j - 1, k - 1, Temp_spec);
					dum = Temp.GetAt(pos_1);
					Temp.SetAt(pos_1, Temp.GetAt(pos_2));
					Temp.SetAt(pos_2, dum);
				}
				det = -det;
				vv.SetAt(static_cast<int64_t>(imax) - 1, vv.GetAt(static_cast<int64_t>(j) - 1));
			}
			pos_1 = GetPosition(j - 1, j - 1, Temp_spec);
			if (Temp.GetAt(pos_1) == 0.0) {
				Temp.SetAt(pos_1, TINY);
			}
			if (j != row) {
				dum = (double)1 / Temp.GetAt(pos_1);
				for (i = j + 1; i <= row; i++) {
					pos_1 = GetPosition(i - 1, j - 1, Temp_spec);
					Temp.SetAt(pos_1, Temp.GetAt(pos_1) * dum);
				}
			}
		}
		for (i = 0; i < row; i++) {
			pos_1 = GetPosition(i, i, Temp_spec);
			det *= Temp.GetAt(pos_1);
		}
		if (fabs(det) < 1e-14) det = (double)0;
	}
	else {
		int pos;
		for (int i = 0; i < row; i++) {
			pos = GetPosition(i, i, A_spec);
			det *= A.GetAt(pos);
		}
	}
	return det;
}

// Determining transpose of a matrix A -> A'
void CModelingandAnalysisofUncertaintyDoc::Transpose(CArray <double>& A, CArray <int>& A_spec, CArray <double>& Atrans, CArray <int>& Atrans_spec) {
	int row = A_spec.GetAt(0), col = A_spec.GetAt(1), pos_1, pos_2;
	Atrans.RemoveAll(), Atrans_spec.RemoveAll();
	Atrans.SetSize(A.GetSize()), Atrans_spec.SetSize(3);
	if (A_spec.GetAt(2) == 1) {
		Atrans_spec.SetAt(0, col), Atrans_spec.SetAt(1, row), Atrans_spec.SetAt(2, 1);
		for (int i = 0; i < row; i++) {
			for (int j = 0; j <= i; j++) {
				pos_1 = GetPosition(i, j, A_spec);
				Atrans.SetAt(pos_1, A.GetAt(pos_1));
			}
		}
	}
	else if (A_spec.GetAt(2) == 2) {
		Atrans_spec.SetAt(0, col), Atrans_spec.SetAt(1, row), Atrans_spec.SetAt(2, 3);
		for (int i = 0; i < row; i++) {
			for (int j = 0; j <= i; j++) {
				pos_1 = GetPosition(j, i, Atrans_spec);
				pos_2 = GetPosition(i, j, A_spec);
				Atrans.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
	}
	else if (A_spec.GetAt(2) == 3) {
		Atrans_spec.SetAt(0, col), Atrans_spec.SetAt(1, row), Atrans_spec.SetAt(2, 2);
		for (int i = 0; i < row; i++) {
			for (int j = i; j < col; j++) {
				pos_1 = GetPosition(j, i, Atrans_spec);
				pos_2 = GetPosition(i, j, A_spec);
				Atrans.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
	}
	else {
		Atrans_spec.SetAt(0, col), Atrans_spec.SetAt(1, row), Atrans_spec.SetAt(2, 0);
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				pos_1 = GetPosition(j, i, A_spec);
				pos_2 = GetPosition(i, j, A_spec);
				Atrans.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
	}
}

// Computing inverse of a matrix : inv(A)
void CModelingandAnalysisofUncertaintyDoc::Inverse(CArray <double>& A, CArray <int>& A_spec, CArray <double>& invA, CArray <int>& invA_spec) {
	if (A_spec.GetAt(0) == A_spec.GetAt(1)) {
		CArray <double> C;
		int dim = 2 * A_spec.GetAt(1);
		int64_t space = static_cast<int64_t>(A_spec.GetAt(0)) * dim;
		C.SetSize(space);
		CArray <int> C_spec;
		C_spec.SetSize(3);
		C_spec.SetAt(0, A_spec.GetAt(0));
		C_spec.SetAt(1, 2 * A_spec.GetAt(0));
		C_spec.SetAt(2, 0);
		int row = A_spec.GetAt(0);
		int col = A_spec.GetAt(1);
		int pos_C, pos_A;
		if ((A_spec.GetAt(2) == 0) || (A_spec.GetAt(2) == 1)) {
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					pos_A = GetPosition(i, j, A_spec);
					pos_C = GetPosition(i, j, C_spec);
					C.SetAt(pos_C, A.GetAt(pos_A));
				}
				for (int j = 0; j < col; j++) {
					pos_C = GetPosition(i, j + col, C_spec);
					if (i == j) C.SetAt(pos_C, (double)1);
					else C.SetAt(pos_C, (double)0);
				}
			}
			for (int i = 0; i < row; i++) {
				SetUpPivot(C, C_spec, i);
				for (int j = i + 1; j < row; j++) ManipulateRowForwardPath(C, C_spec, j, i);
			}
			for (int i = row - 1; i > 0; i--) {
				for (int j = i - 1; j >= 0; j--) ManipulateRowBackwardPath(C, C_spec, j, i);
			}
			invA.SetSize(A.GetSize());
			invA_spec.SetSize(3);
			invA_spec.SetAt(0, row);
			invA_spec.SetAt(1, row);
			if (A_spec.GetAt(2) == 0) {
				invA_spec.SetAt(2, 0);
				for (int i = 0; i < row; i++) {
					for (int j = 0; j < row; j++) {
						pos_C = GetPosition(i, row + j, C_spec);
						invA.SetAt(GetPosition(i, j, invA_spec), C.GetAt(pos_C));
					}
				}
			}
			else {
				invA_spec.SetAt(2, 1);
				for (int i = 0; i < row; i++) {
					for (int j = 0; j <= i; j++) {
						pos_C = GetPosition(i, row + j, C_spec);
						invA.SetAt(GetPosition(i, j, invA_spec), C.GetAt(pos_C));
					}
				}
			}
		}
		else if (A_spec.GetAt(2) == 2) {
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (j <= i) {
						pos_C = GetPosition(i, j, C_spec);
						pos_A = GetPosition(i, j, A_spec);
						C.SetAt(pos_C, A.GetAt(pos_A));
					}
					else {
						pos_C = GetPosition(i, j, C_spec);
						C.SetAt(pos_C, (double)0);
					}
				}
				for (int j = 0; j < col; j++) {
					pos_C = GetPosition(i, j + col, C_spec);
					if (i == j) C.SetAt(pos_C, (double)1);
					else C.SetAt(pos_C, (double)0);
				}
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, i, C_spec);
				double Cii = C.GetAt(pos_C);
				C.SetAt(pos_C, (double)1);
				for (int j = row; j <= row + i; j++) {
					pos_C = GetPosition(i, j, C_spec);
					C.SetAt(pos_C, C.GetAt(pos_C) / Cii);
				}
				for (int j = i + 1; j < row; j++) ManipulateRowForwardPath(C, C_spec, j, i);
			}
			invA.SetSize(A.GetSize());
			invA_spec.SetSize(3);
			invA_spec.SetAt(0, row);
			invA_spec.SetAt(1, row);
			invA_spec.SetAt(2, 2);
			for (int i = 0; i < row; i++) {
				for (int j = 0; j <= i; j++) {
					pos_C = GetPosition(i, row + j, C_spec);
					invA.SetAt(GetPosition(i, j, invA_spec), C.GetAt(pos_C));
				}
			}
		}
		else {
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (j >= i) {
						pos_C = GetPosition(i, j, C_spec);
						pos_A = GetPosition(i, j, A_spec);
						C.SetAt(pos_C, A.GetAt(pos_A));
					}
					else {
						pos_C = GetPosition(i, j, C_spec);
						C.SetAt(pos_C, (double)0);
					}
				}
				for (int j = 0; j < col; j++) {
					pos_C = GetPosition(i, j + col, C_spec);
					if (i == j) C.SetAt(pos_C, (double)1);
					else C.SetAt(pos_C, (double)0);
				}
			}
			for (int i = row - 1; i >= 0; i--) {
				pos_C = GetPosition(i, i, C_spec);
				double Cii = C.GetAt(pos_C);
				C.SetAt(pos_C, (double)1);
				for (int j = i; j < row; j++) {
					pos_C = GetPosition(i, j + row, C_spec);
					C.SetAt(pos_C, C.GetAt(pos_C) / Cii);
				}
				for (int j = i - 1; j >= 0; j--) ManipulateRowBackwardPath(C, C_spec, j, i);
			}
			invA.SetSize(A.GetSize());
			invA_spec.SetSize(3);
			invA_spec.SetAt(0, row);
			invA_spec.SetAt(1, row);
			invA_spec.SetAt(2, 3);
			for (int i = 0; i < row; i++) {
				for (int j = i; j < row; j++) {
					pos_C = GetPosition(i, row + j, C_spec);
					invA.SetAt(GetPosition(i, j, invA_spec), C.GetAt(pos_C));
				}
			}
		}
	}
}

// Adding two matrices : A + B = C
void CModelingandAnalysisofUncertaintyDoc::AddingMatrices(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if ((A_spec.GetAt(0) == B_spec.GetAt(0)) && (A_spec.GetAt(1) == B_spec.GetAt(1))) {
		int row = A_spec.GetAt(0), col = A_spec.GetAt(1);
		double value_1, value_2;
		C_spec.SetSize(3), C_spec.SetAt(0, row), C_spec.SetAt(1, col);
		if (A_spec.GetAt(2) == B_spec.GetAt(2)) C_spec.SetAt(2, A_spec.GetAt(2));
		if (A_spec.GetAt(2) == 0) C.SetSize(static_cast <int64_t>(row * col));
		else C.SetSize(static_cast <int64_t>(row * (row + 1) / 2));
		if ((A_spec.GetAt(2) == 1) || (A_spec.GetAt(2) == 2)) {
			for (int i = 0; i < row; i++) {
				for (int j = 0; j <= i; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 + value_2);
				}
			}
		}
		else if (A_spec.GetAt(2) == 3) {
			for (int i = 0; i < row; i++) {
				for (int j = i; j < col; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 + value_2);
				}
			}
		}
		else {
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 + value_2);
				}
			}
		}
	}
}

// Subtracting two matrices : A - B = C
void CModelingandAnalysisofUncertaintyDoc::SubtractingMatrices(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if ((A_spec.GetAt(0) == B_spec.GetAt(0)) && (A_spec.GetAt(1) == B_spec.GetAt(1))) {
		int row = A_spec.GetAt(0), col = A_spec.GetAt(1);
		double value_1, value_2;
		C_spec.SetSize(3), C_spec.SetAt(0, row), C_spec.SetAt(1, col);
		if (A_spec.GetAt(2) == B_spec.GetAt(2)) C_spec.SetAt(2, A_spec.GetAt(2));
		if (A_spec.GetAt(2) == 0) C.SetSize(static_cast <int64_t>(row * col));
		else C.SetSize(static_cast <int64_t>(row * (row + 1) / 2));
		if ((A_spec.GetAt(2) == 1) || (A_spec.GetAt(2) == 2)) {
			for (int i = 0; i < row; i++) {
				for (int j = 0; j <= i; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 - value_2);
				}
			}
		}
		else if (A_spec.GetAt(2) == 3) {
			for (int i = 0; i < row; i++) {
				for (int j = i; j < col; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 - value_2);
				}
			}
		}
		else {
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 - value_2);
				}
			}
		}
	}
}

// Computing matrix-vector product :  y = A � x 
void CModelingandAnalysisofUncertaintyDoc::MatrixVectorProduct(CArray <double>& A, CArray <int>& A_spec, CArray <double>& x, CArray <double>& y) {
	if (x.GetSize() == A_spec.GetAt(1)) {
		y.SetSize(A_spec.GetAt(0));
		int pos_A;
		double temp;
		if ((A_spec.GetAt(2) == 0) || (A_spec.GetAt(2) == 1)) {
			for (int i = 0; i < A_spec.GetAt(0); i++) {
				temp = 0;
				for (int j = 0; j < A_spec.GetAt(1); j++) {
					pos_A = GetPosition(i, j, A_spec);
					temp += A.GetAt(pos_A) * x.GetAt(j);
				}
				y.SetAt(i, temp);
			}
		}
		else if (A_spec.GetAt(2) == 2) {
			for (int i = 0; i < A_spec.GetAt(0); i++) {
				temp = 0;
				for (int j = 0; j <= i; j++) {
					pos_A = GetPosition(i, j, A_spec);
					temp += A.GetAt(pos_A) * x.GetAt(j);
				}
				y.SetAt(i, temp);
			}
		}
		else if (A_spec.GetAt(2) == 3) {
			for (int i = 0; i < A_spec.GetAt(0); i++) {
				temp = 0;
				for (int j = i; j < A_spec.GetAt(1); j++) {
					pos_A = GetPosition(i, j, A_spec);
					temp += A.GetAt(pos_A) * x.GetAt(j);
				}
				y.SetAt(i, temp);
			}
		}
	}
}

// Computing matrix product : C = A � B 
void CModelingandAnalysisofUncertaintyDoc::MatrixProduct(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if (A_spec.GetAt(1) == B_spec.GetAt(0)) {
		int row = A_spec.GetAt(0);
		int col = B_spec.GetAt(1);
		C_spec.SetSize(3);
		C_spec.SetAt(0, row);
		C_spec.SetAt(1, col);
		C_spec.SetAt(2, 0);
		int dim = B_spec.GetAt(1);
		int64_t space = static_cast<int64_t>(A_spec.GetAt(0)) * dim;
		C.SetSize(space);
		int pos_A, pos_B, pos_C;
		double temp;
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				temp = 0;
				for (int k = 0; k < A_spec.GetAt(1); k++) {
					pos_A = GetPosition(i, k, A_spec);
					pos_B = GetPosition(k, j, B_spec);
					temp += A.GetAt(pos_A) * B.GetAt(pos_B);
				}
				pos_C = GetPosition(i, j, C_spec);
				C.SetAt(pos_C, temp);
			}
		}
	}
}

// Computing matrix product : X_tr_X = X' � X   
void CModelingandAnalysisofUncertaintyDoc::X_tr_X(CArray <double>& X, CArray <int>& X_spec, CArray <double>& X_tr_X, CArray <int>& X_tr_X_spec) {
	int dim = X_spec.GetAt(1) + 1, pos_X1, pos_X2;
	int64_t space = static_cast<int64_t>(X_spec.GetAt(1)) * dim;
	X_tr_X.SetSize((int)(space / 2));
	X_tr_X_spec.SetSize(3);
	X_tr_X_spec.SetAt(0, X_spec.GetAt(1));
	X_tr_X_spec.SetAt(1, X_spec.GetAt(1));
	X_tr_X_spec.SetAt(2, 1);
	double temp;
	for (int i = 0; i < X_spec.GetAt(1); i++) {
		for (int j = 0; j <= i; j++) {
			temp = (double)0;
			for (int k = 0; k < X_spec.GetAt(0); k++) {
				pos_X1 = GetPosition(k, i, X_spec);
				pos_X2 = GetPosition(k, j, X_spec);
				temp += X.GetAt(pos_X1) * X.GetAt(pos_X2);
			}
			X_tr_X.SetAt(GetPosition(i, j, X_tr_X_spec), temp);
		}
	}
}

// Computing matrix product : X_tr_Y = X' � Y        
void CModelingandAnalysisofUncertaintyDoc::X_tr_Y(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if (A_spec.GetAt(0) == B_spec.GetAt(0)) {
		int dim = A_spec.GetAt(1), pos_A, pos_B;
		int64_t space = static_cast<int64_t>(B_spec.GetAt(1)) * dim;
		C.SetSize(space);
		C_spec.SetSize(3);
		C_spec.SetAt(0, A_spec.GetAt(1));
		C_spec.SetAt(1, B_spec.GetAt(1));
		C_spec.SetAt(2, 0);
		double temp;
		for (int i = 0; i < A_spec.GetAt(1); i++) {
			for (int j = 0; j < B_spec.GetAt(1); j++) {
				temp = (double)0;
				for (int k = 0; k < A_spec.GetAt(0); k++) {
					pos_A = GetPosition(k, i, A_spec);
					pos_B = GetPosition(k, j, B_spec);
					temp += A.GetAt(pos_A) * B.GetAt(pos_B);
				}
				C.SetAt(GetPosition(i, j, C_spec), temp);
			}
		}
	}
}

// Computing matrix product : X_Y_tr = X � Y'
void CModelingandAnalysisofUncertaintyDoc::X_Y_tr(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if (A_spec.GetAt(1) == B_spec.GetAt(1)) {
		int dim = A_spec.GetAt(0), pos_A, pos_B;
		int64_t space = static_cast <int64_t>(B_spec.GetAt(0)) * dim;
		C.SetSize(space);
		C_spec.SetSize(3);
		C_spec.SetAt(0, A_spec.GetAt(0));
		C_spec.SetAt(1, B_spec.GetAt(0));
		C_spec.SetAt(2, 0);
		double temp;
		for (int i = 0; i < A_spec.GetAt(0); i++) {
			for (int j = 0; j < B_spec.GetAt(0); j++) {
				temp = (double)0;
				for (int k = 0; k < A_spec.GetAt(1); k++) {
					pos_A = GetPosition(i, k, A_spec);
					pos_B = GetPosition(j, k, B_spec);
					temp += A.GetAt(pos_A) * B.GetAt(pos_B);
				}
				C.SetAt(GetPosition(i, j, C_spec), temp);
			}
		}
	}
}

// Computing matrix product : X_X_tr = X � X'
void CModelingandAnalysisofUncertaintyDoc::X_X_tr(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec) {
	int col = A_spec.GetAt(1), row = A_spec.GetAt(0);
	double temp, value_1, value_2;
	B.RemoveAll(), B_spec.RemoveAll(), B_spec.SetSize(3);
	B_spec.SetAt(0, row), B_spec.SetAt(1, row), B_spec.SetAt(2, 1);
	B.SetSize(static_cast <int64_t>(row * (row + 1) / 2));
	for (int i = 0; i < row; i++) {
		for (int j = 0; j <= i; j++) {
			temp = 0.0;
			for (int k = 0; k < col; k++) {
				value_1 = A.GetAt(GetPosition(i, k, A_spec));
				value_2 = A.GetAt(GetPosition(j, k, A_spec));
				temp += value_1 * value_2;
			}
			B.SetAt(GetPosition(i, j, B_spec), temp);
		}
	}
}

// Determining transpose of a matrix A -> A' using multithreading
void CModelingandAnalysisofUncertaintyDoc::TransposeParallel(CArray <double>& A, CArray <int>& A_spec, CArray <double>& Atrans, CArray <int>& Atrans_spec) {
	int row = A_spec.GetAt(0), col = A_spec.GetAt(1), pos_1, pos_2;
	Atrans.RemoveAll(), Atrans_spec.RemoveAll();
	Atrans.SetSize(A.GetSize()), Atrans_spec.SetSize(3);

	if (A_spec.GetAt(2) == 1) {
		Atrans_spec.SetAt(0, col), Atrans_spec.SetAt(1, row), Atrans_spec.SetAt(2, 1);
		#pragma omp parallel for private(pos_1) collapse(2)
		for (int i = 0; i < row; i++) {
			for (int j = 0; j <= i; j++) {
				pos_1 = GetPosition(i, j, A_spec);
				Atrans.SetAt(pos_1, A.GetAt(pos_1));
			}
		}
	}
	else if (A_spec.GetAt(2) == 2) {
		Atrans_spec.SetAt(0, col), Atrans_spec.SetAt(1, row), Atrans_spec.SetAt(2, 3);
		#pragma omp parallel for private(pos_1, pos_2) collapse(2)
		for (int i = 0; i < row; i++) {
			for (int j = 0; j <= i; j++) {
				pos_1 = GetPosition(j, i, Atrans_spec);
				pos_2 = GetPosition(i, j, A_spec);
				Atrans.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
	}
	else if (A_spec.GetAt(2) == 3) {
		Atrans_spec.SetAt(0, col), Atrans_spec.SetAt(1, row), Atrans_spec.SetAt(2, 2);
		#pragma omp parallel for private(pos_1, pos_2) collapse(2)
		for (int i = 0; i < row; i++) {
			for (int j = i; j < col; j++) {
				pos_1 = GetPosition(j, i, Atrans_spec);
				pos_2 = GetPosition(i, j, A_spec);
				Atrans.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
	}
	else {
		Atrans_spec.SetAt(0, col), Atrans_spec.SetAt(1, row), Atrans_spec.SetAt(2, 0);
		#pragma omp parallel for private(pos_1, pos_2) collapse(2)
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				pos_1 = GetPosition(j, i, A_spec);
				pos_2 = GetPosition(i, j, A_spec);
				Atrans.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
	}
}


// Adding two matrices using multithreading : A + B = C
void CModelingandAnalysisofUncertaintyDoc::AddingMatricesParallel(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if ((A_spec.GetAt(0) == B_spec.GetAt(0)) && (A_spec.GetAt(1) == B_spec.GetAt(1))) {
		int row = A_spec.GetAt(0), col = A_spec.GetAt(1);
		double value_1, value_2;
		C_spec.SetSize(3), C_spec.SetAt(0, row), C_spec.SetAt(1, col);
		if (A_spec.GetAt(2) == B_spec.GetAt(2)) C_spec.SetAt(2, A_spec.GetAt(2));
		if (A_spec.GetAt(2) == 0) C.SetSize(static_cast <int64_t>(row * col));
		else C.SetSize(static_cast <int64_t>(row * (row + 1) / 2));
		if ((A_spec.GetAt(2) == 1) || (A_spec.GetAt(2) == 2)) {
			#pragma omp parallel for private(value_1, value_2) collapse(2)
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j <= i; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 + value_2);
				}
			}
		}
		else if (A_spec.GetAt(2) == 3) {
			#pragma omp parallel for private(value_1, value_2) collapse(2)
			for (int i = 0; i < row; i++)
			{
				for (int j = i; j < col; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 + value_2);
				}
			}
		}
		else {
			#pragma omp parallel for private(value_1, value_2) collapse(2)
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 + value_2);
				}
			}
		}
	}
}

// Subtracting two matrices using multithreading : A - B = C
void CModelingandAnalysisofUncertaintyDoc::SubtractingMatricesParallel(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if ((A_spec.GetAt(0) == B_spec.GetAt(0)) && (A_spec.GetAt(1) == B_spec.GetAt(1))) {
		int row = A_spec.GetAt(0), col = A_spec.GetAt(1);
		double value_1, value_2;
		C_spec.SetSize(3), C_spec.SetAt(0, row), C_spec.SetAt(1, col);
		if (A_spec.GetAt(2) == B_spec.GetAt(2)) C_spec.SetAt(2, A_spec.GetAt(2));
		if (A_spec.GetAt(2) == 0) C.SetSize(static_cast <int64_t>(row * col));
		else C.SetSize(static_cast <int64_t>(row * (row + 1) / 2));
		if ((A_spec.GetAt(2) == 1) || (A_spec.GetAt(2) == 2)) {
			#pragma omp parallel for private(value_1, value_2) collapse(2)
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j <= i; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 - value_2);
				}
			}
		}
		else if (A_spec.GetAt(2) == 3) {
			#pragma omp parallel for private(value_1, value_2) collapse(2)
			for (int i = 0; i < row; i++)
			{
				for (int j = i; j < col; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 - value_2);
				}
			}
		}
		else {
			#pragma omp parallel for private(value_1, value_2) collapse(2)
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++) {
					value_1 = A.GetAt(GetPosition(i, j, A_spec));
					value_2 = B.GetAt(GetPosition(i, j, B_spec));
					C.SetAt(GetPosition(i, j, C_spec), value_1 - value_2);
				}
			}
		}
	}
}

// Computing matrix-vector product using multithreading :  y = A � x 
void CModelingandAnalysisofUncertaintyDoc::MatrixVectorProductParallel(CArray <double>& A, CArray <int>& A_spec, CArray <double>& x, CArray <double>& y) {
	if (x.GetSize() == A_spec.GetAt(1)) {
		y.SetSize(A_spec.GetAt(0));
		int pos_A;
		double temp;

		#pragma omp parallel for private(pos_A, temp)
		for (int i = 0; i < A_spec.GetAt(0); i++) {
			temp = 0;
			if ((A_spec.GetAt(2) == 0) || (A_spec.GetAt(2) == 1)) {
				for (int j = 0; j < A_spec.GetAt(1); j++) {
					pos_A = GetPosition(i, j, A_spec);
					temp += A.GetAt(pos_A) * x.GetAt(j);
				}
			}
			else if (A_spec.GetAt(2) == 2) {
				for (int j = 0; j <= i; j++) {
					pos_A = GetPosition(i, j, A_spec);
					temp += A.GetAt(pos_A) * x.GetAt(j);
				}
			}
			else if (A_spec.GetAt(2) == 3) {
				for (int j = i; j < A_spec.GetAt(1); j++) {
					pos_A = GetPosition(i, j, A_spec);
					temp += A.GetAt(pos_A) * x.GetAt(j);
				}
			}
			y.SetAt(i, temp);
		}
	}
}

// Computing matrix product using multithreading : C = A � B 
void CModelingandAnalysisofUncertaintyDoc::MatrixProductParallel(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if (A_spec.GetAt(1) == B_spec.GetAt(0)) {
		int row = A_spec.GetAt(0);
		int col = B_spec.GetAt(1);
		C_spec.SetSize(3);
		C_spec.SetAt(0, row);
		C_spec.SetAt(1, col);
		C_spec.SetAt(2, 0);
		int64_t space = static_cast<int64_t>(A_spec.GetAt(0)) * col;
		C.SetSize(space);
		int pos_A, pos_B, pos_C;
		double temp;

		#pragma omp parallel for private(pos_A, pos_B, pos_C, temp) collapse(2)
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				temp = 0;
				for (int k = 0; k < A_spec.GetAt(1); k++) {
					pos_A = GetPosition(i, k, A_spec);
					pos_B = GetPosition(k, j, B_spec);
					temp += A.GetAt(pos_A) * B.GetAt(pos_B);
				}
				pos_C = GetPosition(i, j, C_spec);
				C.SetAt(pos_C, temp);
			}
		}
	}
}

// Computing transpose of matrix product using multithreading : X_tr_X = X' � X   
void CModelingandAnalysisofUncertaintyDoc::X_tr_X_Parallel(CArray <double>& X, CArray <int>& X_spec, CArray <double>& X_tr_X, CArray <int>& X_tr_X_spec) {
	int dim = X_spec.GetAt(1) + 1, pos_X1, pos_X2;
	int64_t space = static_cast<int64_t>(X_spec.GetAt(1)) * dim;
	X_tr_X.SetSize((int)(space / 2));
	X_tr_X_spec.SetSize(3);
	X_tr_X_spec.SetAt(0, X_spec.GetAt(1));
	X_tr_X_spec.SetAt(1, X_spec.GetAt(1));
	X_tr_X_spec.SetAt(2, 1);
	double temp;

	#pragma omp parallel for private(pos_X1, pos_X2, temp) collapse(2)
	for (int i = 0; i < X_spec.GetAt(1); i++) {
		for (int j = 0; j <= i; j++) {
			temp = (double)0;
			for (int k = 0; k < X_spec.GetAt(0); k++) {
				pos_X1 = GetPosition(k, i, X_spec);
				pos_X2 = GetPosition(k, j, X_spec);
				temp += X.GetAt(pos_X1) * X.GetAt(pos_X2);
			}
			X_tr_X.SetAt(GetPosition(i, j, X_tr_X_spec), temp);
		}
	}
}

// Computing matrix product using multithreading : X_tr_Y = X' � Y        
void CModelingandAnalysisofUncertaintyDoc::X_tr_Y_Parallel(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if (A_spec.GetAt(0) == B_spec.GetAt(0)) {
		int dim = A_spec.GetAt(1), pos_A, pos_B;
		int64_t space = static_cast<int64_t>(B_spec.GetAt(1)) * dim;
		C.SetSize(space);
		C_spec.SetSize(3);
		C_spec.SetAt(0, A_spec.GetAt(1));
		C_spec.SetAt(1, B_spec.GetAt(1));
		C_spec.SetAt(2, 0);
		double temp;

		#pragma omp parallel for private(pos_A, pos_B, temp) collapse(2)
		for (int i = 0; i < A_spec.GetAt(1); i++) {
			for (int j = 0; j < B_spec.GetAt(1); j++) {
				temp = (double)0;
				for (int k = 0; k < A_spec.GetAt(0); k++) {
					pos_A = GetPosition(k, i, A_spec);
					pos_B = GetPosition(k, j, B_spec);
					temp += A.GetAt(pos_A) * B.GetAt(pos_B);
				}
				C.SetAt(GetPosition(i, j, C_spec), temp);
			}
		}
	}
}

// Computing matrix product using multithreading : X_Y_tr = X � Y'
void CModelingandAnalysisofUncertaintyDoc::X_Y_tr_Parallel(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& C, CArray <int>& C_spec) {
	if (A_spec.GetAt(1) == B_spec.GetAt(1)) {
		int dim = A_spec.GetAt(0), pos_A, pos_B;
		int64_t space = static_cast<int64_t>(B_spec.GetAt(0)) * dim;
		C.SetSize(space);
		C_spec.SetSize(3);
		C_spec.SetAt(0, A_spec.GetAt(0));
		C_spec.SetAt(1, B_spec.GetAt(0));
		C_spec.SetAt(2, 0);
		double temp;

		#pragma omp parallel for private(pos_A, pos_B, temp) collapse(2)
		for (int i = 0; i < A_spec.GetAt(0); i++) {
			for (int j = 0; j < B_spec.GetAt(0); j++) {
				temp = (double)0;
				for (int k = 0; k < A_spec.GetAt(1); k++) {
					pos_A = GetPosition(i, k, A_spec);
					pos_B = GetPosition(j, k, B_spec);
					temp += A.GetAt(pos_A) * B.GetAt(pos_B);
				}
				C.SetAt(GetPosition(i, j, C_spec), temp);
			}
		}
	}
}

// Computing matrix product using multithreading : X_X_tr = X � X'
void CModelingandAnalysisofUncertaintyDoc::X_X_tr_Parallel(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec) {
	int col = A_spec.GetAt(1), row = A_spec.GetAt(0);
	double temp, value_1, value_2;
	B.RemoveAll(), B_spec.RemoveAll(), B_spec.SetSize(3);
	B_spec.SetAt(0, row), B_spec.SetAt(1, row), B_spec.SetAt(2, 1);
	B.SetSize(static_cast <int64_t>(row * (row + 1) / 2));

	#pragma omp parallel for private(value_1, value_2, temp) collapse(2)
	for (int i = 0; i < row; i++) {
		for (int j = 0; j <= i; j++) {
			temp = 0.0;
			for (int k = 0; k < col; k++) {
				value_1 = A.GetAt(GetPosition(i, k, A_spec));
				value_2 = A.GetAt(GetPosition(j, k, A_spec));
				temp += value_1 * value_2;
			}
			B.SetAt(GetPosition(i, j, B_spec), temp);
		}
	}
}

//Computes the time taken for normal and parallel functions and outputs the speed up of the parallel
void CModelingandAnalysisofUncertaintyDoc::MatrixParallelTest() {
	//Setup
	std::ofstream FILE;
	FILE.open("matrix_Test.txt");
	int size = 1000;
	FILE << "Matrix Size: " << size << "\n";
	CArray <double> A, B, C, D, x, y, z;
	CArray <int> A_Spec, B_Spec, C_Spec, D_Spec;
	A_Spec.SetSize(3), A_Spec.SetAt(0, size), A_Spec.SetAt(1, size), A_Spec.SetAt(2, 0);
	B_Spec.SetSize(3), B_Spec.SetAt(0, size), B_Spec.SetAt(1, size), B_Spec.SetAt(2, 0);

	A.SetSize(static_cast <int64_t>(size * size));
	B.SetSize(static_cast <int64_t>(size * size));
	x.SetSize(static_cast <int64_t>(size));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			A.SetAt(GetPosition(i, j, A_Spec), i);
			B.SetAt(GetPosition(i, j, B_Spec), i);
		}
		x.SetAt(i, i);
	}

	//For GausJorden Testing with matrix of 3x3
	/*
	A.SetAt(GetPosition(0, 0, A_Spec), 1);
	A.SetAt(GetPosition(0, 1, A_Spec), 0);
	A.SetAt(GetPosition(0, 2, A_Spec), 0);
	A.SetAt(GetPosition(1, 0, A_Spec), 0);
	A.SetAt(GetPosition(1, 1, A_Spec), 1);
	A.SetAt(GetPosition(1, 2, A_Spec), 0);
	A.SetAt(GetPosition(2, 0, A_Spec), 0);
	A.SetAt(GetPosition(2, 1, A_Spec), 0);
	A.SetAt(GetPosition(2, 2, A_Spec), 1);
	*/

	//Normal
	auto t_start = std::chrono::high_resolution_clock::now();
	std::clock_t c_start = std::clock();
	//AddingMatrices(A, A_Spec, B, B_Spec, C, C_Spec);
	//SubtractingMatrices(A, A_Spec, B, B_Spec, C, C_Spec);
	//MatrixProduct(A, A_Spec, B, B_Spec, C, C_Spec);
	//MatrixVectorProduct(A, A_Spec, x, y);
	//X_tr_X(A, A_Spec, C, C_Spec);
	//X_tr_Y(A, A_Spec, B, B_Spec, C, C_Spec);
	//X_Y_tr(A, A_Spec, B, B_Spec, C, C_Spec);
	//X_X_tr(A, A_Spec, C, C_Spec);
	//ManipulateRowForwardPath(A, A_Spec, 500, 500);
	//ManipulateRowBackwardPath(A, A_Spec, 500, 500);
	//GaussJordanElimination(A, A_Spec, y, x);
	auto t_end = std::chrono::high_resolution_clock::now();
	std::clock_t c_end = std::clock();
	auto elapsed_time_ms_normal = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();
	FILE << "Finished time of normal clock: " << elapsed_time_ms_normal << "ms" << std::endl;
	FILE << "Finished time of normal CPU: " << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << "ms\n\n";

	//Parallel
	t_start = std::chrono::high_resolution_clock::now();
	c_start = std::clock();
	//AddingMatricesParallel(A, A_Spec, B, B_Spec, D, D_Spec);
	//SubtractingMatricesParallel(A, A_Spec, B, B_Spec, D, D_Spec);
	//MatrixProductParallel(A, A_Spec, B, B_Spec, D, D_Spec);
	//MatrixVectorProductParallel(A, A_Spec, x, z);
	//X_tr_X_Parallel(A, A_Spec, D, D_Spec);
	//X_tr_Y_Parallel(A, A_Spec, B, B_Spec, D, D_Spec);
	//X_Y_tr_Parallel(A, A_Spec, B, B_Spec, D, D_Spec);
	//X_X_tr_Parallel(A, A_Spec, D, D_Spec);
	//ManipulateRowForwardPathParallel(B, B_Spec, 500, 500);
	//ManipulateRowBackwardPathParallel(B, B_Spec, 500, 500);
	//GaussJordanEliminationParallel(A, A_Spec, z, x);
	t_end = std::chrono::high_resolution_clock::now();
	c_end = std::clock();
	auto elapsed_time_ms_parallel = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();

	//Speed up
	double difference = double(elapsed_time_ms_normal) / double(elapsed_time_ms_parallel);
	FILE << "Finished time of parallel clock: " << elapsed_time_ms_parallel << "ms" << std::endl;
	FILE << "Finished time of parallel CPU: " << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << "ms\n\n";
	FILE << "Parallel is " << std::fixed << std::setprecision(2) << difference << " times faster" << "\n";

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			//assert(C.GetAt(GetPosition(i, j, C_Spec)) == D.GetAt(GetPosition(i, j, D_Spec)));
			assert(A.GetAt(GetPosition(i, j, A_Spec)) == B.GetAt(GetPosition(i, j, B_Spec)));
		}
		//assert(y.GetAt(i) == z.GetAt(i));
	}
	FILE << "\nResults are the same\n";
	FILE.close();
}

// *** Gauss-Jordan elimination : A톥=y (x unknown)
// *** Modules : 1 -> Set up pivot for ith column  
// ***           2 -> row operation  (forward path)
// ***           3 -> row operation (backward path)
// ***           4 -> compute Gauss-Jordan method

// Finds the largest element in a column and swaps rows
void CModelingandAnalysisofUncertaintyDoc::SetUpPivot(CArray <double>& C, CArray <int>& C_spec, int row) {
	double max = 0, temp;
	int pivot = row, pos_C;
	for (int i = row; i < C_spec.GetAt(0); i++) {
		pos_C = GetPosition(i, row, C_spec);
		if (C.GetAt(pos_C) > max) {
			max = C.GetAt(pos_C);
			pivot = i;
		}
	}
	if (pivot > row) {
		CArray <double> SwapRow;
		CArray <double> PivotRow;
		GetRow(C, C_spec, SwapRow, row);
		GetRow(C, C_spec, PivotRow, pivot);
		pos_C = GetPosition(pivot, row, C_spec);
		temp = C.GetAt(pos_C);
		ScaleVector(PivotRow, temp);
		InsertRow(C, C_spec, PivotRow, row);
		InsertRow(C, C_spec, SwapRow, pivot);
	}
	else {
		CArray <double> PivotRow;
		GetRow(C, C_spec, PivotRow, row);
		pos_C = GetPosition(row, row, C_spec);
		temp = C.GetAt(pos_C);
		ScaleVector(PivotRow, temp);
		InsertRow(C, C_spec, PivotRow, row);
	}
}

// Converts the matrix A in the Gauss-Jordan scheme 'C' into a upper triangular form
void CModelingandAnalysisofUncertaintyDoc::ManipulateRowForwardPath(CArray <double>& C, CArray <int>& C_spec, int row, int col) {
	CArray <double> c;
	int pos_C = GetPosition(row, col, C_spec);
	double Cji = C.GetAt(pos_C), temp;
	GetRow(C, C_spec, c, col);
	for (int i = col + 1; i < C_spec.GetAt(1); i++) {
		pos_C = GetPosition(row, i, C_spec);
		temp = C.GetAt(pos_C) - Cji * c.GetAt(i);
		C.SetAt(pos_C, temp);
	}
	pos_C = GetPosition(row, col, C_spec);
	C.SetAt(pos_C, (double)0);
}

// Converts the matrix A in the Gauss-Jordan scheme 'C' into a upper triangular form in multithreading
void CModelingandAnalysisofUncertaintyDoc::ManipulateRowForwardPathParallel(CArray <double>& C, CArray <int>& C_spec, int row, int col) {
	CArray <double> c;
	int pos_C = GetPosition(row, col, C_spec);
	double Cji = C.GetAt(pos_C), temp;
	GetRow(C, C_spec, c, col);
	#pragma omp parallel for private(temp, pos_C)
	for (int i = col + 1; i < C_spec.GetAt(1); i++) {
		pos_C = GetPosition(row, i, C_spec);
		temp = C.GetAt(pos_C) - Cji * c.GetAt(i);
		C.SetAt(pos_C, temp);
	}
	pos_C = GetPosition(row, col, C_spec);
	C.SetAt(pos_C, (double)0);
}

// Converts the upper triangular form of A in C to the identity matrix
void CModelingandAnalysisofUncertaintyDoc::ManipulateRowBackwardPath(CArray <double>& C, CArray <int>& C_spec, int row, int col) {
	int pos_C = GetPosition(row, col, C_spec), pos;
	double Cij = C.GetAt(pos_C), temp = 0;
	int dim = C_spec.GetAt(1) - C_spec.GetAt(0);
	for (int i = 0; i < dim; i++) {
		pos_C = GetPosition(row, C_spec.GetAt(0) + i, C_spec);
		pos = GetPosition(col, C_spec.GetAt(0) + i, C_spec);
		temp = C.GetAt(pos_C) - Cij * C.GetAt(pos);
		pos = GetPosition(row, C_spec.GetAt(0) + i, C_spec);
		C.SetAt(pos, temp);
	}
	pos_C = GetPosition(row, col, C_spec);
	C.SetAt(pos_C, (double)0);
}

// Converts the upper triangular form of A in C to the identity matrix in multithreading
void CModelingandAnalysisofUncertaintyDoc::ManipulateRowBackwardPathParallel(CArray <double>& C, CArray <int>& C_spec, int row, int col) {
	int pos_C = GetPosition(row, col, C_spec), pos;
	double Cij = C.GetAt(pos_C), temp = 0;
	int dim = C_spec.GetAt(1) - C_spec.GetAt(0);
	#pragma omp parallel for private(temp, pos_C, pos)
	for (int i = 0; i < dim; i++) {
		pos_C = GetPosition(row, C_spec.GetAt(0) + i, C_spec);
		pos = GetPosition(col, C_spec.GetAt(0) + i, C_spec);
		temp = C.GetAt(pos_C) - Cij * C.GetAt(pos);
		pos = GetPosition(row, C_spec.GetAt(0) + i, C_spec);
		C.SetAt(pos, temp);
	}
	pos_C = GetPosition(row, col, C_spec);
	C.SetAt(pos_C, (double)0);
}

// Computes the Gauss-Jordan elimination
void CModelingandAnalysisofUncertaintyDoc::GaussJordanElimination(CArray<double>& A, CArray<int>& A_spec, CArray<double>& x, CArray<double>& y) {
	if ((A_spec.GetAt(0) == A_spec.GetAt(1)) && (y.GetSize() == A_spec.GetAt(0))) {
		x.SetSize(A_spec.GetAt(1));
		CArray <double> C;
		int dim = A_spec.GetAt(1) + 1;
		int64_t space = static_cast<int64_t>(A_spec.GetAt(0)) * dim;
		C.SetSize(space);
		CArray <int> C_spec;
		C_spec.SetSize(3);
		C_spec.SetAt(0, A_spec.GetAt(0));
		C_spec.SetAt(1, A_spec.GetAt(1) + 1);
		C_spec.SetAt(2, 0);
		int row = A_spec.GetAt(0);
		int col = A_spec.GetAt(1);
		int pos_C, pos_A;
		if ((A_spec.GetAt(2) == 0) || (A_spec.GetAt(2) == 1)) {
			// unspecified or symmetric matrix
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					pos_C = GetPosition(i, j, C_spec);
					pos_A = GetPosition(i, j, A_spec);
					C.SetAt(pos_C, A.GetAt(pos_A));
				}
				pos_C = GetPosition(i, row, C_spec);
				C.SetAt(pos_C, y.GetAt(i));
			}
			for (int i = 0; i < row; i++) {
				SetUpPivot(C, C_spec, i);
				for (int j = i + 1; j < row; j++) ManipulateRowForwardPath(C, C_spec, j, i);
			}
			for (int i = row - 1; i > 0; i--) {
				for (int j = i - 1; j >= 0; j--) ManipulateRowBackwardPath(C, C_spec, j, i);
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, row, C_spec);
				x.SetAt(i, C.GetAt(pos_C));
			}
			for (int i = 0; i < row; i++) {
				for (int j = 0; j <= col; j++) {
					pos_C = GetPosition(i, j, C_spec);
				}
			}
		}
		else if (A_spec.GetAt(2) == 2) {
			// lower triangular matrix
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (j <= i) {
						pos_C = GetPosition(i, j, C_spec);
						pos_A = GetPosition(i, j, A_spec);
						C.SetAt(pos_C, A.GetAt(pos_A));
					}
					else {
						pos_C = GetPosition(i, j, C_spec);
						C.SetAt(pos_C, (double)0);
					}
				}
				pos_C = GetPosition(i, row, C_spec);
				C.SetAt(pos_C, y.GetAt(i));
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, i, C_spec);
				double Cii = C.GetAt(pos_C);
				C.SetAt(pos_C, (double)1);
				pos_C = GetPosition(i, row, C_spec);
				C.SetAt(pos_C, C.GetAt(pos_C) / Cii);
				for (int j = i + 1; j < row; j++) ManipulateRowForwardPath(C, C_spec, j, i);
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, row, C_spec);
				x.SetAt(i, C.GetAt(pos_C));
			}
		}
		else {
			// upper triangular matrix
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (j >= i) {
						pos_C = GetPosition(i, j, C_spec);
						pos_A = GetPosition(i, j, A_spec);
						C.SetAt(pos_C, A.GetAt(pos_A));
					}
					else {
						pos_C = GetPosition(i, j, C_spec);
						C.SetAt(pos_C, (double)0);
					}
				}
				pos_C = GetPosition(i, row, C_spec);
				C.SetAt(pos_C, y.GetAt(i));
			}
			for (int i = row - 1; i >= 0; i--) {
				pos_C = GetPosition(i, i, C_spec);
				double Cii = C.GetAt(pos_C);
				for (int j = i; j < col + 1; j++) {
					pos_C = GetPosition(i, j, C_spec);
					C.SetAt(pos_C, C.GetAt(pos_C) / Cii);
				}
				for (int j = i - 1; j >= 0; j--) ManipulateRowBackwardPath(C, C_spec, j, i);
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, row, C_spec);
				x.SetAt(i, C.GetAt(pos_C));
			}
		}
	}
}

// Computes the Gauss-Jordan elimination with multithreading
void CModelingandAnalysisofUncertaintyDoc::GaussJordanEliminationParallel(CArray<double>& A, CArray<int>& A_spec, CArray<double>& x, CArray<double>& y) {
	if ((A_spec.GetAt(0) == A_spec.GetAt(1)) && (y.GetSize() == A_spec.GetAt(0))) {
		x.SetSize(A_spec.GetAt(1));
		CArray <double> C;
		int dim = A_spec.GetAt(1) + 1;
		int64_t space = static_cast<int64_t>(A_spec.GetAt(0)) * dim;
		C.SetSize(space);
		CArray <int> C_spec;
		C_spec.SetSize(3);
		C_spec.SetAt(0, A_spec.GetAt(0));
		C_spec.SetAt(1, A_spec.GetAt(1) + 1);
		C_spec.SetAt(2, 0);
		int row = A_spec.GetAt(0);
		int col = A_spec.GetAt(1);
		int pos_C, pos_A;
		if ((A_spec.GetAt(2) == 0) || (A_spec.GetAt(2) == 1)) {
			// unspecified or symmetric matrix
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					pos_C = GetPosition(i, j, C_spec);
					pos_A = GetPosition(i, j, A_spec);
					C.SetAt(pos_C, A.GetAt(pos_A));
				}
				pos_C = GetPosition(i, row, C_spec);
				C.SetAt(pos_C, y.GetAt(i));
			}
			for (int i = 0; i < row; i++) {
				SetUpPivot(C, C_spec, i);
				for (int j = i + 1; j < row; j++) ManipulateRowForwardPathParallel(C, C_spec, j, i);
			}
			for (int i = row - 1; i > 0; i--) {
				for (int j = i - 1; j >= 0; j--) ManipulateRowBackwardPathParallel(C, C_spec, j, i);
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, row, C_spec);
				x.SetAt(i, C.GetAt(pos_C));
			}
			for (int i = 0; i < row; i++) {
				for (int j = 0; j <= col; j++) {
					pos_C = GetPosition(i, j, C_spec);
				}
			}
		}
		else if (A_spec.GetAt(2) == 2) {
			// lower triangular matrix
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (j <= i) {
						pos_C = GetPosition(i, j, C_spec);
						pos_A = GetPosition(i, j, A_spec);
						C.SetAt(pos_C, A.GetAt(pos_A));
					}
					else {
						pos_C = GetPosition(i, j, C_spec);
						C.SetAt(pos_C, (double)0);
					}
				}
				pos_C = GetPosition(i, row, C_spec);
				C.SetAt(pos_C, y.GetAt(i));
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, i, C_spec);
				double Cii = C.GetAt(pos_C);
				C.SetAt(pos_C, (double)1);
				pos_C = GetPosition(i, row, C_spec);
				C.SetAt(pos_C, C.GetAt(pos_C) / Cii);
				for (int j = i + 1; j < row; j++) ManipulateRowForwardPathParallel(C, C_spec, j, i);
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, row, C_spec);
				x.SetAt(i, C.GetAt(pos_C));
			}
		}
		else {
			// upper triangular matrix
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					if (j >= i) {
						pos_C = GetPosition(i, j, C_spec);
						pos_A = GetPosition(i, j, A_spec);
						C.SetAt(pos_C, A.GetAt(pos_A));
					}
					else {
						pos_C = GetPosition(i, j, C_spec);
						C.SetAt(pos_C, (double)0);
					}
				}
				pos_C = GetPosition(i, row, C_spec);
				C.SetAt(pos_C, y.GetAt(i));
			}
			for (int i = row - 1; i >= 0; i--) {
				pos_C = GetPosition(i, i, C_spec);
				double Cii = C.GetAt(pos_C);
				for (int j = i; j < col + 1; j++) {
					pos_C = GetPosition(i, j, C_spec);
					C.SetAt(pos_C, C.GetAt(pos_C) / Cii);
				}
				for (int j = i - 1; j >= 0; j--) ManipulateRowBackwardPathParallel(C, C_spec, j, i);
			}
			for (int i = 0; i < row; i++) {
				pos_C = GetPosition(i, row, C_spec);
				x.SetAt(i, C.GetAt(pos_C));
			}
		}
	}
}

//*****************************************************************
//***   BEGIN   ->           MATRIX DECOMPOSITIONS              ***
//*****************************************************************

// Computes the QR Decomposition : X = Q � R (R returned)
void CModelingandAnalysisofUncertaintyDoc::QR(CArray <double>& X, CArray <int>& X_spec, CArray <double>& R, CArray <int>& R_spec, bool& flag) {
	int n_rows = X_spec.GetAt(0), n_cols = X_spec.GetAt(1);
	int dim = n_cols + 1;
	int64_t space = static_cast<int64_t>(dim) * n_cols;
	R.SetSize((int)(space / 2));
	R_spec.SetSize(3);
	R_spec.SetAt(0, n_cols);
	R_spec.SetAt(1, n_cols);
	R_spec.SetAt(2, 3);
	if (n_cols == n_rows) {
		CArray <double> A;
		CArray <double> c, d;
		A.SetSize(X.GetSize());
		c.SetSize(n_cols);
		d.SetSize(n_cols);
		int i, j, k, pos_1, pos_2;
		double scale, sigma, sum, tau;
		for (i = 0; i < n_rows; i++)
			for (j = 0; j < n_cols; j++) {
				pos_1 = GetPosition(i, j, X_spec);
				A.SetAt(pos_1, X.GetAt(pos_1));
			}
		for (k = 1; k < n_cols; k++) {
			for (scale = (double)0, i = k; i <= n_rows; i++) scale = DMAX(scale, fabs(A.GetAt(static_cast<int64_t>(GetPosition(i - 1, k - 1, X_spec)))));
			if (scale == (double)0) {
				c.SetAt(static_cast<int64_t>(k) - 1, (double)0);
				d.SetAt(static_cast<int64_t>(k) - 1, (double)0);
				flag = true;
			}
			else {
				for (i = k; i <= n_rows; i++) {
					pos_1 = GetPosition(i - 1, k - 1, X_spec);
					A.SetAt(pos_1, A.GetAt(pos_1) / scale);
				}
				for (sum = (double)0, i = k; i <= n_rows; i++) {
					pos_1 = GetPosition(i - 1, k - 1, X_spec);
					sum += SQR(A.GetAt(pos_1));
				}
				pos_1 = GetPosition(k - 1, k - 1, X_spec);
				sigma = SIGN(sqrt(sum), A.GetAt(pos_1));
				A.SetAt(pos_1, A.GetAt(pos_1) + sigma); // add sign(yk) * e to kth element
				c.SetAt(static_cast<int64_t>(k) - 1, sigma * A.GetAt(pos_1));
				d.SetAt(static_cast<int64_t>(k) - 1, -scale * sigma);
				for (j = k + 1; j <= n_cols; j++) {
					for (sum = (double)0, i = k; i <= n_rows; i++) {
						pos_1 = GetPosition(i - 1, k - 1, X_spec);
						pos_2 = GetPosition(i - 1, j - 1, X_spec);
						sum += A.GetAt(pos_1) * A.GetAt(pos_2);
					}
					tau = sum / c.GetAt(static_cast<int64_t>(k) - 1);
					for (i = k; i <= n_rows; i++) {
						pos_1 = GetPosition(i - 1, j - 1, X_spec);
						pos_2 = GetPosition(i - 1, k - 1, X_spec);
						A.SetAt(pos_1, A.GetAt(pos_1) - tau * A.GetAt(pos_2));
					}
				}
			}
		}
		pos_1 = GetPosition(n_cols - 1, n_cols - 1, X_spec);
		d.SetAt(static_cast<int64_t>(n_cols) - 1, A.GetAt(pos_1));
		if (d.GetAt(static_cast<int64_t>(n_cols) - 1) == (double)0) flag = true;
		std::ofstream FILE;
		FILE.open("QR.txt");
		for (int i = 0; i < X_spec.GetAt(0); i++) {
			for (int j = 0; j < X_spec.GetAt(1); j++) {
				FILE << X.GetAt(GetPosition(i, j, X_spec)) << "\t";
			}
			FILE << std::endl;
		}
		FILE.close();
		for (int i = 0; i < n_rows; i++) {
			for (int j = i; j < n_cols; j++) {
				pos_1 = GetPosition(i, j, R_spec);
				pos_2 = GetPosition(i, j, X_spec);
				if (i == j) R.SetAt(pos_1, d.GetAt(i));
				else R.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
	}
	else {
		CArray <double> A;
		CArray <double> d, v;
		A.SetSize(X.GetSize());
		d.SetSize(n_cols);
		v.SetSize(n_rows);
		int i, j, k, pos;
		double scale, sum, sigma, temp;
		for (i = 0; i < n_rows; i++) {
			for (j = 0; j < n_cols; j++) {
				pos = GetPosition(i, j, X_spec);
				A.SetAt(pos, X.GetAt(pos));
			}
		}
		for (k = 0; k < n_cols; k++) {
			for (scale = (double)0, i = k; i < n_rows; i++) {
				pos = GetPosition(i, k, X_spec);
				scale = DMAX(scale, fabs(A.GetAt(pos))); // Find largest value in kth row
			}
			if (scale == (double)0) {
				flag = true;
				break;
			}
			else {
				for (sum = (double)0, i = k; i < n_rows; i++) {
					pos = GetPosition(i, k, X_spec);
					temp = A.GetAt(pos) / scale;
					A.SetAt(pos, temp); // Condition the kth column by dividing each element with the largest one
					sum += SQR(temp); // Working out the squared length of the scaled column of A(k:n_rows,k)
					v.SetAt(i, temp); // Storing the elements of A into v
				}
				pos = GetPosition(k, k, X_spec);
				sigma = SIGN(sqrt(sum), A.GetAt(pos)); // Calculate sign(yk)*||y||
				v.SetAt(k, v.GetAt(k) + sigma); // Compute v = y + sign(yk)*||y||*ek
				sum += sigma * sigma + 2 * sigma * A.GetAt(pos); // Updating the squared length of v
				sigma = sqrt(sum);
				for (i = k; i < n_rows; i++) v.SetAt(i, v.GetAt(i) / sigma); // Set v to unit length
				for (sigma = (double)0, i = k; i < n_rows; i++) {
					pos = GetPosition(i, k, X_spec);
					sigma += A.GetAt(pos) * v.GetAt(i); // computer vk'*A(:,k)
				}
				pos = GetPosition(k, k, X_spec);
				d.SetAt(k, scale * (A.GetAt(pos) - (double)2 * v.GetAt(k) * sigma));
				for (int i = k; i < n_rows; i++) {
					pos = GetPosition(i, k, X_spec);
					A.SetAt(pos, v.GetAt(i)); // Storing the kth v-vector as the kth column of A
				}
				for (int i = k + 1; i < n_cols; i++) {
					for (sigma = (double)0, j = k; j < n_rows; j++) {
						pos = GetPosition(j, i, X_spec);
						sigma += v.GetAt(j) * A.GetAt(pos); // Scalar product v(k:n_rows)*A(k:n_rows,k)
					}
					for (j = k; j < n_rows; j++) {
						pos = GetPosition(j, i, X_spec);
						A.SetAt(pos, A.GetAt(pos) - 2 * v.GetAt(j) * sigma);
					}
				}
			}
		}
		for (i = 0; i < n_cols; i++) {
			for (j = i; j < n_cols; j++) {
				pos = GetPosition(i, j, R_spec);
				if (i == j) R.SetAt(pos, d.GetAt(i));
				else R.SetAt(pos, A.GetAt(GetPosition(i, j, X_spec)));
			}
		}
	}
}

// Computes the Cholesky Decomposition : A = L � L' (L returned) 
void CModelingandAnalysisofUncertaintyDoc::CHOL(CArray <double>& A, CArray <int>& A_spec, CArray <double>& L, CArray <int>& L_spec, bool& flag) {
	if (A_spec.GetAt(2) == 1) {
		int row = A_spec.GetAt(0), dim = row + 1, pos_1, pos_2;
		int64_t space = static_cast<int64_t>(dim) * row;
		L.SetSize((int)(space / 2));
		L_spec.SetSize(3);
		L_spec.SetAt(0, row);
		L_spec.SetAt(1, row);
		L_spec.SetAt(2, 2);
		CArray <double> Temp;
		Temp.SetSize(A.GetSize());
		int i, j, k;
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				pos_1 = GetPosition(i, j, A_spec);
				Temp.SetAt(pos_1, A.GetAt(pos_1));
			}
		}
		double sum;
		for (i = 1; i <= row; i++) {
			for (j = i; j <= row; j++) {
				pos_1 = GetPosition(i - 1, j - 1, A_spec);
				sum = Temp.GetAt(pos_1);
				for (k = i - 1; k >= 1; k--) {
					pos_1 = GetPosition(i - 1, k - 1, A_spec);
					pos_2 = GetPosition(j - 1, k - 1, A_spec);
					sum -= Temp.GetAt(pos_1) * Temp.GetAt(pos_2);
				}
				if (i == j) {
					if (sum <= 0.0) {
						flag = true;
					}
					pos_1 = GetPosition(i - 1, i - 1, L_spec);
					L.SetAt(pos_1, sqrt(sum));
				}
				else {
					pos_1 = GetPosition(j - 1, i - 1, A_spec);
					pos_2 = GetPosition(i - 1, i - 1, L_spec);
					Temp.SetAt(pos_1, (sum / L.GetAt(pos_2)));
				}
			}
		}
		for (i = 0; i < row; i++) {
			for (j = i + 1; j < row; j++) {
				pos_1 = GetPosition(j, i, L_spec);
				pos_2 = GetPosition(j, i, A_spec);
				L.SetAt(pos_1, Temp.GetAt(pos_2));
			}
		}
	}
	else flag = true;
}

void CModelingandAnalysisofUncertaintyDoc::SortSVD(CArray <double>& U, CArray <int>& U_spec, CArray <double>& w, CArray <double>& V, CArray <int>& V_spec) {
	CArray <double> wCopy;
	CArray <double> UCopy;
	CArray <double> VCopy;
	int n_rows = U_spec.GetAt(0), pos_1, pos_2, n_cols = U_spec.GetAt(1);
	CArray <int> Index;
	Index.SetSize(n_cols);
	wCopy.SetSize(n_cols);
	UCopy.SetSize(U.GetSize());
	VCopy.SetSize(V.GetSize());
	for (int i = 0; i < n_cols; i++) {
		wCopy.SetAt(i, w.GetAt(i));
		for (int j = 0; j < n_rows; j++) {
			pos_1 = GetPosition(j, i, U_spec);
			UCopy.SetAt(pos_1, U.GetAt(pos_1));
		}
		for (int j = 0; j < n_cols; j++) {
			pos_1 = GetPosition(j, i, V_spec);
			VCopy.SetAt(pos_1, V.GetAt(pos_1));
		}
	}
	QuickSort(w, 0, n_cols - 1);
	for (int i = 0; i < n_cols; i++) {
		int j = 0;
		while (w.GetAt(i) != wCopy.GetAt(j)) j++;
		Index.SetAt(i, j);
	}
	for (int i = 0; i < n_cols; i++) {
		w.SetAt(i, wCopy.GetAt(Index.GetAt(static_cast<int64_t>(n_cols) - i - 1)));
		for (int j = 0; j < n_rows; j++) {
			pos_1 = GetPosition(j, i, U_spec);
			pos_2 = GetPosition(j, Index.GetAt(static_cast<int64_t>(n_cols) - i - 1), U_spec);
			U.SetAt(pos_1, UCopy.GetAt(pos_2));
		}
		for (int j = 0; j < n_cols; j++) {
			pos_1 = GetPosition(j, i, V_spec);
			pos_2 = GetPosition(j, Index.GetAt(static_cast<int64_t>(n_cols) - i - 1), V_spec);
			V.SetAt(pos_1, VCopy.GetAt(pos_2));
		}
	}
}

void CModelingandAnalysisofUncertaintyDoc::SVD(CArray <double>& X, CArray <int>& X_spec, CArray <double>& U, CArray <int>& U_spec, CArray <double>& w, CArray <double>& V, CArray <int>& V_spec, bool& FLAG) {
	int m = X_spec.GetAt(0), n = X_spec.GetAt(1);
	int64_t space;
	if (m >= n) {
		space = static_cast <int64_t>(m) * n;
		U.SetSize(space);
		U_spec.SetSize(3);
		U_spec.SetAt(0, m);
		U_spec.SetAt(1, n);
		U_spec.SetAt(2, 0);
		space = static_cast <int64_t>(n) * n;
		V.SetSize(space);
		V_spec.SetSize(3);
		V_spec.SetAt(0, n);
		V_spec.SetAt(1, n);
		V_spec.SetAt(2, 0);
		w.SetSize(n);

		int flag, i, its, j, jj, k, l, nm, pos_1, pos_2, pos_3;
		double anorm, c, f, g, h, s, scale, x, y, z;
		CArray <double> A;
		space = static_cast <int64_t>(m) * n;
		A.SetSize(space);
		CArray <double> rv1;
		rv1.SetSize(n);
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= n; j++) {
				pos_1 = GetPosition(i - 1, j - 1, X_spec);
				A.SetAt(pos_1, X.GetAt(pos_1));
			}
		}
		g = scale = anorm = (double)0;
		for (i = 1; i <= n; i++) {
			l = i + 1;
			rv1.SetAt(static_cast <int64_t>(i) - 1, scale * g);
			g = s = scale = (double)0;
			if (i <= m) {
				for (k = i; k <= m; k++) {
					pos_1 = GetPosition(k - 1, i - 1, X_spec);
					scale += fabs(A.GetAt(pos_1));
				}
				if (scale) {
					for (k = i; k <= m; k++) {
						pos_1 = GetPosition(k - 1, i - 1, X_spec);
						A.SetAt(pos_1, (A.GetAt(pos_1) / scale));
						s += A.GetAt(pos_1) * A.GetAt(pos_1);
					}
					pos_1 = GetPosition(i - 1, i - 1, X_spec);
					f = A.GetAt(pos_1);
					g = -SIGN(sqrt(s), f);
					h = f * g - s;
					A.SetAt(pos_1, f - g);
					for (j = l; j <= n; j++) {
						for (s = (double)0, k = i; k <= m; k++) {
							pos_1 = GetPosition(k - 1, i - 1, X_spec);
							pos_2 = GetPosition(k - 1, j - 1, X_spec);
							s += A.GetAt(pos_1) * A.GetAt(pos_2);
						}
						f = s / h;
						for (k = i; k <= m; k++) {
							pos_1 = GetPosition(k - 1, j - 1, X_spec);
							pos_2 = GetPosition(k - 1, i - 1, X_spec);
							A.SetAt(pos_1, (A.GetAt(pos_1) + f * A.GetAt(pos_2)));
						}
					}
					for (k = i; k <= m; k++) {
						pos_1 = GetPosition(k - 1, i - 1, X_spec);
						A.SetAt(pos_1, (A.GetAt(pos_1) * scale));
					}
				}
			}
			w.SetAt(static_cast <int64_t>(i) - 1, scale * g);
			g = s = scale = (double)0;
			if ((i <= m) && (i != n)) {
				for (k = l; k <= n; k++) {
					pos_1 = GetPosition(i - 1, k - 1, X_spec);
					scale += fabs(A.GetAt(pos_1));
				}
				if (scale) {
					for (k = l; k <= n; k++) {
						pos_1 = GetPosition(i - 1, k - 1, X_spec);
						A.SetAt(pos_1, (A.GetAt(pos_1) / scale));
						s += A.GetAt(pos_1) * A.GetAt(pos_1);
					}
					pos_1 = GetPosition(i - 1, l - 1, X_spec);
					f = A.GetAt(pos_1);
					g = -SIGN(sqrt(s), f);
					h = f * g - s;
					A.SetAt(pos_1, (f - g));
					for (k = l; k <= n; k++) {
						pos_1 = GetPosition(i - 1, k - 1, X_spec);
						rv1.SetAt(static_cast <int64_t>(k) - 1, A.GetAt(pos_1) / h);
					}
					for (j = l; j <= m; j++) {
						for (s = (double)0, k = l; k <= n; k++) {
							pos_1 = GetPosition(j - 1, k - 1, X_spec);
							pos_2 = GetPosition(i - 1, k - 1, X_spec);
							s += A.GetAt(pos_1) * A.GetAt(pos_2);
						}
						for (k = l; k <= n; k++) {
							pos_1 = GetPosition(j - 1, k - 1, X_spec);
							A.SetAt(pos_1, (A.GetAt(pos_1) + s * rv1.GetAt(static_cast <int64_t>(k) - 1)));
						}
					}
					for (k = l; k <= n; k++) {
						pos_1 = GetPosition(i - 1, k - 1, X_spec);
						A.SetAt(pos_1, (A.GetAt(pos_1) * scale));
					}
				}
			}
			anorm = DMAX(anorm, (fabs(w.GetAt(static_cast <int64_t>(i) - 1)) + fabs(rv1.GetAt(static_cast <int64_t>(i) - 1))));
		}
		for (i = n; i >= 1; i--) {
			if (i < n) {
				if (g) {
					for (j = l; j <= n; j++) {
						pos_1 = GetPosition(j - 1, i - 1, V_spec);
						pos_2 = GetPosition(i - 1, j - 1, X_spec);
						pos_3 = GetPosition(i - 1, l - 1, X_spec);
						V.SetAt(pos_1, ((A.GetAt(pos_2) / A.GetAt(pos_3)) / g));
					}
					for (j = l; j <= n; j++) {
						for (s = (double)0, k = l; k <= n; k++) {
							pos_1 = GetPosition(i - 1, k - 1, X_spec);
							pos_2 = GetPosition(k - 1, j - 1, V_spec);
							s += A.GetAt(pos_1) * V.GetAt(pos_2);
						}
						for (k = l; k <= n; k++) {
							pos_1 = GetPosition(k - 1, j - 1, V_spec);
							pos_2 = GetPosition(k - 1, i - 1, V_spec);
							V.SetAt(pos_1, (V.GetAt(pos_1) + s * V.GetAt(pos_2)));
						}
					}
				}
				for (j = l; j <= n; j++) {
					pos_1 = GetPosition(i - 1, j - 1, V_spec);
					pos_2 = GetPosition(j - 1, i - 1, V_spec);
					V.SetAt(pos_1, (double)0);
					V.SetAt(pos_2, (double)0);
				}
			}
			pos_1 = GetPosition(i - 1, i - 1, V_spec);
			V.SetAt(pos_1, (double)1);
			g = rv1.GetAt(static_cast <int64_t>(i) - 1);
			l = i;
		}
		for (i = IMIN(m, n); i >= 1; i--) {
			l = i + 1;
			g = w.GetAt(static_cast <int64_t>(i) - 1);
			for (j = l; j <= n; j++) {
				pos_1 = GetPosition(i - 1, j - 1, X_spec);
				A.SetAt(pos_1, (double)0);
			}
			if (g) {
				g = (double)1 / g;
				for (j = l; j <= n; j++) {
					for (s = (double)0, k = l; k <= m; k++) {
						pos_1 = GetPosition(k - 1, i - 1, X_spec);
						pos_2 = GetPosition(k - 1, j - 1, X_spec);
						s += A.GetAt(pos_1) * A.GetAt(pos_2);
					}
					pos_1 = GetPosition(i - 1, i - 1, X_spec);
					f = (s / A.GetAt(pos_1)) * g;
					for (k = i; k <= m; k++) {
						pos_1 = GetPosition(k - 1, j - 1, X_spec);
						pos_2 = GetPosition(k - 1, i - 1, X_spec);
						A.SetAt(pos_1, (A.GetAt(pos_1) + f * A.GetAt(pos_2)));
					}
				}
				for (j = i; j <= m; j++) {
					pos_1 = GetPosition(j - 1, i - 1, X_spec);
					A.SetAt(pos_1, (A.GetAt(pos_1) * g));
				}
			}
			else for (j = i; j <= m; j++) {
				pos_1 = GetPosition(j - 1, i - 1, X_spec);
				A.SetAt(pos_1, (double)0);
			}
			pos_1 = GetPosition(i - 1, i - 1, X_spec);
			A.SetAt(pos_1, A.GetAt(pos_1) + (double)1);
		}
		for (k = n; k >= 1; k--) {
			for (its = 1; its <= 30; its++) {
				flag = 1;
				for (l = k; l >= 1; l--) {
					nm = l - 1;
					if ((fabs(rv1.GetAt(static_cast <int64_t>(l) - 1)) + anorm) == anorm) {
						flag = 0;
						break;
					}
					if ((double)(fabs(w.GetAt(static_cast <int64_t>(nm) - 1)) + anorm) == anorm) break;
				}
				if (flag) {
					c = (double)0, s = (double)1;
					for (i = l; i <= k; i++) {
						f = s * rv1.GetAt(static_cast <int64_t>(i) - 1);
						rv1.GetAt(static_cast <int64_t>(i) - 1) = c * rv1.GetAt(static_cast <int64_t>(i) - 1);
						if ((double)(fabs(f) + anorm) == anorm) break;
						g = w.GetAt(static_cast <int64_t>(i) - 1);
						h = PYTHAG(f, g);
						w.SetAt(static_cast <int64_t>(i) - 1, h);
						h = (double)1 / h;
						c = g * h;
						s = -f * g;
						for (j = 0; j <= m; j++) {
							pos_1 = GetPosition(j - 1, nm - 1, X_spec);
							pos_2 = GetPosition(j - 1, i - 1, X_spec);
							y = A.GetAt(pos_1);
							z = A.GetAt(pos_2);
							A.SetAt(pos_1, (y * c + z * s));
							A.SetAt(pos_2, (z * c - y * s));
						}
					}
				}
				z = w.GetAt(static_cast <int64_t>(k) - 1);
				if (l == k) {
					if (z < (double)0) {
						w.SetAt(static_cast <int64_t>(k) - 1, -z);
						for (j = 1; j <= n; j++) {
							pos_1 = GetPosition(j - 1, k - 1, V_spec);
							V.SetAt(pos_1, -V.GetAt(pos_1));
						}
					}
					break;
				}
				if (its == 30) {
					FLAG = true;
				}
				x = w.GetAt(static_cast <int64_t>(l) - 1);
				nm = k - 1;
				y = w.GetAt(static_cast <int64_t>(nm) - 1);
				g = rv1.GetAt(static_cast <int64_t>(nm) - 1);
				h = rv1.GetAt(static_cast <int64_t>(k) - 1);
				f = ((y - z) * (y + z) + (g - h) * (g + h)) / ((double)2 * h * y);
				g = PYTHAG(f, (double)1);
				f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
				c = s = (double)1;
				for (j = l; j <= nm; j++) {
					i = j + 1;
					g = rv1.GetAt(static_cast <int64_t>(i) - 1);
					y = w.GetAt(static_cast <int64_t>(i) - 1);
					h = s * g;
					g = c * g;
					z = PYTHAG(f, h);
					rv1.SetAt(static_cast <int64_t>(j) - 1, z);
					c = f / z;
					s = h / z;
					f = x * c + g * s;
					g = g * c - x * s;
					h = y * s;
					y *= c;
					for (jj = 1; jj <= n; jj++) {
						pos_1 = GetPosition(jj - 1, j - 1, V_spec);
						pos_2 = GetPosition(jj - 1, i - 1, V_spec);
						x = V.GetAt(pos_1);
						z = V.GetAt(pos_2);
						V.SetAt(pos_1, (x * c + z * s));
						V.SetAt(pos_2, (z * c - x * s));
					}
					z = PYTHAG(f, h);
					w.SetAt(static_cast <int64_t>(j) - 1, z);
					if (z) {
						z = (double)1 / z;
						c = f * z;
						s = h * z;
					}
					f = c * g + s * y;
					x = c * y - s * g;
					for (jj = 1; jj <= m; jj++) {
						pos_1 = GetPosition(jj - 1, j - 1, X_spec);
						pos_2 = GetPosition(jj - 1, i - 1, X_spec);
						y = A.GetAt(pos_1);
						z = A.GetAt(pos_2);
						A.SetAt(pos_1, (y * c + z * s));
						A.SetAt(pos_2, (z * c - y * s));
					}
				}
				rv1.SetAt(static_cast <int64_t>(l) - 1, (double)0);
				rv1.SetAt(static_cast <int64_t>(k) - 1, f);
				w.SetAt(static_cast <int64_t>(k) - 1, x);
			}
		}
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				pos_1 = GetPosition(i, j, U_spec);
				pos_2 = GetPosition(i, j, X_spec);
				U.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
		SortSVD(U, U_spec, w, V, V_spec);
	}
	else FLAG = true;
}

// *** Computes standard eigendecomposition : A � x = l 톥 ( A = X � l � X', X and L returned) 
// *** Modules                              : 1 -> Lanzcos tridiagonalization
// ***                                        2 -> QL decomposition
// ***                                        3 -> Sorting eigendecomposition
// ***                                        4 -> Computing eigendecomposition

// Lanzcos tridiagonalization of a symmetric positive definite matrix 
void CModelingandAnalysisofUncertaintyDoc::LANCZOS(CArray <double>& Temp, CArray <int>& Temp_spec, CArray <double>& d, CArray <double>& e, bool& EV) {
	int l, k, j, i, n_rows = Temp_spec.GetAt(0), pos_1, pos_2;
	double scale, hh, h, g, f;
	for (i = n_rows; i >= 2; i--) {
		l = i - 1;
		h = scale = (double)0;
		if (l > 1) {
			for (k = 1; k <= l; k++) {
				pos_1 = GetPosition(i - 1, k - 1, Temp_spec);
				scale += fabs(Temp.GetAt(pos_1));
			}
			if (scale == 0.0) /* Skip transformation. */ {
				pos_1 = GetPosition(i - 1, l - 1, Temp_spec);
				e.SetAt(static_cast<int64_t>(i) - 1, Temp.GetAt(pos_1));
			}
			else {
				for (k = 1; k <= l; k++) {
					pos_1 = GetPosition(i - 1, k - 1, Temp_spec);
					Temp.SetAt(pos_1, Temp.GetAt(pos_1) / scale);
					h += Temp.GetAt(pos_1) * Temp.GetAt(pos_1);
				}
				pos_1 = GetPosition(i - 1, l - 1, Temp_spec);
				f = Temp.GetAt(pos_1);
				g = (f >= 0.0 ? -sqrt(h) : sqrt(h));
				e.SetAt(static_cast<int64_t>(i) - 1, scale * g);
				h -= f * g; /* Now h is equation (11.2.4). */
				Temp.SetAt(pos_1, (f - g)); /* Store u in the ith row of a. */
				f = (double)0;
				for (j = 1; j <= l; j++) {
					/* Next statement can be omitted if eigenvectors not wanted */
					if (EV) {
						pos_1 = GetPosition(j - 1, i - 1, Temp_spec);
						pos_2 = GetPosition(i - 1, j - 1, Temp_spec);
						Temp.SetAt(pos_1, (Temp.GetAt(pos_2) / h));
					}
					g = (double)0; /* Form an element of A.u in g. */
					for (k = 1; k <= j; k++) {
						pos_1 = GetPosition(j - 1, k - 1, Temp_spec);
						pos_2 = GetPosition(i - 1, k - 1, Temp_spec);
						g += Temp.GetAt(pos_1) * Temp.GetAt(pos_2);
					}
					for (k = j + 1; k <= l; k++) {
						pos_1 = GetPosition(k - 1, j - 1, Temp_spec);
						pos_2 = GetPosition(i - 1, k - 1, Temp_spec);
						g += Temp.GetAt(pos_1) * Temp.GetAt(pos_2);
					}
					e.SetAt(static_cast<int64_t>(j) - 1, g / h);
					pos_1 = GetPosition(i - 1, j - 1, Temp_spec);
					f += e.GetAt(static_cast<int64_t>(j) - 1) * Temp.GetAt(pos_1);
				}
				hh = f / (h + h); /* Form K, equation (11.2.11). */
				for (j = 1; j <= l; j++) { /* Form q and store in e overwriting p. */
					pos_1 = GetPosition(i - 1, j - 1, Temp_spec);
					f = Temp.GetAt(pos_1);
					g = e.GetAt(static_cast<int64_t>(j) - 1) - hh * f;
					e.SetAt(static_cast<int64_t>(j) - 1, g);
					for (k = 1; k <= j; k++) {
						pos_1 = GetPosition(j - 1, k - 1, Temp_spec);
						pos_2 = GetPosition(i - 1, k - 1, Temp_spec);
						Temp.SetAt(pos_1, Temp.GetAt(pos_1) - (f * e.GetAt(static_cast<int64_t>(k) - 1) + g * Temp.GetAt(pos_2)));
					}
				}
			}
		}
		else {
			pos_1 = GetPosition(i - 1, l - 1, Temp_spec);
			e.SetAt(static_cast<int64_t>(i) - 1, Temp.GetAt(pos_1));
		}
		d.SetAt(static_cast<int64_t>(i) - 1, h);
	}
	/* Next statement can be omitted if eigenvectors not wanted */
	d.SetAt(0, (double)0);
	e.SetAt(0, (double)0);
	/* Contents of this loop can be omitted if eigenvectors not
	   wanted except for statement d[i]=a[i][i]; */
	for (i = 1; i <= n_rows; i++) { /* Begin accumulation of transformation matrices. */
		if (EV) {
			l = i - 1;
			if (d.GetAt(static_cast<int64_t>(i) - 1)) { /* This block skipped when i=1. */
				for (j = 1; j <= l; j++) {
					g = (double)0;
					for (k = 1; k <= l; k++) {
						pos_1 = GetPosition(i - 1, k - 1, Temp_spec);
						pos_2 = GetPosition(k - 1, j - 1, Temp_spec);
						g += Temp.GetAt(pos_1) * Temp.GetAt(pos_2);
					}
					for (k = 1; k <= l; k++) {
						pos_1 = GetPosition(k - 1, j - 1, Temp_spec);
						pos_2 = GetPosition(k - 1, i - 1, Temp_spec);
						Temp.SetAt(pos_1, Temp.GetAt(pos_1) - g * Temp.GetAt(pos_2));
					}
				}
			}
		}
		pos_1 = GetPosition(i - 1, i - 1, Temp_spec);
		d.SetAt(static_cast<int64_t>(i) - 1, Temp.GetAt(pos_1));
		if (EV) {
			Temp.SetAt(pos_1, (double)1); /* Reset row and column of a to identity matrix for next iteration. */
			for (j = 1; j <= l; j++) {
				pos_1 = GetPosition(j - 1, i - 1, Temp_spec);
				pos_2 = GetPosition(i - 1, j - 1, Temp_spec);
				Temp.SetAt(pos_1, (double)0);
				Temp.SetAt(pos_2, (double)0);
			}
		}
	}
}

// QL decomposition of tridiagonal matrix Temp (eigenvalues appear in the diagonal of L, stored in the vector 'd')
void CModelingandAnalysisofUncertaintyDoc::QL(CArray <double>& Temp, CArray <int>& Temp_spec, CArray <double>& d, CArray <double>& e, bool& EV) {
	int m = 0, l, iter, i, k, n_rows = Temp_spec.GetAt(0), pos_1, pos_2;
	double s, r, p, g, f, dd, c, b;
	for (i = 2; i <= n_rows; i++) e.SetAt(static_cast<int64_t>(i) - 2, e.GetAt(static_cast<int64_t>(i) - 1)); /* Convenient to renumber the elements of e. */
	e.SetAt(static_cast<int64_t>(n_rows) - 1, (double)0);
	for (l = 1; l <= n_rows; l++) {
		iter = 0;
		do {
			for (m = l; m <= n_rows - 1; m++) { /* Look for a single small subdiagonal element to split the matrix. */
				dd = fabs(d.GetAt(static_cast<int64_t>(m) - 1)) + fabs(d.GetAt(m));
				if ((double)(fabs(e.GetAt(static_cast<int64_t>(m) - 1)) + dd) == dd) break;
			}
			if (m != l) {
				if (iter++ == 30) break;
				g = (d.GetAt(l) - d.GetAt(static_cast<int64_t>(l) - 1)) / ((double)2 * e.GetAt(static_cast<int64_t>(l) - 1)); /* Form shift. */
				r = PYTHAG(g, (double)1);
				g = d.GetAt(static_cast<int64_t>(m) - 1) - d.GetAt(static_cast<int64_t>(l) - 1) + e.GetAt(static_cast<int64_t>(l) - 1) / (g + SIGN(r, g)); /* This is dm - ks. */
				s = c = (double)1;
				p = (double)0;
				for (i = m - 1; i >= l; i--) { /* A plane rotation as in the original QL, followed by Givens */
					f = s * e.GetAt(static_cast<int64_t>(i) - 1);          /* rotations to restore tridiagonal form.                     */
					b = c * e.GetAt(static_cast<int64_t>(i) - 1);
					r = PYTHAG(f, g);
					e.SetAt(i, r);
					if (r == 0.0) { /* Recover from underflow. */
						d.SetAt(i, d.GetAt(i) - p);
						e.SetAt(static_cast<int64_t>(m) - 1, (double)0);
						break;
					}
					s = f / r;
					c = g / r;
					g = d.GetAt(i) - p;
					r = (d.GetAt(static_cast<int64_t>(i) - 1) - g) * s + (double)2 * c * b;
					p = s * r;
					d.SetAt(i, g + p);
					g = c * r - b;
					/* Next loop can be omitted if eigenvectors not wanted */
					if (EV) {
						for (k = 1; k <= n_rows; k++) { /* Form eigenvectors. */
							pos_1 = GetPosition(k - 1, i, Temp_spec);
							pos_2 = GetPosition(k - 1, i - 1, Temp_spec);
							f = Temp.GetAt(pos_1);
							Temp.SetAt(pos_1, s * Temp.GetAt(pos_2) + c * f);
							Temp.SetAt(pos_2, c * Temp.GetAt(pos_2) - s * f);
						}
					}
				}
				if (r == 0.0 && i >= l) continue;
				d.SetAt(static_cast<int64_t>(l) - 1, d.GetAt(static_cast<int64_t>(l) - 1) - p);
				e.SetAt(static_cast<int64_t>(l) - 1, g);
				e.SetAt(static_cast<int64_t>(m) - 1, (double)0);
			}
		} while (m != l);
	}
}

// Re-arranging eigendecomposition such first column of X is associated with the largest eigenvalue ...
void CModelingandAnalysisofUncertaintyDoc::SortEigendecomposition(CArray <double>& A, CArray <int>& A_spec, CArray <double>& a) {
	CArray <double> aCopy;
	CArray <double> ACopy;
	int n_rows = A_spec.GetAt(0), pos_1, pos_2;
	CArray <int> Index;
	Index.SetSize(n_rows);
	a.SetSize(n_rows);
	aCopy.SetSize(n_rows);
	ACopy.SetSize(A.GetSize());
	for (int i = 0; i < n_rows; i++) {
		aCopy.SetAt(i, a.GetAt(i));
		for (int j = 0; j < n_rows; j++) {
			pos_1 = GetPosition(i, j, A_spec);
			ACopy.SetAt(pos_1, A.GetAt(pos_1));
		}
	}
	QuickSort(a, 0, n_rows - 1);
	for (int i = 0; i < n_rows; i++) {
		int j = 0;
		while (a.GetAt(i) != aCopy.GetAt(j)) j++;
		Index.SetAt(i, j);
	}
	for (int i = 0; i < n_rows; i++) {
		a.SetAt(i, aCopy.GetAt(Index.GetAt(static_cast<int64_t>(n_rows) - i - 1)));
		for (int j = 0; j < n_rows; j++) {
			pos_1 = GetPosition(j, i, A_spec);
			pos_2 = GetPosition(j, Index.GetAt(static_cast<int64_t>(n_rows) - i - 1), A_spec);
			A.SetAt(pos_1, ACopy.GetAt(pos_2));
		}
	}
}

// Solving standard eigenvalue problem: l � x = A � x if A is symmetric and positive semidefinite
void CModelingandAnalysisofUncertaintyDoc::EVD(CArray <double>& A, CArray <int>& A_spec, CArray <double>& X, CArray <int>& X_spec, CArray <double>& lambda, bool& EV, bool& flag) {
	if (A_spec.GetAt(2) == 1) {
		int n_rows = A_spec.GetAt(0), dim = n_rows, pos_1, pos_2;
		int64_t space = static_cast<int64_t>(n_rows) * dim;
		X.SetSize(space);
		X_spec.SetSize(3);
		X_spec.SetAt(0, n_rows);
		X_spec.SetAt(1, n_rows);
		X_spec.SetAt(2, 0);
		lambda.SetSize(n_rows);
		CArray <double> Temp;
		CArray <double> MainDiagonal, SubDiagonal;
		CArray <int> Temp_spec;
		Temp.SetSize(space);
		MainDiagonal.SetSize(n_rows);
		SubDiagonal.SetSize(n_rows);
		Temp_spec.SetSize(3);
		Temp_spec.SetAt(0, n_rows);
		Temp_spec.SetAt(1, n_rows);
		Temp_spec.SetAt(2, 0);
		for (int i = 0; i < n_rows; i++) {
			for (int j = 0; j < n_rows; j++) {
				pos_1 = GetPosition(i, j, Temp_spec);
				pos_2 = GetPosition(i, j, A_spec);
				Temp.SetAt(pos_1, A.GetAt(pos_2));
			}
		}
		LANCZOS(Temp, Temp_spec, MainDiagonal, SubDiagonal, EV);
		QL(Temp, Temp_spec, MainDiagonal, SubDiagonal, EV);
		SortEigendecomposition(Temp, Temp_spec, MainDiagonal);
		for (int i = 0; i < n_rows; i++) {
			for (int j = 0; j < n_rows; j++) {
				pos_1 = GetPosition(i, j, X_spec);
				X.SetAt(pos_1, Temp.GetAt(pos_1));
			}
			lambda.SetAt(i, MainDiagonal.GetAt(i));
		}
	}
	else {
		flag = true;
	}
}

// *** Computes general eigendecomposition : l텮톥 = B톥
// *** Modules : 1 -> Transformation l x = B* x, where B* = inv(L) � B � inv(L') and L is the Cholesky decomposition of B
// ***         : 2 -> Computing eigendecomposition using standard EVD algorithm above

// Computes the transformation required for solving generalized eigenvalue problem
// l � L � L' � x = B � x : Step 1 -> l � L' � x = inv(L) � B � x
// l � L � L' � x = B � x : Step 2 -> l � z = inv(L) � B � inv(L') � z with x = inv(L') � z 
void CModelingandAnalysisofUncertaintyDoc::Transformation(CArray <double>& Temp, CArray <int>& Temp_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& L, CArray <int>& L_spec) {
	int row = B_spec.GetAt(0), col = B_spec.GetAt(1), dim = row + 1, pos_1, pos_2;
	double temp;
	int64_t space = static_cast<int64_t>(col) * dim;
	Temp.SetSize((int)(space / 2));
	Temp_spec.SetSize(3);
	Temp_spec.SetAt(0, row);
	Temp_spec.SetAt(1, col);
	Temp_spec.SetAt(2, 1);
	CArray <double> Temp_2;
	CArray <int> Temp_2_spec;
	space = static_cast<int64_t>(row) * col;
	Temp_2.SetSize(space);
	Temp_2_spec.SetSize(3);
	Temp_2_spec.SetAt(0, row);
	Temp_2_spec.SetAt(1, col);
	Temp_2_spec.SetAt(2, 0);
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++) {
			temp = (double)0;
			for (int k = 0; k <= i; k++) {
				pos_1 = GetPosition(i, k, L_spec);
				pos_2 = GetPosition(k, j, B_spec);
				temp += L.GetAt(pos_1) * B.GetAt(pos_2);
			}
			pos_1 = GetPosition(i, j, Temp_2_spec);
			Temp_2.SetAt(pos_1, temp);
		}
	for (int i = 0; i < row; i++)
		for (int j = 0; j <= i; j++) {
			temp = (double)0;
			for (int k = 0; k <= i; k++) {
				pos_1 = GetPosition(j, k, Temp_2_spec);
				pos_2 = GetPosition(i, k, L_spec);
				temp += Temp_2.GetAt(pos_1) * L.GetAt(pos_2);
			}
			pos_1 = GetPosition(i, j, Temp_spec);
			Temp.SetAt(pos_1, temp);
		}
}

// Solving generalized eigenvalue problem of the form l � A � x = B � x
void CModelingandAnalysisofUncertaintyDoc::GEVD(CArray <double>& A, CArray <int>& A_spec, CArray <double>& B, CArray <int>& B_spec, CArray <double>& X, CArray <int>& X_spec, CArray <double>& s, int n, bool& flag) {
	if ((A_spec.GetAt(2) == 1) && (B_spec.GetAt(2) == 1)) {
		if (A_spec.GetAt(0) == B_spec.GetAt(0)) {
			int row = B_spec.GetAt(0);
			int col = B_spec.GetAt(1);
			CArray <double> Temp, L, Linv, Z, Ltrans;
			CArray <double> z, x, lambda;
			CArray <int> Temp_spec, L_spec, Linv_spec, Z_spec;
			int64_t space = static_cast <int64_t> (row) * n;
			X.RemoveAll();
			X_spec.RemoveAll();
			X.SetSize(space);
			X_spec.SetSize(3);
			X_spec.SetAt(0, row);
			X_spec.SetAt(1, n);
			X_spec.SetAt(2, 0);
			lambda.SetSize(col);
			s.SetSize(n);
			bool flag = false;
			CHOL(A, A_spec, L, L_spec, flag);
			Inverse(L, L_spec, Linv, Linv_spec);
			SaveMatrix("inv(L).txt", Linv, Linv_spec);
			Transformation(Temp, Temp_spec, B, B_spec, Linv, Linv_spec);
			bool EV = true;
			EVD(Temp, Temp_spec, Z, Z_spec, lambda, EV, flag);
			/*			Transpose(Linv, Linv_spec, Ltrans, Linv_spec);
						CString Text;
						Text.Empty(), Text.Format(L"%d \t %d \t%d", Linv_spec.GetAt(0), Linv_spec.GetAt(1), Linv_spec.GetAt(2));
						SaveMatrix("inv(Ltrans).txt", Ltrans, Linv_spec);
						for (int i = 0; i < n; i++) {
							GetColumn(Z, Z_spec, z, i);
							MatrixVectorProduct(Ltrans, Linv_spec, z, x);
							z.RemoveAll();
							NormalizeVector(x);
							InsertColumn(X, X_spec, x, i);
							x.RemoveAll();
							s.SetAt(i, lambda.GetAt(i));
						}*/
		}
	}
	else flag = true;
}

//*****************************************************************
//***    BEGIN   ->   STANDARDIZE DATA/GET CORRELATION MATRIX   ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::StandardizeDataMatrix(CArray <double>& Data_0, CArray <double>& bar, CArray <double>& std) {
	Data_0.SetSize(Data.GetSize());
	bar.SetSize(n_Var);
	std.SetSize(n_Var);
	double sum, sum_sqr, element, scale;
	int j, pos_Data;
	for (int i = 0; i < n_Var; i++) {
		for (scale = (double)0, j = 0; j < n_Obs; j++) {
			scale = DMAX(scale, Data.GetAt(GetPosition(j, i, Data_spec)));
		}
		for (j = 0; j < n_Obs; j++) {
			Data_0.SetAt(GetPosition(j, i, Data_spec), Data.GetAt(GetPosition(j, i, Data_spec)) / scale);
		}
		for (sum = (double)0, sum_sqr = (double)0, j = 0; j < n_Obs; j++) {
			element = Data_0.GetAt(GetPosition(j, i, Data_spec));
			sum += element;
			sum_sqr += SQR(element);
		}
		sum /= (double)n_Obs;
		sum_sqr -= (double)n_Obs * SQR(sum);
		sum_sqr /= ((double)n_Obs - (double)1);
		bar.SetAt(i, sum * scale);
		std.SetAt(i, sqrt(sum_sqr) * scale);
		if (std.GetAt(i) > 1e-8) {
			for (j = 0; j < n_Obs; j++) {
				pos_Data = GetPosition(j, i, Data_spec);
				scale = Data.GetAt(pos_Data) - bar.GetAt(i);
				scale /= std.GetAt(i);
				Data_0.SetAt(pos_Data, scale);
			}
		}
		else {
			for (j = 0; j < n_Obs; j++) {
				Data_0.SetAt(GetPosition(j, i, Data_spec), (double)0);
			}
		}
	}
}

void CModelingandAnalysisofUncertaintyDoc::GetCorrelationMatrix(CArray <double>& Data0, CArray <int>& Data_0_spec, CArray <double>& R, CArray <int>& R_spec, bool OnlyX) {
	if (OnlyX == true) {
		CArray <int> Data0_spec;
		Data0_spec.SetSize(3);
		Data0_spec.SetAt(0, n_Obs);
		Data0_spec.SetAt(1, n_Var - 1);
		Data0_spec.SetAt(2, Data_spec.GetAt(2));
		X_tr_X(Data0, Data0_spec, R, R_spec);
	}
	else X_tr_X(Data0, Data_spec, R, R_spec);
	for (int i = 0; i < R.GetSize(); i++) R.SetAt(i, R.GetAt(i) / (double)(n_Obs - (int)1));
}


//*****************************************************************
//***    BEGIN -> FILE OPERATIONS (OPEN and SAVE DATA FILES)    ***
//*****************************************************************

BOOL CModelingandAnalysisofUncertaintyDoc::OnOpenDocument(LPCTSTR lpszPathName) {
	LPCTSTR FileAndPathName = lpszPathName;					// global CString equal to the path of the file selected to be opened
	CStdioFile	ReadFile;									// CStdioFile object used to hold file data
	CString ReadRow;
	PathAndFileName.Empty();
	PathAndFileName.Append(lpszPathName);
	Application.Empty();
	OnOpenedFile = true;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = false;
	ANOVA = false;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Loadings = false;
	PCA_Display_Scores = false;
	if (!ReadFile.Open(FileAndPathName, CFile::modeRead | CFile::modeNoTruncate)) {	// file opened for Read only and its a text file
		TRACE(_T("CGuardianDoc::OnOpenDocument() - Failed to open file %s\n"), ReadRow);
		return false;
	}
	BOOL b = ReadFile.ReadString(ReadRow);
	ASSERT(TRUE == b);
	if (b == false) {
		TRACE(_T("CGuardianDoc::OnOpenDocument() - Failed to open file %s\n"), ReadRow);
		return false;
	}
	ReadRow.TrimLeft();
	ReadRow.TrimRight();
	// Read number of variables
	int start = 0, end = ReadRow.Find(_T(','), start);
	CString n_var = ReadRow.Mid(start, end - start);
	n_Var = _ttoi(n_var);
	// Read number of observations
	bool EndOfRow = false;
	start = end + 1;
	end = ReadRow.Find(_T(','), start);
	CString n_obs = ReadRow.Mid(start, end - start);
	n_Obs = _ttoi(n_obs);
	// Read date when datafile was established
	start = end + 1;
	if (start <= ReadRow.GetLength()) {
		end = ReadRow.Find(_T(','), start);
		DateFirst = ReadRow.Mid(start, end - start);
	}
	// Time when the datafile was established
	start = end + 1;
	if (start <= ReadRow.GetLength()) {
		end = ReadRow.Find(_T(','), start);
		TimeFirst = ReadRow.Mid(start, end - start);
	}
	// Data when datafile was last accessed
	start = end + 1;
	if (start <= ReadRow.GetLength()) {
		end = ReadRow.Find(_T(','), start);
		DateLast = ReadRow.Mid(start, end - start);
	}
	// Time when datafile was last accessed
	start = end + 1;
	if (start <= ReadRow.GetLength()) {
		end = ReadRow.Find(_T(','), start);
		if (end > 0) TimeLast = ReadRow.Mid(start, end - start);
		else {
			TimeLast = ReadRow.Mid(start, ReadRow.GetLength() - start);
			EndOfRow = true;
		}
	}
	// Name of Application
	if (EndOfRow == false) {
		start = end + 1;
		if (start <= ReadRow.GetLength()) {
			end = ReadRow.GetLength();
			Application.Append(ReadRow.Mid(start, end - start));
		}
	}
	else Application.Append(_T("Not specified"));
	// allocate memory for Tag, Name, Unit and Data
	Tag.SetSize(n_Var);
	Name.SetSize(n_Var);
	Unit.SetSize(n_Var);
	int64_t space = static_cast<int64_t>(n_Obs) * n_Var;
	Data.SetSize(space);
	Data_spec.SetSize(3);
	Data_spec.SetAt(0, n_Obs);
	Data_spec.SetAt(1, n_Var);
	Data_spec.SetAt(2, 0);
	ReadFile.ReadString(ReadRow);
	ReadRow.TrimLeft();
	ReadRow.TrimRight();
	start = 0;
	for (int i = 0; i < n_Var; i++) {
		end = ReadRow.Find(',', start);
		if (i == n_Var - 1) end = ReadRow.GetLength();
		if (end - start > 0) Tag[i] = ReadRow.Mid(start, end - start);
		else Tag[i] = _T("");
		start = end + 1;
	}
	ReadFile.ReadString(ReadRow);
	ReadRow.TrimLeft();
	ReadRow.TrimRight();
	start = 0;
	for (int i = 0; i < n_Var; i++) {
		end = ReadRow.Find(',', start);
		if (i == n_Var - 1) end = ReadRow.GetLength();
		if (end - start > 0) Name[i] = ReadRow.Mid(start, end - start);
		else Name[i] = _T("");
		start = end + 1;
	}
	ReadFile.ReadString(ReadRow);
	ReadRow.TrimLeft();
	ReadRow.TrimRight();
	start = 0;
	for (int i = 0; i < n_Var; i++) {
		end = ReadRow.Find(',', start);
		if (i == n_Var - 1) end = ReadRow.GetLength();
		if (end - start > 0) Unit[i] = ReadRow.Mid(start, end - start);
		else Unit[i] = _T("");
		start = end + 1;
	}
	double temp;
	for (int i = 0; i < n_Obs; i++) {
		ReadFile.ReadString(ReadRow);
		ReadRow.TrimLeft();
		ReadRow.TrimRight();
		start = 0;
		int pos;
		for (int j = 0; j < n_Var; j++) {
			end = ReadRow.Find(',', start);
			if (j == n_Var - 1) end = ReadRow.GetLength();
			pos = GetPosition(i, j, Data_spec);
			temp = _ttof(ReadRow.Mid(start, end - start));
			Data.SetAt(pos, temp);
			start = end + 1;
		}
	}
	ReadFile.Close();
	UpdateAllViews(NULL);
	CString WriteRow;
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	int Year = 1900 + ltm.tm_year;
	int Month = 1 + ltm.tm_mon;
	int Day = ltm.tm_mday;
	int Hour = ltm.tm_hour;
	int Minute = ltm.tm_min;
	int Second = ltm.tm_sec;
	DateLast.Empty();
	DateLast.Format(L"%d/%d/%d", Month, Day, Year);
	TimeLast.Empty();
	TimeLast.Format(L"%d:%d:%d", Hour, Minute, Second);
	ReadFile.Open(FileAndPathName, CFile::modeWrite | CFile::modeCreate);
	WriteRow.Empty();
	n_var.Format(L"%d", n_Var);
	n_obs.Format(L"%d", n_Obs);
	WriteRow.Append(n_var + _T(',') + n_obs + _T(',') + DateFirst + _T(',') + TimeFirst + _T(',') + DateLast + _T(',') + TimeLast);
	if (!Application.IsEmpty()) WriteRow.Append(_T(',') + Application + _T('\n'));
	ReadFile.WriteString(WriteRow);
	WriteRow.Empty();
	for (int i = 0; i < n_Var; i++) {
		if (i == n_Var - 1) WriteRow.Append(Tag[i] + _T('\n'));
		else WriteRow.Append(Tag[i] + _T(','));
	}
	ReadFile.WriteString(WriteRow);
	WriteRow.Empty();
	for (int i = 0; i < n_Var; i++) {
		if (i == n_Var - 1) WriteRow.Append(Name[i] + _T('\n'));
		else WriteRow.Append(Name[i] + _T(','));
	}
	ReadFile.WriteString(WriteRow);
	WriteRow.Empty();
	for (int i = 0; i < n_Var; i++) {
		if (i == n_Var - 1) WriteRow.Append(Unit[i] + _T('\n'));
		else WriteRow.Append(Unit[i] + _T(','));
	}
	ReadFile.WriteString(WriteRow);
	CString str;
	int pos;
	for (int i = 0; i < n_Obs; i++) {
		WriteRow.Empty();
		for (int j = 0; j < n_Var; j++) {
			pos = GetPosition(i, j, Data_spec);
			str.Format(L"%f", Data.GetAt(pos));
			if (j == n_Var - 1) WriteRow.Append(str + _T('\n'));
			else WriteRow.Append(str + _T(','));
		}
		ReadFile.WriteString(WriteRow);
	}
	ReadFile.Close();
	CArray <double> nValues;
	double value;
	bool flag;
	nValues.RemoveAll();
	pos = n_Var - 1;
	value = Data.GetAt(GetPosition(0, pos, Data_spec));
	nValues.Add(value);
	if (Tag.GetAt(pos) == "y") {
		for (int i = 1; i < n_Obs; i++) {
			value = Data.GetAt(GetPosition(i, pos, Data_spec));
			flag = false;
			for (int j = 0; j < nValues.GetSize(); j++) {
				if (value == nValues.GetAt(j)) flag = true;
			}
			if (flag == false) nValues.Add(value);
		}
		if (nValues.GetSize() < (int)(n_Obs / 3)) {
			Problem.Empty();
			Problem.Append(_T("Classification problem, "));
			CString Temp;
			Temp.Empty();
			int n_Values = (int)nValues.GetSize();
			Temp.Format(L"%d", n_Values);
			Problem.Append(Temp + _T(" classes"));
			n_classes = n_Values;
			OnlyX = true;
		}
		else {
			Problem.Empty();
			Problem.Append(_T("Regression problem"));
			OnlyX = false;
		}
	}
	else {
		Problem.Empty();
		Problem.Append(_T("Analysis problem"));
		OnlyX = true;
	}
	FileOpen = true;
	StartWindow = false;
	return true;
}

BOOL CModelingandAnalysisofUncertaintyDoc::OnSaveDocument(LPCTSTR lpszPathName) {
	LPCTSTR FileAndPathName = lpszPathName;					// global CString equal to the path of the file selected to be opened
	CStdioFile	WriteFile;									// CStdioFile object used to hold file data
	CString WriteRow;
	if (!WriteFile.Open(FileAndPathName, CFile::modeWrite | CFile::modeCreate)) {	// file opened for Read only and its a text file
		TRACE(_T("CGuardianDoc::OnOpenDocument() - Failed to open file %s\n"), WriteRow);
		return false;
	}
	CArray <double> Data_0;
	CArray <double> Rxx;
	CArray <int> Rxx_spec;
	CArray <double> bar;
	CArray <double> std;
	CArray <double> X;
	CArray <int> X_spec;
	CArray <double> lambda;
	bool EV = true, flag = false;
	CString Temp;
	int pos_A;
	WriteRow.Empty();
	WriteRow.Append(_T("Correlation matrix Rxx:\n"));
	WriteFile.WriteString(WriteRow);
	for (int i = 0; i < Rxx_spec.GetAt(0); i++) {
		WriteRow.Empty();
		for (int j = 0; j <= i; j++) {
			Temp.Empty();
			pos_A = GetPosition(i, j, Rxx_spec);
			Temp.Format(L"%f", Rxx.GetAt(pos_A));
			WriteRow.Append(Temp + _T("\t"));
		}
		WriteRow.Append(_T("\n"));
		WriteFile.WriteString(WriteRow);
	}
	WriteRow.Empty();
	WriteRow.Append(_T("Matrix of eigenvectors X:\n"));
	WriteFile.WriteString(WriteRow);
	for (int i = 0; i < X_spec.GetAt(0); i++) {
		WriteRow.Empty();
		for (int j = 0; j < X_spec.GetAt(1); j++) {
			Temp.Empty();
			pos_A = GetPosition(i, j, X_spec);
			Temp.Format(L"%f", X.GetAt(pos_A));
			WriteRow.Append(Temp + _T("\t"));
		}
		WriteRow.Append(_T("\n"));
		WriteFile.WriteString(WriteRow);
	}
	WriteRow.Empty();
	WriteRow.Append(_T("Eigenvalues:\n"));
	WriteFile.WriteString(WriteRow);
	for (int j = 0; j < lambda.GetSize(); j++) {
		WriteRow.Empty();
		Temp.Empty();
		Temp.Format(L"%f", lambda.GetAt(j));
		WriteRow.Append(Temp + _T("\n"));
		WriteFile.WriteString(WriteRow);
	}
	WriteFile.Close();
	return true;
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CModelingandAnalysisofUncertaintyDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) {
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CModelingandAnalysisofUncertaintyDoc::InitializeSearchContent() {
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CModelingandAnalysisofUncertaintyDoc::SetSearchContent(const CString& value) {
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CModelingandAnalysisofUncertaintyDoc diagnostics


#ifdef _DEBUG
void CModelingandAnalysisofUncertaintyDoc::AssertValid() const {
	CDocument::AssertValid();
}

void CModelingandAnalysisofUncertaintyDoc::Dump(CDumpContext& dc) const {
	CDocument::Dump(dc);
}
#endif

// ****************************************************************
// ***   BEGIN  ->  RESPONSES TO ON COMMAND SELECTIONS BY USER  ***
// ****************************************************************

//*****************************************************************
//***              Compute descriptive statistics               ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnDescriptiveStatistics() {
	int j, k, Tick = 0;
	CArray <int> NTicks, max, min, ticks, ticklabels, tick;
	double mean, var, sum, sum_sqr, element, factor, bandwidth;
	OnOpenedFile = false;
	DescriptiveStatistics = true;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = false;
	ShapiroWilkTest = false;
	AndersonDarlingTest = false;
	ANOVA = false;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Scores = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = false;
	CArray <double> Observations, outliers, max_Drawing, min_Drawing, Factor, SubTicks;
	bool tick_flag, flag;
	Data_ordered.SetSize(Data.GetSize());
	if (MIN.IsEmpty()) {
		MIN.SetSize(n_Var);
		MAX.SetSize(n_Var);
		Q1.SetSize(n_Var);
		Q2.SetSize(n_Var);
		Q3.SetSize(n_Var);
		RANGE.SetSize(n_Var);
		IQR.SetSize(n_Var);
		MEAN.SetSize(n_Var);
		STD.SetSize(n_Var);
		SE.SetSize(n_Var);
		SKEW.SetSize(n_Var);
		KURT.SetSize(n_Var);
		h.SetSize(n_Var);
		OUTLIERS.SetSize(n_Var);
		n_OUTLIERS.SetSize(n_Var);
		UB.SetSize(n_Var);
		LB.SetSize(n_Var);
	}
	else {
		MIN.RemoveAll(), MIN.SetSize(n_Var);
		MAX.RemoveAll(), MAX.SetSize(n_Var);
		Q1.RemoveAll(), Q1.SetSize(n_Var);
		Q2.RemoveAll(), Q2.SetSize(n_Var);
		Q3.RemoveAll(), Q3.SetSize(n_Var);
		RANGE.RemoveAll(), RANGE.SetSize(n_Var);
		IQR.RemoveAll(), IQR.SetSize(n_Var);
		MEAN.RemoveAll(), MEAN.SetSize(n_Var);
		STD.RemoveAll(), STD.SetSize(n_Var);
		SE.RemoveAll(), SE.SetSize(n_Var);
		SKEW.RemoveAll(), SKEW.SetSize(n_Var);
		KURT.RemoveAll(), KURT.SetSize(n_Var);
		h.RemoveAll(), h.SetSize(n_Var);
		OUTLIERS.RemoveAll(), OUTLIERS.SetSize(n_Var);
		n_OUTLIERS.RemoveAll(), n_OUTLIERS.SetSize(n_Var);
		UB.RemoveAll(), UB.SetSize(n_Var);
		LB.RemoveAll(), LB.SetSize(n_Var);
	}
	max.RemoveAll();
	max.SetSize(n_Var);
	min.RemoveAll();
	min.SetSize(n_Var);
	max_Drawing.RemoveAll();
	max_Drawing.SetSize(n_Var);
	min_Drawing.RemoveAll();
	min_Drawing.SetSize(n_Var);
	Factor.SetSize(n_Var);
	int nVar;
	if (Problem == L"Regression problem") nVar = n_Var;
	else nVar = n_Var - 1;
	for (int i = 0; i < nVar; i++) {
		// code for computing descriptive statistics
		flag = false;
		Observations.SetSize(n_Obs);
		for (j = 0; j < n_Obs; j++) {
			Observations.SetAt(j, Data.GetAt(GetPosition(j, i, Data_spec)));
		}
		QuickSort(Observations, 0, n_Obs - 1);
		for (int j = 0; j < n_Obs; j++) Data_ordered.SetAt(GetPosition(j, i, Data_spec), Observations.GetAt(j));
		MIN.SetAt(i, Observations.GetAt(0));
		MAX.SetAt(i, Observations.GetAt(static_cast <int64_t>(n_Obs) - 1));
		int n_ref, rem = (n_Obs % 4), n_25 = (int)(n_Obs / 4), n_75 = (int)((3 * n_Obs) / 4);
		double a = (double)(rem) / (double)(4), x1, x2, skew, kurt;
		x1 = Observations.GetAt(static_cast <int64_t>(n_25) - 1);
		x2 = Observations.GetAt(n_25);
		double q1 = a * x1 + ((double)1 - a) * x2;
		Q1.SetAt(i, q1);
		if ((n_Obs % 2) == 0) {
			n_ref = n_Obs / (int)2;
			x1 = Observations.GetAt(static_cast <int64_t>(n_ref) - 1);
			x2 = Observations.GetAt(n_ref);
			Q2.SetAt(i, (x1 + x2) / (double)2);
		}
		else {
			n_ref = (int)((n_Obs + 1) / 2) - 1;
			Q2.SetAt(i, Observations.GetAt(n_ref));

		}
		x1 = Observations.GetAt(static_cast <int64_t>(n_75) - 1);
		x2 = Observations.GetAt(static_cast <int64_t>(n_75));
		double q3 = ((double)1 - a) * x1 + a * x2;
		Q3.SetAt(i, q3);
		RANGE.SetAt(i, MAX.GetAt(i) - MIN.GetAt(i));
		IQR.SetAt(i, q3 - q1);
		k = n_Obs - 1;
		outliers.RemoveAll();
		while (Observations.GetAt(k) > q3 + 1.5 * IQR.GetAt(i)) {
			outliers.Add(Observations.GetAt(k));
			k--;
		}
		UB.SetAt(i, Observations.GetAt(k));
		k = 0;
		while (Observations.GetAt(k) < q1 - 1.5 * IQR.GetAt(i)) {
			outliers.Add(Observations.GetAt(k));
			k++;
		}
		LB.SetAt(i, Observations.GetAt(k));
		OUTLIERS[i] = new double[(int)outliers.GetSize()];
		for (j = 0; j < (int)outliers.GetSize(); j++) OUTLIERS[i][j] = outliers.GetAt(j);
		n_OUTLIERS.SetAt(i, (int)outliers.GetSize());
		for (j = 0; j < n_Obs; j++) {
			Observations.SetAt(j, Observations.GetAt(j) / MAX.GetAt(i));
		}
		for (sum = (double)0, sum_sqr = (double)0, j = 0; j < n_Obs; j++) {
			element = Observations.GetAt(j);
			sum += element;
			sum_sqr += SQR(element);
		}
		mean = sum / (double)n_Obs;
		MEAN.SetAt(i, mean * MAX.GetAt(i));
		var = sum_sqr / (double)(n_Obs - 1) - ((double)n_Obs / (double)(n_Obs - 1)) * mean * mean;
		STD.SetAt(i, sqrt(var) * MAX.GetAt(i));
		SE.SetAt(i, STD.GetAt(i) / sqrt((double)n_Obs));
		for (skew = 0, kurt = 0, j = 0; j < n_Obs; j++) {
			skew += pow(Observations.GetAt(j) - mean, (double)3);
			kurt += pow(Observations.GetAt(j) - mean, (double)4);
		}
		skew /= pow(var, 1.5);
		skew /= (double)(n_Obs - 1);
		kurt /= var * var;
		kurt /= (double)(n_Obs - 1);
		SKEW.SetAt(i, skew);
		KURT.SetAt(i, kurt);
		// code for constructing boxplot 
		max_Drawing.SetAt(i, MAX.GetAt(i) + 0.025 * RANGE.GetAt(i));
		min_Drawing.SetAt(i, MIN.GetAt(i) - 0.025 * RANGE.GetAt(i));
		factor = 1;
		tick_flag = false;
		// Get optimal bandwidth
		CArray <double> Sample;
		double Value;
		Sample.RemoveAll();
		Sample.SetSize(n_Obs);
		for (int j = 0; j < n_Obs; j++) {
			Value = Data.GetAt(GetPosition(j, i, Data_spec));
			Value -= MEAN.GetAt(i);
			Value /= STD.GetAt(i);
			Sample.SetAt(j, Value);
		}
		bandwidth = GetOptimalBandwidth(Sample);
		h.SetAt(i, bandwidth);
	}
	UpdateAllViews(NULL);
}

double CModelingandAnalysisofUncertaintyDoc::GetOptimalBandwidth(CArray <double>& sample) {
	int nObs = (int)sample.GetSize();
	if (nObs > 1000) {
		CArray <double> sample_copy;
		sample_copy.RemoveAll();
		std::vector<int> index;
		index.clear();
		for (int i = 0; i < n_Obs; i++) index.push_back(i);
		std::random_shuffle(index.begin(), index.end());
		for (int i = 0; i < 1000; i++) {
			sample_copy.Add(sample.GetAt(index[i]));
		}
		sample.RemoveAll();
		for (int i = 0; i < 1000; i++) sample.Add(sample_copy.GetAt(i));
		sample_copy.RemoveAll();
	}
	double h0 = pow((double)(4 / 3), 0.2 * pow((double)nObs, 0.2));
	double h_lower = 0.1 * h0, h_upper = 5 * h0;
	double J1, J2, dJ, h1 = 0.4, h2 = 0.5, dh, eps = 100, lambda = 5;
	J1 = GetObjectiveFunctionKDE(sample, h1);
	while (eps > 1e-4) {
		J2 = GetObjectiveFunctionKDE(sample, h2);
		dJ = (J2 - J1) / (h2 - h1);
		h1 = h2;
		dh = lambda * dJ;
		eps = fabs(dh);
		h2 = h2 - dh;
		if (h2 > h_upper) {
			h2 = h_upper;
			break;
		}
		if (h2 < h_lower) {
			h2 = h_lower;
			break;
		}
		J1 = J2;
	}
	return h2;
}

double CModelingandAnalysisofUncertaintyDoc::GetObjectiveFunctionKDE(CArray <double>& sample, double h) {
	CArray <double> f;
	f.RemoveAll();
	int n = (int)sample.GetSize();
	double par_1 = sqrt(2) * h, par_2 = 2.0 * par_1 * par_1, arg, term;
	double pi = 3.14159265358979311599796346854418516159057617187500;
	double PI_2 = sqrt(2 * pi), J = 0.0, Term_1 = 0.0, Term_2 = 0.0, xi, xj;
	J = 1 / (par_1 * PI_2 * (double)(n - 1));
	for (int i = 0; i < n; i++) {
		xi = sample.GetAt(i);
		for (int j = 0; j < n; j++) {
			if (i != j) {
				xj = sample.GetAt(j);
				arg = pow(xi - xj, (double)2);
				term = exp(-arg / par_2);
				Term_1 += term;
				Term_2 += term * term;
			}
		}
	}
	Term_1 *= (double)(n - 2) / ((double)n * pow((double)(n - 1), 2.0) * PI_2 * par_1);
	Term_2 *= (double)2 / ((double)(n * (n - 1)) * PI_2 * h);
	J += Term_1 - Term_2;
	return J;
}

double CModelingandAnalysisofUncertaintyDoc::NumericalIntegration(CArray <double>& f, double h) {
	int n = (int)f.GetSize(), m = (int)(n / 2);
	double Area = f.GetAt(0) + f.GetAt(static_cast <int64_t>(n - 1));
	double Temp_1 = 0.0, Temp_2 = 0.0;
	for (int i = 1; i <= m; i++) {
		Temp_1 += f.GetAt(static_cast <int64_t>(2 * i - 1));
	}
	for (int i = 1; i < m; i++) {
		Temp_2 += f.GetAt(static_cast <int64_t>(2 * i));
	}
	Area += 4.0 * Temp_1 + 2.0 * Temp_2;
	return h * Area / 3.0;
}

//*****************************************************************
//*** Compute results for one-sample hypothesis testing problem ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnOneSample() {
	CWnd* pParent = nullptr;
	COneSampleHypothesisSelection Selection(pParent, &Name);
	OnOpenedFile = false;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = true;
	HypothesisTesting_TwoSample = false;
	ShapiroWilkTest = false;
	AndersonDarlingTest = false;
	ANOVA = false;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Scores = false;
	PCA_Display_Loadings = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = false;
	std::ofstream FILE;
	FILE.open("HypothesisTesting.txt");
	if (Selection.DoModal() == IDOK) {
		CArray <double> Sample;
		Sample.RemoveAll();
		double value, max = 0;
		int nObs = 0;
		VariableSelected = Selection.SelectedVariable;
		for (int i = 0; i < n_Obs; i++) {
			if ((int)Data.GetAt(static_cast <int64_t>(GetPosition(i, n_Var - 1, Data_spec))) == Selection.SelectedClass) {
				value = Data.GetAt(GetPosition(i, VariableSelected, Data_spec));
				Sample.Add(value);
				if (nObs == 0) {
					max = value;
					nObs++;
				}
				else {
					if (value > max) max = value;
					nObs++;
				}
			}
		}
		double mean = 0, std = 0;
		for (int i = 0; i < nObs; i++) {
			value = Sample.GetAt(i);
			value /= max;
			mean += value;
			std += value * value;
		}
		mean /= (double)nObs;
		std -= (double)nObs * mean * mean;
		std /= ((double)nObs - (double)1);
		std = sqrt(std);
		FILE << mean * max << "\t" << std * max << "\t";
		if (Selection.Mean == 0) {
			FILE << Selection.SelectedVariable << "\t";
			double mu = Selection.Value_0;
			double xbar = mean * max;
			double alpha = Selection.alpha;
			double variance = 1.0;
			double T = 0, Z = 0;
			if (Selection.Selection_knownVariance == 0) {
				variance = Selection.Variance;
				Z = sqrt(nObs) * (xbar - mu) / sqrt(variance);
				ProbabilityDistributions normal('n', 0, 1);
				double zcrit, p_value;
				if (Selection.OneTwoSided == 0) zcrit = normal.CriticalValue(alpha / 2);
				else zcrit = normal.CriticalValue(alpha);
				p_value = 2 * normal.Compute_pValue(Z);
				FILE << "z-Statistic = " << Z << "\t" << zcrit << "\t" << p_value << std::endl;
			}
			else {
				T = sqrt(nObs) * (xbar - mu) / (std * max);
				FILE << "t-Statistic = " << T << std::endl;
				ProbabilityDistributions t('t', (double)(nObs - 1), 0);
				double tcrit, p_value, Conf_U, Conf_L, Crit_L, Crit_U;
				if (Selection.OneTwoSided == 0) {
					tcrit = t.CriticalValue(alpha / 2);
					p_value = 2 * t.Compute_pValue(fabs(T));
					Conf_U = xbar + tcrit * std * max / sqrt(nObs);
					Conf_L = xbar - tcrit * std * max / sqrt(nObs);
					Crit_U = mu + tcrit * std * max / sqrt(nObs);
					Crit_L = mu - tcrit * std * max / sqrt(nObs);
					FILE << "H0 : mu = " << mu << std::endl;
					FILE << "H1 : mu neq " << mu << std::endl;
					FILE << "tcrit = " << tcrit << std::endl;
					FILE << "Two-sided confidence interval, L = " << Conf_L << " and U = " << Conf_U << std::endl;
					FILE << "p-Value = " << p_value << std::endl;
					FILE << "Critical region, L = " << Crit_L << " and U = " << Crit_U << std::endl;
					FILE << "Type I error probability = " << alpha << std::endl;
					if (p_value >= alpha) FILE << "Cannot reject H0" << std::endl;
					else FILE << "Must reject H0" << std::endl;
				}
				else if (Selection.OneTwoSided == 1) {
					CString Text;
					Text.Empty();
					tcrit = t.CriticalValue(alpha);
					p_value = 1 - t.Compute_pValue(T);
					Conf_U = xbar + tcrit * std * max / sqrt(nObs);
					Crit_L = mu - tcrit * std * max / sqrt(nObs);
					FILE << "H0 : mu = " << mu << std::endl;
					FILE << "H1 : mu < " << mu << std::endl;
					FILE << "tcrit = " << -tcrit << std::endl;
					FILE << "One-sided upper confidence interval : mu < U = " << Conf_U << std::endl;
					FILE << "p-Value = " << p_value << std::endl;
					FILE << "Critical region, xbar < L = " << Crit_L << std::endl;
					FILE << "Type I error probability = " << alpha << std::endl;
					if (p_value >= alpha) FILE << "Cannot reject H0" << std::endl;
					else FILE << "Must reject H0" << std::endl;
				}
				else {
					tcrit = t.CriticalValue(alpha);
					p_value = t.Compute_pValue(T);
					Conf_L = xbar - tcrit * std * max / sqrt(nObs);
					Crit_U = mu + tcrit * std * max / sqrt(nObs);
					FILE << "H0 : mu = " << mu << std::endl;
					FILE << "H1 : mu > " << mu << std::endl;
					FILE << "tcrit = " << tcrit << std::endl;
					FILE << "One-sided lower confidence interval : mu > L = " << Conf_L << std::endl;
					FILE << "p-Value = " << p_value << std::endl;
					FILE << "Critical region, xbar > U = " << Crit_U << std::endl;
					FILE << "Type I error probability = " << alpha << std::endl;
					if (p_value >= alpha) FILE << "Cannot reject H0" << std::endl;
					else FILE << "Must reject H0" << std::endl;
				}
			}
		}
		else {
			ProbabilityDistributions Chi('C', (double)(nObs - 1), 0);
		}
	}
	FILE.close();
	UpdateAllViews(NULL);
}

//*****************************************************************
//*** Compute results for two-sample hypothesis testing problem ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnTwoSample() {

	CWnd* pParent = nullptr;
	CTwoSampleHypothesisSelection digSetText(pParent, &Name);

	OnOpenedFile = false;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = true;
	ShapiroWilkTest = true;
	AndersonDarlingTest = false;
	ANOVA = false;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Scores = false;
	PCA_Display_Loadings = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = false;

	std::ofstream FILE;

	FILE.open("HypothesisTesting.txt");

	if (digSetText.DoModal() == IDOK) {

		//Sample1

		Sample1.RemoveAll();
		double value1, max1 = 0.0;
		double good1, min1 = 0.0;
		int nObs1 = 0;

		for (int i = 0; i < n_Obs; i++) {
			if ((int)Data.GetAt(static_cast <int64_t>(GetPosition(i, n_Var - 1, Data_spec))) == digSetText.SelectedClass_1) {
				value1 = Data.GetAt(GetPosition(i, digSetText.SelectedVariable, Data_spec));
				Sample1.Add(value1);
				nObs1++;

				if (nObs1 == 0) {
					max1 = value1;

				}
				else {
					if (value1 > max1) max1 = value1;

				}
			}
		}


		OverallArray.RemoveAll();
		for (int i = 0; i < nObs1; i++) {
			double value = Sample1.GetAt(i);
			OverallArray.Add(value);
		}

		double mean1 = 0, std1 = 0;
		for (int i = 0; i < nObs1; i++) {
			value1 = Sample1.GetAt(i);
			value1 /= max1;
			mean1 += value1;
			std1 += value1 * value1;
		}
		mean1 /= (double)nObs1;
		std1 -= (double)nObs1 * mean1 * mean1;
		std1 /= ((double)nObs1 - (double)1);




		std1 = sqrt(std1);
		FILE << "Mean1 = " << mean1 * max1 << "\t" << "Standard deviation1 = " << std1 * max1 << "\t";




		//Sample2

		Sample2.RemoveAll();
		double value2, max2 = 0.0;
		double good2, min2 = 0.0;
		int nObs2 = 0;

		for (int i = 0; i < n_Obs; i++) {
			if ((int)Data.GetAt(static_cast <int64_t>(GetPosition(i, n_Var - 1, Data_spec))) == digSetText.SelectedClass_2) {
				value2 = Data.GetAt(GetPosition(i, digSetText.SelectedVariable, Data_spec));
				Sample2.Add(value2);
				nObs2++;
				if (nObs2 == 0) {
					max2 = value2;

				}
				else {
					if (value2 > max2) max2 = value2;

				}
			}
		}

		for (int i = 0; i < nObs2; i++) {
			double value = Sample2.GetAt(i);
			OverallArray.Add(value);
		}


		double mean2 = 0, std2 = 0;
		for (int i = 0; i < nObs2; i++) {
			value2 = Sample2.GetAt(i);
			value2 /= max2;
			mean2 += value2;
			std2 += value2 * value2;
		}
		mean2 /= (double)nObs2;
		std2 -= (double)nObs2 * mean2 * mean2;
		show_nObs2 = nObs2;
		std2 /= ((double)nObs2 - (double)1);
		std2 = sqrt(std2);


		FILE << "Mean2 = " << mean2 * max2 << "\t" << "Standard deviation2 = " << std2 * max2 << "\t";


		// ready to draw the box plot

		//sort two samples
		QuickSort(Sample1, 0, nObs1 - 1);
		QuickSort(Sample2, 0, nObs2 - 1);
		FILE << '\n';
		FILE << "\nSample 1 Data:  ";
		for (int i = 0; i < nObs1; i++) {
			FILE << Sample1.GetAt(i) << '\t';

		}
		FILE << '\n';
		FILE << "\nSample 2 Data:  ";
		for (int i = 0; i < nObs2; i++) {
			FILE << Sample2.GetAt(i) << '\t';

		}

		minimum_sample1 = Sample1.GetAt(0);
		minimum_sample2 = Sample2.GetAt(0);


		// find q2, q1, q3

		//q2 is the medium of Sample1 
		//Q2 is the medium of Sample2

		double q2, Q2, q1, Q1, q3, Q3, iqr, IQR;

		if (nObs2 % 2 == 0) {

			Q2 = Sample2.GetAt((nObs2 / 2) - 1) + Sample2.GetAt(nObs2 / 2);
			Q2 /= 2;
			FILE << "\nQ2 for Sample2:  " << Q2;

			int temp = nObs2 / 2; // temp denots the number of items after seprared into two groups
			if (temp % 2 == 0) {
				Q1 = Sample2.GetAt((temp / 2) - 1) + Sample2.GetAt(temp / 2);
				Q1 /= 2;
				FILE << "\nQ1 for Sample2:  " << Q1;

				Q3 = Sample2.GetAt(temp + temp / 2 - 1) + Sample2.GetAt(temp + temp / 2);
				Q3 /= 2;
				FILE << "\nQ3 for Sample2:  " << Q3;
			}

			else {
				Q1 = Sample2.GetAt(((temp + 1) / 2) - 1);
				FILE << "\nQ1 for Sample2:  " << Q1;

				Q3 = Sample2.GetAt(temp + ((temp + 1) / 2) - 1);
				FILE << "\nQ3 for Sample2:  " << Q3;
			}


		}

		else {

			Q2 = Sample2.GetAt(((nObs2 + 1) / 2) - 1);
			FILE << "\nQ2 for Sample2:  " << Q2;

			int temp = (nObs2 - 1) / 2;
			if (temp % 2 == 0) {
				Q1 = Sample2.GetAt((temp / 2) - 1) + Sample2.GetAt(temp / 2);
				Q1 /= 2;
				FILE << "\nQ1 for Sample2:  " << Q1;

				Q3 = Sample2.GetAt(temp + 1 + temp / 2 - 1) + Sample2.GetAt(temp + 1 + temp / 2);
				Q3 /= 2;
				FILE << "\nQ3 for Sample2:  " << Q3;
			}

			else {

				Q1 = Sample2.GetAt(((temp + 1) / 2) - 1);
				FILE << "\nQ1 for Sample2:  " << Q1;

				Q3 = Sample2.GetAt(temp + 1 + ((temp + 1) / 2) - 1);
				FILE << "\nQ3 for Sample2:  " << Q3;


			}


		}

		if (nObs1 % 2 == 0) {

			q2 = Sample1.GetAt((nObs1 / 2) - 1) + Sample1.GetAt(nObs1 / 2);
			q2 /= 2;
			FILE << "\nQ2 for Sample1:  " << q2;

			int temp = nObs1 / 2; // temp denots the number of items after seprared into two groups
			if (temp % 2 == 0) {
				q1 = Sample1.GetAt((temp / 2) - 1) + Sample1.GetAt(temp / 2);
				q1 /= 2;
				FILE << "\nQ1 for Sample1:  " << q1;

				q3 = Sample1.GetAt(temp + temp / 2 - 1) + Sample1.GetAt(temp + temp / 2);
				q3 /= 2;
				FILE << "\nQ3 for Sample1:  " << q3;
			}

			else {
				q1 = Sample1.GetAt(((temp + 1) / 2) - 1);
				FILE << "\nQ1 for Sample1:  " << q1;

				q3 = Sample1.GetAt(temp + ((temp + 1) / 2) - 1);
				FILE << "\nQ3 for Sample1:  " << q3;
			}


		}

		else {

			q2 = Sample1.GetAt(((nObs1 + 1) / 2) - 1);
			FILE << "\nQ2 for Sample1:  " << q2;

			int temp = (nObs1 - 1) / 2;
			if (temp % 2 == 0) {
				q1 = Sample1.GetAt((temp / 2) - 1) + Sample1.GetAt(temp / 2);
				q1 /= 2;
				FILE << "\nQ1 for Sample1:  " << q1;

				q3 = Sample1.GetAt(temp + 1 + temp / 2 - 1) + Sample1.GetAt(temp + 1 + temp / 2);
				q3 /= 2;
				FILE << "\nQ3 for Sample1:  " << q3;
			}

			else {

				q1 = Sample1.GetAt(((temp + 1) / 2) - 1);
				FILE << "\nQ1 for Sample1:  " << q1;

				q3 = Sample1.GetAt(temp + 1 + ((temp + 1) / 2) - 1);
				FILE << "\nQ3 for Sample1:  " << q3;


			}


		}

		low1 = q1;
		low2 = Q1;
		up1 = q3;
		up2 = Q3;
		med1 = q2;
		med2 = Q2;
		// finding the outliers

		double upper_boundary, lower_boundary, UPPER_BOUNDARY, LOWER_BOUNDARY;
		iqr = 1.5 * (q3 - q1);
		IQR = 1.5 * (Q3 - Q1);
		upper_boundary = q3 + iqr;
		lower_boundary = q1 - iqr;
		UPPER_BOUNDARY = Q3 + IQR;
		LOWER_BOUNDARY = Q1 - IQR;

		CArray <double> pure1;
		CArray <double>pure2;


		pure1.RemoveAll();
		pure2.RemoveAll();


		int outlier1 = 0, outlier2 = 0;
		for (int i = 0; i < nObs1; i++) {
			if (Sample1.GetAt(i) > upper_boundary || Sample1.GetAt(i) < lower_boundary) {

				outlier1++;
			}

		}
		FILE << "\nthere are " << outlier1 << " outliers in Sample 1";

		ol1 = outlier1;



		for (int i = 0; i < nObs2; i++) {
			if (Sample2.GetAt(i) > UPPER_BOUNDARY || Sample2.GetAt(i) < LOWER_BOUNDARY) {
				outlier2++;


			}

		}
		FILE << "\nthere are " << outlier2 << "  outliers in Sample 2";

		OutlierArray2.RemoveAll();
		OutlierArray1.RemoveAll();

		for (int i = 0; i < nObs2; i++) {


			if (Sample2.GetAt(i) > UPPER_BOUNDARY || Sample2.GetAt(i) < LOWER_BOUNDARY) {
				double value = Sample2.GetAt(i);
				OutlierArray2.Add(value);


			}
			else {
				double value = Sample2.GetAt(i);
				pure2.Add(value);
			}
		}

		for (int i = 0; i < nObs1; i++) {
			if (Sample1.GetAt(i) > upper_boundary || Sample1.GetAt(i) < lower_boundary) {
				double value = Sample1.GetAt(i);
				OutlierArray1.Add(value);


			}
			else {
				double value = Sample1.GetAt(i);
				pure1.Add(Sample1.GetAt(i));

			}
		}
		FILE << "\nHere is the outliers in Sample 1\n";
		for (int i = 0; i < outlier1; i++) {
			FILE << OutlierArray1.GetAt(i);
			FILE << '\n';
		}

		FILE << "\nHere is the outliers in Sample 2\n";
		for (int i = 0; i < outlier2; i++) {
			FILE << OutlierArray2.GetAt(i);
			FILE << '\n';
		}

		ol2 = outlier2;

		QuickSort(pure1, 0, nObs1 - outlier1 - 1);
		QuickSort(pure2, 0, nObs2 - outlier2 - 1);

		QuickSort(OutlierArray1, 0, outlier1 - 1);
		QuickSort(OutlierArray2, 0, outlier2 - 1);

		min_box = pure1.GetAt(0);
		MIN_BOX = pure2.GetAt(0);
		max_box = pure1.GetAt(nObs1 - outlier1 - 1);
		MAX_BOX = pure2.GetAt(nObs2 - outlier2 - 1);

		//min_box is the minimum value of sample 1 excluding the outliers
		//MIN_BOX is the minimum value of sample 2 excluding the outliers

		FILE << "\n sorted outliers for sample1: ";
		for (int i = 0; i < outlier1; i++) {
			FILE << OutlierArray1.GetAt(i);
			FILE << '\t';
		}

		FILE << "\n sorted outliers for sample2: ";
		for (int i = 0; i < outlier2; i++) {
			FILE << OutlierArray2.GetAt(i);
			FILE << '\t';
		}
		//descriptive statistics  
		show_class1 = digSetText.SelectedClass_1;
		show_class2 = digSetText.SelectedClass_2;
		show_nObs1 = nObs1;
		show_nObs2 = nObs2;
		show_mean1 = mean1 * max1;
		show_mean2 = mean2 * max2;
		show_std1 = std1 * max1;
		show_std2 = std2 * max2;
		show_max1 = max1;
		show_max2 = max2;

		show_mu = digSetText.Value_0;




		if (digSetText.SelectMeanVariance == 0) {

			number = digSetText.SelectedVariable;
			FILE << '\n' << "Selected Variable = " << digSetText.SelectedVariable << "\t";
			double mu = digSetText.Value_0;
			double alpha = digSetText.Significance;

			double xbar1 = mean1 * max1;
			double xbar2 = mean2 * max2;


			double variance1 = 1.0;
			double variance2 = 1.0;
			double T = 0, Z = 0, numerator = 0, denometor = 0, n1 = 0, n2 = 0, d1 = 0, d2 = 0, V = 0, SP = 0;

			if (digSetText.SelectUnknownKnown == 0) {
				show_var_known_unknown = "Known";

				ProbabilityDistributions normal('n', 0, 1);

				//when selecting "Known Variance"
				//Conducting Z-Test
				variance1 = digSetText.EnterVariance; //Variance for Sample1
				variance2 = digSetText.VarianceEnter; //Variance for Sample2
				numerator = (xbar1 - xbar2 - mu);
				denometor = sqrt((variance1 / nObs1) + (variance2 / nObs2));
				show_delta_x = xbar1 - xbar2;
				Z = numerator / denometor;
				if (digSetText.TwoSided == 0) {
					show_tail = "Two-Tailed Test";
				}
				else if (digSetText.TwoSided == 1) {
					show_tail = "Left-Tailed Test";
				}
				else {
					show_tail = "Right-Tailed Test";
				}


				double zcrit, p_value;
				//zcrit is the critical value of with respect to alpha


				if (digSetText.TwoSided == 0) {


					zcrit = normal.CriticalValue(alpha / 2);

					double Conf_U = (xbar1 - xbar2) + fabs(zcrit) * denometor;
					double Conf_L = (xbar1 - xbar2) - fabs(zcrit) * denometor;

					double Crit_U = mu + fabs(zcrit) * denometor;
					double Crit_L = mu - fabs(zcrit) * denometor;
					show_confidence_L = Conf_L;
					show_confidence_U = Conf_U;
					show_critical_U = Crit_U;
					show_critical_L = Crit_L;

					p_value = 2.0 * (1.0 - normal.Compute_pValue(fabs(Z)));
				}

				else if (digSetText.TwoSided == 1) {
					//Left_Tailed test
					zcrit = normal.CriticalValue(alpha);



					double Conf_U = (xbar1 - xbar2) + fabs(zcrit) * denometor;
					double Crit_L = mu - fabs(zcrit) * denometor;
					show_confidence_U = Conf_U;
					show_critical_L = Crit_L;


					p_value = normal.Compute_pValue(Z);



				}

				else {
					//Right_Tailed test
					zcrit = normal.CriticalValue(alpha);


					double Conf_L = (xbar1 - xbar2) - fabs(zcrit) * denometor;
					double Crit_U = mu + fabs(zcrit) * denometor;
					show_confidence_L = Conf_L;
					show_critical_U = Crit_U;


					p_value = 1.0 - normal.Compute_pValue(Z);



				}
				if (p_value >= alpha) {
					FILE << "Cannot reject H0" << std::endl;
					show_decision = "Accept H0";
				}
				else {
					FILE << "Must reject H0" << std::endl;
					show_decision = "Reject H0";
				}

				FILE << "z-Statistic = " << Z << "\t" << zcrit << "\t" << p_value << std::endl;



				// results of known variance
				show_z_statistic = Z;
				show_zcrit = zcrit;
				show_p_value = p_value;
			}






			else {
				show_var_known_unknown = "Unknown";
				//Unknown Variance 
		//First to conduct F test to decide whether the variances are equal

				ProbabilityDistributions Fu('F', (double)(nObs1 - 1), (double)(nObs2 - 1));
				ProbabilityDistributions Fl('F', (double)(nObs2 - 1), (double)(nObs1 - 1));
				double U, L, F;
				char EqualUnequal;
				show_freedom = nObs1 + nObs2 - 2 + 100;
				if (digSetText.TwoSided == 0) {
					show_tail = "Two-Tailed Test";
					// two-tailed test
					U = Fu.CriticalValue(alpha / 2.0);
					L = 1 / Fl.CriticalValue(alpha / 2.0);
					F = (std1 * max1) / (std2 * max2);
					F *= F;
					FILE << "F = " << F << "\t U = " << U << "\t L = " << L << std::endl;

					show_F = F;
					show_Fu = U;
					show_Fl = L;

					if (F > U || F < L) { //Fu and Fl
						// unequal variance
						EqualUnequal = 'U';
					}
					else {
						//equal variance
						EqualUnequal = 'E';
					}

				}

				else if (digSetText.TwoSided == 1) {
					show_tail = "Left-Tailed Test";
					// left-tailed test
					L = 1 / Fl.CriticalValue(alpha);
					F = (std1 * max1) / (std2 * max2);
					F *= F;
					FILE << "F = " << F << "\t L = " << L << std::endl;

					show_F = F;
					show_Fl = L;

					if (F < L) {
						// unequal variance
						EqualUnequal = 'U';
					}
					else {
						//equal variance
						EqualUnequal = 'E';
					}
				}

				else {
					//right-tailed test
					show_tail = "Right-Tailed Test";
					U = Fu.CriticalValue(alpha);
					F = (std1 * max1) / (std2 * max2);
					F *= F;
					FILE << "F = " << F << "\t U = " << U << std::endl;

					show_F = F;
					show_Fu = U;

					if (F > U) {
						// unequal variance
						EqualUnequal = 'U';
					}
					else {
						//equal variance
						EqualUnequal = 'E';
					}
				}


				if (EqualUnequal == 'E') {
					show_var_equal_unequal = "Equal";
					FILE << "Unknwon but equal variance" << '\n';
					//Unknown but equal variance
					SP = sqrt(((nObs1 - 1.0) * std1 * std1 * max1 * max1 + (nObs2 - 1.0) * std2 * std2 * max2 * max2) / (nObs1 + nObs2 - 2.0));
					numerator = xbar1 - xbar2 - mu;
					denometor = SP * sqrt(((1.0 / nObs1)) + ((1.0 / nObs2)));
					T = numerator / denometor;

					FILE << "t-Statistic = " << T << std::endl;
					show_T = T;
					show_SP = SP;
					show_delta_x = xbar1 - xbar2;
					ProbabilityDistributions t('t', (double)(nObs1 + nObs2 - 2.0), 0.0);

					double tcrit, p_value, Conf_U, Conf_L, Crit_L, Crit_U;

					if (digSetText.TwoSided == 0) {
						tcrit = t.CriticalValue(alpha / 2.0);


						p_value = 2.0 * t.Compute_pValue(fabs(T));



						Conf_U = (xbar1 - xbar2) + tcrit * denometor;
						Conf_L = (xbar1 - xbar2) - tcrit * denometor;
						Crit_U = mu + tcrit * denometor;
						Crit_L = mu - tcrit * denometor;
						FILE << "H0 : mu = " << mu << std::endl;
						FILE << "H1 : mu neq " << mu << std::endl;
						FILE << "tcrit = " << tcrit << std::endl;




						FILE << "Two-sided confidence interval, L = " << Conf_L << " and U = " << Conf_U << std::endl;
						FILE << "p-Value = " << p_value << std::endl;
						FILE << "Critical region, L = " << Crit_L << " and U = " << Crit_U << std::endl;
						FILE << "Type I error probability = " << alpha << std::endl;

						show_tcrit = tcrit;
						show_p_value = p_value;
						show_confidence_L = Conf_L;
						show_confidence_U = Conf_U;
						show_critical_U = Crit_U;
						show_critical_L = Crit_L;

						if (p_value >= alpha) {
							FILE << "Cannot reject H0" << std::endl;
							show_decision = "Accept H0";
						}
						else {
							FILE << "Must reject H0" << std::endl;
							show_decision = "Reject H0";
						}
					}

					else if (digSetText.TwoSided == 1) {
						//Left_Tailed test 
						CString Text;
						Text.Empty();
						tcrit = t.CriticalValue(alpha);
						p_value = 1.0 - t.Compute_pValue(T);


						Conf_U = (xbar1 - xbar2) + tcrit * denometor;
						Crit_L = mu - tcrit * denometor;


						FILE << "H0 : mu = " << mu << std::endl;
						FILE << "H1 : mu < " << mu << std::endl;
						FILE << "tcrit = " << -tcrit << std::endl;


						FILE << "One-sided upper confidence interval : mu < U = " << Conf_U << std::endl;
						FILE << "p-Value = " << p_value << std::endl;
						FILE << "Critical region, xbar < L = " << Crit_L << std::endl;
						FILE << "Type I error probability = " << alpha << std::endl;


						show_tcrit = -tcrit;
						show_p_value = p_value;
						show_confidence_U = Conf_U;
						show_critical_L = Crit_L;
						tcrit = -tcrit;

						if (p_value >= alpha) {
							FILE << "Cannot reject H0" << std::endl;
							show_decision = "Accept H0";
						}
						else {
							FILE << "Must reject H0" << std::endl;
							show_decision = "Reject H0";
						}
					}

					else {
						tcrit = t.CriticalValue(alpha);
						p_value = t.Compute_pValue(T);

						Conf_L = (xbar1 - xbar2) - tcrit * denometor;
						Crit_U = mu + tcrit * denometor;

						FILE << "H0 : mu = " << mu << std::endl;
						FILE << "H1 : mu > " << mu << std::endl;
						FILE << "tcrit = " << tcrit << std::endl;
						FILE << "One-sided lower confidence interval : mu > L = " << Conf_L << std::endl;
						FILE << "p-Value = " << p_value << std::endl;
						FILE << "Critical region, xbar > U = " << Crit_U << std::endl;
						FILE << "Type I error probability = " << alpha << std::endl;

						show_tcrit = tcrit;
						show_p_value = p_value;
						show_confidence_L = Conf_L;
						show_critical_U = Crit_U;

						if (p_value >= alpha) {
							FILE << "Cannot reject H0" << std::endl;
							show_decision = "Accept H0";
						}
						else {
							FILE << "Must reject H0" << std::endl;
							show_decision = "Reject H0";
						}
					}
				}
				else {
					show_var_equal_unequal = "Unequal";
					FILE << "Unknwon and unequal variance" << "\n";
					//Unknown and unequal variance
					n1 = (std1 * std1 * max1 * max1) / nObs1;
					n2 = (std2 * std2 * max2 * max2) / nObs2;
					numerator = (n1 + n2) * (n1 + n2);
					d1 = ((std1 * std1 * max1 * max1) / nObs1) * ((std1 * std1 * max1 * max1) / nObs1);
					d2 = ((std2 * std2 * max2 * max2) / nObs2) * ((std2 * std2 * max2 * max2) / nObs2);
					denometor = (d1 / (nObs1 - 1.0)) + (d2 / (nObs2 - 1.0));
					V = numerator / denometor;

					T = (xbar1 - xbar2 - mu) / sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));

					FILE << "t-Statistic = (test) " << T << std::endl;

					show_delta_x = xbar1 - xbar2;
					show_SP = V;
					show_T = T;

					ProbabilityDistributions t('t', (double)(nObs1 + nObs2 - 2), 0.0);
					double tcrit, p_value, Conf_U, Conf_L, Crit_L, Crit_U;

					if (digSetText.TwoSided == 0) {
						tcrit = t.CriticalValue(alpha / 2.0);
						p_value = 2.0 * t.Compute_pValue(fabs(T));

						Conf_U = (xbar1 - xbar2) + tcrit * sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));
						Conf_L = (xbar1 - xbar2) - tcrit * sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));
						Crit_U = mu + tcrit * sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));
						Crit_L = mu - tcrit * sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));

						show_tcrit = tcrit;
						show_p_value = p_value;
						show_confidence_L = Conf_L;
						show_confidence_U = Conf_U;
						show_critical_U = Crit_U;
						show_critical_L = Crit_L;

						FILE << "H0 : mu = " << mu << std::endl;
						FILE << "H1 : mu neq " << mu << std::endl;
						FILE << "tcrit = " << tcrit << std::endl;
						FILE << "Two-sided confidence interval, L = " << Conf_L << " and U = " << Conf_U << std::endl;
						FILE << "p-Value = " << p_value << std::endl;
						FILE << "Critical region, L = " << Crit_L << " and U = " << Crit_U << std::endl;
						FILE << "Type I error probability = " << alpha << std::endl;
						if (p_value >= alpha) {
							show_decision = "Accept H0";
							FILE << "Cannot reject H0" << std::endl;
						}
						else {
							show_decision = "Reject H0";
							FILE << "Must reject H0" << std::endl;
						}
					}

					else if (digSetText.TwoSided == 1) {
						CString Text;
						Text.Empty();
						tcrit = t.CriticalValue(alpha);
						p_value = 1.0 - t.Compute_pValue(T);


						Conf_U = (xbar1 - xbar2) + tcrit * sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));
						Crit_L = mu - tcrit * sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));

						FILE << "H0 : mu = " << mu << std::endl;
						FILE << "H1 : mu < " << mu << std::endl;
						FILE << "tcrit = " << -tcrit << std::endl;
						FILE << "One-sided upper confidence interval : mu < U = " << Conf_U << std::endl;
						FILE << "p-Value = " << p_value << std::endl;
						FILE << "Critical region, xbar < L = " << Crit_L << std::endl;
						FILE << "Type I error probability = " << alpha << std::endl;

						show_tcrit = -tcrit;
						show_p_value = p_value;
						show_confidence_U = Conf_U;
						show_critical_L = Crit_L;
						tcrit = -tcrit;

						if (p_value >= alpha) {
							FILE << "Cannot reject H0" << std::endl;
							show_decision = "Acceopt H0";
						}
						else {
							show_decision = "Reject H0";
							FILE << "Must reject H0" << std::endl;
						}
					}

					else {
						tcrit = t.CriticalValue(alpha);
						p_value = t.Compute_pValue(T);

						Conf_L = (xbar1 - xbar2) - tcrit * sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));
						Crit_U = mu + tcrit * sqrt((std1 * std1 * max1 * max1 / nObs1) + (std2 * std2 * max2 * max2 / nObs2));

						FILE << "H0 : mu = " << mu << std::endl;
						FILE << "H1 : mu > " << mu << std::endl;
						FILE << "tcrit = " << tcrit << std::endl;
						FILE << "One-sided lower confidence interval : mu > L = " << Conf_L << std::endl;
						FILE << "p-Value = " << p_value << std::endl;
						FILE << "Critical region, xbar > U = " << Crit_U << std::endl;
						FILE << "Type I error probability = " << alpha << std::endl;

						show_tcrit = tcrit;
						show_p_value = p_value;
						show_confidence_L = Conf_L;
						show_critical_U = Crit_U;

						if (p_value >= alpha) {
							FILE << "Cannot reject H0" << std::endl;
							show_decision = "Accept H0";
						}
						else {
							FILE << "Must reject H0" << std::endl;
							show_decision = "Reject H0";
						}
					}
				}


			}

		}
		else {
			ProbabilityDistributions Chi('C', (double)(nObs1 + nObs2 - 2.0), 0);
		}


	}
	FILE.close();
	UpdateAllViews(NULL);
}

//*****************************************************************
//***   Compute  results  for  Shapiro-Wilk  hypothesis  test   ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnSWTest() {
	OnOpenedFile = false;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = false;
	ShapiroWilkTest = true;
	AndersonDarlingTest = false;
	ANOVA = false;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Scores = false;
	PCA_Display_Loadings = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = false;
	AfxMessageBox(L"Now we are working on computing the Shapiro-Wilk test");
	UpdateAllViews(NULL);
}

//*****************************************************************
//*** Compute  results  for  Anderson-Darling  hypothesis  test ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnADTest() {
	AfxMessageBox(L"Now we are working on computing the Anderson-Darling test");
}

//*****************************************************************
//*** Compute results for analysis of variance hypothesis test  ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnANOVA() {
	OnOpenedFile = false;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = false;
	ShapiroWilkTest = false;
	AndersonDarlingTest = false;
	ANOVA = true;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Loadings = false;
	PCA_Display_Scores = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = false;
	UpdateAllViews(NULL);
}

//*****************************************************************
//***        Compute principal component analysis model         ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnPCA_model() {
	CArray <double> Data_0;
	CArray <int> Rxx_spec;
	CArray <double> bar, std, Rxx;
	bool EV = true, flag;
	int nVar;
	bar.RemoveAll();
	std.RemoveAll();
	Rxx.RemoveAll();
	Data_0.RemoveAll();
	Rxx_spec.RemoveAll();
	lambda.RemoveAll();
	P.RemoveAll();
	P_spec.RemoveAll();
	OnOpenedFile = false;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = false;
	PCA_Select_PCs = true;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Scores = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = false;
	StandardizeDataMatrix(Data_0, bar, std);
	GetCorrelationMatrix(Data_0, Data_spec, R, R_spec, OnlyX);
	EVD(R, R_spec, P, P_spec, lambda, EV, flag);
	if (OnlyX == true) nVar = n_Var - 1;
	else nVar = n_Var;
	// Preparing for histogram showing eigenvalues
	n_kg = 0;
	while (lambda.GetAt(n_kg) > (double)1) {
		n_kg++;
	}
	vre.RemoveAll();
	vpc.RemoveAll();
	VPC(R, R_spec, n_vpc);
	VRE(R, R_spec, n_vre);
	double value;
	CumVar.RemoveAll();
	double temp = 0, cum = 0;
	for (int i = 0; i < lambda.GetSize(); i++) temp += lambda.GetAt(i);
	for (int i = 0; i < lambda.GetSize(); i++) {
		cum += lambda.GetAt(i) / temp;
		CumVar.Add(cum);
	}
	UpdateAllViews(NULL);
	CSelectNumberPCs GetNumberPCs;
	if (GetNumberPCs.DoModal() == IDOK) n = GetNumberPCs.n_PCs;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = true;
	UpdateAllViews(NULL);
	T.RemoveAll();
	T_spec.RemoveAll();
	CArray <double> P_red;
	P_red.RemoveAll();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < nVar; j++) P_red.Add(P.GetAt(GetPosition(j, i, P_spec)));
	}
	P.RemoveAll();
	P_spec.SetAt(1, n);
	int pos;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < nVar; j++) {
			pos = GetPosition(j, i, P_spec);
			P.Add(P_red.GetAt(pos));
		}
	}
	P_red.RemoveAll();
	double product, temp_1, temp_2;
	T_spec.SetSize(3);
	T_spec.SetAt(0, n_Obs);
	T_spec.SetAt(1, n);
	T_spec.SetAt(2, Data_spec.GetAt(2));
	T.SetSize(static_cast <int64_t>(n_Obs) * n);
	for (int i = 0; i < n_Obs; i++) {
		for (int j = 0; j < n; j++) {
			if (OnlyX == true) {
				product = (double)0;
				for (int k = 0; k < n_Var - 1; k++) {
					temp_1 = Data_0.GetAt(GetPosition(i, k, Data_spec));
					temp_2 = P.GetAt(GetPosition(k, j, P_spec));
					product += temp_1 * temp_2;
				}
				T.SetAt(GetPosition(i, j, T_spec), product);
			}
			else {
				product = (double)0;
				for (int k = 0; k < n_Var; k++) {
					temp_1 = Data_0.GetAt(GetPosition(i, k, Data_spec));
					temp_2 = P.GetAt(GetPosition(k, j, P_spec));
					product += temp_1 * temp_2;
				}
				T.SetAt(GetPosition(i, j, T_spec), product);
			}
		}
	}
	CArray <double> Xpred, E;
	CArray <int> Xpred_spec;
	Xpred_spec.SetSize(3);
	if (OnlyX == true) {
		Xpred_spec.SetAt(0, n_Obs);
		Xpred_spec.SetAt(1, n_Var - 1);
		Xpred_spec.SetAt(2, Data_spec.GetAt(2));
		Xpred.SetSize(static_cast <int64_t>(n_Obs) * (static_cast <int64_t>(n_Var) - 1));
		E.SetSize(Xpred.GetSize());
		for (int i = 0; i < n_Obs; i++) {
			for (int j = 0; j < n_Var - 1; j++) {
				value = (double)0;
				for (int k = 0; k < n; k++) {
					temp_1 = T.GetAt(GetPosition(i, k, T_spec));
					temp_2 = P.GetAt(GetPosition(j, k, P_spec));
					value += temp_1 * temp_2;
				}
				Xpred.SetAt(GetPosition(i, j, Xpred_spec), value);
				value = Data_0.GetAt(GetPosition(i, j, Data_spec));
				value -= Xpred.GetAt(GetPosition(i, j, Xpred_spec));
				E.SetAt(GetPosition(i, j, Xpred_spec), value);
			}
		}
	}
	else {
		Xpred.SetSize(static_cast <int64_t>(n_Obs) * n_Var);
		Xpred_spec.SetAt(0, n_Obs);
		Xpred_spec.SetAt(1, n_Var);
		Xpred_spec.SetAt(2, Data_spec.GetAt(2));
		E.SetSize(Xpred.GetSize());
		for (int i = 0; i < n_Obs; i++) {
			for (int j = 0; j < n_Var; j++) {
				value = (double)0;
				for (int k = 0; k < n; k++) {
					temp_1 = T.GetAt(GetPosition(i, k, T_spec));
					temp_2 = P.GetAt(GetPosition(j, k, P_spec));
					value += temp_1 * temp_2;
				}
				Xpred.SetAt(GetPosition(i, j, Xpred_spec), value);
				value = Data_0.GetAt(GetPosition(i, j, Data_spec));
				value -= Xpred.GetAt(GetPosition(i, j, Xpred_spec));
				E.SetAt(GetPosition(i, j, Xpred_spec), value);
			}
		}
	}
	T_2.RemoveAll();
	SPE.RemoveAll();
	for (int i = 0; i < n_Obs; i++) {
		temp = (double)0;
		for (int j = 0; j < n; j++) {
			value = T.GetAt(GetPosition(i, j, T_spec));
			value *= value;
			value /= lambda.GetAt(j);
			temp += value;
		}
		T_2.Add(temp);
		temp = (double)0;
		if (OnlyX == true) {
			for (int j = 0; j < n_Var - 1; j++) {
				value = E.GetAt(GetPosition(i, j, Xpred_spec));
				value *= value;
				temp += value;
			}
			SPE.Add(temp);
		}
		else {
			for (int j = 0; j < n_Var; j++) {
				value = E.GetAt(GetPosition(i, j, Xpred_spec));
				value *= value;
				temp += value;
			}
			SPE.Add(temp);
		}
	}
	ProbabilityDistributions F('F', n, n_Obs - n);
	ProbabilityDistributions normal('n', 0, 1);
	double fcrit = F.CriticalValue(alpha);
	T95 = fcrit;
	T95 *= (double)((double)n * ((double)n_Obs * (double)n_Obs - (double)1));
	T95 /= (double)n_Obs * ((double)n_Obs - (double)n);
	double theta_1 = 0;
	if (OnlyX == true) {
		for (int i = n; i < n_Var - 1; i++) {
			theta_1 += lambda.GetAt(i);
		}
	}
	else {
		for (int i = n; i < n_Var; i++) {
			theta_1 += lambda.GetAt(i);
		}
	}
	if (theta_1 >= 1e-5) {
		double theta_2 = 0, theta_3 = 0, h0 = 0;
		if (OnlyX == true) {
			for (int i = n; i < n_Var - 1; i++) {
				theta_2 += pow(lambda.GetAt(i), (double)2);
				theta_3 += pow(lambda.GetAt(i), (double)3);
			}
		}
		else {
			for (int i = n; i < n_Var; i++) {
				theta_2 += pow(lambda.GetAt(i), (double)2);
				theta_3 += pow(lambda.GetAt(i), (double)3);
			}
		}
		h0 = 1 - (2 * theta_1 * theta_3) / (3 * pow(theta_2, (double)2));
		if (abs(h0) < 1e-5) {
			Q95 = (double)0;
		}
		else {
			double c95 = normal.CriticalValue(1 - alpha), sign = 0, h1, h2;
			if (h0 > 0) sign = (double)1;
			else sign = (double)(-1);
			h1 = sign * c95 * sqrt(2 * theta_2 * pow(h0, (double)2)) / theta_1;
			h2 = theta_2 * h0 * (h0 - 1) / pow(theta_1, (double)2);
			Q95 = theta_1 * pow((double)1 + h1 + h2, ((double)1 / h0));
		}
	}
	else Q95 = (double)0;
}

void CModelingandAnalysisofUncertaintyDoc::VPC(CArray <double>& R, CArray <int>& R_spec, int& n_min) {
	vpc.RemoveAll();
	CArray <double> Rk, Sxx, sk;
	CArray <int> Temp_spec;
	double value, sum, den, temp, vpc_min;
	Temp_spec.SetSize(3);
	Temp_spec.SetAt(0, R_spec.GetAt(0));
	Temp_spec.SetAt(1, R_spec.GetAt(1));
	Temp_spec.SetAt(2, R_spec.GetAt(2));
	sk.SetSize(R_spec.GetAt(0));
	Sxx.SetSize(R.GetSize());
	Rk.SetSize(R.GetSize());
	vpc.SetSize(static_cast <int64_t>(R_spec.GetAt(0)) - 1);
	for (int i = 0; i < R_spec.GetAt(0); i++) {
		for (int j = 0; j <= i; j++) {
			Sxx.SetAt(GetPosition(i, j, Temp_spec), R.GetAt(GetPosition(i, j, Temp_spec)));
		}
	}
	for (int k = 0; k < R_spec.GetAt(0) - 1; k++) {
		for (int j = 0; j < R_spec.GetAt(0); j++) {
			sk.SetAt(j, (double)1 / sqrt(Sxx.GetAt(GetPosition(j, j, Temp_spec))));
		}
		for (int j = 0; j < R_spec.GetAt(0); j++) {
			for (int i = 0; i <= j; i++) {
				value = Sxx.GetAt(GetPosition(j, i, Temp_spec));
				value *= sk.GetAt(j) * sk.GetAt(i);
				Rk.SetAt(GetPosition(j, i, Temp_spec), value);
			}
		}
		sum = 0;
		for (int j = 1; j < R_spec.GetAt(0); j++) {
			for (int i = 0; i < j; i++) {
				value = Rk.GetAt(GetPosition(j, i, Temp_spec));
				sum += 2 * SQR(value);
			}
		}
		den = (double)(R_spec.GetAt(0) - (double)1) * (double)R_spec.GetAt(0) / (double)2;
		sum /= den;
		vpc.SetAt(k, sum);
		for (int j = 0; j < R_spec.GetAt(0); j++) {
			for (int i = 0; i <= j; i++) {
				value = P.GetAt(GetPosition(j, k, P_spec));
				value *= P.GetAt(GetPosition(i, k, P_spec));
				value *= lambda.GetAt(k);
				temp = Sxx.GetAt(GetPosition(j, i, Temp_spec));
				temp -= value;
				Sxx.SetAt(GetPosition(j, i, Temp_spec), temp);
			}
		}
		if (k == 0) {
			vpc_min = vpc.GetAt(k);
			n_min = k;
		}
		else {
			if (vpc.GetAt(k) < vpc_min) {
				vpc_min = vpc.GetAt(k);
				n_min = k;
			}
		}
	}
}

void CModelingandAnalysisofUncertaintyDoc::VRE(CArray <double>& R, CArray <int>& R_spec, int& n_min) {
	vre.RemoveAll();
	CArray <double> PStar;
	CArray <int> PStar_spec;
	CArray <double> zeta;
	double vre_k, u, v, value, vre_min;
	int dim = R_spec.GetAt(0);
	PStar_spec.SetSize(3);
	PStar_spec.SetAt(0, R_spec.GetAt(0));
	PStar_spec.SetAt(1, R_spec.GetAt(1));
	PStar_spec.SetAt(2, R_spec.GetAt(2));
	PStar.SetSize(R.GetSize());
	zeta.SetSize(dim);
	vre.SetSize(static_cast <int64_t>(dim) - 1);
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j <= i; j++) {
			if (i == j) PStar.SetAt(GetPosition(i, j, PStar_spec), (double)1);
			else PStar.SetAt(GetPosition(i, j, PStar_spec), (double)0);
		}
	}
	for (int k = 0; k < dim - 1; k++) {
		for (int j = 0; j < dim; j++) {
			for (int i = 0; i <= j; i++) {
				value = PStar.GetAt(GetPosition(j, i, PStar_spec));
				value -= P.GetAt(GetPosition(i, k, P_spec)) * P.GetAt(GetPosition(j, k, P_spec));
				PStar.SetAt(GetPosition(j, i, PStar_spec), value);
			}
		}
		vre_k = (double)0;
		for (int j = 0; j < dim; j++) {
			for (int i = 0; i < dim; i++) {
				value = PStar.GetAt(GetPosition(i, j, PStar_spec));
				zeta.SetAt(i, value);
			}
			u = (double)0, v = (double)0;
			for (int i = 0; i < dim; i++) {
				value = R.GetAt(GetPosition(i, i, R_spec));
				value *= SQR(zeta.GetAt(i));
				u += value;
				v += SQR(zeta.GetAt(i));
				for (int l = i + 1; l < dim; l++) {
					value = 2 * R.GetAt(GetPosition(l, i, R_spec));
					value *= zeta.GetAt(l) * zeta.GetAt(i);
					u += value;
				}
			}
			v = SQR(v);
			v *= R.GetAt(GetPosition(j, j, R_spec));
			vre_k += u / v;
		}
		vre.SetAt(k, vre_k);
		if (k == 0) {
			vre_min = vre.GetAt(k);
			n_min = k;
		}
		else {
			if (vre.GetAt(k) < vre_min) {
				vre_min = vre.GetAt(k);
				n_min = k;
			}
		}
	}
}

//*****************************************************************
//***               Compute factor analysis model               ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnFA_Model() {
	CArray <double> Data_0;
	CArray <double> bar, std;
	bool EV = true, flag;
	bar.RemoveAll();
	std.RemoveAll();
	Data_0.RemoveAll();
	lambda.RemoveAll();
	P.RemoveAll();
	P_spec.RemoveAll();
	OnOpenedFile = false;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = false;
	PCA_Select_PCs = true;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Scores = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = false;
	StandardizeDataMatrix(Data_0, bar, std);
	GetCorrelationMatrix(Data_0, Data_spec, R, R_spec, OnlyX);
	EVD(R, R_spec, P, P_spec, lambda, EV, flag);
	CArray <int> Temp;
	Temp.RemoveAll();
	CPoint TopLeftCorner, BottomRightCorner;
	n_kg = 0;
	while (lambda.GetAt(n_kg) > (double)1) n_kg++;
	vre.RemoveAll(), vpc.RemoveAll(), CumVar.RemoveAll();
	VPC(R, R_spec, n_vpc), VRE(R, R_spec, n_vre);
	double min_vpc = vpc.GetAt(n_vpc), min_vre = vre.GetAt(n_vre);
	double temp = 0, cum = 0;
	for (int i = 0; i < lambda.GetSize(); i++) temp += lambda.GetAt(i);
	for (int i = 0; i < lambda.GetSize(); i++) {
		cum += lambda.GetAt(i) / temp;
		CumVar.Add(cum);
	}
	UpdateAllViews(NULL);
	CSpecifyFactorAnalysis FAModel;
	if (FAModel.DoModal() == IDOK) {
		n = FAModel.n_Factors;
		double lambda_ave = (double)0, temp, temp_X, temp_P;
		int nVar;
		if (OnlyX == true) nVar = n_Var - 1;
		else nVar = n_Var;
		CArray <double> Pred;
		CArray <int> Pred_spec;
		Pred.SetSize(static_cast <int64_t>(nVar) * n);
		Pred_spec.SetSize(3);
		Pred_spec.SetAt(0, nVar), Pred_spec.SetAt(1, n), Pred_spec.SetAt(2, 0);
		for (int i = 0; i < nVar; i++) {
			for (int j = 0; j < n; j++) Pred.SetAt(GetPosition(i, j, P_spec), P.GetAt(GetPosition(i, j, P_spec)));
		}
		CArray <double> T_temp;
		CArray <int> Ttemp_spec;
		T_temp.RemoveAll(), Ttemp_spec.RemoveAll(), T_temp.SetSize(static_cast <int64_t>(n_Obs * n));
		Ttemp_spec.SetSize(3), Ttemp_spec.SetAt(0, n_Obs), Ttemp_spec.SetAt(1, n), Ttemp_spec.SetAt(2, 0);
		for (int i = 0; i < n_Obs; i++) {
			for (int j = 0; j < n; j++) {
				temp = (double)0;
				for (int k = 0; k < nVar; k++) {
					temp_X = Data_0.GetAt(GetPosition(i, k, Data_spec));
					temp_P = P.GetAt(GetPosition(k, j, P_spec));
					temp += temp_X * temp_P;
				}
				temp /= sqrt(lambda.GetAt(j));
				T_temp.SetAt(GetPosition(i, j, Ttemp_spec), temp);
			}
		}
		for (int i = n; i < nVar; i++) lambda_ave += lambda.GetAt(i);
		lambda_ave /= (double)(nVar - n);
		for (int i = 0; i < n; i++) {
			temp = lambda.GetAt(i);
			temp -= lambda_ave;
			lambda.SetAt(i, temp);
		}
		for (int i = 0; i < nVar; i++) {
			for (int j = 0; j < n; j++) {
				temp = Pred.GetAt(GetPosition(i, j, Pred_spec));
				temp *= sqrt(lambda.GetAt(j));
				Pred.SetAt(GetPosition(i, j, Pred_spec), temp);
			}
		}
		if (FAModel.OrthogonalRotation == 0) {
			double gamma, EPS = 1e-8, d_old = (double)1, d = d_old, error = (double)100;
			if (FAModel.WhichOrthogonalRotation == 0) gamma = (double)1;
			else if (FAModel.WhichOrthogonalRotation == 1) gamma = (double)0;
			else if (FAModel.WhichOrthogonalRotation == 2) gamma = (double)n / (double)2;
			else gamma = ((double)n_Obs * ((double)n - (double)1)) / ((double)n_Obs + (double)n - (double)2);
			RT.RemoveAll(), RT_spec.RemoveAll();
			RT.SetSize(static_cast <int64_t>(n * n));
			RT_spec.SetSize(3);
			RT_spec.SetAt(0, n), RT_spec.SetAt(1, n), RT_spec.SetAt(2, 0);
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					if (i == j) RT.SetAt(GetPosition(i, j, RT_spec), (double)1);
					else RT.SetAt(GetPosition(i, j, RT_spec), (double)0);
				}
			}
			CArray <double> Temp_1, Temp_2, Temp_3, M, U, V, w;
			CArray <int> Temp_1_spec, M_spec, U_spec, V_spec;
			double factor = gamma / nVar, temp_1, temp_2;
			int pos;
			bool flag;
			Temp_2.RemoveAll();
			Temp_2.SetSize(static_cast <int64_t>(nVar) * n);
			Temp_3.RemoveAll();
			Temp_3.SetSize(static_cast <int64_t>(nVar) * n);
			while (error > EPS) {
				d_old = d;
				A.RemoveAll();
				A_spec.RemoveAll();
				MatrixProduct(Pred, Pred_spec, RT, RT_spec, A, A_spec);
				Temp_1.RemoveAll();
				Temp_1_spec.RemoveAll();
				X_tr_X(A, A_spec, Temp_1, Temp_1_spec);
				for (int i = 0; i < nVar; i++) {
					for (int j = 0; j < n; j++) {
						temp_1 = A.GetAt(GetPosition(i, j, A_spec));
						temp_2 = Temp_1.GetAt(GetPosition(j, j, Temp_1_spec));
						temp_1 *= temp_2;
						Temp_2.SetAt(GetPosition(i, j, A_spec), temp_1);
					}
				}
				for (int i = 0; i < nVar; i++) {
					for (int j = 0; j < n; j++) {
						pos = GetPosition(i, j, A_spec);
						temp_1 = Temp_2.GetAt(pos);
						temp_1 *= factor;
						temp_2 = A.GetAt(pos);
						temp_2 = pow(temp_2, (double)3);
						temp_2 -= temp_1;
						Temp_3.SetAt(pos, temp_2);
					}
				}
				X_tr_Y(Pred, Pred_spec, Temp_3, A_spec, M, M_spec);
				SVD(M, M_spec, U, U_spec, w, V, V_spec, flag);
				RT.RemoveAll();
				RT_spec.RemoveAll();
				X_Y_tr(U, U_spec, V, V_spec, RT, RT_spec);
				d = (double)0;
				for (int i = 0; i < n; i++) d += w.GetAt(i);
				error = fabs(d / d_old - (double)1);
				U.RemoveAll(), V.RemoveAll(), w.RemoveAll(), U_spec.RemoveAll(), V_spec.RemoveAll(), M.RemoveAll(), M_spec.RemoveAll();
			}
			CArray <double> RT_inv, RT_inv_transpose;
			Inverse(RT, RT_spec, RT_inv, RT_spec);
			Transpose(RT_inv, RT_spec, RT_inv_transpose, RT_spec);
			MatrixProduct(T_temp, Ttemp_spec, RT_inv, RT_spec, F, F_spec);
			X_X_tr(A, A_spec, Sf, S_spec);
			SubtractingMatrices(R, R_spec, Sf, S_spec, Se, S_spec);
			for (int i = 0; i < n; i++) lambda.SetAt(i, (double)1);
		}
		else {

		}
		PCA_Select_PCs = false;
		FA_Display_Standard = true;
		UpdateAllViews(NULL);
	}
}

//*****************************************************************
//***       Compute independence component analysis model       ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnICA() {
	AfxMessageBox(L"Now we are working on establishing an ICA model");
}

//*****************************************************************
//***       Compute canonical correlation analysis model        ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnCCA() {
	AfxMessageBox(L"Now we are working on establishing an CCA model");
}

//*****************************************************************
//***     Compute linear Fisher discriminant analysis model     ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnFDA() {
	OnOpenedFile = false;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = false;
	ShapiroWilkTest = false;
	AndersonDarlingTest = false;
	ANOVA = false;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Loadings = false;
	PCA_Display_Scores = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = true;
	RegressionAnalysis = false;
	CArray <double> Data_0, bar, std;
	StandardizeDataMatrix(Data_0, bar, std);
	CArray <double> Sb, Sw;
	CArray <int> S_spec;
	bool flag = false;
	SetUpFDAMatrices(Sb, Sw, S_spec, Data_0);
	//getting eigenvectors and eigenvalues
	GEVD(Sw, S_spec, Sb, S_spec, P, P_spec, lambda, n_classes - 1, flag);
	CString Text;
	Text.Empty(), Text.Format(L"%d, %d and %d", P_spec.GetAt(0), P_spec.GetAt(1), P_spec.GetAt(2));
	AfxMessageBox(Text);
	// getting FDA scores
	Text.Empty(), Text.Format(L"%d, %d and %d", Data_spec.GetAt(0), Data_spec.GetAt(1), Data_spec.GetAt(2));
	AfxMessageBox(Text);
	double temp, value_1, value_2;
	T.RemoveAll();
	T.SetSize(static_cast <int64_t>(n_Obs * (n_classes - 1)));
	T_spec.SetSize(3), T_spec.SetAt(0, n_Obs), T_spec.SetAt(1, n_classes - 1), T_spec.SetAt(2, 0);
	for (int i = 0; i < n_Obs; i++) {
		for (int j = 0; j < n_classes - 1; j++) {
			temp = 0;
			for (int k = 0; k < n_Var - 1; k++) {
				value_1 = Data_0.GetAt(GetPosition(i, k, Data_spec));
				value_2 = P.GetAt(GetPosition(k, j, P_spec));
				temp += value_1 * value_2;
			}
			T.SetAt(GetPosition(i, j, T_spec), temp);
		}
	}
}

void CModelingandAnalysisofUncertaintyDoc::SetUpFDAMatrices(CArray <double>& Sb, CArray <double>& Sw, CArray <int>& S_spec, CArray <double>& Data_0) {
	Sb.RemoveAll();
	Sw.RemoveAll();
	Sb.SetSize(static_cast <int64_t>(n_Var - 1) * n_Var / 2);
	Sw.SetSize(static_cast <int64_t>(n_Var - 1) * n_Var / 2);
	S_spec.RemoveAll();
	S_spec.SetSize(3);
	S_spec.SetAt(0, n_Var - 1), S_spec.SetAt(1, n_Var - 1), S_spec.SetAt(2, 1);
	CArray <double> X_bar;
	CArray <int> X_bar_spec;
	//n_var - 1 is amount of x
	X_bar.SetSize(static_cast <int64_t>(n_Var - 1) * n_classes);
	X_bar_spec.SetSize(3);
	//rows, columns
	X_bar_spec.SetAt(0, n_Var - 1), X_bar_spec.SetAt(1, n_classes), X_bar_spec.SetAt(2, 0);
	int CLASS, pos;
	double temp, element;
	CArray <int> n_Obs_Class;
	n_Obs_Class.RemoveAll();
	n_Obs_Class.SetSize(n_classes);
	for (int i = 0; i < n_Obs; i++) {
		CLASS = (int)Data.GetAt(static_cast <int64_t>(GetPosition(i, n_Var - 1, Data_spec)));
		n_Obs_Class.SetAt(static_cast <int64_t>(CLASS - 1), (n_Obs_Class.GetAt(static_cast <int64_t>(CLASS - 1)) + 1));

		for (int j = 0; j < n_Var - 1; j++) {
			//element of the matrix data_0
			temp = Data_0.GetAt(GetPosition(i, j, Data_spec));
			element = X_bar.GetAt(static_cast <int64_t>(GetPosition(j, CLASS - 1, X_bar_spec)));
			element += temp;
			X_bar.SetAt(static_cast <int64_t>(GetPosition(j, CLASS - 1, X_bar_spec)), element);
		}
	}
	//creating xbar
	for (int i = 0; i < n_classes; i++) {
		for (int j = 0; j < n_Var - 1; j++) {
			pos = GetPosition(j, i, X_bar_spec);
			element = X_bar.GetAt(pos);
			element /= (double)n_Obs_Class.GetAt(i);
			X_bar.SetAt(pos, element);
		}
	}
	double temp_1, temp_2;
	for (int k = 0; k < n_classes; k++) {
		for (int i = 0; i < n_Var - 1; i++) {
			for (int j = 0; j <= i; j++) {
				pos = GetPosition(i, j, S_spec);
				element = Sb.GetAt(pos);
				temp_1 = X_bar.GetAt(GetPosition(i, k, X_bar_spec));
				temp_2 = X_bar.GetAt(GetPosition(j, k, X_bar_spec));
				temp = temp_1 * temp_2;
				element += temp;
				Sb.SetAt(pos, element);
			}
		}
	}
	//The next step is to establish an sw matrix
	CArray <double> Sww;
	int n_elements = (n_Var - 1) * n_Var / 2;
	Sww.SetSize(static_cast <int64_t>(n_elements) * n_classes);
	double xbar_1, xbar_2, element_1, element_2;
	for (int k = 0; k < n_Obs; k++) {
		CLASS = (int)Data.GetAt(static_cast <int64_t>(GetPosition(k, n_Var - 1, Data_spec)));
		for (int i = 0; i < n_Var - 1; i++) {
			for (int j = 0; j <= i; j++) {
				//Getting respective mean elements
				xbar_1 = X_bar.GetAt(static_cast<int64_t>(GetPosition(i, CLASS - 1, X_bar_spec)));
				xbar_2 = X_bar.GetAt(static_cast<int64_t>(GetPosition(j, CLASS - 1, X_bar_spec)));
				//Getting actual elements (x)
				element_1 = Data_0.GetAt(GetPosition(k, i, Data_spec));
				element_2 = Data_0.GetAt(GetPosition(k, j, Data_spec));
				pos = GetPosition(i, j, S_spec);
				temp = Sww.GetAt(static_cast <int64_t>(CLASS - 1) * n_elements + pos);
				temp += (element_1 - xbar_1) * (element_2 - xbar_2);
				pos += (CLASS - 1) * n_elements;
				Sww.SetAt(pos, temp);
			}
		}
	}
	//correcting sww and creating sw
	for (int c = 0; c < n_classes; c++) {
		for (int i = 0; i < n_Var - 1; i++) {
			for (int j = 0; j <= i; j++) {
				pos = GetPosition(i, j, S_spec);
				int sw_pos = pos;
				pos += c * n_elements;
				temp = Sww.GetAt(pos);
				temp /= (n_Obs_Class[c] - 1);
				Sww.SetAt(pos, temp);
				//Grabbing the values for Sw
				double temp_sw = Sw.GetAt(sw_pos);
				Sw.SetAt(sw_pos, temp + temp_sw);
			}
		}
	}
}

//*****************************************************************
//***            Compute linear classification model            ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnLinearClassification() {
	AfxMessageBox(L"Now we are working on establishing an linear classification model");
}

//*****************************************************************
//***              Compute linear regression model              ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnMultivariateRegression() {
	if (Problem == L"Regression problem") {
		OnOpenedFile = false;
		DescriptiveStatistics = false;
		HypothesisTesting_OneSample = false;
		HypothesisTesting_TwoSample = false;
		ShapiroWilkTest = false;
		AndersonDarlingTest = false;
		ANOVA = false;
		PCA_Select_PCs = false;
		PCA_Display_PCs_Standard = false;
		PCA_Display_Scores = false;
		FA_Display_Standard = false;
		FA_Display_Loadings = false;
		FA_Display_Scores = false;
		FA_Display_Matrices = false;
		RegressionAnalysis = true;
		CWnd* pParent = nullptr;
		CSpecifyRegressionModel RegressionModel(pParent, &Name);
		if (RegressionModel.DoModal() == IDOK) {
			int predictor, n_predictor = (int)RegressionModel.SelectedPredictors.GetSize();
			SelectedPredictors.RemoveAll();
			for (int i = 0; i < n_predictor; i++) SelectedPredictors.Add(RegressionModel.SelectedPredictors.GetAt(i));
			CArray <double> PredictorMatrix;
			CArray <double> ResponseVector;
			CArray <int> PredictorMatrix_spec;
			PredictorMatrix.SetSize(static_cast <int64_t>(n_Obs) * n_predictor);
			ResponseVector.SetSize(static_cast <int64_t>(n_Obs));
			PredictorMatrix_spec.SetSize(3);
			PredictorMatrix_spec.SetAt(0, n_Obs), PredictorMatrix_spec.SetAt(1, n_predictor), PredictorMatrix_spec.SetAt(2, 0);
			double value;
			for (int i = 0; i < n_predictor; i++) {
				predictor = RegressionModel.SelectedPredictors.GetAt(i);
				for (int j = 0; j < n_Obs; j++) {
					value = Data.GetAt(GetPosition(j, predictor, Data_spec));
					PredictorMatrix.SetAt(GetPosition(j, i, PredictorMatrix_spec), value);
				}
			}
			for (int i = 0; i < n_Obs; i++) {
				value = Data.GetAt(static_cast <int64_t>(GetPosition(i, n_Var - 1, Data_spec)));
				ResponseVector.SetAt(i, value);
			}
			CArray <double> w;
			CArray <double> Sww;
			GetStandardRegressionModel(PredictorMatrix, PredictorMatrix_spec, ResponseVector, w, Sww);
			if (RegressionModel.FactorAnalysis == 0) {
				alpha = RegressionModel.alpha;
				GetStatisticalRegressorAnalysis(Sww);
			}
			if (RegressionModel.FactorAnalysis == 0) ShowStatisticalAnalysis = true;
			else ShowStatisticalAnalysis = false;
			/*if (RegressionModel.ValidationMethod == 0) GetRegressionModel_LOOCV(PredictorMatrix, PredictorMatrix_spec, ResponseVector);
			else if (RegressionModel.ValidationMethod == 1) GetRegressionModel_10_Fold_CV(PredictorMatrix, PredictorMatrix_spec, ResponseVector);
			else if (RegressionModel.ValidationMethod == 2) GetRegressionModel_5_Fold_CV(PredictorMatrix, PredictorMatrix_spec, ResponseVector);
			else {
				double train;
				train = ((double)100 - RegressionModel.PercentageTestingData) / (double)100;
				Metrics.SetSize(static_cast <int64_t>(RegressionModel.NumberRuns) * 5);
				CArray <int> Metrics_spec;
				Metrics_spec.SetSize(3);
				Metrics_spec.SetAt(0, RegressionModel.NumberRuns), Metrics_spec.SetAt(1, 5), Metrics_spec.SetAt(2, 0);
				GetRegressionModel_MonteCarlo(PredictorMatrix, PredictorMatrix_spec, ResponseVector, RegressionModel.NumberRuns, train);
				SaveMatrix("RegressionAnalysis_Independent.txt", Metrics, Metrics_spec);
			}
			if (RegressionModel.ValidationMethod != 3) SaveVector("RegressionAnalysis_Independent.txt", Metrics);*/
		}
	}
	UpdateAllViews(NULL);
}

void CModelingandAnalysisofUncertaintyDoc::GetStandardRegressionModel(CArray <double>& X, CArray <int>& X_spec, CArray <double>& y, CArray <double>& w, CArray <double>& Sww) {
	int n_var = X_spec.GetAt(1), pos;
	double ybar, ystd, value;
	w.RemoveAll();
	Sww.RemoveAll();
	w.SetSize(static_cast <int64_t>(n_var));
	Sww.SetSize(static_cast <int64_t>(n_var));
	CArray <double> X0, y0;
	CArray <int> X0_spec;
	CArray <double> xbar, xstd;
	X0.RemoveAll();
	y0.RemoveAll();
	X0_spec.RemoveAll();
	X0_spec.SetSize(3);
	X0_spec.SetAt(0, n_Obs), X0_spec.SetAt(1, n_var), X0_spec.SetAt(2, 0);
	xbar.RemoveAll();
	xstd.RemoveAll();
	X0.SetSize(static_cast <int64_t>(n_Obs) * n_var);
	y0.SetSize(static_cast <int64_t>(n_Obs));
	xbar.SetSize(static_cast <int64_t>(n_var));
	xstd.SetSize(static_cast <int64_t>(n_var));
	for (int i = 0; i < n_Obs; i++) {
		value = y.GetAt(i);
		y0.SetAt(i, value);
		for (int j = 0; j < n_var; j++) {
			pos = GetPosition(i, j, X_spec);
			value = X.GetAt(pos);
			X0.SetAt(pos, value);
		}
	}
	StandardizeRegressionData(X0, X0_spec, y0, xbar, xstd, ybar, ystd);
	bool validation = false;
	GetRegressionVector(X0, X0_spec, y0, Sww, validation);
	CArray <double> e, y_pred;
	y_pred.RemoveAll();
	e.RemoveAll();
	MatrixVectorProduct(X0, X0_spec, w_raw, y_pred);
	ypred.RemoveAll();
	ypred.SetSize(y_pred.GetSize());
	pos = static_cast <int64_t>(n_Var - 1);
	for (int i = 0; i < y_pred.GetSize(); i++) {
		e.Add(y0.GetAt(i) - y_pred.GetAt(i));
		value = y_pred.GetAt(i);
		value *= ystd;
		value += ybar;
		ypred.SetAt(i, value);
	}
	CArray <double> metric;
	GetRegressionMetrics(y0, e, metric);
	Metrics_Raw.RemoveAll();
	Metrics_Raw.Add(metric.GetAt(0)), Metrics_Raw.Add(metric.GetAt(1)), Metrics_Raw.Add(metric.GetAt(2)),
		Metrics_Raw.Add(metric.GetAt(3)), Metrics_Raw.Add(metric.GetAt(4));
	double R2_adj = 1 - (1 - Metrics_Raw.GetAt(4)) * (n_Obs - 1) / (n_Obs - n_var - 1);
	Metrics_Raw.Add(R2_adj);
	double SSy = (double)0, SSr = (double)0, SSyhat = (double)0;
	for (int i = 0; i < n_Obs; i++) {
		value = y0.GetAt(i);
		SSy += value * value;
		value = y_pred.GetAt(i);
		SSyhat += value * value;
		value = e.GetAt(i);
		SSr += value * value;
	}
	value = ystd;
	SSy *= value * value;
	SSyhat *= value * value;
	SSr *= value * value;
	Metrics_Raw.Add(SSy), Metrics_Raw.Add(SSyhat), Metrics_Raw.Add(SSr);
}

void CModelingandAnalysisofUncertaintyDoc::GetRegressionModel_LOOCV(CArray <double>& X, CArray <int>& X_spec, CArray <double>& y) {
	bool validation = true;
	int n_var = X_spec.GetAt(1);
	CArray <double> Xtrain, xbar, xstd, ytrain, xtest, ytest, e, w, Sww;
	CArray <int> Xtrain_spec;
	double ybar, ystd, ypred, value;
	Xtrain.RemoveAll();
	ytrain.RemoveAll();
	xtest.RemoveAll();
	ytest.RemoveAll();
	e.RemoveAll();
	Xtrain_spec.RemoveAll();
	Xtrain_spec.SetSize(3);
	Xtrain_spec.SetAt(0, n_Obs - 1), Xtrain_spec.SetAt(1, n_var), Xtrain_spec.SetAt(2, 0);
	Xtrain.SetSize(static_cast <int64_t>(n_Obs - 1) * n_var);
	xtest.SetSize(static_cast <int64_t>(n_var));
	ytrain.SetSize(static_cast <int64_t>(n_Obs - 1));
	ytest.SetSize(static_cast <int64_t>(n_Obs));
	e.SetSize(static_cast <int64_t>(n_Obs));
	xbar.RemoveAll();
	xstd.RemoveAll();
	xbar.SetSize(static_cast <int64_t>(n_var));
	xstd.SetSize(static_cast <int64_t>(n_var));
	int counter;
	for (int i = 0; i < n_Obs; i++) {
		counter = 0;
		for (int k = 0; k < n_Obs; k++) {
			if (i == k) {
				for (int j = 0; j < n_var; j++) {
					value = X.GetAt(GetPosition(k, j, X_spec));
					xtest.SetAt(j, value);
				}
				ytest.GetAt(i) = y.GetAt(k);
			}
			else {
				for (int j = 0; j < n_var; j++) {
					value = X.GetAt(GetPosition(k, j, X_spec));
					Xtrain.SetAt(GetPosition(counter, j, Xtrain_spec), value);
				}
				ytrain.SetAt(counter, y.GetAt(k));
				counter++;
			}
		}
		StandardizeRegressionData(Xtrain, Xtrain_spec, ytrain, xbar, xstd, ybar, ystd);
		ytest.GetAt(i) -= ybar;
		ytest.GetAt(i) /= ystd;
		for (int j = 0; j < n_var; j++) {
			value = xtest.GetAt(j);
			value -= xbar.GetAt(j);
			value /= xstd.GetAt(j);
			xtest.SetAt(j, value);
		}
		GetRegressionVector(Xtrain, Xtrain_spec, ytrain, Sww, validation);
		ypred = (double)0;
		for (int j = 0; j < n_var; j++) ypred += xtest.GetAt(j) * w.GetAt(j);
		e.SetAt(i, ytest.GetAt(i) - ypred);
	}
	CArray <double> metrics;
	GetRegressionMetrics(ytest, e, metrics);
}

void CModelingandAnalysisofUncertaintyDoc::GetRegressionModel_5_Fold_CV(CArray <double>& X, CArray <int>& X_spec, CArray <double>& y) {
	bool validation = true;
	int n_var = X_spec.GetAt(1), pos, n_block = (int)(0.2 * n_Obs), n_0 = (int)0, n_1 = (int)n_block - 1, counter_train = 0, counter_test = 0, counter_e = 0;
	CArray <double> X0, Xtrain, Xtest, xbar, xstd, ytrain, ytest, e, w, Sww, y0;
	CArray <int> Xtrain_spec, Xtest_spec;
	double ybar, ystd, value, ypred;
	e.RemoveAll();
	y0.RemoveAll();
	X0.RemoveAll();
	Xtrain_spec.RemoveAll();
	Xtrain_spec.SetSize(3);
	Xtest_spec.RemoveAll();
	Xtest_spec.SetSize(3);
	e.SetSize(n_Obs);
	y0.SetSize(n_Obs);
	xbar.RemoveAll();
	xstd.RemoveAll();
	xbar.SetSize(n_var);
	xstd.SetSize(n_var);
	X0.SetSize(static_cast <int64_t>(n_Obs) * n_var);
	std::vector<int> index;
	for (int i = 0; i < n_Obs; ++i) index.push_back(i);
	std::random_shuffle(index.begin(), index.end());
	for (int i = 0; i < n_Obs; i++) {
		for (int j = 0; j < n_var; j++) {
			value = X.GetAt(GetPosition(index[i], j, X_spec));
			X0.SetAt(GetPosition(i, j, X_spec), value);
		}
	}
	for (int i = 0; i < 5; i++) {
		Xtrain.RemoveAll();
		Xtest.RemoveAll();
		ytrain.RemoveAll();
		ytest.RemoveAll();
		counter_train = 0, counter_test = 0;
		if (i < 4) {
			Xtrain.SetSize(static_cast <int64_t>(n_Obs - n_block) * n_var);
			Xtrain_spec.SetSize(3);
			Xtrain_spec.SetAt(0, n_Obs - n_block), Xtrain_spec.SetAt(1, n_var), Xtrain_spec.SetAt(2, 0);
			Xtest.SetSize(static_cast <int64_t>(n_block) * n_var);
			Xtest_spec.SetAt(0, n_block), Xtest_spec.SetAt(1, n_var), Xtest_spec.SetAt(2, 0);
			ytrain.SetSize(static_cast <int64_t>(n_Obs - n_block));
			ytest.SetSize(static_cast <int64_t>(n_block));
		}
		else {
			Xtrain.SetSize(static_cast <int64_t>(4 * n_block) * n_var);
			Xtrain_spec.SetSize(3);
			Xtrain_spec.SetAt(0, 4 * n_block), Xtrain_spec.SetAt(1, n_var), Xtrain_spec.SetAt(2, 0);
			Xtest.SetSize(static_cast <int64_t>(n_Obs - 4 * n_block) * n_var);
			Xtest_spec.SetAt(0, n_Obs - 4 * n_block), Xtest_spec.SetAt(1, n_var), Xtest_spec.SetAt(2, 0);
			ytrain.SetSize(static_cast <int64_t>(4 * n_block));
			ytest.SetSize(static_cast <int64_t>(n_Obs - 4 * n_block));
		}
		for (int k = 0; k < n_Obs; k++) {
			if ((k >= n_0) && (k <= n_1)) {
				for (int j = 0; j < n_var; j++) {
					value = X.GetAt(GetPosition(k, j, X_spec));
					Xtest.SetAt(GetPosition(counter_test, j, Xtest_spec), value);
				}
				value = y.GetAt(k);
				ytest.SetAt(counter_test, value);
				counter_test++;
			}
			else {
				for (int j = 0; j < n_var; j++) {
					value = X.GetAt(GetPosition(k, j, X_spec));
					Xtrain.SetAt(GetPosition(counter_train, j, Xtrain_spec), value);
				}
				value = y.GetAt(k);
				ytrain.SetAt(counter_train, value);
				counter_train++;
			}
		}
		if (i < 3) {
			n_0 += n_block;
			n_1 += n_block;
		}
		else {
			n_0 += n_block;
			n_1 = n_Obs;
		}
		StandardizeRegressionData(Xtrain, Xtrain_spec, ytrain, xbar, xstd, ybar, ystd);
		GetRegressionVector(Xtrain, Xtrain_spec, ytrain, Sww, validation);
		for (int k = 0; k < counter_test; k++) {
			ypred = (double)0;
			for (int j = 0; j < n_var; j++) {
				pos = GetPosition(k, j, Xtest_spec);
				value = Xtest.GetAt(pos);
				value -= xbar.GetAt(j);
				value /= xstd.GetAt(j);
				Xtest.SetAt(pos, value);
				ypred += value * w.GetAt(j);
			}
			value = ytest.GetAt(k);
			value -= ybar;
			value /= ystd;
			y0.SetAt(counter_e, value);
			e.SetAt(counter_e, value - ypred);
			counter_e++;
		}
	}
	CArray <double> metrics;
	GetRegressionMetrics(y0, e, metrics);
}

void CModelingandAnalysisofUncertaintyDoc::GetRegressionModel_10_Fold_CV(CArray <double>& X, CArray <int>& X_spec, CArray <double>& y) {
	bool validation = true;
	int n_var = X_spec.GetAt(1), pos, n_block = (int)(0.1 * n_Obs), n_0 = (int)0, n_1 = (int)n_block - 1, counter_train = 0, counter_test = 0, counter_e = 0;
	CArray <double> X0, Xtrain, Xtest, xbar, xstd, ytrain, ytest, e, w, Sww, y0;
	CArray <int> Xtrain_spec, Xtest_spec;
	double ybar, ystd, value, ypred;
	e.RemoveAll();
	y0.RemoveAll();
	X0.RemoveAll();
	Xtrain_spec.RemoveAll();
	Xtrain_spec.SetSize(3);
	Xtest_spec.RemoveAll();
	Xtest_spec.SetSize(3);
	e.SetSize(n_Obs);
	y0.SetSize(n_Obs);
	xbar.RemoveAll();
	xstd.RemoveAll();
	xbar.SetSize(n_var);
	xstd.SetSize(n_var);
	std::vector<int> index;
	for (int i = 0; i < n_Obs; ++i) index.push_back(i);
	std::random_shuffle(index.begin(), index.end());
	X0.SetSize(static_cast <int64_t>(n_Obs) * n_var);
	for (int i = 0; i < n_Obs; i++) {
		for (int j = 0; j < n_var; j++) {
			value = X.GetAt(GetPosition(index[i], j, X_spec));
			X0.SetAt(GetPosition(i, j, X_spec), value);
		}
	}
	for (int i = 0; i < 10; i++) {
		Xtrain.RemoveAll();
		Xtest.RemoveAll();
		ytrain.RemoveAll();
		ytest.RemoveAll();
		counter_train = 0, counter_test = 0;
		if (i < 9) {
			Xtrain.SetSize(static_cast <int64_t>(n_Obs - n_block) * n_var);
			Xtrain_spec.SetSize(3);
			Xtrain_spec.SetAt(0, n_Obs - n_block), Xtrain_spec.SetAt(1, n_var), Xtrain_spec.SetAt(2, 0);
			Xtest.SetSize(static_cast <int64_t>(n_block) * n_var);
			Xtest_spec.SetAt(0, n_block), Xtest_spec.SetAt(1, n_var), Xtest_spec.SetAt(2, 0);
			ytrain.SetSize(static_cast <int64_t>(n_Obs - n_block));
			ytest.SetSize(static_cast <int64_t>(n_block));
		}
		else {
			Xtrain.SetSize(static_cast <int64_t>(9 * n_block) * n_var);
			Xtrain_spec.SetSize(3);
			Xtrain_spec.SetAt(0, 9 * n_block), Xtrain_spec.SetAt(1, n_var), Xtrain_spec.SetAt(2, 0);
			Xtest.SetSize(static_cast <int64_t>(n_Obs - 9 * n_block) * n_var);
			Xtest_spec.SetAt(0, n_Obs - 9 * n_block), Xtest_spec.SetAt(1, n_var), Xtest_spec.SetAt(2, 0);
			ytrain.SetSize(static_cast <int64_t>(9 * n_block));
			ytest.SetSize(static_cast <int64_t>(n_Obs - 9 * n_block));
		}
		for (int k = 0; k < n_Obs; k++) {
			if ((k >= n_0) && (k <= n_1)) {
				for (int j = 0; j < n_var; j++) {
					value = X0.GetAt(GetPosition(k, j, X_spec));
					Xtest.SetAt(GetPosition(counter_test, j, Xtest_spec), value);
				}
				value = y.GetAt(k);
				ytest.SetAt(counter_test, value);
				counter_test++;
			}
			else {
				for (int j = 0; j < n_var; j++) {
					value = X0.GetAt(GetPosition(k, j, X_spec));
					Xtrain.SetAt(GetPosition(counter_train, j, Xtrain_spec), value);
				}
				value = y.GetAt(k);
				ytrain.SetAt(counter_train, value);
				counter_train++;
			}
		}
		if (i < 8) {
			n_0 += n_block;
			n_1 += n_block;
		}
		else {
			n_0 += n_block;
			n_1 = n_Obs;
		}
		StandardizeRegressionData(Xtrain, Xtrain_spec, ytrain, xbar, xstd, ybar, ystd);
		GetRegressionVector(Xtrain, Xtrain_spec, ytrain, Sww, validation);
		for (int k = 0; k < counter_test; k++) {
			ypred = (double)0;
			for (int j = 0; j < n_var; j++) {
				pos = GetPosition(k, j, Xtest_spec);
				value = Xtest.GetAt(pos);
				value -= xbar.GetAt(j);
				value /= xstd.GetAt(j);
				Xtest.SetAt(pos, value);
				ypred += value * w.GetAt(j);
			}
			value = ytest.GetAt(k);
			value -= ybar;
			value /= ystd;
			y0.SetAt(counter_e, value);
			e.SetAt(counter_e, value - ypred);
			counter_e++;
		}
	}
	CArray <double> metrics;
	GetRegressionMetrics(y0, e, metrics);
}

void CModelingandAnalysisofUncertaintyDoc::GetRegressionModel_MonteCarlo(CArray <double>& X, CArray <int>& X_spec, CArray <double>& y, int nRuns, double train) {
	bool validation = true;
	int n_var = X_spec.GetAt(1), n_train = (int)(train * n_Obs), counter_train, counter_test;
	CArray <double> Xtrain, Xtest, ytrain, ytest, xbar, xstd, e, w, Sww, y0;
	CArray <int> Xtrain_spec, Xtest_spec;
	double ybar, ystd, value, ypred;
	Xtrain.SetSize(static_cast <int64_t>(n_train) * n_var);
	Xtrain_spec.SetSize(3);
	Xtrain_spec.SetAt(0, n_train), Xtrain_spec.SetAt(1, n_var), Xtrain_spec.SetAt(2, 0);
	Xtest.SetSize(static_cast <int64_t>(n_Obs - n_train) * n_var);
	Xtest_spec.SetSize(3);
	Xtest_spec.SetAt(0, n_Obs - n_train), Xtest_spec.SetAt(1, n_var), Xtest_spec.SetAt(2, 0);
	ytrain.SetSize(n_train);
	ytest.SetSize(static_cast <int64_t>(n_Obs - n_train));
	std::vector<int> index;
	xbar.SetSize(n_var);
	xstd.SetSize(n_var);
	e.SetSize(static_cast <int64_t>(n_Obs - n_train));
	y0.SetSize(static_cast <int64_t>(n_Obs - n_train));
	CString Text;
	for (int k = 0; k < nRuns; k++) {
		index.clear();
		for (int i = 0; i < n_Obs; i++) index.push_back(i);
		std::random_shuffle(index.begin(), index.end());
		counter_train = 0, counter_test = 0;
		for (int i = 0; i < n_Obs; i++) {
			if (i < n_train) {
				for (int j = 0; j < n_var; j++) {
					value = X.GetAt(GetPosition(index[i], j, X_spec));
					Xtrain.SetAt(GetPosition(counter_train, j, Xtrain_spec), value);
				}
				value = y.GetAt(index[i]);
				ytrain.SetAt(counter_train, value);
				counter_train++;
			}
			else {
				for (int j = 0; j < n_var; j++) {
					value = X.GetAt(GetPosition(index[i], j, X_spec));
					Xtest.SetAt(GetPosition(counter_test, j, Xtest_spec), value);
				}
				value = y.GetAt(index[i]);
				ytest.SetAt(counter_test, value);
				counter_test++;
			}
		}
		StandardizeRegressionData(Xtrain, Xtrain_spec, ytrain, xbar, xstd, ybar, ystd);
		GetRegressionVector(Xtrain, Xtrain_spec, ytrain, Sww, validation);
		for (int i = 0; i < n_Obs - n_train; i++) {
			ypred = (double)0;
			for (int j = 0; j < n_var; j++) {
				value = Xtest.GetAt(GetPosition(i, j, Xtest_spec));
				value -= xbar.GetAt(j);
				value /= xstd.GetAt(j);
				ypred += value * w.GetAt(j);
			}
			value = ytest.GetAt(i);
			value -= ybar;
			value /= ystd;
			e.SetAt(i, value - ypred);
			y0.SetAt(i, value);
		}
		CArray <double> metrics;
		GetRegressionMetrics(y0, e, metrics);
		for (int j = 0; j < 5; j++) Metrics.SetAt(GetPosition(k, j, Metrics_spec), Metrics.GetAt(j));
	}
}

void CModelingandAnalysisofUncertaintyDoc::StandardizeRegressionData(CArray <double>& X0, CArray <int>& X_spec, CArray <double>& y0, CArray <double>& Xbar, CArray <double>& Xstd, double& ybar, double& ystd) {
	int pos, n_obs = X_spec.GetAt(0), n_var = X_spec.GetAt(1);
	double xbar, xstd, value, factor;
	for (int i = 0; i < n_var; i++) {
		xbar = (double)0, xstd = (double)0, factor = (double)0;
		for (int j = 0; j < n_obs; j++) {
			value = X0.GetAt(GetPosition(j, i, X_spec));
			if (j == 0) factor = value;
			else {
				if (value > factor) factor = value;
			}
		}
		for (int j = 0; j < n_obs; j++) {
			pos = GetPosition(j, i, X_spec);
			value = X0.GetAt(pos);
			value /= factor;
			xbar += value;
			xstd += value * value;
			X0.SetAt(pos, value);
		}
		xbar /= (double)X_spec.GetAt(0);
		xstd -= (double)(X_spec.GetAt(0)) * xbar * xbar;
		xstd /= (double)(X_spec.GetAt(0) - (int)1);
		xstd = sqrt(xstd);
		for (int j = 0; j < n_obs; j++) {
			pos = GetPosition(j, i, X_spec);
			value = X0.GetAt(pos);
			value -= xbar;
			value /= xstd;
			X0.SetAt(pos, value);
		}
		xbar *= factor;
		xstd *= factor;
		Xbar.SetAt(i, xbar);
		Xstd.SetAt(i, xstd);
	}
	factor = (double)0;
	for (int i = 0; i < n_obs; i++) {
		value = y0.GetAt(i);
		if (i == 0) factor = value;
		if (value > factor) factor = value;
	}
	ybar = (double)0, ystd = (double)0;
	for (int i = 0; i < n_obs; i++) {
		value = y0.GetAt(i);
		value /= factor;
		ybar += value;
		ystd += value * value;
		y0.SetAt(i, value);
	}
	ybar /= (double)(y0.GetSize());
	ystd -= (double)(y0.GetSize()) * ybar * ybar;
	ystd /= (double)(y0.GetSize() - (int)1);
	ystd = sqrt(ystd);
	for (int i = 0; i < n_obs; i++) {
		value = y0.GetAt(i);
		value -= ybar;
		value /= ystd;
		y0.SetAt(i, value);
	}
	ybar *= factor;
	ystd *= factor;
}

void CModelingandAnalysisofUncertaintyDoc::GetRegressionVector(CArray <double>& X, CArray <int>& X_spec, CArray <double>& y, CArray <double>& Sww, bool validation) {
	CArray <double> Z;
	CArray <int> Z_spec;
	int n_obs = X_spec.GetAt(0), n_var = X_spec.GetAt(1);
	double value;
	w.RemoveAll();
	w.SetSize(n_var);
	Z.RemoveAll();
	Z_spec.RemoveAll();
	Z.SetSize(static_cast <int64_t>(n_var + 1) * n_obs);
	Z_spec.SetSize(3);
	Z_spec.SetAt(0, n_obs), Z_spec.SetAt(1, n_var + 1), Z_spec.SetAt(2, 0);
	for (int i = 0; i < n_obs; i++) {
		for (int j = 0; j < n_var; j++) {
			value = X.GetAt(GetPosition(i, j, X_spec));
			Z.SetAt(GetPosition(i, j, Z_spec), value);
		}
		value = y.GetAt(i);
		Z.SetAt(GetPosition(i, n_var, Z_spec), value);
	}
	CArray <double>R;
	CArray <int>R_spec;
	bool flag;
	QR(Z, Z_spec, R, R_spec, flag);
	CArray <double> Rx;
	CArray <double> rxy;
	CArray <int> Rx_spec;
	Rx.RemoveAll(), rxy.RemoveAll(), Rx_spec.RemoveAll();
	Rx.SetSize(static_cast <int64_t>(n_var + 1) * n_var / 2);
	rxy.SetSize(static_cast <int64_t>(n_var));
	Rx_spec.SetSize(3);
	Rx_spec.SetAt(0, n_var), Rx_spec.SetAt(1, n_var), Rx_spec.SetAt(2, 3);
	for (int i = 0; i < n_var; i++) {
		for (int j = i; j < n_var; j++) {
			value = R.GetAt(GetPosition(i, j, R_spec));
			Rx.SetAt(GetPosition(i, j, Rx_spec), value);
		}
		value = R.GetAt(GetPosition(i, n_var, R_spec));
		rxy.SetAt(i, value);
	}
	CArray <double> Rx_inv;
	Inverse(Rx, Rx_spec, Rx_inv, Rx_spec);
	if (validation == true) MatrixVectorProduct(Rx_inv, Rx_spec, rxy, w);
	else MatrixVectorProduct(Rx_inv, Rx_spec, rxy, w_raw);
	if (validation == false) {
		double se = GetSquaredLength(y) - GetSquaredLength(rxy), value, temp;
		se /= (double)(n_obs - n_var - 1);
		Rx_spec.SetAt(2, 3);
		int pos;
		for (int i = 0; i < n_var; i++) {
			value = (double)0;
			for (int k = i; k < n_var; k++) {
				pos = GetPosition(i, k, Rx_spec);
				temp = Rx_inv.GetAt(pos);
				value += temp * temp;

			}
			Sww.SetAt(i, value * se);
		}
	}
}

void CModelingandAnalysisofUncertaintyDoc::GetStatisticalRegressorAnalysis(CArray <double>& Sww) {
	int n_var = (int)w_raw.GetSize();
	CString Text;
	double value, temp1, temp2;
	ConfIntervals.RemoveAll();
	t_values.RemoveAll();
	ConfIntervals.SetSize(static_cast <int64_t>(n_var) * 2);
	t_values.SetSize(static_cast <int64_t>(n_var));
	ProbabilityDistributions t('t', (double)(n_Obs - n_var - 1), (double)0);
	t_alpha = t.CriticalValue(alpha / (double)2);
	for (int i = 0; i < n_var; i++) {
		temp1 = w_raw.GetAt(i);
		temp2 = sqrt(Sww.GetAt(i));
		value = temp1 / temp2;
		t_values.SetAt(i, value);
		value = t_alpha * temp2;
		ConfIntervals.SetAt(static_cast <int64_t>(2 * i), temp1 - value), ConfIntervals.SetAt(static_cast <int64_t>(2 * i + 1), temp1 + value);
	}
}

void CModelingandAnalysisofUncertaintyDoc::GetRegressionMetrics(CArray <double>& y, CArray <double>& e, CArray <double>& metrics) {
	metrics.RemoveAll();
	metrics.SetSize(5);
	CArray <double> ytemp;
	ytemp.RemoveAll();
	for (int i = 0; i < y.GetSize(); i++) ytemp.Add(y.GetAt(i));
	CenterVector(ytemp);
	double MSE = (double)0, RMSE, MAE = (double)0, R2, rho = (double)0, value;
	for (int i = 0; i < y.GetSize(); i++) {
		value = e.GetAt(i);
		MSE += value * value;
		MAE += fabs(value);
	}
	MSE /= (double)(y.GetSize());
	MAE /= (double)(y.GetSize());
	RMSE = sqrt(MSE);
	CArray <double> ypred;
	ypred.RemoveAll();
	for (int i = 0; i < y.GetSize(); i++) ypred.Add(y.GetAt(i) - e.GetAt(i));
	CenterVector(ypred);
	CenterVector(ytemp);
	double ypred_length = GetLength(ypred), y_length = GetLength(ytemp);
	for (int i = 0; i < y.GetSize(); i++) rho += ypred.GetAt(i) * ytemp.GetAt(i);
	rho /= ypred_length * y_length;
	R2 = 1 - GetSquaredLength(e) / GetSquaredLength(ytemp);
	metrics.SetAt(0, MSE), metrics.SetAt(1, RMSE), metrics.SetAt(2, MAE), metrics.SetAt(3, rho), metrics.SetAt(4, R2);
}

//*****************************************************************
//***            Compute partial least squares model            ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnPLS() {
	AfxMessageBox(L"Now we are working on establishing PLS model");
}

//*****************************************************************
//***        Compute  kernel  principal component  model        ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnKPCA() {
	AfxMessageBox(L"Now we are working on establishing kPCA model");
}

//*****************************************************************
//***            Compute  logistic regression  model            ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnLR() {
	AfxMessageBox(L"Now we are working on establishing logistic regression model");
}

//*****************************************************************
//***         Compute  support  vector  machine  model          ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnSVM() {
	AfxMessageBox(L"Now we are working on establishing SVM model");
}

//*****************************************************************
//***     Compute  kernel  support  vector  machine  model      ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnKSVM() {
	AfxMessageBox(L"Now we are working on establishing kSVM model");
}

//*****************************************************************
//***  Compute  kernel  Fisher  discriminant  analysis  model   ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnKFDA() {
	AfxMessageBox(L"Now we are working on establishing kFDA model");
}

void CModelingandAnalysisofUncertaintyDoc::OnL1_Regularization() {
	AfxMessageBox(L"Now we are working on establishing L1 regularized regression model");
}

void CModelingandAnalysisofUncertaintyDoc::OnL2_Regularization() {
	AfxMessageBox(L"Now we are working on establishing L2 regularized regression model");
}

//*****************************************************************
//***        Compute kernel partial least squares model         ***
//*****************************************************************

void CModelingandAnalysisofUncertaintyDoc::OnKPLS() {
	AfxMessageBox(L"Now we are working on establishing KPLS model");
}

//*****************************************************************
//***       Compute artificial neural network (ANN) model       ***
//*****************************************************************

/****************** Helper functions for loading dataset ******************/
/* This function read the FDA files, load the features into a dlib matrix
   and load the label into a vector. Then output the number of features. */
int load_data(const std::string& filename, std::vector<dlib::matrix<float>>& data, std::vector<unsigned long>& labels) {
	// Check if files can be opened
	CString message;
	std::ifstream file(filename);
	if (!file) {
		message.Format(_T("Unable to open file: %S\n"), filename.c_str());
		AfxMessageBox(message);
		return -1;
	}

	std::string line;
	// Read the first line to get the number of features
	if (!std::getline(file, line)) {
		message.Format(_T("File description 1 format incorrect: %S\n"), filename.c_str());
		AfxMessageBox(message);
		return -1;
	}

	std::stringstream firstLine(line);
	int num_features;
	char comma;
	if (!(firstLine >> num_features >> comma)) {
		message.Format(_T("File description 2 format incorrect: %S\n"), filename.c_str());
		AfxMessageBox(message);
		return -1;
	}

	// Decrement by 1 to exclude the label
	num_features -= 1;

	// Skip the remaining description lines
	for (int i = 0; i < 3; ++i) {
		if (!std::getline(file, line)) {
			message.Format(_T("File description 3 format incorrect: %S\n"), filename.c_str());
			AfxMessageBox(message);
			return -1;
		}
	}

	// Read the data
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		dlib::matrix<float> datapoint;
		datapoint.set_size(1, num_features);

		// Parse the features
		for (int i = 0; i < num_features; ++i) {
			if (!(ss >> datapoint(0, i))) {
				message.Format(_T("File feature data format incorrect: %S\n"), filename.c_str());
				AfxMessageBox(message);
				return -1;
			}

			// Skip the comma
			if (i < num_features - 1 && ss.peek() == ',') {
				ss.ignore();
			}
		}

		// Skip the comma before the label
		if (ss.peek() == ',') {
			ss.ignore();
		}

		// Get the label
		unsigned long label;
		if (!(ss >> label)) {
			message.Format(_T("File label data format incorrect: %S\n"), filename.c_str());
			AfxMessageBox(message);
			return -1;
		}

		data.push_back(datapoint);
		labels.push_back(label - 1); // Dlib's labels are zero-indexed, but our data is 1-indexed
	}

	return num_features;
}

/************** Helper functions for standardize dataset **************/
/* This function standardize the dataset based on number of features */
void standardize_data(int num_features, std::vector<dlib::matrix<float>>& data) {
	// Compute the mean and standard deviation for each feature
	dlib::matrix<float> mean(1, num_features);
	dlib::matrix<float> stddev(1, num_features);
	for (int i = 0; i < num_features; ++i) {
		float sum = 0.0;
		for (const auto& datapoint : data) {
			sum += datapoint(0, i);
		}
		mean(0, i) = sum / data.size();

		float sq_diff_sum = 0.0;
		for (const auto& datapoint : data) {
			sq_diff_sum += pow(datapoint(0, i) - mean(0, i), 2);
		}
		stddev(0, i) = sqrt(sq_diff_sum / data.size());
	}

	// Standardize the data
	for (auto& datapoint : data) {
		for (int i = 0; i < num_features; ++i) {
			datapoint(0, i) = (datapoint(0, i) - mean(0, i)) / stddev(0, i);
		}
	}
}

/*************** Helper functions for splitting and shuffling dataset ***************/
/* This function split the dataset into training and testing sets and shuffle them */
void split_data(const std::vector<dlib::matrix<float>>& data, const std::vector<unsigned long>& labels,
	std::vector<dlib::matrix<float>>& training_data, std::vector<unsigned long>& training_labels,
	std::vector<dlib::matrix<float>>& testing_data, std::vector<unsigned long>& testing_labels, float ratio = 0.85) {

	std::vector<int> indices(data.size());
	std::iota(indices.begin(), indices.end(), 0);  // Fill with 0, 1, ..., data.size() - 1

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(indices.begin(), indices.end(), g);

	int training_size = ratio * data.size();
	training_data.resize(training_size);
	training_labels.resize(training_size);
	testing_data.resize(data.size() - training_size);
	testing_labels.resize(data.size() - training_size);

	for (int i = 0; i < indices.size(); ++i) {
		if (i < training_size) {
			training_data[i] = data[indices[i]];
			training_labels[i] = labels[indices[i]];
		}
		else {
			testing_data[i - training_size] = data[indices[i]];
			testing_labels[i - training_size] = labels[indices[i]];
		}
	}
}

/*************** Helper functions for saving training status ***************/
/* This function takes in epoch number and loss and record into a txt file */
void record_loss(const std::string& message) {
	std::ofstream outFile("ANN_Update/training_loss.txt", std::ios::app); // Open in append mode
	if (!outFile.is_open())
	{
		AfxMessageBox(L"Failed to open the output file!");
		return;
	}

	outFile << message << std::endl;
	outFile.close();
}

// record the accuracy of the training and testing set
void record_acc(const std::string& message) {
	std::ofstream outFile("ANN_Update/training_acc.txt", std::ios::app); // Open in append mode
	if (!outFile.is_open())
	{
		AfxMessageBox(L"Failed to open the output file!");
		return;
	}

	outFile << message << std::endl;
	outFile.close();
}

/***************************************************/
/******************* EEA Dataset *******************/
/***************************************************/

/* Run the neural network through the EEA dataset allow user to define
   Learning rate, total number of epoch, batch size, and the frequency
   of training status */

   // Data Structure for user input
struct ANNStruct {
	double lr;
	int total_epoch;
	int batch_sizes;
	std::vector<double> losses;
	std::vector<double> training_accuracies;
	std::vector<double> testing_accuracies;

	HANDLE hEvent;
};

// EEA Neural Network 1 layers
void ANN_EEA1(double lr, int total_epoch, int batch_sizes, HANDLE hEvent) {
	// Load the data from file
	std::vector<dlib::matrix<float>> data;
	std::vector<unsigned long> labels;
	int num_features = load_data("datasets/EEA1-Tom20-6Classes-R21.FDA", data, labels);

	// Clear the status txt file
	std::ofstream clear_file_loss("ANN_Update/training_loss.txt", std::ios::trunc);
	clear_file_loss.close();
	std::ofstream clear_file_acc("ANN_Update/training_acc.txt", std::ios::trunc);
	clear_file_acc.close();

	// Standardize the data
	standardize_data(num_features, data);

	// Split and shuffle data
	std::vector<dlib::matrix<float>> training_data;
	std::vector<unsigned long>       training_labels;
	std::vector<dlib::matrix<float>> testing_data;
	std::vector<unsigned long>       testing_labels;
	split_data(data, labels, training_data, training_labels, testing_data, testing_labels);

	std::vector<double> losses;
	std::vector<double> training_accuracies;
	std::vector<double> testing_accuracies;

	// define the network
	using net_type = dlib::loss_multiclass_log<
		dlib::fc< 6,
		dlib::htan<dlib::fc<100,
		dlib::input<dlib::matrix<float>>
		>>>>;

	//make a network instance
	net_type net;

	// And then train it using the EEA1 data.
	dlib::dnn_trainer<net_type, dlib::adam> trainer(net, dlib::adam(0.0005, 0.9, 0.999));
	trainer.set_learning_rate(lr);

	// training setting
	int num_epochs = total_epoch;
	int batch_size = batch_sizes;

	// Begin training.
	std::vector<dlib::matrix<float>> batch_data;
	std::vector<unsigned long>		 batch_labels;
	dlib::rand rnd(time(0));
	double batch_loss;

	auto start = std::chrono::high_resolution_clock::now(); // Record start time
	// Calculate total number of mini-batches
	int total_batches = (training_data.size() + batch_size - 1) / batch_size;

	for (int epoch = 0; epoch < num_epochs; ++epoch)
	{
		// Loop over mini-batches
		for (int batch_idx = 0; batch_idx < total_batches; ++batch_idx)
		{
			// clear the previous batch
			batch_data.clear();
			batch_labels.clear();

			// Create a batch
			int start_idx = batch_idx * batch_size;
			batch_loss = 0.0;
			for (int idx = start_idx; idx < std::min(start_idx + batch_size, (int)training_data.size()); ++idx)
			{
				batch_data.push_back(training_data[idx]);
				batch_labels.push_back(training_labels[idx]);
			}

			// train with batch data
			trainer.train_one_step(batch_data, batch_labels);

			// Compute the loss for the batch
			batch_loss = trainer.get_average_loss();
			trainer.clear_average_loss();
			losses.push_back(batch_loss);

			// Record the loss
			std::ostringstream message;
			message << "Epoch " << epoch + 1 << " iteration " << (batch_idx + 1) << "/" << total_batches << " loss: " << batch_loss;
			record_loss(message.str());
		}

		// Calculate and record the accuracy for the training set
		auto training_predictions = net(training_data);
		int correct_predictions = std::inner_product(training_predictions.begin(), training_predictions.end(), training_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double training_accuracy = correct_predictions / static_cast<double>(training_data.size());
		std::ostringstream message;
		message << "Epoch " << epoch + 1 << " training accuracy: " << training_accuracy;
		record_acc(message.str());
		training_accuracies.push_back(training_accuracy);

		// Calculate and record the accuracy for the testing set
		auto testing_predictions = net(testing_data);
		correct_predictions = std::inner_product(testing_predictions.begin(), testing_predictions.end(), testing_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double testing_accuracy = correct_predictions / static_cast<double>(testing_data.size());
		message.str("");
		message << "Epoch " << epoch + 1 << " testing accuracy: " << testing_accuracy;
		record_acc(message.str());
		testing_accuracies.push_back(testing_accuracy);

		SetEvent(hEvent);
	}

	// synchronization for seperate thread
	trainer.get_net();
	auto end = std::chrono::high_resolution_clock::now(); // Record end time

	// Calculate time taken
	std::chrono::duration<double> diff = end - start;

	// Display the training duration
	CString durationText;
	durationText.Format(L"Training took %f seconds", diff.count());
	AfxMessageBox(durationText);

	// clean the network of the state from last batch
	net.clean();

	//dlib::serialize("ANN_Update/eea_network.dat") << net;
	//dlib::deserialize("ANN_Update/meta_network.dat") >> net;

	// Test the trained neural network
	int num_right = 0;
	int num_wrong = 0;
	CString numRightText;
	CString numWrongText;
	CString accuracyText;

	// training set
	std::vector<unsigned long> predicted_labels = net(training_data);
	for (size_t i = 0; i < training_data.size(); i++)
	{
		if (predicted_labels[i] == training_labels[i])
			num_right++;
		else
			num_wrong++;

	}
	numRightText.Format(L"training num_right: %d", num_right);
	numWrongText.Format(L"training num_wrong: %d", num_wrong);
	accuracyText.Format(L"training accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);

	// testing set
	predicted_labels = net(testing_data);
	num_right = 0;
	num_wrong = 0;
	for (size_t i = 0; i < testing_data.size(); ++i)
	{
		if (predicted_labels[i] == testing_labels[i])
			++num_right;
		else
			++num_wrong;

	}
	numRightText.Empty(), numRightText.Format(L"testing num_right: %d", num_right);
	numWrongText.Empty(), numWrongText.Format(L"testing num_wrong: %d", num_wrong);
	accuracyText.Empty(), accuracyText.Format(L"testing accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);
}

// EEA Neural Network 2 layers
void ANN_EEA2(double lr, int total_epoch, int batch_sizes, HANDLE hEvent) {
	// Load the data from file
	std::vector<dlib::matrix<float>> data;
	std::vector<unsigned long> labels;
	int num_features = load_data("datasets/EEA1-Tom20-6Classes-R21.FDA", data, labels);

	// Clear the status txt file
	std::ofstream clear_file_loss("ANN_Update/training_loss.txt", std::ios::trunc);
	clear_file_loss.close();
	std::ofstream clear_file_acc("ANN_Update/training_acc.txt", std::ios::trunc);
	clear_file_acc.close();

	// Standardize the data
	standardize_data(num_features, data);

	// Split and shuffle data
	std::vector<dlib::matrix<float>> training_data;
	std::vector<unsigned long>       training_labels;
	std::vector<dlib::matrix<float>> testing_data;
	std::vector<unsigned long>       testing_labels;
	split_data(data, labels, training_data, training_labels, testing_data, testing_labels);

	std::vector<double> losses;
	std::vector<double> training_accuracies;
	std::vector<double> testing_accuracies;

	// define the network
	using net_type = dlib::loss_multiclass_log<
		dlib::fc< 6,
		dlib::htan<dlib::fc<50,
		dlib::htan<dlib::fc<100,
		dlib::input<dlib::matrix<float>>
		>>>>>>;

	//make a network instance
	net_type net;

	// And then train it using the EEA1 data.
	dlib::dnn_trainer<net_type, dlib::adam> trainer(net, dlib::adam(0.0005, 0.9, 0.999));
	trainer.set_learning_rate(lr);

	// training setting
	int num_epochs = total_epoch;
	int batch_size = batch_sizes;

	// Begin training.
	std::vector<dlib::matrix<float>> batch_data;
	std::vector<unsigned long>		 batch_labels;
	dlib::rand rnd(time(0));
	double batch_loss;

	auto start = std::chrono::high_resolution_clock::now(); // Record start time
	// Calculate total number of mini-batches
	int total_batches = (training_data.size() + batch_size - 1) / batch_size;

	for (int epoch = 0; epoch < num_epochs; ++epoch)
	{
		// Loop over mini-batches
		for (int batch_idx = 0; batch_idx < total_batches; ++batch_idx)
		{
			// clear the previous batch
			batch_data.clear();
			batch_labels.clear();

			// Create a batch
			int start_idx = batch_idx * batch_size;
			batch_loss = 0.0;
			for (int idx = start_idx; idx < std::min(start_idx + batch_size, (int)training_data.size()); ++idx)
			{
				batch_data.push_back(training_data[idx]);
				batch_labels.push_back(training_labels[idx]);
			}

			// train with batch data
			trainer.train_one_step(batch_data, batch_labels);

			// Compute the loss for the batch
			batch_loss = trainer.get_average_loss();
			trainer.clear_average_loss();
			losses.push_back(batch_loss);

			// Record the loss
			std::ostringstream message;
			message << "Epoch " << epoch + 1 << " iteration " << (batch_idx + 1) << "/" << total_batches << " loss: " << batch_loss;
			record_loss(message.str());
		}

		// Calculate and record the accuracy for the training set
		auto training_predictions = net(training_data);
		int correct_predictions = std::inner_product(training_predictions.begin(), training_predictions.end(), training_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double training_accuracy = correct_predictions / static_cast<double>(training_data.size());
		std::ostringstream message;
		message << "Epoch " << epoch + 1 << " training accuracy: " << training_accuracy;
		record_acc(message.str());
		training_accuracies.push_back(training_accuracy);

		// Calculate and record the accuracy for the testing set
		auto testing_predictions = net(testing_data);
		correct_predictions = std::inner_product(testing_predictions.begin(), testing_predictions.end(), testing_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double testing_accuracy = correct_predictions / static_cast<double>(testing_data.size());
		message.str("");
		message << "Epoch " << epoch + 1 << " testing accuracy: " << testing_accuracy;
		record_acc(message.str());
		testing_accuracies.push_back(testing_accuracy);

		SetEvent(hEvent);
	}

	// synchronization for seperate thread
	trainer.get_net();
	auto end = std::chrono::high_resolution_clock::now(); // Record end time

	// Calculate time taken
	std::chrono::duration<double> diff = end - start;

	// Display the training duration
	CString durationText;
	durationText.Format(L"Training took %f seconds", diff.count());
	AfxMessageBox(durationText);

	// clean the network of the state from last batch
	net.clean();

	//dlib::serialize("ANN_Update/eea_network.dat") << net;
	//dlib::deserialize("ANN_Update/meta_network.dat") >> net;

	// Test the trained neural network
	int num_right = 0;
	int num_wrong = 0;
	CString numRightText;
	CString numWrongText;
	CString accuracyText;

	// training set
	std::vector<unsigned long> predicted_labels = net(training_data);
	for (size_t i = 0; i < training_data.size(); i++)
	{
		if (predicted_labels[i] == training_labels[i])
			num_right++;
		else
			num_wrong++;

	}
	numRightText.Format(L"training num_right: %d", num_right);
	numWrongText.Format(L"training num_wrong: %d", num_wrong);
	accuracyText.Format(L"training accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);

	// testing set
	predicted_labels = net(testing_data);
	num_right = 0;
	num_wrong = 0;
	for (size_t i = 0; i < testing_data.size(); ++i)
	{
		if (predicted_labels[i] == testing_labels[i])
			++num_right;
		else
			++num_wrong;

	}
	numRightText.Empty(), numRightText.Format(L"testing num_right: %d", num_right);
	numWrongText.Empty(), numWrongText.Format(L"testing num_wrong: %d", num_wrong);
	accuracyText.Empty(), accuracyText.Format(L"testing accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);
}

// EEA Neural Network 3 layers
void ANN_EEA3(double lr, int total_epoch, int batch_sizes, HANDLE hEvent) {
	// Load the data from file
	std::vector<dlib::matrix<float>> data;
	std::vector<unsigned long> labels;
	int num_features = load_data("datasets/EEA1-Tom20-6Classes-R21.FDA", data, labels);

	// Clear the status txt file
	std::ofstream clear_file_loss("ANN_Update/training_loss.txt", std::ios::trunc);
	clear_file_loss.close();
	std::ofstream clear_file_acc("ANN_Update/training_acc.txt", std::ios::trunc);
	clear_file_acc.close();

	// Standardize the data
	standardize_data(num_features, data);

	// Split and shuffle data
	std::vector<dlib::matrix<float>> training_data;
	std::vector<unsigned long>       training_labels;
	std::vector<dlib::matrix<float>> testing_data;
	std::vector<unsigned long>       testing_labels;
	split_data(data, labels, training_data, training_labels, testing_data, testing_labels);

	std::vector<double> losses;
	std::vector<double> training_accuracies;
	std::vector<double> testing_accuracies;

	// define the network
	using net_type = dlib::loss_multiclass_log<
		dlib::fc< 6,
		dlib::htan< dlib::fc<25,
		dlib::htan<dlib::fc<50,
		dlib::htan<dlib::fc<100,
		dlib::input<dlib::matrix<float>>
		>>>>>>>>;

	//make a network instance
	net_type net;

	// And then train it using the EEA1 data.
	dlib::dnn_trainer<net_type, dlib::adam> trainer(net, dlib::adam(0.0005, 0.9, 0.999));
	trainer.set_learning_rate(lr);

	// training setting
	int num_epochs = total_epoch;
	int batch_size = batch_sizes;

	// Begin training.
	std::vector<dlib::matrix<float>> batch_data;
	std::vector<unsigned long>		 batch_labels;
	dlib::rand rnd(time(0));
	double batch_loss;

	auto start = std::chrono::high_resolution_clock::now(); // Record start time
	// Calculate total number of mini-batches
	int total_batches = (training_data.size() + batch_size - 1) / batch_size;

	for (int epoch = 0; epoch < num_epochs; ++epoch)
	{
		// Loop over mini-batches
		for (int batch_idx = 0; batch_idx < total_batches; ++batch_idx)
		{
			// clear the previous batch
			batch_data.clear();
			batch_labels.clear();

			// Create a batch
			int start_idx = batch_idx * batch_size;
			batch_loss = 0.0;
			for (int idx = start_idx; idx < std::min(start_idx + batch_size, (int)training_data.size()); ++idx)
			{
				batch_data.push_back(training_data[idx]);
				batch_labels.push_back(training_labels[idx]);
			}

			// train with batch data
			trainer.train_one_step(batch_data, batch_labels);

			// Compute the loss for the batch
			batch_loss = trainer.get_average_loss();
			trainer.clear_average_loss();
			losses.push_back(batch_loss);

			// Record the loss
			std::ostringstream message;
			message << "Epoch " << epoch + 1 << " iteration " << (batch_idx + 1) << "/" << total_batches << " loss: " << batch_loss;
			record_loss(message.str());
		}

		// Calculate and record the accuracy for the training set
		auto training_predictions = net(training_data);
		int correct_predictions = std::inner_product(training_predictions.begin(), training_predictions.end(), training_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double training_accuracy = correct_predictions / static_cast<double>(training_data.size());
		std::ostringstream message;
		message << "Epoch " << epoch + 1 << " training accuracy: " << training_accuracy;
		record_acc(message.str());
		training_accuracies.push_back(training_accuracy);

		// Calculate and record the accuracy for the testing set
		auto testing_predictions = net(testing_data);
		correct_predictions = std::inner_product(testing_predictions.begin(), testing_predictions.end(), testing_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double testing_accuracy = correct_predictions / static_cast<double>(testing_data.size());
		message.str("");
		message << "Epoch " << epoch + 1 << " testing accuracy: " << testing_accuracy;
		record_acc(message.str());
		testing_accuracies.push_back(testing_accuracy);

		SetEvent(hEvent);
	}

	// synchronization for seperate thread
	trainer.get_net();
	auto end = std::chrono::high_resolution_clock::now(); // Record end time

	// Calculate time taken
	std::chrono::duration<double> diff = end - start;

	// Display the training duration
	CString durationText;
	durationText.Format(L"Training took %f seconds", diff.count());
	AfxMessageBox(durationText);

	// clean the network of the state from last batch
	net.clean();

	//dlib::serialize("ANN_Update/eea_network.dat") << net;
	//dlib::deserialize("ANN_Update/meta_network.dat") >> net;

	// Test the trained neural network
	int num_right = 0;
	int num_wrong = 0;
	CString numRightText;
	CString numWrongText;
	CString accuracyText;

	// training set
	std::vector<unsigned long> predicted_labels = net(training_data);
	for (size_t i = 0; i < training_data.size(); i++)
	{
		if (predicted_labels[i] == training_labels[i])
			num_right++;
		else
			num_wrong++;

	}
	numRightText.Format(L"training num_right: %d", num_right);
	numWrongText.Format(L"training num_wrong: %d", num_wrong);
	accuracyText.Format(L"training accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);

	// testing set
	predicted_labels = net(testing_data);
	num_right = 0;
	num_wrong = 0;
	for (size_t i = 0; i < testing_data.size(); ++i)
	{
		if (predicted_labels[i] == testing_labels[i])
			++num_right;
		else
			++num_wrong;

	}
	numRightText.Empty(), numRightText.Format(L"testing num_right: %d", num_right);
	numWrongText.Empty(), numWrongText.Format(L"testing num_wrong: %d", num_wrong);
	accuracyText.Empty(), accuracyText.Format(L"testing accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);
}

// EEA Neural Network 4 layers
void ANN_EEA4(double lr, int total_epoch, int batch_sizes, HANDLE hEvent) {
	// Load the data from file
	std::vector<dlib::matrix<float>> data;
	std::vector<unsigned long> labels;
	int num_features = load_data("datasets/EEA1-Tom20-6Classes-R21.FDA", data, labels);

	// Clear the status txt file
	std::ofstream clear_file_loss("ANN_Update/training_loss.txt", std::ios::trunc);
	clear_file_loss.close();
	std::ofstream clear_file_acc("ANN_Update/training_acc.txt", std::ios::trunc);
	clear_file_acc.close();

	// Standardize the data
	standardize_data(num_features, data);

	// Split and shuffle data
	std::vector<dlib::matrix<float>> training_data;
	std::vector<unsigned long>       training_labels;
	std::vector<dlib::matrix<float>> testing_data;
	std::vector<unsigned long>       testing_labels;
	split_data(data, labels, training_data, training_labels, testing_data, testing_labels);

	std::vector<double> losses;
	std::vector<double> training_accuracies;
	std::vector<double> testing_accuracies;

	// define the network
	using net_type = dlib::loss_multiclass_log<
		dlib::fc< 6,
		dlib::htan< dlib::fc<25,
		dlib::htan<dlib::fc<50,
		dlib::htan<dlib::fc<100,
		dlib::htan<dlib::fc<200,
		dlib::input<dlib::matrix<float>>
		>>>>>>>>>>;

	//make a network instance
	net_type net;

	// And then train it using the EEA1 data.
	dlib::dnn_trainer<net_type, dlib::adam> trainer(net, dlib::adam(0.0005, 0.9, 0.999));
	trainer.set_learning_rate(lr);

	// training setting
	int num_epochs = total_epoch;
	int batch_size = batch_sizes;

	// Begin training.
	std::vector<dlib::matrix<float>> batch_data;
	std::vector<unsigned long>		 batch_labels;
	dlib::rand rnd(time(0));
	double batch_loss;

	auto start = std::chrono::high_resolution_clock::now(); // Record start time
	// Calculate total number of mini-batches
	int total_batches = (training_data.size() + batch_size - 1) / batch_size;

	for (int epoch = 0; epoch < num_epochs; ++epoch)
	{
		// Loop over mini-batches
		for (int batch_idx = 0; batch_idx < total_batches; ++batch_idx)
		{
			// clear the previous batch
			batch_data.clear();
			batch_labels.clear();

			// Create a batch
			int start_idx = batch_idx * batch_size;
			batch_loss = 0.0;
			for (int idx = start_idx; idx < std::min(start_idx + batch_size, (int)training_data.size()); ++idx)
			{
				batch_data.push_back(training_data[idx]);
				batch_labels.push_back(training_labels[idx]);
			}

			// train with batch data
			trainer.train_one_step(batch_data, batch_labels);

			// Compute the loss for the batch
			batch_loss = trainer.get_average_loss();
			trainer.clear_average_loss();
			losses.push_back(batch_loss);

			// Record the loss
			std::ostringstream message;
			message << "Epoch " << epoch + 1 << " iteration " << (batch_idx + 1) << "/" << total_batches << " loss: " << batch_loss;
			record_loss(message.str());
		}

		// Calculate and record the accuracy for the training set
		auto training_predictions = net(training_data);
		int correct_predictions = std::inner_product(training_predictions.begin(), training_predictions.end(), training_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double training_accuracy = correct_predictions / static_cast<double>(training_data.size());
		std::ostringstream message;
		message << "Epoch " << epoch + 1 << " training accuracy: " << training_accuracy;
		record_acc(message.str());
		training_accuracies.push_back(training_accuracy);

		// Calculate and record the accuracy for the testing set
		auto testing_predictions = net(testing_data);
		correct_predictions = std::inner_product(testing_predictions.begin(), testing_predictions.end(), testing_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double testing_accuracy = correct_predictions / static_cast<double>(testing_data.size());
		message.str("");
		message << "Epoch " << epoch + 1 << " testing accuracy: " << testing_accuracy;
		record_acc(message.str());
		testing_accuracies.push_back(testing_accuracy);

		SetEvent(hEvent);
	}

	// synchronization for seperate thread
	trainer.get_net();
	auto end = std::chrono::high_resolution_clock::now(); // Record end time

	// Calculate time taken
	std::chrono::duration<double> diff = end - start;

	// Display the training duration
	CString durationText;
	durationText.Format(L"Training took %f seconds", diff.count());
	AfxMessageBox(durationText);

	// clean the network of the state from last batch
	net.clean();

	//dlib::serialize("ANN_Update/eea_network.dat") << net;
	//dlib::deserialize("ANN_Update/meta_network.dat") >> net;

	// Test the trained neural network
	int num_right = 0;
	int num_wrong = 0;
	CString numRightText;
	CString numWrongText;
	CString accuracyText;

	// training set
	std::vector<unsigned long> predicted_labels = net(training_data);
	for (size_t i = 0; i < training_data.size(); i++)
	{
		if (predicted_labels[i] == training_labels[i])
			num_right++;
		else
			num_wrong++;

	}
	numRightText.Format(L"training num_right: %d", num_right);
	numWrongText.Format(L"training num_wrong: %d", num_wrong);
	accuracyText.Format(L"training accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);

	// testing set
	predicted_labels = net(testing_data);
	num_right = 0;
	num_wrong = 0;
	for (size_t i = 0; i < testing_data.size(); ++i)
	{
		if (predicted_labels[i] == testing_labels[i])
			++num_right;
		else
			++num_wrong;

	}
	numRightText.Empty(), numRightText.Format(L"testing num_right: %d", num_right);
	numWrongText.Empty(), numWrongText.Format(L"testing num_wrong: %d", num_wrong);
	accuracyText.Empty(), accuracyText.Format(L"testing accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);
}

// EEA Neural Network 5 hidden layers
void ANN_EEA5(double lr, int total_epoch, int batch_sizes, HANDLE hEvent) {
	// Load the data from file
	std::vector<dlib::matrix<float>> data;
	std::vector<unsigned long> labels;
	int num_features = load_data("datasets/EEA1-Tom20-6Classes-R21.FDA", data, labels);

	// Clear the status txt file
	std::ofstream clear_file_loss("ANN_Update/training_loss.txt", std::ios::trunc);
	clear_file_loss.close();
	std::ofstream clear_file_acc("ANN_Update/training_acc.txt", std::ios::trunc);
	clear_file_acc.close();

	// Standardize the data
	standardize_data(num_features, data);

	// Split and shuffle data
	std::vector<dlib::matrix<float>> training_data;
	std::vector<unsigned long>       training_labels;
	std::vector<dlib::matrix<float>> testing_data;
	std::vector<unsigned long>       testing_labels;
	split_data(data, labels, training_data, training_labels, testing_data, testing_labels);

	std::vector<double> losses;
	std::vector<double> training_accuracies;
	std::vector<double> testing_accuracies;

	// define the network
	using net_type = dlib::loss_multiclass_log<
		dlib::fc< 6,
		dlib::htan< dlib::fc<25,
		dlib::htan<dlib::fc<50,
		dlib::htan<dlib::fc<100,
		dlib::htan<dlib::fc<200,
		dlib::htan<dlib::fc<400,
		dlib::input<dlib::matrix<float>>
		>>>>>>>>>>>>;

	//make a network instance
	net_type net;

	// And then train it using the EEA1 data.
	dlib::dnn_trainer<net_type, dlib::adam> trainer(net, dlib::adam(0.0005, 0.9, 0.999));
	trainer.set_learning_rate(lr);

	// training setting
	int num_epochs = total_epoch;
	int batch_size = batch_sizes;

	// Begin training.
	std::vector<dlib::matrix<float>> batch_data;
	std::vector<unsigned long>		 batch_labels;
	dlib::rand rnd(time(0));
	double batch_loss;

	auto start = std::chrono::high_resolution_clock::now(); // Record start time
	// Calculate total number of mini-batches
	int total_batches = (training_data.size() + batch_size - 1) / batch_size;

	for (int epoch = 0; epoch < num_epochs; ++epoch) {
		// Loop over mini-batches
		for (int batch_idx = 0; batch_idx < total_batches; ++batch_idx) {
			// clear the previous batch
			batch_data.clear();
			batch_labels.clear();

			// Create a batch
			int start_idx = batch_idx * batch_size;
			batch_loss = 0.0;
			for (int idx = start_idx; idx < std::min(start_idx + batch_size, (int)training_data.size()); ++idx) {
				batch_data.push_back(training_data[idx]);
				batch_labels.push_back(training_labels[idx]);
			}

			// train with batch data
			trainer.train_one_step(batch_data, batch_labels);

			// Compute the loss for the batch
			batch_loss = trainer.get_average_loss();
			trainer.clear_average_loss();
			losses.push_back(batch_loss);

			// Record the loss
			std::ostringstream message;
			message << "Epoch " << epoch + 1 << " iteration " << (batch_idx + 1) << "/" << total_batches << " loss: " << batch_loss;
			record_loss(message.str());
		}


		// Calculate and record the accuracy for the training set
		auto training_predictions = net(training_data);
		int correct_predictions = std::inner_product(training_predictions.begin(), training_predictions.end(), training_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double training_accuracy = correct_predictions / static_cast<double>(training_data.size());
		std::ostringstream message;
		message << "Epoch " << epoch + 1 << " training accuracy: " << training_accuracy;
		record_acc(message.str());
		training_accuracies.push_back(training_accuracy);

		// Calculate and record the accuracy for the testing set
		auto testing_predictions = net(testing_data);
		correct_predictions = std::inner_product(testing_predictions.begin(), testing_predictions.end(), testing_labels.begin(), 0,
			std::plus<>(), std::equal_to<>());
		double testing_accuracy = correct_predictions / static_cast<double>(testing_data.size());
		message.str("");
		message << "Epoch " << epoch + 1 << " testing accuracy: " << testing_accuracy;
		record_acc(message.str());
		testing_accuracies.push_back(testing_accuracy);

		SetEvent(hEvent);
	}

	// synchronization for seperate thread
	trainer.get_net();
	auto end = std::chrono::high_resolution_clock::now(); // Record end time

	// Calculate time taken
	std::chrono::duration<double> diff = end - start;

	// Display the training duration
	CString durationText;
	durationText.Format(L"Training took %f seconds", diff.count());
	AfxMessageBox(durationText);

	// clean the network of the state from last batch
	net.clean();

	//dlib::serialize("ANN_Update/eea_network.dat") << net;
	//dlib::deserialize("ANN_Update/meta_network.dat") >> net;

	// Test the trained neural network
	int num_right = 0;
	int num_wrong = 0;
	CString numRightText;
	CString numWrongText;
	CString accuracyText;

	// training set
	std::vector<unsigned long> predicted_labels = net(training_data);
	for (size_t i = 0; i < training_data.size(); i++)
	{
		if (predicted_labels[i] == training_labels[i])
			num_right++;
		else
			num_wrong++;

	}
	numRightText.Format(L"training num_right: %d", num_right);
	numWrongText.Format(L"training num_wrong: %d", num_wrong);
	accuracyText.Format(L"training accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);

	// testing set
	predicted_labels = net(testing_data);
	num_right = 0;
	num_wrong = 0;
	for (size_t i = 0; i < testing_data.size(); ++i)
	{
		if (predicted_labels[i] == testing_labels[i])
			++num_right;
		else
			++num_wrong;

	}
	numRightText.Empty(), numRightText.Format(L"testing num_right: %d", num_right);
	numWrongText.Empty(), numWrongText.Format(L"testing num_wrong: %d", num_wrong);
	accuracyText.Empty(), accuracyText.Format(L"testing accuracy: %f", num_right / static_cast<double>(num_right + num_wrong));
	AfxMessageBox(numRightText);
	AfxMessageBox(numWrongText);
	AfxMessageBox(accuracyText);
}

// EEA1 worker thread to run in back ground
UINT ANN_EEA1_ThreadProc(LPVOID pParam) {
	// Convert the LPVOID parameter back to the data structure
	ANNStruct* pData = (ANNStruct*)pParam;

	// Call ANN_EEA with parameters from the structure
	ANN_EEA1(pData->lr, pData->total_epoch, pData->batch_sizes, pData->hEvent);

	AfxMessageBox(L"Dataset: EEA Topology: 100");

	return 0;
}

// EEA2 worker thread to run in back ground
UINT ANN_EEA2_ThreadProc(LPVOID pParam) {
	// Convert the LPVOID parameter back to the data structure
	ANNStruct* pData = (ANNStruct*)pParam;

	// Call ANN_EEA with parameters from the structure
	ANN_EEA2(pData->lr, pData->total_epoch, pData->batch_sizes, pData->hEvent);

	AfxMessageBox(L"Dataset: EEA Topology: 100, 50");

	return 0;
}

// EEA3 worker thread to run in back ground
UINT ANN_EEA3_ThreadProc(LPVOID pParam) {
	// Convert the LPVOID parameter back to the data structure
	ANNStruct* pData = (ANNStruct*)pParam;

	// Call ANN_EEA with parameters from the structure
	ANN_EEA3(pData->lr, pData->total_epoch, pData->batch_sizes, pData->hEvent);

	AfxMessageBox(L"Dataset: EEA Topology: 100, 50, 25");

	return 0;
}

// EEA4 worker thread to run in back ground
UINT ANN_EEA4_ThreadProc(LPVOID pParam) {
	// Convert the LPVOID parameter back to the data structure
	ANNStruct* pData = (ANNStruct*)pParam;

	// Call ANN_EEA with parameters from the structure
	ANN_EEA4(pData->lr, pData->total_epoch, pData->batch_sizes, pData->hEvent);

	AfxMessageBox(L"Dataset: EEA Topology: 200, 100, 50, 25");

	return 0;
}

// EEA5 worker thread to run in background
UINT ANN_EEA5_ThreadProc(LPVOID pParam) {
	// Convert the LPVOID parameter back to the data structure
	ANNStruct* pData = (ANNStruct*)pParam;

	// Call ANN_EEA with parameters from the structure
	ANN_EEA5(pData->lr, pData->total_epoch, pData->batch_sizes, pData->hEvent);

	AfxMessageBox(L"Dataset: EEA Topology: 400, 200, 100, 50, 25");

	return 0;
}

// Wait for processing message
void ProcessPendingMessages() {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


void CModelingandAnalysisofUncertaintyDoc::OnANN() {
	CWnd* pParent = nullptr;
	CANNForm Selection(pParent);
	OnOpenedFile = false;
	DescriptiveStatistics = false;
	HypothesisTesting_OneSample = false;
	HypothesisTesting_TwoSample = false;
	ShapiroWilkTest = false;
	AndersonDarlingTest = false;
	ANOVA = false;
	PCA_Select_PCs = false;
	PCA_Display_PCs_Standard = false;
	PCA_Display_Scores = false;
	PCA_Display_Loadings = false;
	FA_Display_Standard = false;
	FA_Display_Loadings = false;
	FA_Display_Scores = false;
	FA_Display_Matrices = false;
	FDA = false;
	ANN_Training = true;

	// Get training setting from user
	Selection.DoModal();
	double lr = Selection.learning_rate;
	int epoch_num = Selection.total_epoch;
	int batch_num = Selection.batch_size;

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// Passing in training settings
	ANNStruct* pData = new ANNStruct;
	pData->lr = lr;
	pData->total_epoch = epoch_num;
	pData->batch_sizes = batch_num;
	pData->hEvent = hEvent;

	if (Selection.m_selectedTask == 1) {
		AfxBeginThread(ANN_EEA1_ThreadProc, pData);
	}
	else if (Selection.m_selectedTask == 2) {
		AfxBeginThread(ANN_EEA2_ThreadProc, pData);
	}
	else if (Selection.m_selectedTask == 3) {
		AfxBeginThread(ANN_EEA3_ThreadProc, pData);
	}
	else if (Selection.m_selectedTask == 4) {
		AfxBeginThread(ANN_EEA4_ThreadProc, pData);
	}
	else if (Selection.m_selectedTask == 5) {
		AfxBeginThread(ANN_EEA5_ThreadProc, pData);
	}

	for (int i = 0; i < epoch_num; ++i) {
		DWORD dwResult = WaitForSingleObject(hEvent, INFINITE);
		if (dwResult == WAIT_OBJECT_0) {
			ProcessPendingMessages();
			// The event is signaled, so update the views
			UpdateAllViews(NULL);
			ResetEvent(hEvent);  // Reset the event for the next epoch
		}
	}
	CloseHandle(hEvent);
}

// Function to calculate the sum of squared errors
double CModelingandAnalysisofUncertaintyDoc::sum_squared_error(const std::vector<std::vector<double>>& Y1, const std::vector<std::vector<double>>& Y2) {
	double error = 0.0;
	int numRows = Y1.size();
	int numCols = Y1[0].size();

	for (int i = 0; i < numRows; ++i) {
		for (int j = 0; j < numCols; ++j) {
			error += pow(Y1[i][j] - Y2[i][j], 2);
		}
	}
	return error / (numRows * numCols);
}

// Function to calculate the sum of squared errors in parallel
double CModelingandAnalysisofUncertaintyDoc::sum_squared_error_parallel(const std::vector<std::vector<double>>& Y1, const std::vector<std::vector<double>>& Y2) {
	double error = 0.0;
	int numRows = Y1.size();
	int numCols = Y1[0].size();

	#pragma omp parallel for reduction(+:error)
	for (int i = 0; i < numRows; ++i) {
		for (int j = 0; j < numCols; ++j) {
			error += pow(Y1[i][j] - Y2[i][j], 2);
		}
	}
	return error / (numRows * numCols);
}

// Function to calculate z-score normalization
std::vector<std::vector<double>> CModelingandAnalysisofUncertaintyDoc::zscore(const std::vector<std::vector<double>>& data) {
	std::vector<std::vector<double>> normalized;
	int numRows = data.size();
	int numCols = data[0].size();

	for (int j = 0; j < numCols; ++j) {
		double mean = 0.0;
		double stdDev = 0.0;
		for (int i = 0; i < numRows; ++i) {
			mean += data[i][j];
		}
		mean /= numRows;
		for (int i = 0; i < numRows; ++i) {
			stdDev += pow(data[i][j] - mean, 2);
		}
		stdDev = sqrt(stdDev / (numRows - 1));

		std::vector<double> column;
		for (int i = 0; i < numRows; ++i) {
			double normalizedValue = (data[i][j] - mean) / stdDev;
			column.push_back(normalizedValue);
		}
		normalized.push_back(column);
	}

	return normalized;
}

// Function to calculate z-score normalization in parallel
std::vector<std::vector<double>> CModelingandAnalysisofUncertaintyDoc::zscoreParallel(const std::vector<std::vector<double>>& data) {
	std::vector<std::vector<double>> normalized;
	int numRows = data.size();
	int numCols = data[0].size();

	double mean = 0.0;
	double stdDev = 0.0;
	int i = 0;
	std::vector<double> column;
	double normalizedValue;
	#pragma omp parallel for private(mean, stdDev, i, column, normalizedValue)
	for (int j = 0; j < numCols; ++j) {
		for (i = 0; i < numRows; ++i) {
			mean += data[i][j];
		}
		mean /= numRows;
		for (i = 0; i < numRows; ++i) {
			stdDev += pow(data[i][j] - mean, 2);
		}
		stdDev = sqrt(stdDev / (numRows - 1));
		
		for (i = 0; i < numRows; ++i) {
			normalizedValue = (data[i][j] - mean) / stdDev;
			column.push_back(normalizedValue);
		}
		#pragma omp critical
		normalized.push_back(column);
	}

	return normalized;
}

// Function to get a random sample from a vector
std::vector<int> CModelingandAnalysisofUncertaintyDoc::randsample(int n, int k) {
	std::vector<int> indices(n);
	for (int i = 0; i < n; ++i) {
		indices[i] = i;
	}
	std::random_shuffle(indices.begin(), indices.end());
	indices.resize(k);
	return indices;
}

void CModelingandAnalysisofUncertaintyDoc::VecTranspose(std::vector<std::vector<double> >& b)
{
	if (b.size() == 0)
		return;

	std::vector<std::vector<double> > trans_vec(b[0].size(), std::vector<double>());

	for (int i = 0; i < b.size(); i++)
	{
		for (int j = 0; j < b[i].size(); j++)
		{
			trans_vec[j].push_back(b[i][j]);
		}
	}

	b = trans_vec;    // <--- reassign here
}
void CModelingandAnalysisofUncertaintyDoc::VecTransposeInt(std::vector<std::vector<int> >& b)
{
	if (b.size() == 0)
		return;

	std::vector<std::vector<int> > trans_vec(b[0].size(), std::vector<int>());

	for (int i = 0; i < b.size(); i++)
	{
		for (int j = 0; j < b[i].size(); j++)
		{
			trans_vec[j].push_back(b[i][j]);
		}
	}

	b = trans_vec;    // <--- reassign here
}

void CModelingandAnalysisofUncertaintyDoc::OnANN_MFC() {
	// Define constants
	const int N = n_Obs;
	const int M = n_Var - 1;
	const int C = n_classes;
	const int H = 5;
	const int n_epochs = 1000;
	const int train = 20;
	const double eta = 1e-1 / train;  
	int batch_size = 5;
	// Initialize random number generator seed
	std::srand(1);

	// Generate random data for three classes
	//std::vector<std::vector<double>> Xclass1(N, std::vector<double>(M));
	//std::vector<std::vector<double>> Xclass2(N, std::vector<double>(M));
	//std::vector<std::vector<double>> Xclass3(N, std::vector<double>(M));

	//for (int i = 0; i < N; ++i) {
	//	for (int j = 0; j < M; ++j) {
	//		Xclass1[i][j] = static_cast<double>(std::rand()) / RAND_MAX + 1.0;
	//		Xclass2[i][j] = static_cast<double>(std::rand()) / RAND_MAX;
	//		Xclass3[i][j] = static_cast<double>(std::rand()) / RAND_MAX - 1.0;
	//	}
	//}
	//// Combine data for all classes
	//std::vector<std::vector<double>> X = Xclass1;
	//X.insert(X.end(), Xclass2.begin(), Xclass2.end());
	//X.insert(X.end(), Xclass3.begin(), Xclass3.end());
	////Normalize data
	//X = zscore(X);

	//VecTranspose(X);

	//// Generate ytrue as described in MATLAB
	//std::vector<int> ytrue;
	//for (int c = 0; c < C; ++c) {
	//	for (int i = 0; i < N; ++i) {
	//		ytrue.push_back(c + 1);
	//	}
	//}

	//// Generate y1, y2, and y3
	//std::vector<double> y1(N, 1.0);
	//std::vector<double> y2(N, 0.0);
	//std::vector<double> y3(N, 0.0);


	//for (int i = 0; i < 2000; i++) {
	//	y1.push_back(0.0);
	//	if (i < 1000) {
	//		y2.push_back(1);
	//		y3.push_back(0);
	//		continue;
	//	}
	//	y2.push_back(0);
	//	y3.push_back(1);
	//}
	//// Concatenate y1, y2, and y3 to create Y
	//std::vector<std::vector<double>> Y;
	//Y.push_back(y1);
	//Y.push_back(y2);
	//Y.push_back(y3);

	CArray<double> Data0, bar, std;
	Data0.RemoveAll();
	bar.RemoveAll();
	std.RemoveAll();
	StandardizeDataMatrix(Data0, bar, std);

	//std::ofstream FILE_CHECK;
	//FILE_CHECK.open("check_file.txt");

	std::vector<std::vector<double>> X(n_Obs, std::vector<double>(n_Var-1));
	std::vector<double> Input_Y(n_Obs);

	for (int i = 0; i < n_Obs * (n_Var-1); i++) {
		int row = i % n_Obs;
		int col = i / n_Obs;
		X[row][col] = Data0[i];
	}
	for (int i = 0; i < n_Obs; i++) {
		Input_Y[i] = Data[n_Obs * (n_Var - 1) + i];
	}

	std::vector<std::vector<int>> Y(n_Obs, std::vector<int>(C));
	for (int i = 0; i < n_Obs; i++) {
		Y[i][(Input_Y[i] - 1)] = 1;
	}
	//FILE_CHECK << "n_Obs: " << n_Obs << "\nn_Var: " << n_Var << "\nn_Classes: " << n_classes << "\n";
	//FILE_CHECK.close();


	//VecTranspose(Y);

	// Initialize weight (w) and bias (b) matrices for the neural network
	//n_weights = 20; n_biases = 6;
	int n_weights = H * (M + 1);
	int n_biases = H + 1;

	const double trainFraction = 0.85;

	// Calculate Ntrain as described in MATLAB
	int Ntrain = static_cast<int>(round(N * trainFraction));
	// Vector of 0-3000 shuffled

	int Ntest = N - Ntrain;
	std::vector<std::vector<double>> Xtrain(Ntrain, std::vector<double>(M));
	std::vector<std::vector<int>> Ytrain(Ntrain, std::vector<int>(C));
	std::vector<std::vector<double>> Xtest(Ntest, std::vector<double>(M));
	std::vector<std::vector<int>> Ytest(Ntest, std::vector<int>(C));
	std::vector<int> index = randsample(N, N);

	for (int i = 0; i < Ntrain; i++) {
		Xtrain[i] = X[index[i]];
		for (int c = 0; c < C; c++) {
			Ytrain[i][c] = Y[index[i]][c];
		}
	}
	for (int i = Ntrain; i < N; i++) {
		Xtest[i - Ntrain] = X[index[i]];
		for (int c = 0; c < C; c++) {
			Ytest[i - Ntrain][c] = Y[index[i]][c];
		}
	}
	std::ofstream FILE_CHECK;
	FILE_CHECK.open("check_file.txt");

	for (int i = 0; i < Ytest.size(); i++) {
		for (int j = 0; j < Ytest[i].size(); j++) {
			FILE_CHECK << Ytest[i][j] << ", ";
		}
		FILE_CHECK << "\n";
	}
	FILE_CHECK.close();
	// 1x60, 1x18
	std::vector<double> w(C * n_weights);
	std::vector<double> b(C * n_biases);

	// Looks like matlab code ?? 
	for (int i = 0; i < C * n_weights; ++i) {
		w[i] = 0.2 * static_cast<double>(std::rand()) / RAND_MAX - 0.1;
	}

	for (int i = 0; i < C * n_biases; ++i) {
		b[i] = 0.2 * static_cast<double>(std::rand()) / RAND_MAX - 0.1;
	}

	// Calculates spe_old
	double spe_old = 0.0;
	for (int c = 0; c < C; ++c) {
		for (int i = 0; i < Ntest; ++i) {
			spe_old += std::pow(Ytest[i][c], 2);
		}
	}
	// Might just be 1/3
	spe_old /= N;

	// Initialize MIN and other variables
	double MIN = 1e7;
	//double eta = 1e-1 / train;

	// 450x3 matrix of 0's
	std::vector<std::vector<double>> yhat0(Ntest, std::vector<double>(C, 0.0));
	//Need to fix
	/*
	// Initialize some variables for tracking training progress
	*/

	// Create variables for Yhat0 and delta0
	// These are both 450x3 uninitialized matrices
	std::vector<std::vector<double>> Yhat0(Ntest, std::vector<double>(C));
	std::vector<std::vector<double>> delta0(Ntest, std::vector<double>(C));

	std::ofstream FILE;
	FILE.open("outfile.txt");
	int slice_index = 0;
	// Iterate through 200000 times
	for (int epoch = 1; epoch <= n_epochs; ++epoch) {
		double spe_new = 0.0;
		// Iterate through three times
		for (int c = 0; c < C; ++c) {
			//Draw 20 random numbers from 0-2550
			std::vector<int> index = randsample(Ntrain, train);
			slice_index += train;
			slice_index %= Ntrain;
			// Random numbers from w and b somehow
			std::vector<double> weights(w.begin() + c * n_weights, w.begin() + (c + 1) * n_weights);
			std::vector<double> biases(b.begin() + c * n_biases, b.begin() + (c + 1) * n_biases);

			// Get network predictions for the training data
			//CString version;
			//version.Append(L"classification");
			//What is F and yhat
			// F = 1000x5
			std::vector<std::vector<double>> F = std::vector<std::vector<double>>(train, std::vector<double>(H, 0.0));
			std::vector<double> yhat = std::vector<double>(train, 0.0);

			std::vector<std::vector<double> > Xslice;
			/*for (int i = 0; i < train; i++)
				Xslice.push_back(Xtrain[index[i]]);*/
			for (int i = slice_index; i < slice_index + train; i++) {
				if (i >= Ntrain) {
					Xslice.push_back(Xtrain[i % Ntrain]);
					continue;
				}
				Xslice.push_back(Xtrain[i]);
			}

			GetNetworkPrediction(Xslice, H, weights, biases, F, yhat);
			/*for (int i = 0; i < yhat.size(); i++) {
				FILE << yhat[i] << ", ";
			}
			FILE << "\n";*/
			// Compute the error (d) for the current class
			//If we replaced Ytrain with Y slice we would be chilling
			std::vector<double> d(train, 0.0);
			/*for (int i = 0; i < train; ++i) {
				d[i] = Ytrain[index[i]][c] - yhat[i];
			}*/
			int d_index = 0;
			for (int i = slice_index; i < slice_index + train; i++) {
				if (i >= Ntrain) {
					d[d_index] = Ytrain[i % Ntrain][c] - yhat[d_index];
					continue;
				}
				d[d_index] = Ytrain[i][c] - yhat[d_index];
				d_index++;
			}


			// Update weights and biases
			int pos = c * n_weights + M * H;
			std::vector<double> dphi_t(train, 0.0);
			for (int i = 0; i < train; ++i) {
				dphi_t[i] = yhat[i] * (1.0 - yhat[i]);
			}

			// Update weights connecting the hidden to the output layer
			for (int h = 0; h < H; ++h) {
				double sum = 0.0;
				for (int i = 0; i < d.size(); i++) {
					sum += dphi_t[i] * F[i][h] * d[i] * eta;
				}
				w[pos + h] += sum;
			}

			// Update bias term connecting hidden to output layer
			int posB = c * H + (H + 1);
			double sumB = 0.0;
			for (int i = 0; i < train; i++)
				sumB += dphi_t[i] * d[i] * eta;
			b[posB] += sumB;

			// Update weights connecting the input to the hidden layer
			for (int h = 0; h < H; ++h) {
				std::vector<double> dphi_fh(train, 0.0);
				for (int i = 0; i < train; ++i) {
					dphi_fh[i] = F[i][h] * (1.0 - F[i][h]);
				}

				for (int j = 0; j < M; ++j) {
					double sumDphi = 0;
					int posW = c * n_weights + j * H + h;
					for (int i = 0; i < train; ++i) {
						sumDphi += d[i] * (dphi_t[i] * dphi_fh[i] * Xslice[i][j]);
					}
					w[posW] += eta * w[c * n_weights + M * H + h] * sumDphi;
				}

				int posB = c * n_biases + h;
				double sumB = 0;
				for (int i = 0; i < train; i++) {
					sumB += d[i] * (dphi_t[i] * dphi_fh[i]);
				}
				b[posB] += eta * w[c * n_weights + M * H + h] * sumB;
			}

			// Update yhat0 for the current class (TODO: Implement GetNetworkPrediction)
			std::vector<double> yhat0(Ntest, 0.0);
			std::vector<std::vector<double> > Ftemp = std::vector<std::vector<double>>(Ntest, std::vector<double>(H, 0.0));
			GetNetworkPrediction(Xtest, H, weights, biases, Ftemp, yhat0);
			

			/*for (int i = 0; i < yhat0.size(); i++) {
				FILE << yhat0[i] << ", ";
			}
			FILE << "\n";*/

			// Compute delta0 for the current class
			std::vector<double> delta0(Ntest, 0.0);
			double sumDelta = 0;
			for (int i = 0; i < Ntest; ++i) {
				delta0[i] = Ytest[i][c] - yhat0[i];
				sumDelta += delta0[i] * delta0[i];
			}
			// Update spe_new for the current class
			spe_new += sumDelta / Ntest;
			if (spe_new < MIN) {
				for (int i = 0; i < yhat0.size(); i++) {
					Yhat0[i][c] = yhat0[i];
				}
			}
		}

		// Check if spe_new is smaller than MIN and update wopt, bopt, and Yhat0 accordingly
		if (spe_new < MIN) {
			std::vector<double> wopt;
			wopt.empty();
			wopt = w;
			std::vector<double> bopt;
			bopt.empty();
			bopt = b;
			MIN = spe_new;
			//Yhat0 = yhat0;
		}
		// Print the current epoch and spe_new
		if (epoch % batch_size == 0) {
			FILE << epoch << "\t" << spe_new << "\n";
		}
	}
	FILE << "Yhat0 (Prediction):\n";
	for (int i = 0; i < Yhat0.size(); i++) {
		for (int j = 0; j < Yhat0[i].size(); j++) {
			FILE << Yhat0[i][j] << ", ";
		}
		FILE << "\n";
	}

	FILE << "YTest:\n";
	for (int i = 0; i < Ytest.size(); i++) {
		for (int j = 0; j < Ytest[i].size(); j++) {
			FILE << Ytest[i][j] << ", ";
		}
		FILE << "\n";
	}
	FILE.close();
	
}

void CModelingandAnalysisofUncertaintyDoc::OnANN_MFC_Parallel() {
	// Define constants
	const int N = n_Obs;
	const int M = n_Var - 1;
	const int C = n_classes;
	const int H = 5;
	const int n_epochs = 1000;
	const int train = 20;
	const double eta = 1e-1 / train;
	int batch_size = 5;
	// Initialize random number generator seed
	std::srand(1);

	CArray<double> Data0, bar, std;
	Data0.RemoveAll();
	bar.RemoveAll();
	std.RemoveAll();
	StandardizeDataMatrix(Data0, bar, std);

	//std::ofstream FILE_CHECK;
	//FILE_CHECK.open("check_file.txt");

	std::vector<std::vector<double>> X(n_Obs, std::vector<double>(n_Var - 1));
	std::vector<double> Input_Y(n_Obs);

	for (int i = 0; i < n_Obs * (n_Var - 1); i++) {
		int row = i % n_Obs;
		int col = i / n_Obs;
		X[row][col] = Data0[i];
	}
	for (int i = 0; i < n_Obs; i++) {
		Input_Y[i] = Data[n_Obs * (n_Var - 1) + i];
	}

	std::vector<std::vector<int>> Y(n_Obs, std::vector<int>(C));
	for (int i = 0; i < n_Obs; i++) {
		Y[i][(Input_Y[i] - 1)] = 1;
	}
	//FILE_CHECK << "n_Obs: " << n_Obs << "\nn_Var: " << n_Var << "\nn_Classes: " << n_classes << "\n";
	//FILE_CHECK.close();


	//VecTranspose(Y);

	// Initialize weight (w) and bias (b) matrices for the neural network
	//n_weights = 20; n_biases = 6;
	int n_weights = H * (M + 1);
	int n_biases = H + 1;

	const double trainFraction = 0.85;

	// Calculate Ntrain as described in MATLAB
	int Ntrain = static_cast<int>(round(N * trainFraction));
	// Vector of 0-3000 shuffled

	int Ntest = N - Ntrain;
	std::vector<std::vector<double>> Xtrain(Ntrain, std::vector<double>(M));
	std::vector<std::vector<int>> Ytrain(Ntrain, std::vector<int>(C));
	std::vector<std::vector<double>> Xtest(Ntest, std::vector<double>(M));
	std::vector<std::vector<int>> Ytest(Ntest, std::vector<int>(C));
	std::vector<int> index = randsample(N, N);

	for (int i = 0; i < Ntrain; i++) {
		Xtrain[i] = X[index[i]];
		for (int c = 0; c < C; c++) {
			Ytrain[i][c] = Y[index[i]][c];
		}
	}
	for (int i = Ntrain; i < N; i++) {
		Xtest[i - Ntrain] = X[index[i]];
		for (int c = 0; c < C; c++) {
			Ytest[i - Ntrain][c] = Y[index[i]][c];
		}
	}
	std::ofstream FILE_CHECK;
	FILE_CHECK.open("check_file.txt");

	for (int i = 0; i < Ytest.size(); i++) {
		for (int j = 0; j < Ytest[i].size(); j++) {
			FILE_CHECK << Ytest[i][j] << ", ";
		}
		FILE_CHECK << "\n";
	}
	FILE_CHECK.close();
	// 1x60, 1x18
	std::vector<double> w(C * n_weights);
	std::vector<double> b(C * n_biases);

	// Looks like matlab code ?? 
	for (int i = 0; i < C * n_weights; ++i) {
		w[i] = 0.2 * static_cast<double>(std::rand()) / RAND_MAX - 0.1;
	}

	for (int i = 0; i < C * n_biases; ++i) {
		b[i] = 0.2 * static_cast<double>(std::rand()) / RAND_MAX - 0.1;
	}

	// Calculates spe_old
	double spe_old = 0.0;
	for (int c = 0; c < C; ++c) {
		for (int i = 0; i < Ntest; ++i) {
			spe_old += std::pow(Ytest[i][c], 2);
		}
	}
	// Might just be 1/3
	spe_old /= N;

	// Initialize MIN and other variables
	double MIN = 1e7;
	//double eta = 1e-1 / train;

	// 450x3 matrix of 0's
	std::vector<std::vector<double>> yhat0(Ntest, std::vector<double>(C, 0.0));
	//Need to fix
	/*
	// Initialize some variables for tracking training progress
	*/

	// Create variables for Yhat0 and delta0
	// These are both 450x3 uninitialized matrices
	std::vector<std::vector<double>> Yhat0(Ntest, std::vector<double>(C));
	std::vector<std::vector<double>> delta0(Ntest, std::vector<double>(C));

	std::ofstream FILE;
	FILE.open("outfile.txt");
	int slice_index = 0;
	// Iterate through 200000 times
	for (int epoch = 1; epoch <= n_epochs; ++epoch) {
		double spe_new = 0.0;
		// Iterate through three times
		for (int c = 0; c < C; ++c) {
			//Draw 20 random numbers from 0-2550
			std::vector<int> index = randsample(Ntrain, train);
			slice_index += train;
			slice_index %= Ntrain;
			// Random numbers from w and b somehow
			std::vector<double> weights(w.begin() + c * n_weights, w.begin() + (c + 1) * n_weights);
			std::vector<double> biases(b.begin() + c * n_biases, b.begin() + (c + 1) * n_biases);

			// Get network predictions for the training data
			//CString version;
			//version.Append(L"classification");
			//What is F and yhat
			// F = 1000x5
			std::vector<std::vector<double>> F = std::vector<std::vector<double>>(train, std::vector<double>(H, 0.0));
			std::vector<double> yhat = std::vector<double>(train, 0.0);

			std::vector<std::vector<double> > Xslice;
			/*for (int i = 0; i < train; i++)
				Xslice.push_back(Xtrain[index[i]]);*/
			for (int i = slice_index; i < slice_index + train; i++) {
				if (i >= Ntrain) {
					Xslice.push_back(Xtrain[i % Ntrain]);
					continue;
				}
				Xslice.push_back(Xtrain[i]);
			}

			GetNetworkPredictionParallel(Xslice, H, weights, biases, F, yhat);
			/*for (int i = 0; i < yhat.size(); i++) {
				FILE << yhat[i] << ", ";
			}
			FILE << "\n";*/
			// Compute the error (d) for the current class
			//If we replaced Ytrain with Y slice we would be chilling
			std::vector<double> d(train, 0.0);
			/*for (int i = 0; i < train; ++i) {
				d[i] = Ytrain[index[i]][c] - yhat[i];
			}*/
			int d_index = 0;
			for (int i = slice_index; i < slice_index + train; i++) {
				if (i >= Ntrain) {
					d[d_index] = Ytrain[i % Ntrain][c] - yhat[d_index];
					continue;
				}
				d[d_index] = Ytrain[i][c] - yhat[d_index];
				d_index++;
			}


			// Update weights and biases
			int pos = c * n_weights + M * H;
			std::vector<double> dphi_t(train, 0.0);
			for (int i = 0; i < train; ++i) {
				dphi_t[i] = yhat[i] * (1.0 - yhat[i]);
			}

			// Update weights connecting the hidden to the output layer
			for (int h = 0; h < H; ++h) {
				double sum = 0.0;
				for (int i = 0; i < d.size(); i++) {
					sum += dphi_t[i] * F[i][h] * d[i] * eta;
				}
				w[pos + h] += sum;
			}

			// Update bias term connecting hidden to output layer
			int posB = c * H + (H + 1);
			double sumB = 0.0;
			for (int i = 0; i < train; i++)
				sumB += dphi_t[i] * d[i] * eta;
			b[posB] += sumB;

			// Update weights connecting the input to the hidden layer
			for (int h = 0; h < H; ++h) {
				std::vector<double> dphi_fh(train, 0.0);
				for (int i = 0; i < train; ++i) {
					dphi_fh[i] = F[i][h] * (1.0 - F[i][h]);
				}

				for (int j = 0; j < M; ++j) {
					double sumDphi = 0;
					int posW = c * n_weights + j * H + h;
					for (int i = 0; i < train; ++i) {
						sumDphi += d[i] * (dphi_t[i] * dphi_fh[i] * Xslice[i][j]);
					}
					w[posW] += eta * w[c * n_weights + M * H + h] * sumDphi;
				}

				int posB = c * n_biases + h;
				double sumB = 0;
				for (int i = 0; i < train; i++) {
					sumB += d[i] * (dphi_t[i] * dphi_fh[i]);
				}
				b[posB] += eta * w[c * n_weights + M * H + h] * sumB;
			}

			// Update yhat0 for the current class (TODO: Implement GetNetworkPrediction)
			std::vector<double> yhat0(Ntest, 0.0);
			std::vector<std::vector<double> > Ftemp = std::vector<std::vector<double>>(Ntest, std::vector<double>(H, 0.0));
			GetNetworkPredictionParallel(Xtest, H, weights, biases, Ftemp, yhat0);


			/*for (int i = 0; i < yhat0.size(); i++) {
				FILE << yhat0[i] << ", ";
			}
			FILE << "\n";*/

			// Compute delta0 for the current class
			std::vector<double> delta0(Ntest, 0.0);
			double sumDelta = 0;
			for (int i = 0; i < Ntest; ++i) {
				delta0[i] = Ytest[i][c] - yhat0[i];
				sumDelta += delta0[i] * delta0[i];
			}
			// Update spe_new for the current class
			spe_new += sumDelta / Ntest;
			if (spe_new < MIN) {
				for (int i = 0; i < yhat0.size(); i++) {
					Yhat0[i][c] = yhat0[i];
				}
			}
		}

		// Check if spe_new is smaller than MIN and update wopt, bopt, and Yhat0 accordingly
		if (spe_new < MIN) {
			std::vector<double> wopt;
			wopt.empty();
			wopt = w;
			std::vector<double> bopt;
			bopt.empty();
			bopt = b;
			MIN = spe_new;
			//Yhat0 = yhat0;
		}
		// Print the current epoch and spe_new
		if (epoch % batch_size == 0) {
			FILE << epoch << "\t" << spe_new << "\n";
		}
	}
	FILE << "Yhat0 (Prediction):\n";
	for (int i = 0; i < Yhat0.size(); i++) {
		for (int j = 0; j < Yhat0[i].size(); j++) {
			FILE << Yhat0[i][j] << ", ";
		}
		FILE << "\n";
	}

	FILE << "YTest:\n";
	for (int i = 0; i < Ytest.size(); i++) {
		for (int j = 0; j < Ytest[i].size(); j++) {
			FILE << Ytest[i][j] << ", ";
		}
		FILE << "\n";
	}
	FILE.close();

}

//GetNetworkPrediction(Xtrain, H, weights, biases, F, yhat);
void CModelingandAnalysisofUncertaintyDoc::GetNetworkPrediction(const std::vector<std::vector<double>>& X, const int H,
	const std::vector<double>& w, const std::vector<double>& b,
	std::vector<std::vector<double>>& F, std::vector<double>& yhat) {

	// 20, 
	int N = X.size();
	int M = X[0].size();


	for (int h = 0; h < H; ++h) {
		std::vector<double> z(N, 0.0);
		for (int j = 0; j < M; ++j) {
			int pos = j * H + h;
			for (int i = 0; i < N; ++i) {
				z[i] += w[pos] * X[i][j];
			}
		}

		for (int i = 0; i < N; ++i) {
			z[i] += b[h];
			F[i][h] = 1.0 / (1.0 + exp(-z[i]));
		}

		int pos = M * H + h;
		for (int i = 0; i < N; ++i) {
			yhat[i] += w[pos] * F[i][h];
		}
	}
	for (int i = 0; i < N; ++i) {
		yhat[i] = 1.0 / (1.0 + exp(-yhat[i]));
	}
}

////GetNetworkPrediction(Xtrain, H, weights, biases, F, yhat) in parallel
void CModelingandAnalysisofUncertaintyDoc::GetNetworkPredictionParallel(const std::vector<std::vector<double>>& X, const int H,
	const std::vector<double>& w, const std::vector<double>& b,
	std::vector<std::vector<double>>& F, std::vector<double>& yhat) {

	// 20, 
	int N = X.size();
	int M = X[0].size();
	int pos;
	int i;
	int j;
	std::vector<double> z(N, 0.0);
	#pragma omp parallel for private(pos, z, i, j)
	for (int h = 0; h < H; ++h) {
		for (j = 0; j < M; ++j) {
			pos = j * H + h;
			for (i = 0; i < N; ++i) {
				z[i] += w[pos] * X[i][j];
			}
		}

		for (i = 0; i < N; ++i) {
			z[i] += b[h];
			F[i][h] = 1.0 / (1.0 + exp(-z[i]));
		}

		pos = M * H + h;
		for (i = 0; i < N; ++i) {
			#pragma omp atomic
			yhat[i] += w[pos] * F[i][h];
		}
	}
	#pragma omp parallel for
	for (int i = 0; i < N; ++i) {
		yhat[i] = 1.0 / (1.0 + exp(-yhat[i]));
	}
}

// Enablers for modeling methods after datafile was read
void CModelingandAnalysisofUncertaintyDoc::OnUpdateDescriptiveStatistics(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateOnesample(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateTwosample(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateSWTest(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateADTest(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateANOVA(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdatePCA(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateFA(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateICA(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateCCA(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateFDA(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateLinearClassification(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateMultivariateRegression(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdatePLS(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateKPCA(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateLR(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateSVM(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateKSVM(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateKFDA(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateL1_Regularization(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateL2_Regularization(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateKPLS(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}

void CModelingandAnalysisofUncertaintyDoc::OnUpdateANN(CCmdUI* pCmdUI) {
	pCmdUI->Enable(FileOpen);
}
