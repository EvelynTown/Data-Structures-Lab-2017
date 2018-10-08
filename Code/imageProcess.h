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
	//ͼƬ����
	std::string name;
	//ͼƬ�ߴ硢�ߡ���
	int size;
	int height;
	int width;
	//������Ϣ
	uchar **color;
	uchar **colorR;
	uchar **colorG;
	uchar **colorB;
	//����������ͼƬ
	cv::Mat imageCR;
	cv::Mat imageBluring;
	cv::Mat imageSobel;
	//��ȡͼƬ������Ϣ
	void colorRead(cv::Mat &image);
	//���ؼ��ٴ���
	void colorReduce(cv::Mat &image, cv::Mat &imageColorReduce);
	//ģ������
	void Bluring(cv::Mat &image, cv::Mat &imageBluring, int winSize);
	//����ͨ��ͼƬת���ɵ�ͨ��
	int GrayImage(cv::Mat &image, cv::Mat &imageSobel);
	//�������������Ӽ��ͼƬ��Ե
	void Sobel(int thresh, cv::Mat &imageSobel, cv::Mat &gray);

public:
	//����ͼƬ���ƹ��캯��
	PixImage(string imageName) {
		image = cv::imread(imageName);
		if (image.empty()) {
			cout << "ͼ�����ʧ��" << endl;
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
	//��������
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
	//����Ļ����ʾͼƬ
	void PicShow();
	//����ͼƬ
	void PicSave(string name);
};

#endif