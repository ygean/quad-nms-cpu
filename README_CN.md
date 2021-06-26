# quad-nms-cpu
使用c++ clipper实现四边形的NMS

## 使用说明

python脚本用来验证和可视化结果，和c++结果计算的iou进行比较；
quad_iou.py使用pyclipper来计算两非凹四边形的交并比，用来与c++计算结果比对；
visualize.py 用来可视化四边形的分布；

## 编译c++

```
mkdir build
cd build
cmake ..
```
执行可执行文件，即可看到计算结果
