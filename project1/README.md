# SC3020 Project 1

[Video](https://www.youtube.com/watch?v=Wzydj5Eykmg) / [Report](https://pufanyi.github.io/SC3020-Projects/project1/docs/report/SC3020_Project1.pdf) / [Code](https://github.com/pufanyi/SC3020-Projects/tree/main/project1)

This project is done by

| Name | Matric Num |
| -------- | --------- |
| Cui Nan  | U2221495L |
| Pu Fanyi | U2220175K |
| Shan Yi  | U2222846C |
| Tian Yidong |  U2220492B |
| Zhang Kaichen | U2123722J |

## Setup

Download and install [CMake](https://cmake.org/download/).

```bash
cd project1

mkdir build
cd build
cmake ..

# Complie
make

# Run main program
./project

# Run unittest
./unittest
```

## Test

We use [GoogleTest](https://github.com/google/googletest) to build up our testing framework and mainly ensure our result are correct here. To check the result, you can simply run the `./unittest` and then check the info from the side bar. The Linear scan result is printed at the LinearScan Test. The result from the range query is from the `./project1`. The default cache value we set is 500

We also provide a python test case to double check with the result. To run the result, you can run

```bash
cd test
python3 -m unittest test_query_result
```
