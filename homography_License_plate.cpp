#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;

Mat srcImg,dstImg;

//放置座標點
vector <Point2f>pts_src;
vector <Point2f>pts_dst;

//滑鼠事件函數，
void getHomographyMat_src(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		pts_src.push_back(Point2f(x, y));
		circle(srcImg, Point(x, y), 3, Scalar(255, 255, 0), 1, CV_AA);
		cout << "Source image:" << Point2i(x, y) << endl;
		imshow("Source image", srcImg);
	}
}

void getHomographyMat_dst(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		pts_dst.push_back(Point2f(x, y));
		circle(dstImg, Point(x, y), 3, Scalar(255, 255, 0), 1, CV_AA);
		cout << "License plate:" <<  Point2i(x, y) << endl;
		imshow("License plate", dstImg);
	}
}

int main(void) {

	srcImg = imread("f_1.jpg");
	Mat initial_srcImg = srcImg.clone();  //初始化用
	namedWindow("Source image");
	imshow("Source image", srcImg);

	//啟動滑鼠:來源影像
	cout << "在來源影像中，點選車牌範圍，選取4個點" << endl;
	setMouseCallback("Source image", getHomographyMat_src);

	dstImg = imread("plate_size.jpg");
	Mat initial_dstImg = dstImg.clone();  //初始化用
	namedWindow("License plate");
	imshow("License plate", dstImg);

	//啟動滑鼠:目標影像
	cout << "在標準的車牌影像中，選取4個點" << endl;
	setMouseCallback("License plate", getHomographyMat_dst);

	while (1) {
		//key Enter show出 Homography 
		if (waitKey(0) == 13) {
			Mat homoMat = findHomography(pts_src, pts_dst);
			Mat outImg;
			warpPerspective(srcImg, outImg, homoMat, dstImg.size());
			imshow("Warped Image", outImg);
		}
		//初始化，vector及圖片
		else if (waitKey(0) == 67 || waitKey(0) == 99) {
			pts_src.clear();
			pts_dst.clear();
			initial_srcImg.copyTo(srcImg);
			initial_dstImg.copyTo(dstImg);
			imshow("SrcWindow", srcImg);
			imshow("dstWindow", dstImg);
			cout << "clear points" << endl;
		}
	}

	waitKey(0);

}

