﻿# go_sim

Self-contained and competitive tests on simulated data. The two tests are used to detect non-null gene.

### System Requirement
g++ >= 4.2.1. (double check it! it is said g++ contains C++11 lib)
https://stackoverflow.com/questions/7482026/can-i-use-the-latest-features-of-c11-in-xcode-4-or-osx-lion

The following codes help to check version
```
g++ --version
```

### Installation

In terminal, the followings enable you to install boost library which will be needed in calculating chi-square quantile and generating normal random variable.
```
sudo apt-get install libboost-all-dev
```

Download the code to a location, for instance, your desktop. Next, extract zip file. (I cannot explain why 'git clone --recursive https://github.com/zhiyuan8/go_sim.git' cannot copy all files). 
Then, use 'cd' to locate at the codes ```folder```. Here is a line of examplary code.
```
cd Desktop/go_sim-master/codes/
```
Furthermore, complie all cpp files.
```
g++ main.cpp generate_data.cpp io.cpp tests.cpp utils.cpp -o GO_DAG -std=c++11

```
In this way, ```GO_DAG``` is created. 	This process takes a while.

## Running the tests

The ```test_data``` folder contains datasets as a demo. You can replace them with your json files, but remember to keep the file name as the same. Firstly, use ```cd``` in terminal to locate at ```codes``` folder, and examplary code has been given before. Secondly, create a ```result``` folder to store outputs, and then return to ```codes``` folder.
```
cd ..
mkdir result
cd ..
cd codes
```
Finally, input codes in terminal.
```
./GO_DAG      /home/zhiyuan/Desktop/Json_files/data      /home/zhiyuan/Desktop/Json_files/result  100
```
The first argument enables terminal to run the code. The second and third arguments are paths for reading in and writing out files. The fourth number is a seed used in generating data. If you keep the seed number unchanged, you will get same outputs.

## Related publication


## Authors

* **Zhiyuan Jim Li** - *Initial work* - contact: zhiyuan.li1995@hotmail.com


