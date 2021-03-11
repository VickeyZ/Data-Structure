/*Algorithm2 iterative version*/
double AL2Iteration(double x, int n)
{          
    double result = x;         //Set the return value and assign the value of x
    while(n > 1){                               
		if(n == 3) {
			//if N is odd, X(n) = X((n-1)/2) * X((n-1)/2) * X. So n equals 3 is a separate case
			result = result * result * x;     
			break;
		}
		     
		else result = result * result;
			//if N is even, X(n) = X(n/2) * X(n/2)
    	if(n % 2 == 0) n = n / 2;                    
			//If n is even, n goes into 2 every time
		else n = n / 2 + 1;		                  
			//If n is odd
	}
    return result;
}