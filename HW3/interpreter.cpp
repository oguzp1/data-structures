/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 25.12.2016
 */
#include <cstring>
#include <cctype>
#include <climits>
#include <cstdio>
#include "structs.h"

void unaryAbs(long *ans, long operand);
bool unarySqrt(long *ans, long operand);
void unaryExp(long *ans, long operand);
bool unaryLog(long *ans, long operand);
void unaryModulo(long *ans, long operand);
bool binaryExp(long *ans, long operand1, long operand2);
bool binaryLog(long *ans, long operand1, long operand2);
bool binaryModulo(long *ans, long operand1, long operand2);
bool binarySubtraction(long *ans, long operand1, long operand2);
bool binaryDivision(long *ans, long operand1, long operand2);
bool chainSum(long *sum, long chainOp);
bool chainProduct(long *prd, long chainOp);
void chainMax(long *max, long chainOp);
void chainMin(long *min, long chainOp);
bool polynarySum(long *ans, long inputs[], int size);
bool polynaryProduct(long *ans, long inputs[], int size);
void polynaryMax(long *ans, long inputs[], int size);
void polynaryMin(long *ans, long inputs[], int size);

//decides operation mode
int operandMode(char *str) {
	if (strcmp(str, "(") == 0)
		return 0;
	else if (strcmp(str, "abs") == 0 || strcmp(str, "sqrt") == 0)
		return 1;
	else if (strcmp(str, "div") == 0 || strcmp(str, "sub") == 0)
		return 2;
	else if (strcmp(str, "exp") == 0 || strcmp(str, "log") == 0 || strcmp(str, "mod") == 0)
		return 3;
	else
		return 4;
}

//checks if str is a number
bool isNumber(char *str) {
	bool flag = true;

	//loop the string, if any non-digit chars are seen flag = false
	for (char *t = str; *t; ++t) {
		if (t == str && *t == '-')
			continue;
		
		if (!isdigit(*t)) {
			flag = false;
			break;
		}
	}

	//if the string is inf, -inf or ans, flag = true (these are valid numbers)
	if (strcmp(str, "inf") == 0 || strcmp(str, "-inf") == 0 || strcmp(str, "ans") == 0)
		flag = true;

	return flag;
}

//converts given string to a long number
long toNumber(char *str, long prevAns) {
	long number;

	//inf = LONG_MAX
	if (strcmp(str, "inf") == 0)
		number = LONG_MAX;
	//-inf = LONG_MIN
	else if (strcmp(str, "-inf") == 0)
		number = LONG_MIN;
	//ans = answer of the previous calculation
	else if (strcmp(str, "ans") == 0)
		number = prevAns;
	//else, convert number
	else {
		bool isNegative = false;
		int i;
		number = 0;

		//tilda sign is ignored, negative sign is stored in a bool variable to be used later
		for (char *t = str; *t; ++t) {
			if (t == str) {
				if (*t == '~') {
					++t;
					if (*t == '-') {
						isNegative = true;
						continue;
					}
				}
				else if (*t == '-') {
					isNegative = true;
					continue;
				}
			}

			//digit-by-digit conversion
			if (i = isdigit(*t)) {
				number *= 10;
				number += *t - '0';
			}
		}

		//if negative, multiply by -1 to make the long value negative
		if (isNegative)
			number *= (-1);
	}

	return number;
}

