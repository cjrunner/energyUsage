//
//  SetupForMultiFit.hpp
//  energyUsage
//
//  Created by Clifford Campo on 5/13/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//


#ifndef __SETUPFORMULTIFIT__
#define __SETUPFORMULTIFIT__

// #include "insertIBucketizedEnergyValuesIntoTable.hpp" //The Class for SetupForMultiFit is defined here.
//
//  2018-05-20T11:45:10 Name chaged to setupForMultifit.hpp from
//  insertIBucketizedEnergyValuesIntoTable.hpp
//  energyUsage
//
//  Created by Clifford Campo on 5/5/18, 2018-05-20
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
//  The class, SetupForMultiFit , inherits from the BaseClass
//

#include <iostream>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_statistics.h>
#include "baseClass.hpp" //Need this to resolve the inherited MakeConnectionString class.

// ¿enum bucketColNames is already defined in BaseClass, so why define it again? enum bucketColNames  {_avgtemp, _avgeu, _stddeveu, _mineu, _maxeu, _countu};
class SetupForMultiFit  {  //SetupForMultiFit inherits from BaseClass
private:
    size_t polynomialDegree; //From constructor parameter;
    size_t numberOfEntries;  // From count
    const double *arrayOfValues; //Pointer to array of current values, per enum, above, bucketColNames
    size_t &maxNumberOfRows = numberOfEntries;  // Use a c++-type reference rather than a C-type pointer.
    
public:
    BaseClass *bc;
    size_t onePlusPolynomialDegree;
    const  double initialValue = 1;
    double dblwork;
    double computedValue;
    double chiSquared;
    double *ptrChiSquared;
    double TSS; //total sum of squares
    double goodnessOfFit;
    double correlationCoefficient;
    double meanOfIndependentVariable;
    double meanOfDependentVariables;
    double traceOfCovarientMatrix;
    size_t loopCounter;
    gsl_multifit_linear_workspace *work;
    gsl_matrix *independentVariable;
    gsl_matrix *covarienceMatrix;
    gsl_vector *independentVariableTemp;
    gsl_vector *dependentVariable;
    gsl_vector *computedDependetVariable;
    gsl_vector *weights;
    gsl_vector *coefficients;
    int rc;
    //Declaration of various member functions
    ~SetupForMultiFit();
    SetupForMultiFit(BaseClass *, size_t,  size_t, double *)  ;
    int doMultiFit(void) ;
    
    void  setIntoMatrix(gsl_matrix *, int, int, double);
    
    void  setIntoVector(gsl_vector *, int, double);
    
    double  getFromMatrix(gsl_matrix *, int, int);
    
    double  getFromVector(gsl_vector *, int); 
    
    int  outputPolynomial(const char *, const char *);
    
    double  outputCovarianceMatrix(const char *, const char *);
    
    void captureIndependentVariableValues( int, double []);  //☞Note  that the `[]` is necessary in this declaration
    
    double computeTrace(double *, int);
    
    double  computeCorrelationBetweenIndependentAndDependentVariables(BaseClass *);
    
    double computeGoodnessOfResults (BaseClass *);
    //    SetupForMultiFit(size_t polynomialDegree, size_t count, double *);
    //    ~SetupForMultiFit();
    //void setIntoMatrix(gsl_matrix *, int, int, double);
    //void setIntoVector(gsl_vector *, int, double);
    //double getFromMatrix(gsl_matrix *, int, int);
    //double getFromVector(gsl_vector *, int);
    //void captureIndependentVariableValues( int, double * );//
    // int doMultiFit(void );
    //int outputPolynomial(const char *);
    //double outputCovarianceMatrix(const char *);  //Prints out covarience matrix and returns value of χ-squared.
};
#endif /* __SETUPFORMULTIFIT__ */
