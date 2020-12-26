// quad-nms-cpu.cpp: 定义应用程序的入口点。
//
#include <iostream>

#include "clipper6.4.2/clipper.hpp"

using namespace std;
using namespace ClipperLib;

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
void nms_cpu(const float* boxes, float conf_thresh, int* keep, int keep_num);

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

	*iou = inter_area / (union_area + 1e-16); // 防止下溢
	return;
}

void nms_cpu(const float* boxes, float conf_thresh, int* keep, int keep_num) {

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
	float boxes[] = {
		253,285,824,296,833,435,283,413,0.9,
		215,267,853,296,826,449,246,414,0.8,
		200,267,824,296,800,449,283,414,0.85,
	};
	

	cout << "IoU: " << endl;

	return 0;
}

int main() {
	// 测试 计算IoU
	test_compute_iou();
	return 0;
}
