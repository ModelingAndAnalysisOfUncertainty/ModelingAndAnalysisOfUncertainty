#include "ProbabilityDistributions.h"
#include "pch.h"
//#include "Modeling and Analysis of Uncertainty.h"

ProbabilityDistributions::ProbabilityDistributions(char Distribution, double a, double b) {
	if ((((Distribution == 'n') || (Distribution == 't')) || (Distribution == 'C')) || (Distribution == 'F')) {
		Type = Distribution;
		if ((Type == 't') || (Type == 'C')) {
			Parameters = new double;
			*Parameters = a;
		}
		else if ((Type == 'n') || (Type == 'F')) {
			Parameters = new double[2];
			Parameters[0] = a;
			Parameters[1] = b;
		}
	}
	else if ((Distribution == 'l') || (Distribution == 'w')) {
		Parameters = new double[2];
		Parameters[0] = a;
		Parameters[1] = b;
	}
	else {
		Type = 'E';
		Parameters = new double;
		*Parameters = (double)0;
	}
}

ProbabilityDistributions::~ProbabilityDistributions() {
	if ((Type == 't') || (Type == 'C')) delete Parameters;
	else if ((Type == 'l') || (Type == 'w')) delete Parameters;
	else if ((Type == 'n') || (Type == 'F')) delete[] Parameters;
}

