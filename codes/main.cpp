#include <iostream>
#include <time.h> //show calculation time
#include <chrono>  // for high_resolution_clock
#include <map>
#include <vector>
#include <string>

#include "io.h"
#include "array_size.h"
#include "main.h"
#include "generate_data.h"
#include "utils.h"
#include "tests.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "The path to read in json files is:  " << argv[1] << "\n";
    path_in = argv[1]; // get path where we readin data
    cout << "The path to write result csv files is:  " << argv[2] << "\n";
    path_out = argv[2]; // get path where we store final results
    cout << "The seed used in generating data is:  " << abs ( atoi( argv[3]) ) << "\n";
    input.seed = abs ( atoi( argv[3]) );

    // 1. read in data, intialize all vectors
    startTime = clock(); // begin documenting time
    read_in_json(path_in, json, json_vector, json_length); // get the ID data in json file
    read_in_json2(path_in, readin, decision); // read parameters for simulations
    if ( readin.n_reps  >100 )// if data is too large, stop program
    {
        cout<<"please do not output data and delete 'Data' in json file, or decrease 'n_reps' to less than 100" << endl;
        return 0;
    }
    create_ID(json, ID, input.col, json_vector, json_length, false);// build the map. If true, then show me all ID
    Initialize_all(readin, input, pValue_ttest, sims_test_statistic, geometric_p_value, sims_result, geo_result, json_vector);// initialize all vectors
    read_in_json3(path_in, self_contained, competitive, json, ID, input, json_vector, json_length, readin);// get self_contained and competitive non-null


    // 2. Initialize all dynamic arrays
    sims.FDP = new double [readin.n_regimes*readin.n_reps];
    sims.power = new double [readin.n_regimes*readin.n_reps];
    sims.n_control = new int [readin.n_regimes*readin.n_reps];
    sims.experiment = new int [readin.n_regimes*readin.n_reps];
    hyper.FDP = new double [readin.n_regimes*readin.n_reps];
    hyper.power = new double [readin.n_regimes*readin.n_reps];
    hyper.n_control = new int [readin.n_regimes*readin.n_reps];
    hyper.experiment = new int [readin.n_regimes*readin.n_reps];
    double *data1 = new double [readin.max_n];// normal random variable 1D array for one row of control sample
    double *data2 = new double [readin.max_n];// normal random variable 1D array for one row of case sample
    double **data1_temp = new double*[ readin.max_n ]; // normal random variable matrix for control sample, for output file
    double **data2_temp = new double*[ readin.max_n ]; // normal random variable matrix for case sample, for output file
    for (int i=0; i<int (readin.max_n); i++)
    {
        data1_temp[i] = new double [input.col]; // normal random variable matrix size = (readin.max_n * input.col)
        data2_temp[i] = new double [input.col]; // normal random variable matrix size = (readin.max_n * input.col)
    }
    double **sims_node_result = new double* [json_vector];// sims node specific power for all regimes
    double **hyper_node_result = new double* [json_vector];// hypergeometric node specific power for all regimes
    for (int i=0; i<json_vector; i++)
    {
        sims_node_result[i] = new double [readin.n_regimes];//node specific power size = (json_vector * readin.n_regimes)
        hyper_node_result[i] = new double [readin.n_regimes];//node specific power size = (json_vector * readin.n_regimes)
        for (int j=0; j<readin.n_regimes; j++)
        {
            sims_node_result[i][j]=0; // sims node specific power will be averaged so it need to be initialized
            hyper_node_result[i][j]=0;// sims node specific power will be averaged so it need to be initialized
        }
    }

    // 3. do simulations and tests
    for (int i = 0; i < readin.n_regimes; i++)
    {
        input.row_reject = readin.min_n + int((readin.max_n - readin.min_n + 0.0) / (readin.n_regimes - 1 + 0.0) *i); // linspace
        cout << "\n" << "regime " << i + 1 << endl; // show current progress
        for (int j = 0; j < readin.n_reps; j++) // repeat tests
        {
            position++;
            input.seed = input.seed + (j + 1) * 100; // use different seed in each loop
            start = std::chrono::high_resolution_clock::now();
            if (decision.matrix) // generate p value from t-test and temp it
            {
                generateY(data1, data2, data1_temp, data2_temp, input, pValue_ttest);
            }
            else// generate p value from t-test without temping it
            {
                generateN(data1,data2,input,pValue_ttest);
            }
            finish = std::chrono::high_resolution_clock::now();
            elapsed = finish - start;
            generate_time = generate_time + elapsed.count();

            // hypergeometric test for the competitive null
            start = std::chrono::high_resolution_clock::now();
            mul_bonf(pValue_ttest, Bonf_reject, input, ID, 0);
            gl_geometric(geometric_p_value, Bonf_reject, json, input.col, 0); //fisher's exact test. if 1, show  result
            mul_BH(geo_result, geometric_p_value, input.alpha, hyper_node_result, i, 0);// BH multiple testing. if 1, show result.
            finish = std::chrono::high_resolution_clock::now();
            elapsed = finish - start;
            hyper_time = hyper_time + elapsed.count();

            // sims test for the self-contained null
            start = std::chrono::high_resolution_clock::now();
            gl_sims(pValue_ttest, sims_test_statistic, json, ID, 0); // get the test statistic from sims tes
            mul_BH(sims_result, sims_test_statistic, input.alpha, sims_node_result, i, 0);// BH multiple testing. if 1, show result.
            finish = std::chrono::high_resolution_clock::now();
            elapsed = finish - start;
            sims_time = sims_time + elapsed.count();

            // do confusion matrix
            confusion(hyper, geo_result, competitive, input.row_reject, j, position, 0); // get FDP & beta of hypergeometric test.  if 1, show result.
            confusion(sims, sims_result, self_contained, input.row_reject, j, position, 0); // get FDP & beta of sims test.  if 1, show result.
            start = std::chrono::high_resolution_clock::now();
            if (decision.matrix)
            {
                write_out_matrix(data1_temp, data2_temp, input.row_reject, input.col, j+1, path_out);
            }
            if (decision.NumRej)
            {
                write_out_rej(sims_result, geo_result, input.row_reject, j+1, path_out);
            }
            finish = std::chrono::high_resolution_clock::now();
            elapsed = finish - start;
            save_time = save_time + elapsed.count();
        }

        show_final_result(hyper, sims, readin.n_reps, position, 1);// show FDR and mean beta of two tests. if 1, show result.
    }
    //write out power, FDP and node-specific power
    write_out_results(sims,hyper,sims_node_result,hyper_node_result,json_vector,readin.n_regimes,readin.n_reps,position, path_out);

    //4. delete dynamic array and show total time
    for (int i=0; i<json_vector; i++)
    {
        delete[]sims_node_result[i];
        delete[]hyper_node_result[i];
    }
    delete[]sims_node_result;
    delete[]hyper_node_result;
    delete[]data1;
    delete[]data2;
    for (int i=0; i<readin.max_n; i++)
    {
        delete [] data1_temp[i];
        delete [] data2_temp[i];
    }
    delete [] data1_temp;
    delete [] data2_temp;
    delete [] sims.FDP;
    delete [] sims.power;
    delete [] sims.n_control;
    delete [] sims.experiment;
    delete [] hyper.FDP;
    delete [] hyper.power;
    delete [] hyper.n_control;
    delete [] hyper.experiment;

    endTime = clock();
    cout << "generate_time + t_test_time : " << generate_time <<endl;
    cout << "save_time : " << save_time <<endl;
    cout << "hyper_time : " << hyper_time << endl;
    cout << "sims_time : " << sims_time << endl;
    cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}
