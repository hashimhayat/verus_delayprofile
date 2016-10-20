#include "DelayProfile.hpp"
#include "PerlinNoise.h"

using namespace std;

/* Global Variable Declarations */
double prevAvg = -1;
double xsum = 0, x2sum = 0, ysum = 0, xysum = 0;
double coef, power;
bool delayProfile = false;
std::vector< double > lny;
std::vector< double > SmoothArr;
std::vector< double > ExpoArr;
std::vector< double > Smothest;
bool debug = false;

// --------- //
double MIN = 0;
double MAX = 0;

int max_element_smooth (){
	int max = -1;

	for (int i = 0; i < SmoothArr.size(); i++){
		if (SmoothArr.at(i) > max)
			max = SmoothArr.at(i);
	}
		return max;
}

double f_max_element (){
	double max = -1;

	for (int i = 0; i < Smothest.size(); i++){
		if (Smothest.at(i) > max)
			max = Smothest.at(i);
	}

	return max;
}

double f_min_element(){

	double min_e = SmoothArr.at(0);

	int maxIdx = (int) SmoothArr.size()*(0.25);

	for (int i = 1; i < maxIdx; i++){
		if (SmoothArr.at(i) < min_e){
			min_e = SmoothArr.at(i);
		}
	}
		return min_e;		
}

void endpoints(){

	int maxWindow = (int) SmoothArr.size();
	double avg = 0;
	int cnt = 0;

	for (int i = 0; i < SmoothArr.size(); i++){
		if (i > maxWindow*(0.80) && i < maxWindow) {
			Smothest.push_back(SmoothArr.at(i));
			avg += SmoothArr.at(i);
			cnt += 1;
		}
	}

	avg = avg/cnt;

	MIN = f_min_element();
	MAX = f_max_element();
	//MAX = avg;				HIGHEST AVERAGE VALUE

	if (MAX > 0 && MIN > 0){

		printf("LOWEST VAL:  %f\n", MIN);
		printf("MAX VAL:  %f\n", MAX);
		printf("HIGH AVG VAL:  %f\n", avg);

		Smothest.erase (Smothest.begin(),Smothest.end());
		Smothest.push_back(MIN);
		Smothest.push_back(MAX);

		double value = Smothest.at(1) - Smothest.at(0);
		value = value / SmoothArr.size();
		double first = Smothest.at(0);
		Smothest.erase (Smothest.begin(),Smothest.end());

		for (int i = 0; i < SmoothArr.size(); i++){
			if (i == 0)
			Smothest.push_back(first);
		else
			Smothest.push_back(Smothest.at(i-1)+value);
		}

		exponentialCurveFitting();
	} else {
		printf("No Curve Yet!\n");
	} 
}


void groupAverage (){
	
	int sc = 5;
	int size = SmoothArr.size();
	int chunk = size / sc;
	double avg = 0;
	int str = 0, end = sc;

	for (int i = 0; i < sc; i++){

		printf("start = %d end = %d ", str, end);
		for (int w = str; w < end; w++){
			avg += SmoothArr[w];
		}
		printf("avg = %f \n", avg/sc);
		Smothest.push_back(avg / sc);
		avg = 0;

		if (end + sc < size){
			str += sc; 
			end += sc;
		} else {break;}
	}
}


void addSmoothArr(double delay, int w){

	double aplha = 0.3;									// To maintain the diff
	int size = SmoothArr.size();						// Size of SmoothArr
	double prevSmoothVal;					
	double diff;
	int flag = 0;

	/*
		If the size of the SmoothArr is greater than zero i.e it contains values then
		grab the last value in the prevSmoothVal variable.
		Find the difference between the new delay value and the preceding smoothed delay value.
		If the difference is greater than alpha constant (0.3) push the new delay val in SmoothArr.	
	*/
	if (size > 0){
		prevSmoothVal = SmoothArr[size-1];
		diff = abs(prevSmoothVal - delay);
		if (diff > aplha){  // 
			SmoothArr.push_back(delay);
		} else {
			SmoothArr.push_back(prevSmoothVal); 			
		}

	} else {
		SmoothArr.push_back(delay);
	}
	
}

int setExpontialFunction(double a, double c){
	coef = c;
	power = a;
	printf("Exponential fit: y = %f e^%fx\n",coef, power);
	return 0;
}

bool isMonotonic(){
	int tail = SmoothArr.size(), head = 0;

	if (getDelay(tail) - getDelay(head) < 0)
		return false;
	else
		return true;
}

