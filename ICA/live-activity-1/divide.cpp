/*
Example: Division by zero fault, not handled.
*/

#include <iostream>
using namespace std;

float divide(float numerator, float denominator)
{
	if (denominator == 0)
	{
		cout << "Error: Division by zero." << endl;
		throw runtime_error("Division by zero.");
	}
	return (numerator / denominator);
}

int main()
{
	float numerator;
	float denominator;
	float quotient;

	cout << "Enter the numerator: ";
	cin >> numerator;
	cout << "Enter the denominator: ";
	cin >> denominator;
	try
	{
		quotient = divide(numerator, denominator);
		cout << "The quotient is: " << quotient << endl;
	}
	catch(const runtime_error& e)
	{
		std::cerr << e.what() << endl;
	}
	
	return 0;
	
}
