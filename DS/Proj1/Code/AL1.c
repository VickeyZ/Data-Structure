/*Function of algorithm1*/
double AL1(double x, int n){    
    double result = 1;           //Define the result value/return value and initialize it
    int i; 
    for(i = 0; i < n; i++){      //Multiply the initial return value by x for n times
		result = result * x;
	}
    return result;               //Return x to the n
}