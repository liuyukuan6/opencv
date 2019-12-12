// test4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp> //主要包含了opencv基本数据结构，动态数据结构，绘图函数，数组操作相关函数，辅助功能与系统函数和宏。
#include <opencv2/imgproc.hpp> //主要包换了图像的变换，滤波直方图相关结构分析，形状描述 
#include <opencv2/highgui.hpp>//主要包换了图形交互界面，媒体I/O的输入输出，视频信息的捕捉和提取，图像视频编码等。


///<summary>
///处理鼠标事件的回调函数
///</summary>
///<param name="event">触发回调函数的鼠标事件的类型</param>
///<param name="x">事件发生时鼠标的x位置</param>
///<param name="y">事件发生时鼠标的y位置</param>
///<param name="flags">事件发生时鼠标按下了哪个键</param>
///<param name="param">指向任意对象的指针，作为附加的参数发给函数</param>
///<returns>无</returns>

void onMouse(int event, int x, int y, int flags, void* param) {
	cv::Mat* im = reinterpret_cast<cv::Mat*>(param);
	switch (event) { // 调度事件
	case cv::EVENT_LBUTTONDOWN: // 鼠标左键按下事件
	// 显示像素值(x,y) 
		std::cout << "at (" << x << "," << y << ") value is: "
			<< static_cast<int>(
				im->at<uchar>(cv::Point(x, y))) << std::endl;
		break;
	case cv::EVENT_RBUTTONDOWN: // 鼠标右键按下事件
// 显示像素值(x,y) 
		std::cout << "at (" << x << "," << y << ") value is: "
			<< static_cast<int>(
				im->at<uchar>(cv::Point(x, y))) << std::endl;
		break;
		/*case cv::EVENT_MOUSEMOVE: // 鼠标移动事件
	// 显示像素值(x,y)
			std::cout << "at (" << x << "," << y << ") value is: "
				<< static_cast<int>(
					im->at<uchar>(cv::Point(x, y))) << std::endl;
			break;*/
	}
}

int main()
{
#pragma region 显示图片
	cv::Mat cat;
	std::cout << "This image is " << cat.rows << " x " << cat.cols << std::endl;
	std::cout << "========================================" << std::endl;
	// 读取输入图像
	cat = cv::imread("C:/Users/liuyukuan/Desktop/cat/4.jpg");
	//cat = cv::imread("C:/Users/liuyukuan/Desktop/cat/1.jpg", cv::IMREAD_COLOR);//读入为三通道彩色图像
	//第二个参数为空的话，就按图像的原本情况读入
	//图像的默认路径就是原代码所在的路径

	//输出图像的尺寸
	std::cout << "The image's is " << cat.rows << "*" << cat.cols << std::endl;
	//检查图像的通道数
	std::cout << "The image has " << cat.channels() << " channel(s)" << std::endl;


	//进行一个错误处理
	if (cat.empty()) { // 错误处理
	 // 未创建图像……
		std::cout << "error" << std::endl;
		// 可能显示一个错误消息
		return 0;
		// 并退出程序
	}
	// 定义窗口（可选）
	cv::namedWindow("cat");
	// 显示图像
	cv::imshow("cat", cat);
#pragma endregion


#pragma region 翻转
	cv::Mat result; // 创建另一个空的图像
	cv::flip(cat, result, 1); // 正数表示水平
	 // 0 表示垂直
	 // 负数表示水平和垂直
	cv::namedWindow("Output Image"); // 输出窗口,自命名用来区分窗口
	cv::imshow("Output Image", result);//在哪个窗口输出
	//cv::imwrite("C:/Users/liuyukuan/Desktop/cat/1_1.jpg", result); // 保存结果，保存在哪里，格式是什么
#pragma endregion


#pragma region 鼠标左键点击出现坐标
	cv::setMouseCallback("cat", //与名为“cat”的窗口互动
		onMouse,//处理鼠标事件的回调函数
		reinterpret_cast<void*>(&cat));//cv::Mat *类型指针强制转换为void*类型
	cv::setMouseCallback("Output Image",
		onMouse,
		reinterpret_cast<void*>(&cat));
#pragma endregion

#pragma  region 图像绘图

	cv::namedWindow("Drawig on a cat");


	cv::circle(cat, // 目标图像
		cv::Point(100, 50), // 中心点坐标
		65, // 半径
		0, // 颜色（这里用黑色）
		3); // 厚度

	cv::putText(cat, // 目标图像
		"This is a cute cat.", // 文本
		cv::Point(40, 100), // 文本位置
		cv::FONT_HERSHEY_PLAIN, // 字体类型
		1.0, // 字体大小
		cv::Scalar(255,0,255), // 字体颜色（这里用白色）
		2); // 文本厚度
	cv::imshow("Drawig on a cat", cat);

#pragma endregion



#pragma region 确保图片能显示
	//控制台窗口会在main结束时关闭
	cv::waitKey(0); // 0 表示永远地等待按键
	 // 键入的正数表示等待的毫秒数
#pragma endregion
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
