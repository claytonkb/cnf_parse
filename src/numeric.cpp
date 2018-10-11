// numeric.cpp

#include "numeric.h"
#include "math.h"
#include "stdlib.h"

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                              NUMERIC                                   //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// uniform distribution random variable
//
float rand_range(float range_lo, float range_hi){

    float rand01 = (1.0f * (float)rand()) / (float)RAND_MAX;
    float range_mag = range_hi-range_lo;
    return ((float)rand01 * range_mag + range_lo);

}


// normal distribution random variable
//
double rand_normal(double mean, double stddev){ //Box muller method

    static double n2 = 0.0;
    static int n2_cached = 0;

    if (!n2_cached){

        double x, y, r;
        do{

            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;

        }
        while (r == 0.0 || r > 1.0);{

            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;

        }
    }
    else{

        n2_cached = 0;
        return n2*stddev + mean;

    }

}


//
//
float logistic(float x){

    return 1.0f / (1.0f + expf(-1*x));

}


//
//
float soft_sign(float x){

    return x / (1 + fabs(x));

}



// Clayton Bauman 2018