/*
	Go through the entire smoothest and build a exponential curve out of it.
	If update = true, prerequisite (Updated SmoothArr): Updates the Curve in O[1].
	Correct: return 0
	Non monotonic curve: return -1
*/

int exponentialCurveFitting() {
    
    int i,j,k,n;
    n = Smothest.size();
    double a,b,c;
    int w = 0;
    
    double const e = 2.71828;

    if (!delayProfile){

    	lny.erase (lny.begin(),lny.end());
    	xsum = 0, x2sum = 0, ysum = 0, xysum = 0;

	    for (i = 0; i < n; i++)                        
	        lny.push_back(log(Smothest[i]));        
	    
	    for (i = 0; i < n; i++)
	    {
	        xsum = xsum + i;                       //calculate sigma(xi)
	        ysum = ysum+lny[i];                    //calculate sigma(yi)
	        x2sum = x2sum+pow(i,2);                //calculate sigma(x^2i)
	        xysum = xysum+i*lny[i];                //calculate sigma(xi*yi)
	    }

    } else {

		// Updating Curve in O[1]    	
    	ysum -= lny[w];
    	xysum -= w*lny[w];
    	lny[w] = log(Smothest[w]);
    	ysum += lny[w];
    	xysum += w*lny[w];
    	debug = true;
    }

    //calculate slope(or the the power of exp)
    a = (n*xysum-xsum*ysum)/(n*x2sum-xsum*xsum);  
    //calculate intercept          
    b = (x2sum*ysum-xsum*xysum)/(x2sum*n-xsum*xsum);            
    //since b=ln(c)
    c = pow(e,b);                         

	ExpoArr.erase (ExpoArr.begin(),ExpoArr.end());
	for (i = 0; i < n; i++){
		ExpoArr.push_back(c*pow(e,a*i));
	}

    if (isMonotonic() == false){
    	delayProfile = false;
    	printf("Not a Monotonic Curve, Use previous one.\n");
    	return -1;
    }
    else {
    	delayProfile = false;
    	setExpontialFunction(a,c);
		return 0;
    }
    	
} 


double interpolate(double arr[], int i){

	int strIdx = 0, endIdx = 0;
	double val = 0;
	int cnt = 0, sc = 3;

	strIdx = i - sc;
	endIdx = i + sc;

	while (strIdx < 0)
		strIdx++;
	while (endIdx > 40000)							// MAX ARRAY SIZE
		endIdx--; 

	for (int w = strIdx; w < endIdx; w++){
		if (arr[w] != -1){
			val += arr[w];
			cnt++;	
		}
	}
	if (cnt != 0 && val > 1){
		return (val/cnt);
	}

	return (val/cnt);

}
/*
	Calculates the smoothed values of the last sc values including the very last value.
	Inserts the distinct values into SmoothArr.
*/
double calculateEMA(double arr[], int idx) {

	int i; 
	double avg = 0;
	int sc = 30;			// More the sc (Smooth Constant): Smoother the curve but less sensitive
	int cnt = 0;
	double sd = 100;

	/* 	 
		Loops from the window to the start until Smooth Consant is reached.
		If there is no value at an idx, generates an interpolated value for that window.
		Calculates average of sc (20) values from the last window.
		If there is a new smoothed value, then update the smoothArr.
	*/ 
	for (i = idx; i > 0; i--){

		if (arr[i] == -1){
			arr[i] = interpolate(arr,i);	
		}

		avg += arr[i];
		cnt++;	
		if (sc == cnt)
			break;
	}

	avg = avg/cnt;

	if ((prevAvg < 0 || avg != prevAvg)){
		addSmoothArr(avg, idx);
	}
	prevAvg = avg;
	return avg;
}

/*
	Updates the delay,window MAP and generate an updated delay profile
	Uses the smooth array to smooth the incoming value by taking forward and backwards 
	moving average.
	generates a [new exponential function]
*/
int updateDelayProfile(double newDelay, int w) {

	double oldDelay = SmoothArr[w];
	int sc = 10;					// Takes moving average of -sc & +sc values
	int i, cnt = 0, strIdx, endIdx;
	double avgDelay = 0;

	strIdx = w - sc;
	endIdx = w + sc;

	if (oldDelay != newDelay) {
		while (strIdx < 0)
			strIdx++;
		while (endIdx > SmoothArr.size())
			endIdx--;

		// If a delay value for the window exists then calculate the average
		// otherwise return -1;

		if (w < SmoothArr.size()){
			SmoothArr[w] = newDelay;

			for (i = strIdx; i < endIdx; i++){
				avgDelay += SmoothArr[i];
				cnt++;
			}

			// Updating the SmoothArr at the window with new average
			avgDelay = avgDelay/cnt;
	 		SmoothArr[w] = avgDelay;
		} else {
			return -1;
		} 			
	}

	printf("Updated %f to %f\n", oldDelay, newDelay);		
	return 0;
}

