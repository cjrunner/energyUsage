//
//  setupForMultiFit.cpp
//  energyUsageWithoutThe_PQXX_Bullshit
//
//  Created by Clifford Campo on 5/20/18.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "setupForMultiFit.hpp"
#include "myPrototypes.hpp"
#include "baseClass.hpp"
const std::string currentDateTime(BaseClass *);
SetupForMultiFit::~SetupForMultiFit() {  //Destructor
    gsl_matrix_free(this->independentVariable);
    gsl_matrix_free(this->covarienceMatrix);
    gsl_matrix_set_zero(this->covarienceMatrix);
    gsl_vector_free(this->dependentVariable);
    gsl_vector_free(this->weights);
    gsl_vector_free(this->coefficients);
    gsl_multifit_linear_free(this->work);
    gsl_vector_free(this->computedDependetVariable);
    
}
SetupForMultiFit::SetupForMultiFit(BaseClass *bC, size_t polynomialDegree,  size_t count, double *aOV)   { //Constructor
    this->bc = bC;
    this->ptrChiSquared = &chiSquared;
    this->arrayOfValues = aOV;
    this->polynomialDegree = polynomialDegree;
    this->onePlusPolynomialDegree = 1+polynomialDegree;
    this->numberOfEntries = count;                         //Number of rows
    this->independentVariable = gsl_matrix_alloc(this->numberOfEntries, this->onePlusPolynomialDegree );
    gsl_matrix_set_zero(this->independentVariable);
    this->covarienceMatrix = gsl_matrix_alloc(this->onePlusPolynomialDegree, this->onePlusPolynomialDegree);
    //    gsl_matrix_set_zero(this->covarienceMatrix);
    this->dependentVariable = gsl_vector_alloc(this->numberOfEntries);
    //   gsl_vector_set_zero(this->dependentVariable);
    this->computedDependetVariable = gsl_vector_alloc(this->numberOfEntries);
    this->weights = gsl_vector_alloc(this->numberOfEntries);
    //    gsl_vector_set_zero(this->weights);
    this->coefficients = gsl_vector_alloc(this->onePlusPolynomialDegree);
    //    gsl_vector_set_zero(this->coefficients);
    this->work = gsl_multifit_linear_alloc(this->numberOfEntries, this->onePlusPolynomialDegree);
}
int    SetupForMultiFit::doMultiFit(void) {
    this->rc = gsl_multifit_wlinear(this->independentVariable, \
                                    this->weights, \
                                    this->dependentVariable, \
                                    this->coefficients, \
                                    this->covarienceMatrix, \
                                    this->ptrChiSquared, \
                                    this->work);
    return this->rc;
}
void  SetupForMultiFit::setIntoMatrix(gsl_matrix *whichMatrix, int row, int column, double value) {
    gsl_matrix_set(whichMatrix, row, column, value);
}
void  SetupForMultiFit::setIntoVector(gsl_vector *whichVector, int row, double value) {
    gsl_vector_set(whichVector, row, value);
}
double  SetupForMultiFit::getFromMatrix(gsl_matrix *whichMatrix, int row, int column) {
    return (gsl_matrix_get(whichMatrix, row, column));
}
double  SetupForMultiFit::getFromVector(gsl_vector *whichVector, int row) { 
    return ( gsl_vector_get(whichVector, row) );
}
int  SetupForMultiFit::outputPolynomial(const char *leadOffString, const char *trailingString) {
    *this->bc->outstring << currentDateTime(this->bc) << "\t" << leadOffString;
    for (int coeff = 0; coeff < this->coefficients->size ; coeff++) {
        if (coeff > 0) {
            *this->bc->outstring << " + " << gsl_vector_get(this->coefficients, coeff) << "*T^" << coeff;
        } else {
            *this->bc->outstring << gsl_vector_get(this->coefficients, coeff) ;
        }
    }
    *this->bc->outstring << trailingString << "\n";
    return 0;
}
double  SetupForMultiFit::outputCovarianceMatrix(const char *leadOffString, const char *trailingString) {
    // unused    double firstC = *this->ptrChiSquared;
    for (int row = 0; row < this->covarienceMatrix->size1; row++) {
        *this->bc->outstring << currentDateTime(this->bc) << "\t" << leadOffString;
        for (int col = 0; col < this->covarienceMatrix->size2; col++) {
            *this->bc->outstring  << *(this->covarienceMatrix->data + this->covarienceMatrix->size1*row + col);
            if (col < this->covarienceMatrix->size1 - 1) {
                *this->bc->outstring << ", "; //Output a comma to separate covarient values
            } else {
                //Output closed brackets if we've just output the rightmost covarient value for this row.
                *this->bc->outstring << "]\t " << trailingString << "\n";
            }
        }  //End of inner for loop (col)
    } // End of outter for loop (row)
    return *this->ptrChiSquared;
}



