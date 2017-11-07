#include <cmath>
#include <climits>
#include "structs.h"

void unaryAbs(long *ans, long operand) {
	if (operand < 0) {
		// |-inf| = inf
		if (operand == LONG_MIN)
			*ans = LONG_MAX;
		else
			*ans = (-1) * operand;
	}
	else
		*ans = operand;
}

bool unarySqrt(long *ans, long operand) {
	//reject negative inputs
	if (operand < 0)
		return false;

	// sqrt inf = inf
	if (operand == LONG_MAX)
		*ans = LONG_MAX;
	else
		*ans = (long) floor(sqrt(operand));
	return true;
}

//-------------------------------------------------

void unaryExp(long *ans, long operand) {
	double result = exp(operand);

	// e^x >= LONG_MAX -> result = inf
	if (result >= LONG_MAX)
		*ans = LONG_MAX;
	else
		*ans = (long) floor(result);
}

bool unaryLog(long *ans, long operand) {
	//reject inputs smaller than 1
	if (operand <= 0)
		return false;

	// ln(inf) = inf
	if (operand == LONG_MAX)
		*ans = LONG_MAX;
	else
		*ans = (long) floor(log(operand));
	
	return true;
}

void unaryModulo(long *ans, long operand) {
	if (operand < 0) {
		//negative inputs are elevated to the first positive modulo result
		long result = operand;
		while (result < 0)
			result += 10;
		*ans = result;
	}
	else
		*ans = operand % 10;
}

bool binaryExp(long *ans, long operand1, long operand2) {
	if (operand1 == 0) {
		// 0^inf, 0^-inf, 0^0 are invalid
		if (operand2 == LONG_MAX || operand2 == LONG_MIN || operand2 == 0)
			return false;
	}
	else if (operand1 == LONG_MAX) {
		// inf^0 is invalid
		if (operand2 == 0)
			return false;
	}
	else if (operand1 == LONG_MIN) {
		// (-inf)^inf, (-inf)^-inf, (-inf)^0 are invalid
		if (operand2 == LONG_MAX || operand2 == LONG_MIN || operand2 == 0)
			return false;
	}

	//for valid 2nd inputs, 0^x is 0
	if (operand1 == 0)
		*ans = 0;
	else {
		double result = pow(operand1, operand2);

		// x^y >= LONG_MAX -> result = inf
		if (result >= LONG_MAX)
			*ans = LONG_MAX;
		// x^y <= LONG_MIN -> result = -inf
		else if (result <= LONG_MIN)
			*ans = LONG_MIN;
		else
			*ans = (long) floor(result);
	}

	return true;
}

bool binaryLog(long *ans, long operand1, long operand2) {
	//reject inputs where the base is less than 2 or the input to log is less than 1
	if (operand1 < 2 || operand2 <= 0)
		return false;

	//logx(inf) = inf
	if (operand2 == LONG_MAX)
		*ans = LONG_MAX;
	// logx(y) = ln(y)/ln(x)
	else
		*ans = (long) floor(log(operand2) / log(operand1));

	return true;
}

bool binaryModulo(long *ans, long operand1, long operand2) {
	//reject mods that are less than 2
	if (operand1 < 2)
		return false;

	if (operand2 < 0) {
		//negative inputs are elevated to the first positive modulo result
		long result = operand2;
		while (result < 0)
			result += 10;
		*ans = result;
	}
	else
		*ans = operand2 % operand1;

	return true;
}

//-------------------------------------------------

bool binarySubtraction(long *ans, long operand1, long operand2) {
	// inf - inf is invalid
	if ((operand1 == LONG_MAX && operand2 == LONG_MAX) || (operand1 == LONG_MIN && operand2 == LONG_MIN))
		return false;

	long result = operand1 - operand2;

	//underflow -> result = -inf
	if (operand1 < 0 && operand2 > 0 && result > 0)
		*ans = LONG_MIN;
	//overflow -> result = inf
	else if (operand1 > 0 && operand2 < 0 && result < 0)
		*ans = LONG_MAX;
	else
		*ans = result;

	return true;
}

bool binaryDivision(long *ans, long operand1, long operand2) {
	//division by 0 is invalid
	if (operand2 == 0)
		return false;
	// inf/inf, -inf/inf, inf/-inf and -inf/inf are invalid
	else if (operand1 == LONG_MAX || operand1 == LONG_MIN) {
		if (operand2 == LONG_MAX || operand2 == LONG_MIN)
			return false;
	}

	//integer division
	*ans = operand1 / operand2;

	return true;
}

//-------------------------------------------------

bool chainSum(long *sum, long chainOp) {
	//at any step, inf - inf should be invalid
	if ((*sum == LONG_MAX && chainOp == LONG_MIN) || (*sum == LONG_MIN && chainOp == LONG_MAX))
		return false;

	long result = *sum + chainOp;

	//underflow -> result = -inf
	if (*sum < 0 && chainOp < 0 && result > 0)
		*sum = LONG_MIN;
	//overflow -> result = inf
	else if (*sum > 0 && chainOp > 0 && result < 0)
		*sum = LONG_MAX;
	else
		*sum = result;

	return true;
}

bool chainProduct(long *prd, long chainOp) {
	//at any step, 0.inf, 0.(-inf), inf.0, (-inf).0 should be invalid
	if (chainOp == 0) {
		if (*prd == LONG_MAX || *prd == LONG_MIN)
			return false;
	}
	else if (*prd == 0) {
		if (chainOp == LONG_MAX || chainOp == LONG_MIN)
			return false;
	}

	double result = (double) (*prd) * chainOp;

	//result exceeds or is equal to LONG_MAX -> inf 
	if (result >= LONG_MAX)
		*prd = LONG_MAX;
	//result exceeds or is equal to LONG_MIN -> -inf 
	else if (result <= LONG_MIN)
		*prd = LONG_MIN;
	else
		*prd = (long)result;

	return true;
}

void chainMax(long *max, long chainOp) {
	if (chainOp > *max)
		*max = chainOp;
}

void chainMin(long *min, long chainOp) {
	if (chainOp < *min)
		*min = chainOp;
}

//-------------------------------------------------

bool polynarySum(long *ans, long inputs[], int size) {
	long sum = 0;
	bool flag = true;

	//add all numbers one by one
	for (int i = 0; i < size; ++i) {
		if (!chainSum(&sum, inputs[i])) {
			flag = false;
			break;
		}
	}

	*ans = sum;

	return flag;
}

bool polynaryProduct(long *ans, long inputs[], int size) {
	long prd = 1;
	bool flag = true;

	//multiply all numbers one by one
	for (int i = 0; i < size; ++i) {
		if (!chainProduct(&prd, inputs[i])) {
			flag = false;
			break;
		}
	}

	*ans = prd;

	return flag;
}

void polynaryMax(long *ans, long inputs[], int size) {
	long max = LONG_MIN;

	//check for max in numbers one by one
	for (int i = 0; i < size; ++i)
		chainMax(&max, inputs[i]);

	*ans = max;
}

void polynaryMin(long *ans, long inputs[], int size) {
	long min = LONG_MAX;

	//check for min in numbers one by one
	for (int i = 0; i < size; ++i)
		chainMin(&min, inputs[i]);
	
	*ans = min;
}