//������ϵ� include
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream> //cout, cin �Լ� ����ϱ� ���� (iostream Ŭ���� ���� ��ü)

//namespace : �ĺ���(����, �Լ��� ��)�� �Ҽ� ����(name space), ���� �̸��̾ �ٸ��� ���� �����ϰ� ����(�浹 ����), ���ξ� ���� ����
//*'���ӽ����̽�' 'std'�� cout, cin���� �Լ��� �ҼӵǾ��ְ�, 'iostream'�̶�� '�������'�� �� ������� �ۼ��Ǿ� ����
using namespace std; //STL(���̺귯��): std ���ӽ����̽� �Ҽ�
using namespace cv;


int main(){
	Mat img = imread("test.png");
	imshow("test", img);
	waitKey(0);
	destroyAllWindows();

	return 0;
}