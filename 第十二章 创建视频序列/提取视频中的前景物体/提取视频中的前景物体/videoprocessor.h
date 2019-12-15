#if !defined VPROCESSOR
#define VPROCESSOR

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// ֡����Ľӿ�
class FrameProcessor {

public:
	// ������
	virtual void process(cv::Mat& input, cv::Mat& output) = 0;
};

class VideoProcessor {

private:

	// OpenCV ��Ƶ�������
	cv::VideoCapture capture;
	// // ����ÿһ֡ʱ������õĻص�����
	void (*process)(cv::Mat&, cv::Mat&);
	// the pointer to the class implementing 
	// the FrameProcessor interface
	FrameProcessor* frameProcessor;
	//  �����ͱ�������ʾ�ûص������Ƿ�ᱻ����
	bool callIt;
	// ���봰�ڵ���ʾ����
	std::string windowNameInput;
	// ������ڵ���ʾ����
	std::string windowNameOutput;
	// ֮֡�����ʱ
	int delay;
	// �Ѿ������֡�� 
	long fnumber;
	// �ﵽ���֡��ʱ����
	long frameToStop;
	// ��������
	bool stop;

	// ��Ϊ��������ͼ���ļ�������
	std::vector<std::string> images;
	//  ͼ�������ĵ�����
	std::vector<std::string>::const_iterator itImg;

	// OpenCV д��Ƶ����
	cv::VideoWriter writer;
	// ����ļ���
	std::string outputFile;

	// ���ͼ��ĵ�ǰ���
	int currentIndex;
	// ���ͼ���ļ��������ֵ�λ��
	int digits;
	// ���ͼ�����չ��
	std::string extension;

	// ȡ����һ֡
	// ��������Ƶ�ļ����������ͼ������
	// ���ܸ����������ݣ�ѡ�����Ƶ��ȡ���Ǵ���
	// ����������ȡ���жϷ����ǲ鿴ͼ���ļ��������Ƿ�Ϊ�գ��粻Ϊ�վͱ���������ͼ�����С�
	bool readNextFrame(cv::Mat& frame) {

		if (images.size() == 0)
			return capture.read(frame);
		else {

			if (itImg != images.end()) {

				frame = cv::imread(*itImg);
				itImg++;
				return frame.data != 0;
			}

			return false;
		}
	}

	// д�����֡
	// ��������Ƶ�ļ���ͼ����
	void writeNextFrame(cv::Mat& frame) {

		if (extension.length()) { // д�뵽ͼ����

			std::stringstream ss;
			// ��ϳ�����ļ���
			ss << outputFile << std::setfill('0') << std::setw(digits) << currentIndex++ << extension;
			cv::imwrite(ss.str(), frame);

		}
		else { // д�뵽��Ƶ�ļ�

			writer.write(frame);
		}
	}

public:

	// Constructor setting the default values
	VideoProcessor() : callIt(false), delay(-1),
		fnumber(0), stop(false), digits(0), frameToStop(-1),
		process(0), frameProcessor(0) {}

	// ������Ƶ�ļ�������
	bool setInput(std::string filename) {

		fnumber = 0;
		//  ��ֹ�Ѿ�����Դ�� VideoCapture ʵ������
		capture.release();
		images.clear();

		//  ����Ƶ�ļ�
		return capture.open(filename);
	}

	// set the camera ID
	bool setInput(int id) {

		fnumber = 0;
		// In case a resource was already 
		// associated with the VideoCapture instance
		capture.release();
		images.clear();

		// Open the video file
		return capture.open(id);
	}

	// ��������ͼ�������
	bool setInput(const std::vector<std::string>& imgs) {

		fnumber = 0;
		//  ��ֹ�Ѿ�����Դ�� VideoCapture ʵ������
		capture.release();

		// �����ͼ��������Ϊ�������
		images = imgs;
		itImg = images.begin();

		return true;
	}