//evaluates prefix inputStr, changes it to postfix, updates ans
bool evaluate(char *inputStr, long *ans) {
	char operand_input[MAX_NUMBER_LENGTH + 1], *traverse, *postfix;
	int count = MAX_NUMBER_LENGTH - 1;
	bool operationProcessed = false;
	String s;
	stack tildaConversions;
	stack expressionStack;

	//initialize String
	s.initialize();

	//initialize dictionary stack
	tildaConversions.initialize();

	//initialize stack
	expressionStack.initialize();

	//go to the character before the newline character
	for (traverse = inputStr; *traverse != '\n'; ++traverse);
	--traverse;

	if (*traverse == '\r')
		--traverse;

	//postfix conversion & evaluation begins
	operand_input[MAX_NUMBER_LENGTH] = '\0';

	//until the string's first character has been processed
	while (traverse != inputStr - 1) {
		//when traverse isn't on a space character
		if (*traverse != ' ' && traverse != inputStr)
			//continue writing the current expression
			operand_input[count--] = *traverse;
		//if a space character has been seen
		else {
			//place the first character into final expression when traverse reaches the first character
			if (traverse == inputStr)
				operand_input[count] = *traverse;
			//for all characters except the first character, go to the needed character in the array
			else
				++count;

			//if the current expression is a number or a ')'
			if (strcmp(operand_input + count, ")") == 0 || isNumber(operand_input + count)) {
				//push to stack
				expressionStack.push(operand_input + count);
				//reset operationProcessed
				operationProcessed = false;
			}
			//if the current expression is an operation or a '('
			else {
				char **popped, *extraPar, pushed[MAX_NUMBER_LENGTH + 2];
				//backup previous answer for operations and failures
				long prevAns = *ans,
					//variables to hold operands
					num1, num2, *numarray;
				//variable that indicates unary/binary mode for case 3
				bool isBinary;
				//variable that holds operand count for case 4
				int operandCount;

				//decide the type of the operation
				switch (operandMode(operand_input + count)) {
				//for unary operations
				case 1:
					//only one operand
					popped = new char*[1];
					//if able, pop 1 operand from stack, if not return false
					if (expressionStack.isEmpty()) {
						tildaConversions.clear();
						expressionStack.clear();
						s.closeString();
						*ans = prevAns;
						delete[] popped;
						return false;
					}
					else {
						popped[0] = expressionStack.pop();

						//if parantheses are closed, return false
						if (strcmp(popped[0], ")") == 0) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete[] popped[0];
							delete[] popped;
							return false;
						}
					}

					//handle a single paranthesis after the operation, if it exists
					if (!expressionStack.isEmpty()) {
						extraPar = expressionStack.pop();

						//if not a paranthesis, push it back to the stack
						if (strcmp(extraPar, ")") != 0)
							expressionStack.push(extraPar);

						delete[] extraPar;
					}

					//turn operand into a number
					num1 = toNumber(popped[0], prevAns);

					//if the operation is abs, calculate abs
					if (strcmp(operand_input + count, "abs") == 0)
						unaryAbs(ans, num1);
					//else, the operation is sqrt
					else {
						//calculate sqrt, if sqrt cannot be calculated return false
						if (!unarySqrt(ans, num1)) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete[] popped[0];
							delete[] popped;
							return false;
						}
					}

					//add an expression with proper parantheses to the postfix string
					s.addToEnd("(");
					//if a number with ~ was seen, get the postfix operation representation
					if (*popped[0] != '~') {
						s.addToEnd(" ");
						s.addToEnd(popped[0]);
					}
					else {
						char *temp = tildaConversions.pop();
						s.addToEnd(" ");
						s.addToEnd(temp);
						delete[] temp;
					}
					s.addToEnd(" ");
					s.addToEnd(operand_input + count);
					s.addToEnd(" )");
					postfix = s.getString();
					//clear String for later use
					s.clear();

					//write ans to a string with a special ~ character
					sprintf(pushed, "~%ld", *ans);
					//add conversion to dictionary
					tildaConversions.push(postfix);
					delete[] postfix;
					//push ans to the stack
					expressionStack.push(pushed);

					delete[] popped[0];
					delete[] popped;
					break;
				//for binary operations
				case 2:
					//two operands
					popped = new char*[2];

					//if able, pop the first operand from stack, if not return false
					if (expressionStack.isEmpty()) {
						tildaConversions.clear();
						expressionStack.clear();
						s.closeString();
						*ans = prevAns;
						delete[] popped;
						return false;
					}
					else {
						popped[0] = expressionStack.pop();

						//if parantheses are closed, return false
						if (strcmp(popped[0], ")") == 0) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete[] popped[0];
							delete[] popped;
							return false;
						}
					}

					//if able, pop the second operand from stack, if not return false
					if (expressionStack.isEmpty()) {
						tildaConversions.clear();
						expressionStack.clear();
						s.closeString();
						*ans = prevAns;
						delete[] popped[0];
						delete[] popped;
						return false;
					}
					else {
						popped[1] = expressionStack.pop();

						//if parantheses are closed, return false
						if (strcmp(popped[1], ")") == 0) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete[] popped[0];
							delete[] popped[1];
							delete[] popped;
							return false;
						}
					}

					//handle a single paranthesis after the operation, if it exists
					if (!expressionStack.isEmpty()) {
						extraPar = expressionStack.pop();

						//if not a paranthesis, push it back to the stack
						if (strcmp(extraPar, ")") != 0)
							expressionStack.push(extraPar);

						delete[] extraPar;
					}

					//turn operands into numbers
					num1 = toNumber(popped[0], prevAns);
					num2 = toNumber(popped[1], prevAns);

					//if the operation is sub
					if (strcmp(operand_input + count, "sub") == 0) {
						//calculate sub, if sub cannot be calculated return false
						if (!binarySubtraction(ans, num1, num2)) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete[] popped[0];
							delete[] popped[1];
							delete[] popped;
							return false;
						}
					}
					//else, the operation is div
					else {
						//calculate div, if div cannot be calculated return false
						if (!binaryDivision(ans, num1, num2)) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete[] popped[0];
							delete[] popped[1];
							delete[] popped;
							return false;
						}
					}

					//add an expression with proper parantheses to the postfix string
					s.addToEnd("(");
					if (*popped[0] != '~') {
						s.addToEnd(" ");
						s.addToEnd(popped[0]);
					}
					else {
						char *temp = tildaConversions.pop();
						s.addToEnd(" ");
						s.addToEnd(temp);
						delete[] temp;
					}
					if (*popped[1] != '~') {
						s.addToEnd(" ");
						s.addToEnd(popped[1]);
					}
					else {
						char *temp = tildaConversions.pop();
						s.addToEnd(" ");
						s.addToEnd(temp);
						delete[] temp;
					}
					s.addToEnd(" ");
					s.addToEnd(operand_input + count);
					s.addToEnd(" )");
					postfix = s.getString();
					//clear String for later use
					s.clear();

					//write ans to a string with a special ~ character
					sprintf(pushed, "~%ld", *ans);
					//add conversion to dictionary
					tildaConversions.push(postfix);
					delete[] postfix;
					//push ans to the stack
					expressionStack.push(pushed);

					delete[] popped[0];
					delete[] popped[1];
					delete[] popped;
					break;
				//for unary/binary operations
				case 3:
					//allocate space for 2 operand strings
					popped = new char*[2];
					//boolean for operation mode, and paranthesis state
					isBinary = true;

					//if able, pop the first operand from stack, if not return false
					if (expressionStack.isEmpty()) {
						tildaConversions.clear();
						expressionStack.clear();
						s.closeString();
						*ans = prevAns;
						delete[] popped;
						return false;
					}
					else {
						popped[0] = expressionStack.pop();

						//if parantheses are closed, return false
						if (strcmp(popped[0], ")") == 0) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete[] popped[0];
							delete[] popped;
							return false;
						}
					}

					//if able, pop the second operand from stack, if not operation mode is unary
					if (expressionStack.isEmpty())
						isBinary = false;
					else {
						popped[1] = expressionStack.pop();

						//if parantheses are closed, operation mode is unary
						if (strcmp(popped[1], ")") == 0) {
							isBinary = false;
							delete[] popped[1];
						}
					}

					if (isBinary) {
						//handle a single paranthesis after the operation, if it exists
						if (!expressionStack.isEmpty()) {
							extraPar = expressionStack.pop();

							//if not a paranthesis, push it back to the stack
							if (strcmp(extraPar, ")") != 0)
								expressionStack.push(extraPar);

							delete[] extraPar;
						}
					}
					
					//turn operand1 into a number, if isBinary turn operand2 into a number
					num1 = toNumber(popped[0], prevAns);

					if (isBinary) {
						num2 = toNumber(popped[1], prevAns);

						//if operation is exp
						if (strcmp(operand_input + count, "exp") == 0) {
							//calculate binary exp, if binary exp cannot be calculated return false
							if (!binaryExp(ans, num1, num2)) {
								tildaConversions.clear();
								expressionStack.clear();
								s.closeString();
								*ans = prevAns;
								delete[] popped[0];
								delete[] popped[1];
								delete[] popped;
								return false;
							}
						}
						//else if the operation is log
						else if (strcmp(operand_input + count, "log") == 0) {
							//calculate binary log, if binary log cannot be calculated return false
							if (!binaryLog(ans, num1, num2)) {
								tildaConversions.clear();
								expressionStack.clear();
								s.closeString();
								*ans = prevAns;
								delete[] popped[0];
								delete[] popped[1];
								delete[] popped;
								return false;
							}
						}
						//else, the operation is mod
						else {
							//calculate binary mod, if binary mod cannot be calculated return false
							if (!binaryModulo(ans, num1, num2)) {
								tildaConversions.clear();
								expressionStack.clear();
								s.closeString();
								*ans = prevAns;
								delete[] popped[0];
								delete[] popped[1];
								delete[] popped;
								return false;
							}
						}
					}
					//else, unary calculations
					else {
						//if the operation is exp, calculate unary exp
						if (strcmp(operand_input + count, "exp") == 0)
							unaryExp(ans, num1);
						//else if the operation is log
						else if (strcmp(operand_input + count, "log") == 0) {
							//calculate unary log, if unary log cannot be calculated return false
							if (!unaryLog(ans, num1)) {
								tildaConversions.clear();
								expressionStack.clear();
								s.closeString();
								*ans = prevAns;
								delete[] popped[0];
								delete[] popped;
								return false;
							}
						}
						//else, the operation is mod, calculate unary mod
						else
							unaryModulo(ans, num1);
					}

					//add an expression with proper parantheses to the postfix string
					s.addToEnd("(");
					if (*popped[0] != '~') {
						s.addToEnd(" ");
						s.addToEnd(popped[0]);
					}
					else {
						char *temp = tildaConversions.pop();
						s.addToEnd(" ");
						s.addToEnd(temp);
						delete[] temp;
					}
					if (isBinary && *popped[1] != '~') {
						s.addToEnd(" ");
						s.addToEnd(popped[1]);
					}
					else if (isBinary) {
						char *temp = tildaConversions.pop();
						s.addToEnd(" ");
						s.addToEnd(temp);
						delete[] temp;
					}
					s.addToEnd(" ");
					s.addToEnd(operand_input + count);
					s.addToEnd(" )");
					postfix = s.getString();
					//clear String for later use
					s.clear();

					//write ans to a string with a special ~ character
					sprintf(pushed, "~%ld", *ans);
					//add conversion to dictionary
					tildaConversions.push(postfix);
					delete[] postfix;
					//push ans to the stack
					expressionStack.push(pushed);

					delete[] popped[0];
					if (isBinary)
						delete[] popped[1];
					delete[] popped;
					break;
				//for polynary operations
				case 4:
					//allocate space for the maximum amount of operand strings
					popped = new char*[MAX_EXPRESSION_LENGTH / 2 - 1];

					//get a count on the number of of operands
					operandCount = 0;

					//loop until stack becomes empty or 
					while (true) {
						//if able, pop an operand from stack, if not break
						if (expressionStack.isEmpty())
							break;
						else {
							popped[operandCount] = expressionStack.pop();

							//if parantheses are closed, break
							if (strcmp(popped[operandCount], ")") == 0) {
								delete[] popped[operandCount];
								break;
							}
						}

						//increment operandCount
						++operandCount;
					}

					//if no operands are given, return false
					if (operandCount == 0) {
						tildaConversions.clear();
						expressionStack.clear();
						s.closeString();
						*ans = prevAns;
						delete[] popped;
						return false;
					}

					//turn operands into numbers
					numarray = new long[operandCount];

					for (int i = 0; i < operandCount; ++i)
						numarray[i] = toNumber(popped[i], prevAns);

					//if the operation is sum
					if (strcmp(operand_input + count, "sum") == 0) {
						//calculate sum, if sum cannot be calculated return false
						if (!polynarySum(ans, numarray, operandCount)) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete numarray;
							for (int i = 0; i < operandCount; ++i)
								delete[] popped[i];
							delete[] popped;
							return false;
						}
					}
					//else if the operation is product
					else if (strcmp(operand_input + count, "product") == 0) {
						//calculate product, if product cannot be calculated return false
						if (!polynaryProduct(ans, numarray, operandCount)) {
							tildaConversions.clear();
							expressionStack.clear();
							s.closeString();
							*ans = prevAns;
							delete numarray;
							for (int i = 0; i < operandCount; ++i)
								delete[] popped[i];
							delete[] popped;
							return false;
						}
					}
					//else if the operation is max, calulate max
					else if (strcmp(operand_input + count, "max") == 0)
						polynaryMax(ans, numarray, operandCount);
					//else, the operation is min, calculate min
					else
						polynaryMin(ans, numarray, operandCount);

					delete numarray;

					//otherwise, add an expression with proper parantheses to the postfix string
					s.addToEnd("(");
					for (int i = 0; i < operandCount; ++i) {
						if (*popped[i] != '~') {
							s.addToEnd(" ");
							s.addToEnd(popped[i]);
						}
						else {
							char *temp = tildaConversions.pop();
							s.addToEnd(" ");
							s.addToEnd(temp);
							delete[] temp;
						}
					}
					s.addToEnd(" ");
					s.addToEnd(operand_input + count);
					s.addToEnd(" )");
					postfix = s.getString();
					//clear String for later use
					s.clear();

					//write ans to a string with a special ~ character
					sprintf(pushed, "~%ld", *ans);
					//add conversion to dictionary
					tildaConversions.push(postfix);
					delete[] postfix;
					//push ans to the stack
					expressionStack.push(pushed);

					for (int i = 0; i < operandCount; ++i)
						delete[] popped[i];
					delete[] popped;
					break;
				//for open parantheses
				default:
					//if the operator came after the open paranthesis, reset operationProcessed
					if (operationProcessed)
						operationProcessed = false;
					//else return false
					else {
						tildaConversions.clear();
						expressionStack.clear();
						s.closeString();
						*ans = prevAns;
						return false;
					}
					break;
				}

				//put an indicator that an operation has been done
				operationProcessed = true;
			}
			//reset count
			count = MAX_NUMBER_LENGTH - 1;
		}
		//go backwards in inputStr
		--traverse;
	}

	//write postfix onto inputStr, prefix expression is converted to postfix
	if (!tildaConversions.isEmpty()) {
		postfix = tildaConversions.pop();
		strcpy(inputStr, postfix);
		delete[] postfix;
	}
	
	//empty stacks
	//check for syntax errors at the end as well
	while (!expressionStack.isEmpty()) {
		char *finalcheck = expressionStack.pop();
		if (finalcheck[0] != '~') {
			expressionStack.clear();
			tildaConversions.clear();
			s.closeString();
			return false;
		}
	}

	tildaConversions.clear();
	//close String
	s.closeString();
	return true;
}