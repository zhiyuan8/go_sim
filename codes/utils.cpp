#include <iostream>
#include <fstream>
#include <cmath> // use log
#include <vector>
#include <map>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions/chi_squared.hpp>  //quantile of chi-square
#include <algorithm>    // std::sort , *std::min_element, std::set_difference , std::max
#include <string>

#include "array_size.h"

using namespace std;

void Initialize_all(READIN &readin, INPUT &input, vector <double> &ttest, vector <double> &sims_test, vector <double> &geometric_p,
                    vector <bool> &sims_result, vector <bool> &geo_result, int col2)
{
    input.mu1 = readin.eff_size;
    input.alpha = readin.method_alpha;
    if (input.mu0 <= input.mu1)
        input.a = -1; // use area in right tail as p - value
    else
        input.a = 1;  // use area in left tail as p - value
    ttest.resize(input.col);
    sims_test.resize(col2);
    sims_result.resize(col2);
    geo_result.resize(col2);
    geometric_p.resize(col2);
}

double mean(double p[], int n)
{
    double temp = 0;
    for (int i = 0; i < n; i++)
    {
        temp = temp + p[i];
    }
    return( (temp+0.0) / n);

}

double deviation(double p[], double mean, int n)
{
    double temp = 0;
    for (int i = 0; i < n; i++)
    {
        temp = temp + (p[i] - mean)*(p[i] - mean);
    }
    return(sqrt((temp + 0.0) / (n - 1)));

}

double mean_vector(vector <double> data)
{
    int n = data.size();
    double temp = 0;
    for (int i = 0; i < n; i++)
    {
        temp = temp + data[i];
    }
    return( (temp+0.0) / n);

}

double deviation_vector(vector <double> data, double mean)
{
    int n = data.size();
    double temp = 0;
    for (int i = 0; i < n; i++)
    {
        temp = temp + (data[i] - mean)*(data[i] - mean);
    }
    return(sqrt((temp + 0.0) / (n - 1)));
}

int getKeyByValue(map <int, int> ID, int n)
{
    map<int, int>::iterator it;
    for (it = ID.begin(); it != ID.end(); it++)
    {
        if ( it->second == n )// value
        {
            return(it->first);   // key
        }
    }
    return(0);
}

int set_difference_number(vector <int> A, vector <int> B)
{
    int size_A = A.size();
    int size_B = B.size();
    vector<int> result( max(size_A, size_B) );//  must specify length
    vector<int>::iterator it;
    sort(A.begin(), A.end());     //  not address input here, so no worries for changing sequence of vector
    sort(B.begin(), B.end());   //  not address input here, so no worries for changing sequence of vector
    it = set_union(A.begin(), A.end(), B.begin(), B.end(), result.begin());
    return(it - result.begin());
}

int set_intersection_number(vector <int> A, vector <int> B)
{
    int size_A = A.size();
    int size_B = B.size();
    vector<int> result(max(size_A, size_B));//  must specify length
    vector<int>::iterator it;
    sort(A.begin(), A.end());     //  not address input here, so no worries for changing sequence of vector
    sort(B.begin(), B.end());   //  not address input here, so no worries for changing sequence of vector
    it = set_intersection(A.begin(), A.end(), B.begin(), B.end(), result.begin());
    return(it - result.begin());
}

void set_difference_vector(vector <int> &result, vector <int> A, vector <int> B)
{
    int size_A = A.size();
    int size_B = B.size();
    if (int( result.size() ) < max(size_A, size_B))
    {
        result.resize( max(size_A, size_B) );//  must specify length
    }
    vector<int>::iterator it;
    sort(A.begin(), A.end());     //  not address input here, so no worries for changing sequence of vector
    sort(B.begin(), B.end());   //  not address input here, so no worries for changing sequence of vector
    it = set_difference(A.begin(), A.end(), B.begin(), B.end(), result.begin());
    result.resize(it - result.begin());
}

double comb(int n, int k)
{
    if (k > n) return 0;
    if (k * 2 > n) k = n - k;
    if (k == 0) return 1;

    double result = n;
    for (int i = 2; i <= k; ++i)
    {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

void show_final_result(OUTPUT hyper, OUTPUT sims, int n_reps, int position, int dec)
{
    if (dec == 1)
    {
        double FDR = 0;
        double mean_beta = 0;
        for (int i = (position-n_reps); i < position; i++)
        {
            FDR = hyper.FDP[i] + FDR;
            mean_beta = hyper.power[i] + mean_beta;
        }
        cout << " competitive test   FDP= " << FDR/n_reps << " power= " << mean_beta/n_reps << endl;
        FDR = 0;
        mean_beta = 0;
        for (int i = (position - n_reps); i < position; i++)
        {
            FDR = sims.FDP[i] + FDR;
            mean_beta = sims.power[i] + mean_beta;
        }
        cout << " self-contained test   FDR= " << FDR/n_reps << " power= " << mean_beta/n_reps << endl;
    }
}\

void write_out_FDP_Power(OUTPUT &output, string path, int row)
{
    ofstream c_out;
    c_out.open(path.c_str(), ios::app);
    for (int i = 0; i<row; i++)
    {
        c_out <<output.n_control[i]<<','<< output.experiment[i]<<','<<output.FDP[i]<<','<<output.power[i]<<endl;
    }
    c_out.close(); //close the file
}

void write_out_node_specific_power(double **matrix, string path, int n_reps, int row, int col)
{
    ofstream c_out;
    c_out.open(path.c_str(), ios::app);
    for (int i = 0; i<row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            c_out << (matrix[i][j]+0.0)/(n_reps+0.0) << ','; // make sure nominator and denominator are double
        }
        c_out << endl;
    }
    c_out.close(); //close the file
}