int createSplineInterpolation() {
	std::vector<double> X;

	ofstream spline;
    spline.open ("spline.csv");

	for (int i = 0; i < SmoothArr.size(); i++){
		X.push_back(i);
	}

	tk::spline s;
	s.set_points(X,SmoothArr);    // currently it is required that X is already sorted

	double x = 20;

	printf("spline at %f is %f\n", x, s(x));

	for (int i = 0; i < SmoothArr.size(); i++){
		spline << (to_string(s(i))) + "\n";
	}

	return EXIT_SUCCESS;
}

// returns the value of Delay at a specific window w
double getDelay(int w){

	double const e = 2.71828;
	bool Smooth = false, Exp = true;

	if (Exp)
		return coef*pow(e,power*w);
	else if (w <= SmoothArr.size() && Smooth)
		return SmoothArr.at(w);
	else 
		return -1;
}


// returns the value of window size at a specific delay
double getWindow(double d) {

	bool Smooth = false, Exp = true;
	int w = 0,i = 0;
	int windowArr[10];

	if (Exp)
		return (log(d/coef))/power; 

	// Needs Fix
	if (Smooth){
		for (w = 0; w < SmoothArr.size(); w++){
			if (SmoothArr.at(w) == d && i < 10){
				windowArr[i] = w;
				i++;
			}

		}
		return *windowArr;
	}

	else 
		return -1;
}


/*
	Creates a curve from the first and last value recieved.
*/

void generateCurve(double latest_window){

	double y;
	double x = 50;

	ExpoArr.erase (ExpoArr.begin(),ExpoArr.end());

	for (int x = 0; x < latest_window; x++) {
		y = MIN*(pow((MAX/MIN),(x/latest_window)));
		ExpoArr.push_back(y);
	}

}

int main(){

	ofstream smooth;
    smooth.open ("smooth.csv");
    ofstream normal;
    normal.open ("normal.csv");
    ofstream line;
    line.open ("line.csv");
    ofstream curve;
    curve.open ("curve.csv");

	int arrSize = 1000;
	double wList[arrSize];
	double delay;
	int skip, rwind, idx = 0;
	srand (time(NULL));

	unsigned int seed = 237;
	PerlinNoise pn(seed);
	double n;
	double PerlinDelay[arrSize];

	// Inits the wLists with -1s
	for (int i = 0; i < arrSize; i++){
		wList[i] = -1;
	}

	for(unsigned int i = 0; i < arrSize; ++i) {     // y
			double x = (double)i;

			// Typical Perlin noise
			double n = pn.noise(100 * x, 100, 2.2);
			PerlinDelay[i] = n*100;
	}


	// Generate false delay values for testing purposes
	for (int w = 1; w < arrSize; w++){

		delay = PerlinDelay[w];
		rwind = rand() % 20;
		skip = rand() % 2;

		if (skip == 1)
			idx = w;
		else
			idx = rwind;

		if (wList[idx] == -1 && w > SmoothArr.size()){
			wList[idx] = delay;
			calculateEMA(wList, idx);
		} else {
			updateDelayProfile(delay, idx);
		}

	// FINDS ENPOINTS (MIN & MAX) and generates the curve
		endpoints();
	}

	// Write normal delay values to the file
	for (int i = 0; i < arrSize; i++)
		normal << (to_string(wList[i])) + "\n";

	// Write the smoothed delay values to the smooth to the file
	for (int j = 0; j < SmoothArr.size(); j++){
		smooth << (to_string(SmoothArr[j])) + "\n";
	}

	int k;
	for (k = 0; k < Smothest.size(); k++){
		line << (to_string(Smothest[k])) + "\n";
	}

	// Curve using generatCurve after each update
	for (int j = 0; j < ExpoArr.size(); j++){
		curve << (to_string(ExpoArr[j])) + "\n";
	}

	createSplineInterpolation();

	double dly = getDelay(500);
	printf("delay at 500: %f\n", dly);

	return 0;
}


