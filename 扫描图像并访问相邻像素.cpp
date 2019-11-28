

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <random>

void sharpen(const cv::Mat& image, cv::Mat& result)
{
	//这里不适合调用sharpen(image,image);
	//因为下面计算锐化时，其相邻元素用的是原图像像素的值
	//如果在原图像上修改，其计算时会用到修改后的值，不符合预期
	result.create(image.size(), image.type());

	//获得通道数
	int nchannels = image.channels();

	//遍历所有行（除了第一行和最后一行）
	//第一行没有上一行，最后一行没有下一行,所以跳过
	for (int j = 1; j < image.rows - 1; j++)
	{
		const uchar* previous = image.ptr<const uchar>(j - 1);//上一行
		const uchar* current = image.ptr<const uchar>(j);//当前行
		const uchar* next = image.ptr<const uchar>(j + 1);//下一行

		uchar* output = result.ptr<uchar>(j);//输出图像的当前行

		//遍历所有列（除了第一列和最后一列）
		//第一列没有左边一列，最后一列没有右边一列，所以跳过
		for (int i = nchannels; i < (image.cols - 1) * nchannels; i++)
			//应用锐化算法 sharpened_pixel=5*current-left-right-up-down;
			//cv::saturate_cast<uchar>()函数在类型转换的同时，会把小于0的数值调整为0，大于255的数值调整为255
			output[i] = cv::saturate_cast<uchar>(5 * current[i]
				- current[i - nchannels]//left
				- current[i + nchannels]//right
				- previous[i]//up
				- next[i]);//down
	}
	//把未处理的像素设为0
	result.row(0).setTo(cv::Scalar(0));//第一行
	result.row(result.rows - 1).setTo(cv::Scalar(0));//最后一行
	result.col(0).setTo(cv::Scalar(0));//第一列
	result.col(result.cols - 1).setTo(cv::Scalar(0));//最后一列
}

void sharpenIterator(const cv::Mat& image, cv::Mat& result)
{
	//只在灰度图像下工作
	CV_Assert(image.type() == CV_8UC1);//CV_Assert() 若括号中的表达式值为false，则返回一个错误信息，终止程序

	//初始化迭代器
	cv::Mat_<uchar>::const_iterator it = image.begin<uchar>() + image.cols;//当前行从第二行开始
	cv::Mat_<uchar>::const_iterator itend = image.end<uchar>() - image.cols;//倒数第二行结束
	cv::Mat_<uchar>::const_iterator itup = image.begin<uchar>();//当前行的上一行
	cv::Mat_<uchar>::const_iterator itdown = image.end<uchar>() + 2.0 * image.cols;//当前行的下一行

	//创建输出图像
	result.create(image.size(), image.type());
	//初始化输出图像的迭代器
	cv::Mat_<uchar>::iterator itout = result.begin<uchar>() + result.cols;

	//遍历整个图像
	for (; it != itend; ++it, ++itout, ++itup, ++itdown)
		//应用锐化函数 sharpened_pixel = 5*current -left -right -up -down;
		*itout = cv::saturate_cast<uchar>(*it * 5 - *(it - 1) - *(it + 1) - *itup - *itdown);

	//把未处理的像素设为0
	result.row(0).setTo(cv::Scalar(0));//第一行
	result.row(result.rows - 1).setTo(cv::Scalar(0));//最后一行
	result.col(0).setTo(cv::Scalar(0));//第一列
	result.col(result.cols - 1).setTo(cv::Scalar(0));//最后一列

}

//使用滤波器
void sharpen2D(const cv::Mat& image, cv::Mat& result)
{
	//Construct kernel (all entries initialized to 0)
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	//assigns kernel values
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;

	//filter the image
	cv::filter2D(image, result, image.depth(), kernel);

}

int main()
{
#pragma region 使用指针
	cv::Mat image = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");
	if (!image.data)
		return 0;

	cv::Mat result;

	double time = static_cast<double>(cv::getTickCount());

	//锐化图像
	sharpen(image, result);

	time = (static_cast<double>(cv::getTickCount()) - time) / cv::getTickFrequency();
	std::cout << "time=" << time << std::endl;

	cv::namedWindow("Image sharpen with Ponters");
	cv::imshow("Image sharpen with Ponters", result);

	cv::waitKey();
#pragma endregion

#pragma region 迭代器
	image = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg", cv::IMREAD_GRAYSCALE);

	time = static_cast<double>(cv::getTickCount());

	//锐化图像
	sharpenIterator(image, result);

	time = (static_cast<double>(cv::getTickCount()) - time) / cv::getTickFrequency();
	std::cout << "time3=" << time << std::endl;

	cv::namedWindow("Image Sharpen with Iterators");
	cv::imshow("Image Sharpen with Iterators",result);

	cv::waitKey();
#pragma endregion

#pragma region 使用滤波器
	image = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");

	time = static_cast<double>(cv::getTickCount());

	//锐化图像
	sharpen2D(image, result);

	time = (static_cast<double>(cv::getTickCount()) - time) / cv::getTickFrequency();
	std::cout << "time2D=" << time << std::endl;

	cv::namedWindow("Image Sharpen with Kernel");
	cv::imshow("Image Sharpen with Kernel", result);

	cv::waitKey();


#pragma endregion
	return 0;


}


