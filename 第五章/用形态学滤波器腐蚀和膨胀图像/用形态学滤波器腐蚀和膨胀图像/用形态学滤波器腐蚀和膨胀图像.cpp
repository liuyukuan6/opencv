// 用形态学滤波器腐蚀和膨胀图像.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	cv::Mat image = cv::imread("binary.bmp");
	if (!image.data) return 0;

	cv::namedWindow("Image");
	cv::imshow("Image", image);

#pragma region 腐蚀图像

	//输出图像
	cv::Mat eroded;
	//腐蚀图像
	cv::erode(image, eroded, 
		cv::Mat());//指定结构元素（滤波器），这里采用默认的3*3结构元素（滤波器）

	cv::namedWindow("Eroded Image");
	cv::imshow("Eroded Image", eroded);

#pragma endregion


#pragma region 膨胀图像

	//输出图像
	cv::Mat dilated;

	//膨胀图像
	cv::dilate(image, dilated, cv::Mat());//同样默认

	cv::namedWindow("Dilated Image");

	cv::imshow("Dilated Image", dilated);


#pragma endregion

#pragma region 腐蚀图像（进阶）

	//创建7*7的结构元素（滤波器），所有值为1
	cv::Mat element(7, 7, CV_8U, cv::Scalar(1));

	//用这个结构元素腐蚀图像
	cv::erode(image,eroded,element);

	cv::namedWindow("Eroded Image (7*7)");
	cv::imshow("Eroded Image (7*7)", eroded);

	///<对图像进行三次腐蚀>

	cv::erode(image, eroded,
		cv::Mat(),			//指定结构元素（滤波器），3*3结构元素
		cv::Point(-1, 1),	//表示锚点在结构元素（滤波器）的中心点，默认值，写出来是因为后一个参数要赋值
		3);					//进行3次腐蚀操作

	//显示腐蚀三次后的图像
	cv::namedWindow("Eroded Image (3 times)");
	cv::imshow("Eroded Image (3 times)", eroded);


#pragma endregion

#pragma region 闭合图像(先膨胀后腐蚀)常用于目标检测，可把错误分裂成小碎片的物体连接起来

	//为了让闭合的效果更明显，这里创建了5*5的结构元素
	cv::Mat element5(5, 5, CV_8U, cv::Scalar(1));

	//创建输出图像
	cv::Mat closed;
	//闭合图像
	cv::morphologyEx(
		image, closed,
		cv::MORPH_CLOSE,	//运算标志：闭合运算	
		element5);			//指定结构元素（滤波器）

	cv::namedWindow("Closed Image");
	cv::imshow("Closed Image", closed);

	/*闭合滤波器填满了白色的前景物体中的小空隙，也会把邻近的物体连接起来。
	基本上，所有小到不能容纳完整结构元素的空隙或间隙都会被闭合滤波器消除*/

#pragma endregion

#pragma region 开启图像（先腐蚀后膨胀），可以移除因图像噪声产生的斑点

	cv::Mat opened;

	//开启图像
	cv::morphologyEx(
		image, opened,
		cv::MORPH_OPEN,	//运算标志：开启
		element5);		//指定结构元素（滤波器）

	cv::namedWindow("Opened Image");
	cv::imshow("Opened Image", opened);

	/*开启滤波器消除了背景中的几个小物体。所有小到不能容纳完整结构元素的物体都会被移除*/
#pragma endregion

#pragma region 开启/闭合图像
	cv::Mat closedopened;

	///<先闭合后开启>
	//闭合图像
	cv::morphologyEx(
		image, closedopened,
		cv::MORPH_CLOSE,	//运算标志：闭合运算	
		element5);
	//开启图像
	cv::morphologyEx(
		closedopened, closedopened,
		cv::MORPH_OPEN,	//运算标志：开启
		element5);		//指定结构元素（滤波器）

	cv::namedWindow("ClosedOpened Image");
	cv::imshow("ClosedOpened Image", closedopened);

	///<先开启后闭合>
	cv::Mat openedclosed;

	///<先闭合后开启>
	//闭合图像
	cv::morphologyEx(
		image, openedclosed,
		cv::MORPH_CLOSE,	//运算标志：闭合运算	
		element5);
	//开启图像
	cv::morphologyEx(
		openedclosed, openedclosed,
		cv::MORPH_OPEN,	//运算标志：开启
		element5);		//指定结构元素（滤波器）

	cv::namedWindow("OpenedClosed Image");
	cv::imshow("OpenedClosed Image", openedclosed);
	cv::imwrite("1.jpg",openedclosed);
#pragma endregion

	cv::waitKey();
	cv::destroyAllWindows();

#pragma region 梯度运算（在灰度图像上）可以提取出图像的边缘
	cv::Mat image1 = cv::imread("boldt.jpg", cv::IMREAD_GRAYSCALE);
	if (!image.data) return 0;

	cv::namedWindow("Image1");
	cv::imshow("Image1", image1);

	//输出图像
	cv::Mat result1;

	cv::morphologyEx(
	image1,result1,
		cv::MORPH_GRADIENT,		//运算标志:梯度
		cv::Mat());				//指定滤波器结构，默认

	cv::namedWindow("Edge Image1");
	cv::imshow("Edge Image1", result1);
	cv::namedWindow("Edge Image2");
	cv::imshow("Edge Image2", 255 - result1);


#pragma endregion

#pragma region	黑帽变换（hat-top）变换运算，它可以从图像中提取出局部的小型前景物体

	cv::Mat image2 = cv::imread("book.jpg", cv::IMREAD_GRAYSCALE);
	if (!image.data) return 0;

	//旋转并翻转图像，以便更好地显示图像
	cv::transpose(image2, image2);
	cv::flip(image2, image2, 0);

	//显示翻转后的图像
	cv::namedWindow("Image2");
	cv::imshow("Image2", image2);

	cv::Mat result2;
	cv::Mat element7(7, 7, CV_8U, cv::Scalar(1));
	cv::morphologyEx(image2, result2, cv::MORPH_BLACKHAT, element7);

	cv::namedWindow("1 Black Top-hat Image （7*7）");
	cv::imshow("1 Black Top-hat Image1 （7*7）", result2);
	cv::namedWindow("2 Black Top-hat Image （7*7）");
	cv::imshow("2 Black Top-hat Image （7*7）", 255-result2);

#pragma endregion


	cv::waitKey();

}


