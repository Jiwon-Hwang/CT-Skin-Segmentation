//헤더파일들 include
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream> //cout, cin 함수 사용하기 위해 (iostream 클래스 내의 객체)
#include "stdafx.h" //precompiled header (미리 컴파일 시켜주는 역할, 전체적으로 빌드 시간 감소)

//namespace : 식별자(변수, 함수명 등)의 소속 공간(name space), 같은 이름이어도 다르게 구분 가능하게 해줌(충돌 방지), 접두어 생략 가능
//*'네임스페이스' 'std'에 cout, cin같은 함수가 소속되어있고, 'iostream'이라는 '헤더파일'에 이 내용들이 작성되어 있음
using namespace std; //STL(라이브러리): std 네임스페이스 소속
using namespace cv;

void Overlay(Mat &back, Mat front, int rows, int cols);


void Overlay(Mat &back, Mat front, int rows, int cols){
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (front.at<uchar>(i, j) == 0) continue;

			//linear interpolation
			back.at<Vec3b>(i, j)[0] = back.at<Vec3b>(i, j)[0] * 0.7 + 255 * 0; //b
			back.at<Vec3b>(i, j)[1] = back.at<Vec3b>(i, j)[1] * 0.7 + 255 * 0; //g
			back.at<Vec3b>(i, j)[2] = back.at<Vec3b>(i, j)[2] * 0.7 + 255 * 0.3; //r
		}
	}
}

int main(){
	//0. Absolute Path setting ==> 추후 경로값 저장해두기
	Mat ori = imread("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_default\\Breast0002.png", 0);
	Mat img_copy = ori.clone();

	//1. Bilateral Filtering (noise filtering)
	Mat filtered;
	bilateralFilter(img_copy, filtered, -1, 15, 15); //(src, dst, d(필터링 수행할 지름), sigmaColor(색 공간), sigmaSpace(거리 공간))
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_1_filtered.png", filtered);

	//2. Otsu's Thresholding
	Mat otsu;
	threshold(img_copy, otsu, 0, 255, THRESH_BINARY|THRESH_OTSU); 
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_2_otsu.png", otsu);

	/*
	//3. watershed : marker구하기(with. Erosion, Dilation, connectedComponents) -> watershed
	//Mat markers = Mat::zeros(img_copy.size(), CV_32S);
	watershed(img_copy, markers); //3채널만 가능...(error) ==> 3채널로 변환 or SRG로 수행
	*/

	//3. Morphology_preprocessing(remove outlines with erode)
	Mat pre_erode, pre_dilate;
	Mat mask = getStructuringElement(MORPH_RECT, Size(3,3), Point(1,1));
	erode(otsu, pre_erode, mask, Point(-1, -1), 6); // 6회이상 반복 시 완전히 사라짐...(0002)
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_3-1_pre_erode.png", pre_erode);
	dilate(pre_erode, pre_dilate, mask, Point(-1, -1), 6);
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_3-2_pre_dilate.png", pre_dilate);


	//4. Floodfill (combine background to select hole in body)
	Mat hole = pre_dilate.clone();
	floodFill(hole, Point(0,0), Scalar(255));
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_4_hole.png", hole);

	//4-1. Invert hole
	Mat hole_inv;
	bitwise_not(hole, hole_inv);
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_4-1_hole_inv.png", hole_inv);

	//5. bitwise OR (combine pre(bone) and hole)
	Mat bitor = (pre_dilate | hole_inv);
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_5_bitor.png", bitor);


	//6. overay
	Mat ori3C = imread("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_default\\Breast0002.png");
	Mat back = ori3C.clone();  //3 channel
	Mat front = bitor.clone(); //1 channel
	int rows = back.rows;
	int cols = back.cols;
	//printf("ori channels : %d\n", ori.channels());
	//printf("seg channels : %d\n", bitor.channels());
	Overlay(back, front, rows, cols);
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_6_overlay.png", back);

	//7. dilate (restore to original size)


	return 0;
}
