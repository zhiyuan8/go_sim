#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>
#include <string>
#include <map>

#include "array_size.h" //refer to constant
#include "utils.h" // use set union

using namespace std;
namespace pt = boost::property_tree;

void read_in_json(vector< vector<int> >&json, int &json_vector, vector <int> &json_length)
{
    cout<< "please input the path and file name for all gene IDs"<< endl;
    cout<< "(example: /home/zhiyuan/Desktop/Json_files/meta_gogene_map.json)"<<endl;
    string filename;
    bool dec=false;
    while (dec==false)
    {
        cin >> filename;
        ifstream inFile(filename);
        if (inFile.fail())
        {
            cout << "Could Not Find the File, input path again \n";
        }
        else
        {
            cout << "The path is valid, continue to next step\n";
            dec =true;
            cout << endl;
        }
    }
    pt::ptree pt;
    pt::read_json(filename, pt);
    for (pt::ptree::value_type &row : pt)// read in # of vector
    {
        int y = 0;// start from first ID in current gene
        for (pt::ptree::value_type &cell : row.second)
        {
            y++; // move to next ID
        }
        json_length.push_back(y); // temp number of ID in one gene
        json_vector++; // move to next vector
    }
    json.resize(json_vector);// read data into json_data 2D vector
    int x = 0;// start from first gene
    for (pt::ptree::value_type &row : pt)
    {
        json[x].resize(json_length[x]);
        int y = 0; // start from first ID in current gene
        for (pt::ptree::value_type &cell : row.second)
        {
            json[x][y] = cell.second.get_value<int>();
            y++;
        }
        x++; // move to next vector
    }
}

void read_in_json2(READIN &readin, DECISION & decision)
{
    cout<< "please input the path and file name for parameters that decide your experiments"<<endl;
    cout<< "(example: /home/zhiyuan/Desktop/Json_files/meta_restore_params.json)"<<endl;
    string filename;
    string temp;
    bool dec=false;
    while (dec==false)
    {
        cin >> filename;
        ifstream inFile(filename);
        if (inFile.fail())
        {
            cout << "Could Not Find the File, input path again \n";
        }
        else
        {
            cout << "The path is valid, continue to next step\n";
            dec =true;
            cout << endl;
        }
    }
    pt::ptree pt;
    pt::read_json(filename, pt);
    readin.min_node_size = pt.get<int>("context_params.min_node_size", 0);
    readin.max_node_size = pt.get<int>("context_params.max_node_size", 0);
    for (pt::ptree::value_type &alphas : pt.get_child("test_params.method_alpha"))
    {
        readin.method_alpha = alphas.second.get_value<double>();
    }
    readin.n_regimes = pt.get<int>("oneway_params.n_regimes", 0);
    readin.n_reps = pt.get<int>("oneway_params.n_reps", 0);
    readin.min_n = pt.get<int>("oneway_params.min_n", 0);
    readin.max_n = pt.get<int>("oneway_params.max_n", 0);
    readin.eff_size = pt.get<double>("oneway_params.eff_size", 0);
    cout<<"FDP and power of sims and hypergemetric tests will be output\n";
    cout<<"node-specific power of sims and hypergemetric tests will be output\n";
    for ( pt::ptree::value_type &cell : pt.get_child("test_params.report_metrics") )
    {
        temp = cell.second.get_value<string>();
        if (temp == "NumRej")
        {
            decision.NumRej=true;
            cout<<"Results of Bejamini Hochberg multiple test will be output\n";
        }
        if (temp == "Data")
        {
            decision.matrix=true;
            cout<<"The random normal variable matrix used in t tests will be output\n";
        }
    }
}

void create_ID(vector< vector<int> >&json, map<int, int> &ID, int &index, int json_vector, vector <int> json_length, bool dec)
{
    int ID_size = 0;
    for (int i = 0; i < json_vector; i++)
    {
        for (int j = 0; j < json_length[i]; j++)
        {
            ID_size = ID.size(); // temp the size of ID before insert
            ID.insert(pair<int, int>(json[i][j], index));
            if (int(ID.size()) == ID_size + 1) //check if size of ID is +1
            {
                index++;
            }
        }
    }
    if (dec)
    {
        map<int, int>::const_iterator it; // it gives output of map
        for (it = ID.begin(); it != ID.end(); ++it)
        {
            cout << it->first << "=" << it->second << endl;
        }
    }
}