double ProbabilityDistributions::GAMMALN(double xx) {
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

double ProbabilityDistributions::BETACF(double a, double b, double x) {
	int MAXIT = 100, m, m2;
	double EPS = 3e-7, FPMIN = 1e-30, aa, c, d, del, h, qab, qam, qap;
	qab = a + b;
	qap = a + (double)1;
	qam = a - (double)1;
	c = (double)1;
	d = (double)1 - qab * x / qap;
	if (fabs(d) < FPMIN) d = FPMIN;
	d = (double)1 / d;
	h = d;
	for (m = 1; m < MAXIT; m++) {
		m2 = 2 * m;
		aa = m * (b - m) * x / ((qam + m2) * (a + m2));
		d = (double)1 + aa * d;
		if (fabs(d) < FPMIN) d = FPMIN;
		c = (double)1 + aa / c;
		if (fabs(c) < FPMIN) c = FPMIN;
		d = (double)1 / d;
		h *= d * c;
		aa = -(a + m) * (qab + m) * x / ((a + m2) * (qap + m2));
		d = (double)1 + aa * d;
		if (fabs(d) < FPMIN) d = FPMIN;
		c = (double)1 + aa / c;
		if (fabs(c) < FPMIN) c = FPMIN;
		d = (double)1 / d;
		del = d * c;
		h *= del;
		if (fabs(del - (double)1 < EPS)) break;
	}
	if (m > MAXIT) {
		Error = true;
	}
	return h;
}

double ProbabilityDistributions::BETAi(double a, double b, double x) {
	double bt;
	if ((x >= (double)0) && (x <= (double)1)) {
		if ((x == (double)0) || (x == (double)1)) bt = 0;
		else {
			bt = exp(GAMMALN(a + b) - GAMMALN(a) - GAMMALN(b) + a * log(x) + b * log(1 - x));
		}
		if (x < (a + (double)1) / (a + b + (double)2)) return bt *= BETACF(a, b, x) / a;
		else return (double)1 - bt * BETACF(b, a, (double)1 - x) / b;
	}
	else {
		Error = true;
		return (double)0;
	}
}

double ProbabilityDistributions::GCF(double a, double x) {
	int i, ITMAX = 100;
	double an, b, c, d, del, h, gln, FPMIN = 1e-30, EPS = 1e-8;
	gln = GAMMALN(a);
	b = x + (double)1 - a;
	c = (double)1 / FPMIN;
	d = (double)1 / b;
	h = d;
	for (i = 1; i < ITMAX; i++) {
		an = -(double)i * ((double)i - a);
		b += (double)2;
		d = an * d + b;
		if (fabs(d) < FPMIN) d = FPMIN;
		c = b + an / c;
		if (fabs(c) < FPMIN) c = FPMIN;
		d = (double)1 / d;
		del = d * c;
		h *= del;
		if (fabs(del - (double)1) < EPS) break;
	}
	if (i > ITMAX) {
		ErrorInfo.Empty();
		ErrorInfo = "*** ERROR *** (GCF) - a is too larger or ITMAX is too small!";
		Error = true;
	}
	return exp(-x + a * log(x) - gln) * h;
}

double ProbabilityDistributions::GSER(double a, double x) {
	int n, ITMAX = 100;
	double gamser, sum, del, ap, gln, EPS = 1e-8;
	gln = GAMMALN(a);
	if (x <= (double)0) {
		if (x < (double)0) {
			Error = true;
		}
		return (double)0;
	}
	else {
		ap = a;
		del = sum = (double)1 / a;
		for (n = 1; n < ITMAX; n++) {
			++ap;
			del *= x / ap;
			sum += del;
			if (fabs(del) < fabs(sum) * EPS) {
				gamser = sum * exp(-x + a * log(x) - gln);
				return gamser;
			}
		}
		Error = true;
		return (double)0;
	}
	return (double)0;
}

double ProbabilityDistributions::GAMMAP(double a, double x) {
	if ((x >= 0) && (a > 0)) {
		if (x < (a + (double)1)) return GSER(a, x);
		else return (double)1 - GCF(a, x);
	}
	else {
		Error = true;
		return (double)0;
	}
}

double ProbabilityDistributions::ERF(double x) {
	if (x < (double)0) return -GAMMAP((double)0.5, x * x);
	else return GAMMAP((double)0.5, x * x);

}

double ProbabilityDistributions::Compute_pValue(double x) {
	if (Type == 'n') {
		double z = x / sqrt((double)2);
		return ((double)1 + ERF(z)) / 2;
	}
	else return NumericalIntegration(x, 'p');
}

double ProbabilityDistributions::CriticalValue(double alpha) {
	double xcrit = (double)1;
	if ((alpha > 0) && (alpha < 1)) {
		if (Type == 'n') {
			double EPS = 1e-8, p, dx;
			int MAX_ITER = 100, counter = 0;
			dx = ((double)6 - xcrit) / (double)2;
			p = Compute_pValue(xcrit);
			while (fabs(p - alpha) > EPS) {
				if (p > alpha) xcrit -= dx;
				else xcrit += dx;
				dx /= 2;
				p = Compute_pValue(xcrit);
				counter++;
				if (counter > MAX_ITER) break;
			}
		}
		else xcrit = NumericalIntegration(alpha, 'c');
	}
	return xcrit;
}

double ProbabilityDistributions::NumericalIntegration(double X, char Task) {
	double result = (double)0;
	int N;
	if (Task == 'p') {
		double x0, x, h, value, x_limit = X;
		CArray <double> param;
		CArray <double> Function;
		param.RemoveAll();
		Function.RemoveAll();
		if (Type == 't') {
			x0 = 6;
			param.Add(*Parameters);
		}
		else if (Type == 'C') {
			x0 = 1e-8;
			param.Add(*Parameters);
		}
		else {
			x0 = 1e-8;
			param.Add(Parameters[0]);
			param.Add(Parameters[1]);
		}
		if (fabs(x_limit - x0) < 1) N = 500;
		else if (fabs(x_limit - x0) < 10) N = 5000;
		else N = 50000; 
		h = fabs(x_limit - x0) / (double)N;
		Function.SetSize(static_cast <int64_t>(N + 1));
		for (int i = 0; i <= N; i++) {
			if (Type == 't') {
				x = x_limit + (double)i * h;
				value = PDF(param, x);
				Function.SetAt(i, value);
			}
			else if ( (Type == 'C') || (Type == 'F') ) {
				x = x0 + (double)i * h;
				value = PDF(param, x);
				Function.SetAt(i, value);
			}
			else if ((Type == 'w') || (Type == 'l')) {
				x = x0 + (double)i * h;
				value = PDF(param, x);
				Function.SetAt(i, value);
			}
		}
		double value_1, value_2, value_3;
		int n = (int)(Function.GetSize() - (int)1) / (int)2;
		for (int i = 1; i <= n; i++) {
			value_1 = Function.GetAt(static_cast <int64_t>(2) * i - 2);
			value_2 = Function.GetAt(static_cast <int64_t>(2) * i - 1);
			value_3 = Function.GetAt(static_cast <int64_t>(2) * i);
			result += h * (value_1 + 4 * value_2 + value_3) / (double)3;
		}
		return result;
	}
	else {
		double EPS = 1e-8, alpha = X, p, dx;
		int MAX_ITER = 100, counter = 0;
		if (Type == 't') {
			result = (double)1;
			dx = ((double)6 - result) / (double)2;
		}
		else if ( (Type == 'w') || (Type == 'l') ) {
			result = (double)1;
			dx = 0.5;
		}
		else {
			result = (double)10000;
			dx = (double)5000;
		}
		p = NumericalIntegration(result, 'p');
		if ( (Type == 'C') || (Type == 'F' ) || (Type == 'w') || (Type == 'l') ) p = (double)1 - p;
		while (fabs(p - alpha) > EPS) {
			if (p > alpha) result += dx;
			else result -= dx;
			dx /= 2;
			p = NumericalIntegration(result, 'p');
			if ( (Type == 'C') || (Type == 'F') || (Type == 'w') || (Type == 'l')) p = (double)1 - p;
			counter++;
			if (counter > MAX_ITER) break;
		}
		return result;
	}
}

double ProbabilityDistributions::PDF(CArray <double>& param, double x) {
	double f = (double)0, PI = 3.141592653589793115997963468544185161590576171875;
	if (Type == 't') {
		double const_1 = (param.GetAt(0) + (double)1) / (double)2, const_2 = param.GetAt(0) / (double)2;
		double Gamma_1 = GAMMALN(const_1), Gamma_2 = GAMMALN(const_2), factor = log(sqrt(param.GetAt(0) * PI));
		double Factor = exp(Gamma_1 - (factor + Gamma_2));
		f = Factor * pow((double)1 + (x * x) / param.GetAt(0), -const_1);
	}
	else if (Type == 'C') {
		double const_1 = param.GetAt(0) / (double)2, const_2 = const_1 - 1, Gamma = GAMMALN(const_1), factor = const_1 * log(2);
		double Factor = exp(-(factor + Gamma));
		f = Factor * pow(x,const_2) * exp(- x / (double)2);
	}
	else if (Type == 'F') {
		double const_1 = pow(param.GetAt(0) / param.GetAt(1), param.GetAt(0) / (double)2);
		double const_2 = param.GetAt(0) / (double)2 - (double)1;
		double const_3 = param.GetAt(0) / param.GetAt(1);
		double const_4 = (param.GetAt(0) + param.GetAt(1)) / (double)2;
		double Gamma_1 = GAMMALN(const_4), Gamma_2 = GAMMALN(param.GetAt(0) / (double)2), Gamma_3 = GAMMALN(param.GetAt(1) / (double)2);
		double Factor = exp(Gamma_1 - (Gamma_2 + Gamma_3));
		f = Factor * const_1 * pow(x, const_2) * pow((double)1 + const_3 * x, -const_4);
	}
	else if (Type == 'l') {
		double a = param.GetAt(0), b = param.GetAt(1), value = log(x) - a;
		value /= b;
		f = exp(-0.5 * pow(value, (double)2)) / (x * b * sqrt(2 * PI));
	}
	else if (Type == 'w') {
		double const_1 = param.GetAt(0) / param.GetAt(1);
		double const_2 = param.GetAt(0) - (double)1;
		double const_3 = param.GetAt(0);
		double value = x / param.GetAt(1);
		f = const_1 * pow(value, const_2) * exp(-pow(value, const_3));
	}
	return f;
}

void ProbabilityDistributions::SetProbabilityDistribution(char Distribution, double a, double b) {
	Type = Distribution;
	Parameters = new double[2];
	Parameters[0] = a;
	Parameters[1] = b;
}