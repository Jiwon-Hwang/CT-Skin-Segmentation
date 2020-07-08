//헤더파일들 include
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream> //cout, cin 함수 사용하기 위해 (iostream 클래스 내의 객체)

//namespace : 식별자(변수, 함수명 등)의 소속 공간(name space), 같은 이름이어도 다르게 구분 가능하게 해줌(충돌 방지), 접두어 생략 가능
//*'네임스페이스' 'std'에 cout, cin같은 함수가 소속되어있고, 'iostream'이라는 '헤더파일'에 이 내용들이 작성되어 있음
using namespace std; //STL(라이브러리): std 네임스페이스 소속
using namespace cv;


int main(){
	Mat img = imread("test.png");
	imshow("test", img);
	waitKey(0);
	destroyAllWindows();

	return 0;
}