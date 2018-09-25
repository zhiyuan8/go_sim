# go_sim

Self-contained and competitive tests on simulated data. The two tests are used to detect non-null gene.

### Installation

In terminal, the followings enable you to install boost library which will be needed in calculating chi-square quantile and generating normal random variable.
```
sudo apt-get install libboost-all-dev
```

In terminal, the followings enable you to generate makefile.
```
git clone --recursive https://github.com/zhiyuan8/go_sim.git
cd codes
g++ -std=c++11 -o generate_data.cpp io.cpp main.cpp tests.cpp utils.cpp
make
```

### System Requirement
g++ >= 4.2.1. (double check it! it is said g++ contains C++11 lib)
https://stackoverflow.com/questions/7482026/can-i-use-the-latest-features-of-c11-in-xcode-4-or-osx-lion

The following codes help to check version
```
g++ --version
```

## Running the tests

The ```test_data``` folder contains datasets as a demo. 


## Related publication


## Authors

* **Zhiyuan Jim Li** - *Initial work*


