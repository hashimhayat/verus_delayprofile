#ifndef DELAYPROFILE_H_
#define DELAYPROFILE_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string> 
#include <stdlib.h> 
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <map>
#include "spline.h"

double standard_deviation(double prevAvg, double newData);
void addtoMap(int w, double delay);
void addSmoothArr(double delay, int w);
int setExpontialFunction(double a, double c);
bool isMonotonic();
int exponentialCurveFitting();
double calculateEMA(double arr[], int size);
int updateDelayProfile(int w, double newDelay);
int createSplineInterpolation();
double getDelay(int w);
double getWindow(double d);


#endif /* DELAYPROFILE_H_ */