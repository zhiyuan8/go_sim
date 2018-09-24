#ifndef IO_H
#define IO_H

#include <vector>
#include <map>
#include <string>

#include "array_size.h" //refer to constant
using namespace std;

void read_in_json(vector< vector<int> >&json, int &json_vector, vector <int> &json_length);
void read_in_json2(READIN &readin, DECISION & decision);
void create_ID(vector< vector<int> >&json, map<int, int> &ID, int &index, int json_vector, vector <int> json_length, bool dec);
void read_in_json3(vector<bool> &json_truth, vector<bool> &json_truth2, vector< vector<int>> json_data,
    map<int, int> ID, INPUT &input, int json_vector, vector <int> json_length, READIN readin);
string output_path ();
void write_out_matrix(double ** data1_temp, double ** data2_temp, int row, int col, int rep, string path);
void write_out_rej(vector <bool> sims_result, vector <bool> geo_result, int row, int rep, string path);
void write_out_results(OUTPUT sims, OUTPUT hyper, double **sims_node, double **hyper_node,
     int json_vector,int n_regimes,int n_reps,int position, string path);

#endif
