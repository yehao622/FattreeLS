/*
 * General.cc
 *
 *  Created on: Feb 6, 2023
 *      Author: hao
 */

#include "General.h"

double MM1QueueLen(double lambda, double mu){
    double pho = lambda / mu;
    return pho*pho / (1 - pho);
}

double MM1Delay(double lambda, double mu){
    return 1.0 / (mu - lambda);
}

double fac(int num){
    if(num < 0)
        throw cRuntimeError("Factorial operation not work on negative numbers!\n");

    if(num <= 1) return 1.0;

    return (double)num * fac(num-1);
}

double MMMpmf(double lambda, double mu, int m){
    double pho = lambda / (m * mu);
    double tmp(0.0);

    for(int k=0; k<m; k++){
        tmp += pow(m*pho, k) / fac(k);
    }

    tmp += pow(m*pho, m) / (fac(m) * (1-pho));

    double pi0 = 1.0 / tmp;

    return pow(m*pho, m) * pi0 / (fac(m) * (1-pho));
}

double MMMQueueLen(double lambda, double mu, int m){
    double pho = lambda / (m * mu);
    double pmf = MMMpmf(lambda, mu, m);

    return pho * pmf / (1 - pho);
}

double MMMDelay(double lambda, double mu, int m){
    double pho = lambda / (m * mu);

    return (m*pho + MMMpmf(lambda, mu, m) * pho / (1 - pho)) / lambda;
}