	// ���������Ƶ�ļ�
// Ĭ������»�ʹ����������Ƶ��ͬ�Ĳ���
	bool setOutput(const std::string& filename, int codec = 0, double framerate = 0.0, bool isColor = true) {

		outputFile = filename;
		extension.clear();

		if (framerate == 0.0)
			framerate = getFrameRate(); // ��������ͬ

		char c[4];
		// ʹ����������ͬ�ı������
		if (codec == 0) {
			codec = getCodec(c);
		}

		// �������Ƶ
		return writer.open(outputFile, // �ļ���
			codec, // ���õı������ 
			framerate,      // ��Ƶ��֡����
			getFrameSize(), // ֡�ĳߴ�
			isColor);       // ��ɫ��Ƶ��
	}

	// �������Ϊһϵ��ͼ���ļ�
	// ��չ��������.jpg ��.bmp
	bool setOutput(const std::string& filename, // ǰ׺
		const std::string& ext, // ͼ���ļ�����չ�� 
		int numberOfDigits = 3,   // ���ֵ�λ��
		int startIndex = 0) {     // ��ʼ���

		// ���ֵ�λ������������
		if (numberOfDigits < 0)
			return false;

		// �ļ����ͳ��õ���չ��
		outputFile = filename;
		extension = ext;

		// �ļ���ŷ��������ֵ�λ��
		digits = numberOfDigits;
		// �������ſ�ʼ���
		currentIndex = startIndex;

		return true;
	}

	//  �������ÿһ֡���õĻص�����
	void setFrameProcessor(void (*frameProcessingCallback)(cv::Mat&, cv::Mat&)) {

		// invalidate frame processor class instance
		frameProcessor = 0;
		// this is the frame processor function that will be called
		process = frameProcessingCallback;
		callProcess();
	}

	
	//Ϊ VideoProcessor �������һ�� FrameProcessor ʵ�����������ʵ
	//�����������ĳ�Ա���� frameProcessor�������Ա������ָ�� FrameProcessor �����ָ��
	//����ʵ�� FrameProcessor �ӿڵ�ʵ��
	void setFrameProcessor(FrameProcessor* frameProcessorPtr) {

		// ʹ�ص�����ʧЧ
		process = 0;
		// ������Ǽ��������õ�֡����ʵ��
		frameProcessor = frameProcessorPtr;
		callProcess();
	}

	// ָ������ֹ֡ͣ
	void stopAtFrameNo(long frame) {

		frameToStop = frame;
	}

	// ��Ҫ���ûص����� process
	void callProcess() {

		callIt = true;
	}

	// ����Ҫ���ûص����� process
	void dontCallProcess() {

		callIt = false;
	}

	// ������ʾ�����֡
	void displayInput(std::string wn) {

		windowNameInput = wn;
		cv::namedWindow(windowNameInput);
	}

	// ������ʾ�������֡
	void displayOutput(std::string wn) {

		windowNameOutput = wn;
		cv::namedWindow(windowNameOutput);
	}

	// do not display the processed frames
	void dontDisplay() {

		cv::destroyWindow(windowNameInput);
		cv::destroyWindow(windowNameOutput);
		windowNameInput.clear();
		windowNameOutput.clear();
	}

	

	// a count is kept of the processed frames
	long getNumberOfProcessedFrames() {

		return fnumber;
	}

	// return the size of the video frame
	cv::Size getFrameSize() {

		if (images.size() == 0) {

			// get size of from the capture device
			int w = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
			int h = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));

