//헤더파일들 include
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
	Mat img = imread("test.png");
	imshow("test", img);
	waitKey(0);
	destroyAllWindows();

	return 0;
}