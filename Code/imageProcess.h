#ifndef _IMAGEPROCESS_H_
#define _IMAGEPROCESS_H_

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include"cxcore.h"
#include<cmath>
using namespace cv;
using namespace std;

class PixImage {
private:
	cv::Mat image;
	//图片名称
	std::string name;
	//图片尺寸、高、宽
	int size;
	int height;
	int width;
	//像素信息
	uchar **color;
	uchar **colorR;
	uchar **colorG;
	uchar **colorB;
	//经过处理后的图片
	cv::Mat imageCR;
	cv::Mat imageBluring;
	cv::Mat imageSobel;
	//读取图片像素信息
	void colorRead(cv::Mat &image);
	//像素减少处理
	void colorReduce(cv::Mat &image, cv::Mat &imageColorReduce);
	//模糊处理
	void Bluring(cv::Mat &image, cv::Mat &imageBluring, int winSize);
	//将多通道图片转换成单通道
	int GrayImage(cv::Mat &image, cv::Mat &imageSobel);
	//利用索贝尔算子检测图片边缘
	void Sobel(int thresh, cv::Mat &imageSobel, cv::Mat &gray);

public:
	//根据图片名称构造函数
	PixImage(string imageName) {
		image = cv::imread(imageName);
		if (image.empty()) {
			cout << "图像加载失败" << endl;
			exit(0);
		}
		name = imageName;
		size = image.cols *image.rows;
		width = image.cols;
		height = image.rows;
		color = new uchar* [height];
		colorR = new uchar*[height];
		colorG = new uchar*[height];
		colorB = new uchar*[height];
		colorRead(image);
	}
	//析构函数
	~PixImage() {
		delete []color;
		delete []colorR;
		delete []colorG;
		delete []colorB;
	}

	void colorReduce() {
		colorReduce(image, imageCR);
	}

	void Bluring(int winSize) {
		Bluring(image, imageBluring, winSize);
	}

	void Sobel() {
		Mat gray;
		int thresh = GrayImage(image, gray);
		Sobel(thresh, imageSobel, gray);
	}
	//在屏幕上显示图片
	void PicShow();
	//保存图片
	void PicSave(string name);
};

#endif