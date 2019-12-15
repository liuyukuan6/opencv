// 用低通滤波器进行图像滤波.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	//输入图像
	cv::Mat image = cv::imread("boldt.jpg", cv::IMREAD_GRAYSCALE);
	cv::imshow("Original Image", image);//好像不加namewindow也可以输出窗口
#pragma region 低通滤波器	
	//输出图像
	cv::Mat result1;

	/*cv::blur将函数将每个像素的值替换成该像素邻域的平均值（邻域是矩形的），从而使图像
更加平滑。*/
	cv::blur(image, result1, cv::Size(5, 5)); //此处用5*5的方形滤波器区域，也叫块滤波器
    cv::imshow("Mean filtered Image", result1);
	// Blur the image with a mean filter 9x9
	cv::blur(image, result1, cv::Size(9, 9));

	// Display the blurred image
	cv::namedWindow("Mean filtered Image (9x9)");
	cv::imshow("Mean filtered Image (9x9)", result1);//更模糊了

#pragma endregion

#pragma region 高斯函数
	/*有时需要让邻域内较近的像素具有更高的重要度。因此可计算加权平均值，即较近的像素比
较远的像素具有更大的权重。要得到加权平均值，可采用依据高斯函数（即“钟形曲线”函数）
制定的加权策略。*/
	cv::Mat result2;
	cv::GaussianBlur(image, result2,
		cv::Size(5, 5), // 滤波器尺寸
		1.5); // 控制高斯曲线形状的参数,越大越扁平

	cv::imshow("Gaussian filtered Image", result2);

	cv::GaussianBlur(image, result2,
		cv::Size(5, 5), // 滤波器尺寸
		20); // 控制高斯曲线形状的参数

	cv::imshow("Gaussian filtered Image（1.7）", result2);

#pragma endregion

#pragma region 计算Gaussian的九个格子的系数（因为是3*3的过滤器）
	/*如果 σ = 1.5，得到的系数为：
[0.0076 0.03608 0.1096 0.2135 0.2667 0.2135 0.1096 0.0361 0.0076 ]
	*/


	///<Get the gaussian kernel (1.5)>
	cv::Mat gauss = cv::getGaussianKernel(9, 1.5, CV_32F);//3*3的过滤器

	// Display kernel values
	cv::Mat_<float>::const_iterator it = gauss.begin<float>();
	cv::Mat_<float>::const_iterator itend = gauss.end<float>();
	std::cout << "1.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;


	///<Get the gaussian kernel (0.5)>
	gauss = cv::getGaussianKernel(9, 0.5, CV_32F);

	// Display kernel values
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "0.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	///<Get the gaussian kernel (2.5)>
	gauss = cv::getGaussianKernel(9, 2.5, CV_32F);

	// Display kernel values
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "2.5 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the gaussian kernel (9 elements)
	gauss = cv::getGaussianKernel(9, -1, CV_32F);

	// Display kernel values
	it = gauss.begin<float>();
	itend = gauss.end<float>();
	std::cout << "9 = [";
	for (; it != itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

#pragma endregion




	cv::waitKey();
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
