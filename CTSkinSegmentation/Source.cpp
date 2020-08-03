#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream> 
#include "stdafx.h" //precompiled header (미리 컴파일 시켜주는 역할, 전체적으로 빌드 시간 감소)

#include <Windows.h>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

typedef wstring str_t;

vector<str_t> get_files_in_floder(str_t folder, str_t file_type = L"*.*"); // L : wchar_t의 wide character.. 접두어..

void Overlay(Mat &back, Mat front, int rows, int cols);


vector<str_t> get_files_in_floder(str_t folder, str_t file_type){
	vector<str_t> names;
	wchar_t search_path[200];
	wsprintf(search_path, L"%s/%s", folder.c_str(), file_type.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if(hFind != INVALID_HANDLE_VALUE){
		do{
			//read all (real) files in current folder
			//, delete '!' read other 2 default folder. and ..
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
				names.push_back(fd.cFileName);
			}
		}while(::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}


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
	str_t path = L"C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_default";
	vector<str_t> files = get_files_in_floder(path, L"*.png");
	//wcout<<files[0]<<L"\n"<< files.size()<<L"\n"; //==> Breast0002.png, 159
	
	for (auto f : files){
		//0. Absolute Path setting ==> 추후 경로값 저장해두기
		string f_str, path_str;
		f_str.assign(f.begin(), f.end());
		path_str.assign(path.begin(), path.end());

		string combpath_str = path_str + "\\" + f_str;
		
		Mat ori = imread(combpath_str, 0); //2channel
		int rows = ori.rows;
		int cols = ori.cols;
		Mat img_copy = ori.clone();
	
		//1. Bilateral Filtering (noise filtering)
		Mat filtered;
		bilateralFilter(img_copy, filtered, -1, 15, 15); //(src, dst, d(필터링 수행할 지름), sigmaColor(색 공간), sigmaSpace(거리 공간))
		//imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_1_filtered.png", filtered);

		//2. Otsu's Thresholding
		Mat otsu;
		threshold(img_copy, otsu, 0, 255, THRESH_BINARY|THRESH_OTSU); 
		//imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_2_otsu.png", otsu);


		//3. Morphology_preprocessing(remove outlines with erode)
		Mat pre_erode, pre_dilate;
		Mat mask = getStructuringElement(MORPH_RECT, Size(3,3), Point(1,1));
		erode(otsu, pre_erode, mask, Point(-1, -1), 6); // 6회이상 반복 시 완전히 사라짐...(0002)
		//imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_3-1_pre_erode.png", pre_erode);
		dilate(pre_erode, pre_dilate, mask, Point(-1, -1), 6);
		//imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_3-2_pre_dilate.png", pre_dilate);


		//4. Floodfill (combine background to select hole in body)
		Mat hole = pre_dilate.clone();
		floodFill(hole, Point(0,0), Scalar(255));
		floodFill(hole, Point(cols-1, rows-1), Scalar(255)); // for 0010~0047 seg error clear
		//imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_4_hole.png", hole);

		//4-1. Invert hole
		Mat hole_inv;
		bitwise_not(hole, hole_inv);
		//imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_4-1_hole_inv.png", hole_inv);

		//5. bitwise OR (combine pre(bone) and hole)
		Mat bitor = (pre_dilate | hole_inv);
		//imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\Breast0002_5_bitor.png", bitor);


		//6. overay
		Mat ori3C = imread(combpath_str);
		Mat back = ori3C.clone();  //3 channel
		Mat front = bitor.clone(); //1 channel
		Overlay(back, front, rows, cols);
		imwrite("C:\\Users\\Ryu\\Desktop\\200707_CTSkinSegmentation_SRC\\img_result\\final\\"+f_str, back);
	}
		

	return 0;
}
