#include <cmath>
#include <cstddef>
#include <iostream>
#include <fstream>
#ifndef PROBABILITYDISTRIBUTIONS_H_
#define PROBABILITYDISTRIBUTIONS_H_

#pragma once

class ProbabilityDistributions {
private:
	double* Parameters = NULL;
	char Type = 'n';
	bool Error = false;
	CString ErrorInfo;
	double BETACF(double, double, double);
	double BETAi(double, double, double);
	double GCF(double, double);
	double GSER(double, double);
	double GAMMAP(double, double);
	double ERF(double);
	double NumericalIntegration(double, char);
	double PDF(CArray <double>& param, double x);
public:
	ProbabilityDistributions(char, double, double);
	ProbabilityDistributions() {};
	~ProbabilityDistributions();
	double GAMMALN(double);
	void SetProbabilityDistribution(char, double, double);
	double Compute_pValue(double);
	double CriticalValue(double);
};

#endif
