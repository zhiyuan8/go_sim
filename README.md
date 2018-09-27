# go_sim

Self-contained and competitive tests on simulated data. The two tests are used to detect non-null gene.

### System Requirement
g++ >= 4.2.1. (double check it! it is said g++ contains C++11 lib)
https://stackoverflow.com/questions/7482026/can-i-use-the-latest-features-of-c11-in-xcode-4-or-osx-lion

The following codes help to check version
```
g++ --version
```

### Installation

In terminal, firstly check if there exists boost library. Run the following codes:
```
locate /boost/version.hpp
```
If there exits boost library, then the path will be printed. For example, if it says "", then this line of code enables you to see your boost library version.

```
cat /home/zhiyuan/R/x86_64-pc-linux-gnu-library/3.2/BH/include/boost/version.hpp | grep "BOOST_LIB_VERSION"
```
The followings enable you to install boost library which will be needed in calculating chi-square quantile and generating normal random variable.

```
sudo apt-get install libboost-all-dev
```

Using ```cd``` to locate at the place where you will store the codes. Download the codes on github.
```
git clone -- https://github.com/zhiyuan8/go_sim.git
```
Locates at the ```codes``` file. Then, complie all cpp files.
```
cd go_sim/codes/
g++ main.cpp generate_data.cpp io.cpp tests.cpp utils.cpp -o GO_DAG -std=c++11

```
In this way, ```GO_DAG``` is created. 	This process takes a while.

## Running the tests

The ```test_data``` folder contains datasets as a demo. You can replace them with your json files, but remember to keep the file name as the same. Now create a ```result``` folder.
```
cd ..
mkdir result
```
Finally, input codes in terminal.
```
./GO_DAG      ./test_data      ./result  100
```
The first argument enables terminal to run the code. The second and third arguments are paths for reading in and writing out files. The fourth number is a seed used in generating data. If you keep the seed number unchanged, you will get same outputs.

## Related publication


## Authors

* **Zhiyuan Jim Li** - *Initial work* - contact: zhiyuan.li1995@hotmail.com