void read_in_json3(vector<bool> &json_truth, vector<bool> &json_truth2, vector< vector<int>> json_data,
    map<int, int> ID, INPUT &input,int json_vector, vector <int> json_length, READIN readin)
{
    pt::ptree pt;
    cout<< "please input the path and file name for non-null gene ID"<<endl;
    cout<< "(example: /home/zhiyuan/Desktop/Json_files/meta_nonnull_gene_ids.json)"<<endl;
    string filename;
    bool dec=false;
    while (dec==false)
    {
        cin >> filename;
        ifstream inFile(filename);
        if (inFile.fail())
        {
            cout << "Could Not Find the File, input path again \n";
        }
        else
        {
            cout << "The path is valid, continue to next step\n";
            dec =true;
            cout << endl;
        }
    }
    pt::read_json(filename, pt);
    // To store key of rejection
    for (pt::ptree::value_type &keys : pt)
    {
        input.key_reject.push_back(keys.second.get_value<int>()); // must use push_back
    }
    // According to key, find the index of rejected columns
    int imax = input.key_reject.size();
    input.if_reject.resize(input.col); // initialize size of input.if_reject
    for (int i = 0; i < input.col; i++)
    {
        input.if_reject[i] = false; // initialize all columns as null
    }
    input.col_reject.resize(imax);// quicker to use "resize + loop" than to use push_back
    for (int i = 0; i < imax; i++)
    {
        input.col_reject[i] = ID[input.key_reject[i]];
        input.if_reject[input.col_reject[i]] = true; // some columns have non-null
    }
    // prepare for getting ground_truth vector
    int A1 = imax;
    json_truth.resize(json_vector); // define the length of self_contained ground_truth
    json_truth2.resize(json_vector); // define the length of competitive ground_truth
    double G = input.col; //the number of different elements in json file
    // now get the json_truth (self_contained), it is 1 if there is at least one element from rejection
    for (int j = 0; j < json_vector; j++)
    {
        json_truth[j] = false;
        for (int k = 0; k < json_length[j]; k++) // check each element one by one
        {
            for (int l = 0; l < A1; l++) // compare k th data with l th rejection element
            {
                if (ID[json_data[j][k]] == input.col_reject[l])
                {
                    json_truth[j] = true;
                    break;
                }
            }
            if (json_truth[j] == true)
            {
                break; // I have found a rejection, I don't need to scan the rest elements in this vector
            }
        }
    }
    // now get json_truth2, get 0 by comparing double(|A1^Gi| / |A1|) < double (|Gi|/|G|)
    for (int j = 0; j < json_vector; j++)
    {
        int Gi =json_length[j];
        vector <int> Gi_vector = json_data[j];
        int A1Gi = set_intersection_number(Gi_vector, input.key_reject);
        if (((A1Gi + 0.0) / (A1 + 0.0)) <= ((Gi + 0.0) / (G + 0.0)))
        {
            json_truth2[j] = false;
        }
        else
        {
            json_truth2[j] = true;
        }
    }    
    cout << "There are  " << input.col << " non-repeated index of genes in system" << endl;// check output
    cout << "There are " << json_vector << " genes in system" << endl;// check output
    cout << "n_regimes= " << readin.n_regimes << " n_reps= " << readin.n_reps << endl;// check output

    cout<< "\n please input seed for random data generation (Default value = 100)"<<endl;
    double seed_in;
    cin >> seed_in;
    input.seed = abs ( int (seed_in) );
    cout<<endl;
}

string output_path ()
{
    cout<< "\n please input the path where you will save your files\n" ;
    cout<< "(example: /home/zhiyuan/Desktop/Json_files/result) \n";
    string filename;
    cin >> filename;
    return (filename);
}

void write_out_matrix(double ** data1_temp, double ** data2_temp, int row, int col, int rep, string path)
{
    ofstream c_out;
    string s = to_string(row);
    string X_path = path +"/control_n_" + s + ".csv";
    string Y_path = path +"/case_n_" + s + ".csv";
    c_out.open(X_path.c_str(), ios::app);
    c_out<<"experiment= "<< rep << endl;
    for (int j=0; j<row; j++)
    {
        for (int k=0; k<col; k++)
        {
            c_out << data1_temp [j][k] <<',';
        }
        c_out<<endl;
    }
    c_out.close(); //close the file
    c_out.open(Y_path.c_str(), ios::app);
    c_out<<"experiment= "<< rep << endl;
    for (int j=0; j<row; j++)
    {
        for (int k=0; k<col; k++)
        {
            c_out << data2_temp [j][k] <<',';
        }
        c_out<<endl;
    }
    c_out.close(); //close the file
}


void write_out_rej(vector <bool> sims_result, vector <bool> geo_result, int row, int rep, string path)
{
    ofstream c_out;
    string sims_path = path +"/sims_Numrej.csv";
    string geo_path = path +"/hypergeometric_Numrej.csv";
    c_out.open(sims_path.c_str(), ios::app);
    c_out<< "n control=" << row << ','<< "repeat=" << rep << ',' << "sims test result:" << ',';
    for (int i = 0; i<row; i++)
    {
        c_out << sims_result[i] << ',';
    }
    c_out<<endl;
    c_out.close(); //close the file
    c_out.open(geo_path.c_str(), ios::app);
    c_out<< "n control=" << row << ','<< "repeat=" << rep << ',' << "hypergeometric test result:" << ',';
    for (int i = 0; i<row; i++)
    {
       c_out << geo_result[i] << ',';
    }
    c_out<<endl;
    c_out.close(); //close the file
}

void write_out_results(OUTPUT sims, OUTPUT hyper, double **sims_node, double **hyper_node,
     int json_vector,int n_regimes,int n_reps,int position, string path)
{
    string sims_path = path + "/sims_test_result.csv";
    string hyper_path = path + "/hypergeometric_test_result.csv";
    string sims_node_path = path + "/sims_node_specific_power.csv";
    string hyper_node_path = path + "/hypergeometric_node_specific_power.csv";
    //First write out FDP and power
    write_out_FDP_Power(sims,sims_path,position); //write out regime, experiment,FDP,power
    write_out_FDP_Power(hyper,hyper_path,position); //write out regime, experiment,FDP,power
    // Then write out node-specific power
    write_out_node_specific_power(sims_node,sims_node_path,n_reps,json_vector,n_regimes);//write out averaged node-specific power
    write_out_node_specific_power(hyper_node,hyper_node_path,n_reps,json_vector,n_regimes);//write out averaged node-specific power
}
