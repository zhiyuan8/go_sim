#ifndef ARRAY_SIZE_H
#define ARRAY_SIZE_H

#include <vector>
using namespace std;

struct INPUT
{
    double alpha; int row; int col; double mu0; double sig0;double mu1; double sig1;
    int row_reject; int seed; int a;
    vector <int> col_reject; // it saves the index of columns that is non-null
    vector <int> key_reject; // it saves the key of columns that is non-null
    vector <bool> if_reject; // it saves if this column is null/non-null
};

struct OUTPUT
{
    int *n_control;
    int *experiment;
    double *FDP;
    double *power;
};

struct READIN
{
    int min_node_size; int max_node_size;
    double method_alpha;
    int n_regimes; int n_reps; int min_n;
    int max_n; double eff_size;
};

struct DECISION
{
    bool NumRej; // if true, output all rejected node;
    bool matrix; // if true, output all normal RV;
};

#endif
