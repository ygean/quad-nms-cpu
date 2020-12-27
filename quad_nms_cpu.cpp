// quad-nms-cpu.cpp: 定义应用程序的入口点。
//
#include <iostream>

#include "clipper6.4.2/clipper.hpp"

using namespace std;
using namespace ClipperLib;

void slice(const float* boxes, int box_dim, float* out, const int* indexes, int index_num);

/// <summary>
/// 计算两个四边形的交并比
/// </summary>
/// <param name="box1">四边形</param>
/// <param name="box2">四边形</param>
/// <param name="iou">IoU float类型</param>
void comput_iou(const float* box1, const float* box2, float* iou);

/// <summary>
/// 将float类型的四边形数据转换到clipper Path类型
/// </summary>
/// <param name="box">float四边形数据</param>
/// <param name="path">clipper Paths</param>
void box_to_path(const float* box, Path& path);

/// <summary>
/// 实现基于cpu上的NMS，我们假设不同类别的数据在真实场景下是不会有交集的
/// </summary>
/// <param name="boxes"></param>
/// <param name="conf_thresh"></param>
void nms_cpu(float* boxes, int box_num, float threshold, int* keep, size_t& keep_num);

void box_to_path(const float* box, Path& path) {
	for (size_t i = 0; i < 4; i++)
	{
		// box1 的点转换成Path类型
		int x = (int)box[i * 2];
		int y = (int)box[i * 2 + 1];
		IntPoint pt(x, y);
		path.push_back(pt);
	}
}

void comput_iou(const float* box1, const float* box2, float* iou) {
	// 将数组转换成内置类型
	Path sub;
	// box1 的点转换成Path类型
	box_to_path(box1, sub);
	
	Path clp;
	// box1 的点转换成Path类型
	box_to_path(box2, clp);

	// 准备执行裁剪
	Clipper clpr;
	clpr.AddPath(sub, ptSubject, true);
	clpr.AddPath(clp, ptClip, true);

	Paths inter_solution; // 计算交集合
	Paths union_solution; // 计算并集合

	clpr.Execute(ctIntersection, inter_solution, pftEvenOdd, pftEvenOdd);
	clpr.Execute(ctUnion, union_solution, pftEvenOdd, pftEvenOdd);

	// 判断两者不为空
	if (inter_solution.empty() || union_solution.empty()) {
		*iou = .0;
		return;
	}

	// 计算面积，默认取第一个集合
	float inter_area = Area(inter_solution[0]); // 计算交面积
	float union_area = Area(union_solution[0]); // 计算并面积

	*iou = inter_area / std::max(union_area, 1.f); // 防止下溢
	return;
}

/// <summary>
/// 返回保留的框的index以及个数,确保框经过置信度的排序
/// </summary>
/// <param name="boxes">待处理的四边形with confidence;x1,y1,...,x4,y4,conf,...</param>
/// <param name="box_num">框的个数</param>
/// <param name="threshold">iou的阈值</param>
/// <param name="keep">索引列表</param>
/// <param name="keep_num">保留框的个数</param>
void nms_cpu(float* boxes, int box_num, float threshold, int* keep, size_t& keep_num) {
	int _keep_num = 0;
	for (size_t i = 0; i < box_num; i++)
	{
		// 如果box的置信度设置为-1，则不对该点做任何处理
		if (boxes[i * 9 + 8] == -1.) {
			continue;
		}
		int j = i + 1;
		float* current_box = &boxes[i * 9];
		while (j<box_num)
		{
			if (boxes[j * 9 + 8] == -1.0) {
				j = j + 1;
				continue;
			}
			float* next_box = &boxes[j * 9];
			float _iou;
			comput_iou(current_box, next_box, &_iou);
			// 如果iou大于阈值，说明重合度太高，可以认为是同一个对象，不保留
			printf("iou: %f\n", _iou);
			if (_iou >= threshold) {
				boxes[j * 9 + 8] = -1.0; // 如果重合度太高，则将该box的置信度设置为0
			}
			
			j = j + 1;
		}
		// 结束后，将当前box的index添加到keep中
		keep[_keep_num] = i;
		_keep_num = _keep_num + 1;
		
	}
	// 整个程序结束后，将保留的box的个数返回
	keep_num = _keep_num;
	
	return;
}

void slice(const float* boxes, int box_dim, float* out, const int* indexes, int index_num) {
	// 迭代索引列表
	for (size_t i = 0; i < index_num; i++)
	{
		size_t index = indexes[i]; // index指向boxes中的索引
		for (size_t j = 0; j < box_dim; j++)
		{
			// out的长度即是index_num
			out[i * 8 + j] = boxes[index * 8 + j]; // 将index指向的box数据拷贝给out
		}
	}
	return;
}

int test_compute_iou()
{
	// 253,285,824,296,833,435,283,413
	// 215,267,853,296,826,449,246,414

	// 94,99,155,292,512,219,463,70
	// 76,159,53,292,485,340,505,105
	float quad0[8] = { 253,285,824,296,833,435,283,413 };
	float quad1[8] = { 215,267,853,296,826,449,246,414 };
	float iou;
	comput_iou(quad0, quad1, &iou);

	cout << "IoU: " << iou << endl;

	return 0;
}

int test_nms()
{
	// 253,285,824,296,833,435,283,413
	// 215,267,853,296,826,449,246,414

	// 94,99,155,292,512,219,463,70
	// 76,159,53,292,485,340,505,105
	int box_num = 6;
	float boxes[] = {
		95,71,80,255,332,250,374,80,0.9,
		43,175,461,171,409,308,55,309,0.8,
		62,107,417,114,408,227,34,216,0.85,
		215,426,469,424,474,595,219,595,0.8,
		175,457,440,460,451,635,178,624,0.75,
		255,371,264,609,467,618,449,370,0.6
	};
	
	int* keep = new int[box_num];
	size_t keep_num = 0;
	float threshold = 0.3;
	nms_cpu(&boxes[0], box_num, threshold, keep, keep_num);
	cout << "number of keep boxes: " << keep_num << endl;
	float* out = new float[keep_num * 9];
	slice(boxes, 9, out, keep, keep_num);

	for (size_t i = 0; i < keep_num; i++)
	{
		cout << "{" << out[i * 8] << "," << out[i * 8 + 1] << "," << out[i * 8 + 2] << "," << out[i * 8 + 3] \
			<< "," << out[i * 8 + 4] << "," << out[i * 8 + 5] << "," << out[i * 8 + 6] << "," << out[i * 8 + 7] \
			<< "," << out[i * 8 + 8] << "}";
		cout << endl;
	}

	delete[] keep;
	delete[] out;

	return 0;
}

int main() {
	// 测试 计算IoU
	//test_compute_iou();
	test_nms();
	return 0;
}
