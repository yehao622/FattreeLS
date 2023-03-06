/*
 * General.h
 *
 *  Created on: Feb 5, 2023
 *      Author: hao
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include <string>
#include <omnetpp.h>
#include <map>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include "request_m.h"
#include <regex>

using namespace omnetpp;

double fac(int);

double MM1QueueLen(double, double); // M/M/1 waiting queue length
double MM1Delay(double, double);  // M/M/1 total delay

double MMMpmf(double, double, int);  // M/M/m probability function
double MMMQueueLen(double, double, int); // M/M/m waiting queue length
double MMMDelay(double, double, int);  // M/M/m total delay

#endif /* GENERAL_H_ */
