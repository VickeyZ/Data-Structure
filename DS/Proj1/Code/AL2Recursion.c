/*Algorithm2 recursive version*/
double AL2Recursion(double x, int n)
{
    double result;
	if(n == 1) result = x;
    	/*if N is even, X(n) = X^(n/2) × X^(n/2)*/ 
	else if(n % 2 == 0) 
	{
		double tmp = AL2Recursion(x, n / 2);
		result = tmp * tmp;
	}
		/* if N is odd, X(n) = X^((n-1)/2) × X^((n-1)/2) × X*/
	else
	{
		double tmp = AL2Recursion(x, n / 2);
		result = tmp * tmp * x;
	}
	return result;
}
