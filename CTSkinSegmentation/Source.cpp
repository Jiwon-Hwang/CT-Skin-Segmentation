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
	Mat ori = imread("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_default\\Breast0002.png");
	Mat img_copy = ori.clone();
	Mat filtered;
	bilateralFilter(img_copy, filtered, -1, 15, 15); //(src, dst, d(���͸� ������ ����), sigmaColor(�� ����), sigmaSpace(�Ÿ� ����))
	imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_filtered.png", filtered);

	return 0;
}