			return cv::Size(w, h);

		}
		else { // if input is vector of images

			cv::Mat tmp = cv::imread(images[0]);
			if (!tmp.data) return cv::Size(0, 0);
			else return tmp.size();
		}
	}

	// ������һ֡�ı��
	long getFrameNumber() {

		if (images.size() == 0) {

			// �Ӳ����豸��ȡ��Ϣ
			long f = static_cast<long>(capture.get(cv::CAP_PROP_POS_FRAMES));
			return f;

		}
		else { // if input is vector of images

			return static_cast<long>(itImg - images.begin());
		}
	}

	// return the position in ms
	double getPositionMS() {

		// undefined for vector of images
		if (images.size() != 0) return 0.0;

		double t = capture.get(cv::CAP_PROP_POS_MSEC);
		return t;
	}

	// return the frame rate
	double getFrameRate() {

		// undefined for vector of images
		if (images.size() != 0) return 0;

		double r = capture.get(cv::CAP_PROP_FPS);
		return r;
	}

	// return the number of frames in video
	long getTotalFrameCount() {

		// for vector of images
		if (images.size() != 0) return images.size();

		long t = capture.get(cv::CAP_PROP_FRAME_COUNT);
		return t;
	}

	//  ȡ��������Ƶ�ı������
	int getCodec(char codec[4]) {

		// ��������ͼ������������
		if (images.size() != 0) return -1;

		union { // ��ʾ���ַ���������ݽṹ
			int value;
			char code[4];
		} returned;

		//ȡ�ô���
		returned.value = static_cast<int>(capture.get(cv::CAP_PROP_FOURCC));
		// ȡ���ĸ��ַ�
		codec[0] = returned.code[0];
		codec[1] = returned.code[1];
		codec[2] = returned.code[2];
		codec[3] = returned.code[3];

		//���ش��������ֵ
		return returned.value;
	}

	// go to this frame number
	bool setFrameNumber(long pos) {

		// for vector of images
		if (images.size() != 0) {

			// move to position in vector
			itImg = images.begin() + pos;
			// is it a valid position?
			if (pos < images.size())
				return true;
			else
				return false;

		}
		else { // if input is a capture device

			return capture.set(cv::CAP_PROP_POS_FRAMES, pos);
		}
	}

	// go to this position
	bool setPositionMS(double pos) {

		// not defined in vector of images
		if (images.size() != 0)
			return false;
		else
			return capture.set(cv::CAP_PROP_POS_MSEC, pos);
	}

	// go to this position expressed in fraction of total film length
	bool setRelativePosition(double pos) {

		// for vector of images
		if (images.size() != 0) {

			// move to position in vector
			long posI = static_cast<long>(pos * images.size() + 0.5);
			itImg = images.begin() + posI;
			// is it a valid position?
			if (posI < images.size())
				return true;
			else
				return false;

		}
		else { // if input is a capture device

			return capture.set(cv::CAP_PROP_POS_AVI_RATIO, pos);
		}
	}

	// ����֮֡�����ʱ��
// 0 ��ʾÿһ֡���ȴ���
// ������ʾ����ʱ
	void setDelay(int d) {

		delay = d;
	}

	// ��������
	void stopIt() {

		stop = true;
	}

	// ��������Ƿ��Ѿ�ֹͣ��
	bool isStopped() {

		return stop;
	}

	// �����豸�Ƿ��Ѿ��򿪣�
	bool isOpened() {

		return capture.isOpened() || !images.empty();
	}

	//  ץȡ�������������е�֡
	void run() {

		// ��ǰ֡
		cv::Mat frame;
		// ���֡
		cv::Mat output;

		// ���û�����ò����豸
		if (!isOpened())
			return;

		stop = false;

		while (!isStopped()) {

			// ��ȡ��һ֡������У�
			if (!readNextFrame(frame))
				break;

			// ��ʾ�����֡
			if (windowNameInput.length() != 0)
				cv::imshow(windowNameInput, frame);

			//  ** ���ô������򷽷� **
			if (callIt) {

				//  ����֡
				if (process) //����ǻص�����
					process(frame, output);
				else if (frameProcessor)
					// �������Ľӿ�
					frameProcessor->process(frame, output);//->�Ĺ��ܾ����ṩ��һ�ֶ���ָ�������ķ��ʶ����Ա�ķ���
				// ����֡��
				fnumber++;

			}
			else {
				//û�д���
				output = frame;
			}

			//  ** д�뵽��������� **
			if (outputFile.length() != 0)
				writeNextFrame(output);

			//  ��ʾ�����֡
			if (windowNameOutput.length() != 0)
				cv::imshow(windowNameOutput, output);

			// ������ʱ
			if (delay >= 0 && cv::waitKey(delay) >= 0)
				stopIt();

			// ����Ƿ���Ҫ����
			if (frameToStop >= 0 && getFrameNumber() == frameToStop)
				stopIt();
		}
	}
};

#endif
