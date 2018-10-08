#include <iostream>
#include <sstream>
#include <string>
#include "imageProcess.h"
using namespace std;
//总共要处理照片数量
#define picTotal 1

int main()
{
	string path = "E:/images/";	//存储路径
	for (int i = 0; i < picTotal; i++) {
		stringstream ss;
		string n;
		ss << i;
		ss >> n;
		PixImage image(path+n+".jpg");
		image.colorReduce();
		image.Bluring(3);
		image.Sobel();
		image.PicShow();
		image.PicSave(path + n);
	}
	return 0;
}