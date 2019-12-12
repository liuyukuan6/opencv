// 用积分图像统计像素.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	cv::Mat book1 = cv::imread("book.jpg",cv::IMREAD_GRAYSCALE);
	if (!book1.data)
		return 0;

	cv::Mat book2;
	//旋转图像，使其更适合展示
	cv::transpose(book1, book2);
	cv::flip(book1, book2, 0);

	cv::namedWindow("Original book");
	cv::imshow("Original book", book1);

	cv::namedWindow("Transpose book");
	cv::imshow("Transpose book", book2);

#pragma region 适用固定阈值

	//使用固定的阈值（效果不好）
	cv::Mat binaryFixed;
	cv::threshold(book1, binaryFixed, 70, 255, cv::THRESH_BINARY);
	//>70 -> 255 
	//<=70 -> 0

	cv::namedWindow("Fixed Threshold");
	//不管选用什么阈值，图像总会丢失一部分文本，还有部分文本会消失在阴影中
	cv::imshow("Fixed Threshold", binaryFixed);
#pragma endregion

#pragma region 使用积分图像

	cv::Mat binary = book1.clone();
	
	int nl = binary.rows;//图像的行数
	int nc = binary.cols;//图像的列数

	//计算积分图像
	cv::Mat ibook;
	cv::integral(book1, ibook, CV_32S);//调用opencv的库函数计算积分图像

	//使用自适应阈值
	int blockSize = 21;//邻域的尺寸
	int threshold = 10;//像素与其邻域的平均值比较用的阈值

	///<使用指针遍历积分函数>
	int halfSize = blockSize / 2;

	//遍历行
	for (int j = halfSize; j < nl - halfSize - 1; j++)
	{
		//得到第j行的地址
		uchar* data = binary.ptr<uchar>(j);
		//得到邻域的上边界行地址
		int* idata1 = ibook.ptr<int>(j - halfSize);
		//得到邻域的下边界行地址
		int* idata2 = ibook.ptr<int>(j + halfSize);

		//遍历列
		for (int i = halfSize; i < nc - halfSize - 1; i++)
		{
			//计算邻域的平均值(A-B-C+D)/n
			int sum = (idata2[i + halfSize + 1] - idata2[i - halfSize] - idata1[i + halfSize + 1] + idata1[i - halfSize]) / (blockSize * blockSize);

			//应用自适应阈值，将每个像素的值与邻域的平均值进行比较
			if (data[i] < (sum - threshold))
				data[i] = 0;
			else
				data[i] = 255;//大于阈值的像素设为黑色

		}
	
	
	
	}

	cv::namedWindow("Adapative Threshold (integral)");
	cv::imshow("Adapative Threshold (integral)", binary);


#pragma endregion

#pragma region 使用自适应阈值(使用OpenCV函数)

	cv::Mat binaryAdaptive;

	cv::adaptiveThreshold(
		book1,							//输入图像
		binaryAdaptive,					//输出图像
		255,							//大于阈值的像素的值
		cv::ADAPTIVE_THRESH_MEAN_C,		//自适应方法
		cv::THRESH_BINARY,				//阈值类型
		blockSize,						//邻域的尺寸
		threshold);						//像素与其邻域的平均值比较用的阈值

	cv::namedWindow("Adaptive Threshold");
	cv::imshow("Adaptive Threshold", binaryAdaptive);


#pragma endregion

#pragma region 用图像运算符号编写自适应阈值化

	cv::Mat filtered;
	cv::Mat binaryFiltered;

	//boxFilter 计算矩形区域内像素的平均值
	cv::boxFilter(book1, filtered, CV_8U, cv::Size(blockSize, blockSize));

	//检查像素是否大于等于（mean+threshold）
	binaryFiltered = book1 >= (filtered - threshold);

	cv::namedWindow("Adaptive Threshold (filtered)");
	cv::imshow("Adaptive Threshold (filtered)", binaryFiltered);

#pragma endregion

	cv::waitKey();
	return 0;

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
