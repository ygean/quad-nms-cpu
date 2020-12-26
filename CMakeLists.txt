# CMakeList.txt: quad-nms-cpu 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("quad-nms-cpu")
# 将源代码添加到此项目的可执行文件。
add_executable (quad-nms-cpu "clipper6.4.2/clipper.hpp" "clipper6.4.2/clipper.cpp" "quad_nms_cpu.cpp")

# TODO: 如有需要，请添加测试并安装目标。
