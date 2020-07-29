//헤더파일들 include
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream> //cout, cin 함수 사용하기 위해 (iostream 클래스 내의 객체)
#include "stdafx.h" //precompiled header (미리 컴파일 시켜주는 역할, 전체적으로 빌드 시간 감소)

//namespace : 식별자(변수, 함수명 등)의 소속 공간(name space), 같은 이름이어도 다르게 구분 가능하게 해줌(충돌 방지), 접두어 생략 가능
//*'네임스페이스' 'std'에 cout, cin같은 함수가 소속되어있고, 'iostream'이라는 '헤더파일'에 이 내용들이 작성되어 있음
using namespace std; //STL(라이브러리): std 네임스페이스 소속
using namespace cv;

int main(){
	//0. Absolute Path setting ==> 추후 경로값 저장해두기
	Mat ori = imread("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_default\\Breast0067.png", 0);
	Mat img_copy = ori.clone();

	//1. Bilateral Filtering (noise filtering)
	Mat filtered;
	bilateralFilter(img_copy, filtered, -1, 15, 15); //(src, dst, d(필터링 수행할 지름), sigmaColor(색 공간), sigmaSpace(거리 공간))
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0067_filtered.png", filtered);

	//2. Otsu's Thresholding
	Mat otsu;
	threshold(img_copy, otsu, 0, 255, THRESH_BINARY|THRESH_OTSU); 
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0067_otsu.png", otsu);

	/*
	//3. watershed : marker구하기(with. Erosion, Dilation, connectedComponents) -> watershed
	//Mat markers = Mat::zeros(img_copy.size(), CV_32S);
	watershed(img_copy, markers); //3채널만 가능...(error) ==> 3채널로 변환 or SRG로 수행
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0067_watershed.png", markers);
	*/

	//3. Floodfil
	Mat floodfill = otsu.clone();
	floodFill(floodfill, Point(0,16), Scalar(0)); //from point (0,0)
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0067_floodfill.png", floodfill);

	


	return 0;
}
