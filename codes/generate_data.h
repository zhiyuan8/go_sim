#ifndef GENERATE_DATA_H
#define GENERATE_DATA_H

#include "array_size.h"

void generateN(double *data1, double *data2, INPUT input, vector <double> &pValue_ttest);
void generateY(double *data1, double *data2, double **data1_temp, double **data2_temp,
                INPUT input, vector <double> &pValue_ttest);

#endif
