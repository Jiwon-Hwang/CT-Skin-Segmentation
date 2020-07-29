//������ϵ� include
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream> //cout, cin �Լ� ����ϱ� ���� (iostream Ŭ���� ���� ��ü)
#include "stdafx.h" //precompiled header (�̸� ������ �����ִ� ����, ��ü������ ���� �ð� ����)

//namespace : �ĺ���(����, �Լ��� ��)�� �Ҽ� ����(name space), ���� �̸��̾ �ٸ��� ���� �����ϰ� ����(�浹 ����), ���ξ� ���� ����
//*'���ӽ����̽�' 'std'�� cout, cin���� �Լ��� �ҼӵǾ��ְ�, 'iostream'�̶�� '�������'�� �� ������� �ۼ��Ǿ� ����
using namespace std; //STL(���̺귯��): std ���ӽ����̽� �Ҽ�
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
	//0. Absolute Path setting ==> ���� ��ΰ� �����صα�
	Mat ori = imread("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_default\\Breast0002.png", 0);
	Mat img_copy = ori.clone();

	//1. Bilateral Filtering (noise filtering)
	Mat filtered;
	bilateralFilter(img_copy, filtered, -1, 15, 15); //(src, dst, d(���͸� ������ ����), sigmaColor(�� ����), sigmaSpace(�Ÿ� ����))
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_1_filtered.png", filtered);

	//2. Otsu's Thresholding
	Mat otsu;
	threshold(img_copy, otsu, 0, 255, THRESH_BINARY|THRESH_OTSU); 
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_2_otsu.png", otsu);

	/*
	//3. watershed : marker���ϱ�(with. Erosion, Dilation, connectedComponents) -> watershed
	//Mat markers = Mat::zeros(img_copy.size(), CV_32S);
	watershed(img_copy, markers); //3ä�θ� ����...(error) ==> 3ä�η� ��ȯ or SRG�� ����
	*/

	//3. Morphology_preprocessing(remove outlines with erode)
	Mat pre_erode, pre_dilate;
	Mat mask = getStructuringElement(MORPH_RECT, Size(3,3), Point(1,1));
	erode(otsu, pre_erode, mask, Point(-1, -1), 6); // 6ȸ�̻� �ݺ� �� ������ �����...(0002)
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
