# go_sim

Self-contained and competitive tests on simulated data. The two tests are used to detect non-null gene.

### System Requirement
(1) g++ >= 4.6. (infomation from https://stackoverflow.com/questions/7482026/can-i-use-the-latest-features-of-c11-in-xcode-4-or-osx-lion) The following codes help to check version.
```
g++ --version
```
Foe example, in my computer it says ```g++ (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609```. If g++ does not meet requirement, see this link for updating: https://askubuntu.com/questions/618474/how-to-install-the-latest-gcurrently-5-1-in-ubuntucurrently-14-04 <br/>
(2) Boost >= 1.35, because in Boost 1.35, distribution libraries are added. The following codes help to check version
```
dpkg -s libboost-dev | grep 'Version'
```
For example, in my computer it says ```Version: 1.58.0.1ubuntu1``` which means my boost version is 1.58. If boost version does not meet requirement, see this link for updating: https://codeyarns.com/2013/12/27/how-to-upgrade-the-boost-library-on-ubuntu/

### Preparation (Install boost library)

In previous step, if boost cannot be found and you have the root access, you can easily install it via following commands in terminal:
```
sudo apt-get install libboost-all-dev
```
If boost library is not found, and you don't have the root access, you can find install instructions at http://masumhabib.com/blog/how-to-install-the-boost-library-with-mpi-and-without-root-access/  If you still cannot install it, contact me via zhiyuan.li1995@hotmail.com <br/>

### Installation 

Use ```cd``` to locate at the place where you will download the codes. 
If you clone the codes via HTTP.
```
git clone https://github.com/zhiyuan8/go_sim.git
```
Otherwise, if you clone the codes via SSH.
```
git clone git@github.com:zhiyuan8/go_sim.git 
```
Link boost library and complie all cpp files. *path/boost_version* is the location where you download and extract boost library folder. For me, it is */home/zhiyuan/Downloads/boost_1_68_0*. If you forget where you downloaded, try ```whereis```, ```locate``` and ```find```.  
```
g++ -I *path/boost_version* main.cpp generate_data.cpp io.cpp tests.cpp utils.cpp -o GO_DAG -std=c++11
```
In this way, ```GO_DAG``` is created. 	This process takes a while.

## Customize your tests
The ```test_data``` folder contains datasets as a demo. You can replace them with your json files, but remember to keep the file name as the same.  <br/>
Open the file ```parameter.json```, in "report_metrics", there are four choices (1)"FDP"(2)"Power"(3)"NumRej"(4)"Data". They refer to (1) False Discovery Rate of two tests (2) Power of two tests (3) Node-specific power (4) original normal random variables in Self-contained and competitive tests. (1) and (2) are output mandatorily, and (3) and (4) are output if you type "NumRej" and "Data" in that json file. <br/>
Also in the file ```parameter.json```, you can change "method_alpha" to other criteria, such as 0.05, and you can change  "eff_size" which means the effective size of non-null random variable ( For example, eff_size=0.5, non-null data ~ N(0.5, 1) ). <br/>
In  "oneway_params", "n_rep" means number of repeated experiments in each regime. "n_regimes" means number of regimes. For example,  {"n_regimes": 12, "n_reps": 100, "min_n": 10, "max_n": 120} means "There are 12 regimes, each one will be repeated 100 times with different randomness, and in each regime, number of cases = number of control = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120}"

## Running the tests

Finally, input codes in terminal.
```
./codes/GO_DAG      ./test_data      ./result  100
```
The first argument enables terminal to run the code. The second and third arguments are paths for reading in and writing out files. The fourth number is a seed used in generating data. If you keep the seed number unchanged, you will get same outputs.
noted, if you run my codes for a second time, the output in your first test will not be covered, and new output will be written at the end of previous results. So I suggest you copy the output .csv files in another path and delete them.

### Result plotting
Go to ```drwa_R``` folder, open ```result_plot.R``` file. Remember to change my read in path to the path where you store your data. For Self-contained (Sims) test, you could plot the following figures.
![Test Image 1](https://github.com/zhiyuan8/go_sim/blob/master/figures/Rplot01.png)
![Test Image 2](https://github.com/zhiyuan8/go_sim/blob/master/figures/Rplot03.png)
![Test Image 3](https://github.com/zhiyuan8/go_sim/blob/master/figures/Rplot05.png)
    

## Related publication


## Authors

* **Zhiyuan Jim Li** - *Initial work* - contact: zhiyuan.li1995@hotmail.com