double  SetupForMultiFit::computeCorrelationBetweenIndependentAndDependentVariables (BaseClass *bc) {
   
    double trialComputedDependentVariable=0;
    const double A = gsl_vector_get(this->coefficients, 0);
    const double B = gsl_vector_get(this->coefficients, 1);
    const double C = gsl_vector_get(this->coefficients, 2);
    const double D = gsl_vector_get(this->coefficients, 3);
    double T;
    for (int i=0; i< this->numberOfEntries; i++) {
        T = bc->BaseClass::indVarArray[i];
        trialComputedDependentVariable = A + (B  +  (C  + D *T )* T) * T;
        bc->BaseClass::depVarArray[i]= trialComputedDependentVariable;
    }
    this->meanOfIndependentVariable = gsl_stats_mean(bc->BaseClass::indVarArray, 1, this->numberOfEntries);
    this->meanOfDependentVariables = gsl_stats_mean(bc->BaseClass::depVarArray, 1, this->numberOfEntries);
    this->correlationCoefficient = gsl_stats_correlation(bc->BaseClass::indVarArray, 1, bc->BaseClass::depVarArray, 1, this->numberOfEntries );

    return this->correlationCoefficient ;
}

double  SetupForMultiFit::computeGoodnessOfResults (BaseClass *bc) {
    //computeGoodnessOfResults defines GoodnessOfResults as the square of the accumulated normalized difference between the expected (modeled)
    //and observed dependent variables ( ∑(((modeledResultForEachIndependentVariable - eachObservedDependentVariable)/modeledResultForEachIndependentVariable))^2 ).
    //Return the square root of the sum of the squares of the percent difference between the expected (modeled) and observed dependent variables.
    int i=0;
    double trialComputedDependentVariable=0;
    double accumulatedVarience2 = 0;
    double delta=0;
    double deltaNormalized=0;
    const double A = gsl_vector_get(this->coefficients, 0);
    const double B = gsl_vector_get(this->coefficients, 1);
    const double C = gsl_vector_get(this->coefficients, 2);
    const double D = gsl_vector_get(this->coefficients, 3);
    double T;
    for (i=0; i<  bc->grc; i++) {
        T = *(bc->BaseClass::ptrIndVariableArray + i);
        trialComputedDependentVariable = A + (B  +  (C  + D *T )* T) * T;
        *(bc->BaseClass::ptrExpDepVarArray + i)= trialComputedDependentVariable;
        delta = (*(bc->BaseClass::ptrExpDepVarArray + i) - *(bc->BaseClass::ptrObsDepVarArray + i));
        *(bc->BaseClass::ptrExpMinusObsDepVarArray + i) = delta;
        deltaNormalized =  delta/(*(bc->BaseClass::ptrExpDepVarArray + i) );
        accumulatedVarience2 += ( deltaNormalized *  deltaNormalized ); //Accumulate the square of normalized varience.
    }
    
    return sqrt(accumulatedVarience2) ; //Return the square root of the sum of the squares of the normalized varience
}
double SetupForMultiFit::computeTrace(double *someMatrix, int onePlusPolynomialDegree) {
    int &matrixSize = onePlusPolynomialDegree;
    this->traceOfCovarientMatrix = 0;
    for (this->loopCounter =0; this->loopCounter < matrixSize; this->loopCounter++  ) {
        this->traceOfCovarientMatrix += *(someMatrix + (matrixSize + 1) * this->loopCounter ); //∑ along the diagonal of the covarient matrix.
    }
    return this->traceOfCovarientMatrix; 
}
void  SetupForMultiFit::captureIndependentVariableValues( int theRow, double  valueArray[]) {    
    this->dblwork = *(valueArray +  _stddeveu);
    if (this->bc->debugFlags.debug2) {
        std::cout << theRow << ". Weights, or *(valueArray +  _stddeveu), Looks like: " << *(valueArray +  _stddeveu) << std::endl;
    }
    gsl_vector_set(this->weights, theRow, this->dblwork);
    this->dblwork = *(valueArray +  _avgeu);
    gsl_vector_set(this->dependentVariable, theRow, this->dblwork );
    this->computedValue = this->initialValue;
    for(this->loopCounter = 0; this->loopCounter< this->onePlusPolynomialDegree; this->loopCounter++)  {   // Note that this->loopCounter Serves as the column counter, too.
        gsl_matrix_set(this->independentVariable, theRow, this->loopCounter, this->computedValue);
        this->computedValue *= valueArray[_avgtemp]; //For each column in this current row, place progressively increasing powers of the independent variable
    }
    
}

