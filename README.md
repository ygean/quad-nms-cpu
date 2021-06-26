# quad-nms-cpu

[中文介绍](./README_CN.md)

Use c++ clipper to implement quadrilateral NMS.

## Usage of python scripts

The python script is used to verify and visualize the result, and compare it with the iou calculated by the c++ result;

`quad_iou.py` uses pyclipper to calculate the intersection ratio of two non-concave quadrilaterals for comparison with c++ calculation results;
`visualize.py` is used to visualize the distribution of quadrilaterals;

## compile c++

```
mkdir build
cd build
cmake ..
```
Run and test it.
