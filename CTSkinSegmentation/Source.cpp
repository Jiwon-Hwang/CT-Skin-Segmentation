//������ϵ� include
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream> //cout, cin �Լ� ����ϱ� ���� (iostream Ŭ���� ���� ��ü)
#include "stdafx.h" //precompiled header (�̸� ������ �����ִ� ����, ��ü������ ���� �ð� ����)

//namespace : �ĺ���(����, �Լ��� ��)�� �Ҽ� ����(name space), ���� �̸��̾ �ٸ��� ���� �����ϰ� ����(�浹 ����), ���ξ� ���� ����
//*'���ӽ����̽�' 'std'�� cout, cin���� �Լ��� �ҼӵǾ��ְ�, 'iostream'�̶�� '�������'�� �� ������� �ۼ��Ǿ� ����
using namespace std; //STL(���̺귯��): std ���ӽ����̽� �Ҽ�
using namespace cv;

int main(){
	//0. Absolute Path setting ==> ���� ��ΰ� �����صα�
	Mat ori = imread("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_default\\Breast0067.png", 0);
	Mat img_copy = ori.clone();

	//1. Bilateral Filtering (noise filtering)
	Mat filtered;
	bilateralFilter(img_copy, filtered, -1, 15, 15); //(src, dst, d(���͸� ������ ����), sigmaColor(�� ����), sigmaSpace(�Ÿ� ����))
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0067_filtered.png", filtered);

	//2. Otsu's Thresholding
	Mat otsu;
	threshold(img_copy, otsu, 0, 255, THRESH_BINARY|THRESH_OTSU); 
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0067_otsu.png", otsu);

	/*
	//3. watershed : marker���ϱ�(with. Erosion, Dilation, connectedComponents) -> watershed
	//Mat markers = Mat::zeros(img_copy.size(), CV_32S);
	watershed(img_copy, markers); //3ä�θ� ����...(error) ==> 3ä�η� ��ȯ or SRG�� ����
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0067_watershed.png", markers);
	*/

	//3. Floodfil
	Mat floodfill = otsu.clone();
	floodFill(floodfill, Point(0,16), Scalar(0)); //from point (0,0)
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0067_floodfill.png", floodfill);

	


	return 0;
}
