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
	Mat img = imread("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_default\\Breast0002.png");
	imshow("B0002", img);
	waitKey(0);
	destroyAllWindows();

	return 0;
}
