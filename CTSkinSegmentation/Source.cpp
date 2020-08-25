#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream> 
#include "stdafx.h" //precompiled header (�̸� ������ �����ִ� ����, ��ü������ ���� �ð� ����)

#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/config/osconfig.h" 
#include "dcmtk/dcmdata/dctk.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <typeinfo>

#include <strsafe.h>
#define MAX_PATH 260


using namespace std;
using namespace cv;

typedef wstring str_t;

Mat DcmToMat(const char *path);
vector<string> get_files_in_floder(TCHAR *folder); // L : wchar_t�� wide character.. ���ξ�..
void Overlay(Mat &back, Mat front, int rows, int cols);


Mat DcmToMat(const char *path){
	//cout<<"hi!"<<'\n'; // �� �Լ��� �������� ����.. ..?
	DicomImage *image = new DicomImage(path); // string(x) -> const char * ������ �ٲٴϱ� error clear
	const int width = (int)(image->getWidth());
	const int height = (int)(image->getHeight());
	if (image != NULL){
		if (image->getStatus() == EIS_Normal){ //normal, no error (image status code)
			Uint8 *pixelData = (Uint8 *)(image->getOutputData(8)); //bits per sample ==> Q. short(16bit)��..? (Uint8 : unsigned char.. 0~255.. 8bit)
			if (pixelData != NULL){ //cout << "type is : " << typeid(pixelData).name() << '\n'; //unsigned char * __ptr64
				// do something useful with the pixel data //

				Mat temp = Mat(width, height, CV_8UC1);
				//temp.data = pixelData; // pixel data to Mat ==> cause error!!

				//�Ʒ� ���� ���� ���̹Ƿ� �ܿ��!
				for(int row=0; row< height; row++) {
					for(int col=0; col< width; col++) {
						
						int idx = row*width + col; // 2�����迭�� 1���� �迭ó�� ����
						//temp.at<uchar>(row, col) = pixelData[idx]; ==> at�Լ� : ������ good, but �ð� �����ɸ� (ȿ��x)
						temp.data[idx] = (uchar) pixelData[idx]; // ==> �̷��� ���� �޸𸮿� �����ϴ� ������� ����ϱ�!
						

						/* �̰� ���� ��Ȯ�� ���! (dicom image�� mat ���� �ȼ� �ٸ��� �ε����� �����ؾߵ�!)
						int idx_dicomimage = row*width + col;
						int idx_mat = row*temp.cols + col;

						temp.data[idx_mat] = (uchar) pixelData[idx_dicomimage];
						*/
					}
				}
				
				delete image;
				return temp;
			}
		}
		else{
			cout << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
		}
	}
}


vector<string> get_files_in_floder(TCHAR *folder){
	vector<string> names;
	//wchar_t search_path[200];
	//wsprintf(search_path, L"%s/%s", folder.c_str(), file_type.c_str());
	
	//char search_path[200];
	//TCHAR search_path[MAX_PATH];
	
	//sprintf(search_path, "%s\\%s\\*.*", folder.c_str(), file_type.c_str());

	WIN32_FIND_DATA fd;
	StringCchCat(folder, MAX_PATH, TEXT("\\*.dcm")); // Ư�� Ȯ���ڸ� ���
	HANDLE hFind = FindFirstFile(folder, &fd);
	cout << hFind << '\n';
	if(hFind != INVALID_HANDLE_VALUE){ // ==> Q. ���� if�� �ȵ���...
		do{
			//read all (real) files in current folder
			//, delete '!' read other 2 default folder. and ..
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)){ // FILE_ATTRIBUTE_ARCHIVE : ���ϸ� �˻� (���� ����)
				cout<<fd.cFileName<<endl; // ���⵵ �ȵ���...
				names.push_back(fd.cFileName);
			}
		}while(::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

// at �����ϱ�!
void Overlay(Mat &back, Mat front, int rows, int cols){
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int idx = i*cols*3 + j*3;  // 3 channel indexing
			if (front.data[i*cols+j] == 0) continue; // front : 1 channel ==> ���Ⱑ ������ ��������!! (warning C4715: 'DcmToMat' : not all control paths return a value)

			//linear interpolation
			back.data[idx+0] = uchar(back.data[idx+0] * 0.7 + 255 * 0); //b
			back.data[idx+1] = uchar(back.data[idx+1] * 0.7 + 255 * 0); //g
			back.data[idx+2] = uchar(back.data[idx+2] * 0.7 + 255 * 0.3); //r
		}
	}
}



int main(){
	//Mat ori = DcmToMat("C:\\Users\\Ryu\\Desktop\\180509_SampleData_CT\\CT0002.dcm");
	//imwrite("C:\\Users\\Ryu\\Desktop\\180509_SampleData_CT\\result\\test.png", ori);
	
	TCHAR path[MAX_PATH] = "C:\\Users\\Ryu\\Desktop\\180509_SampleData_CT";
	string str_path = string(path);
	vector<string> files = get_files_in_floder(path);
	//wcout<<files[0]<<L"\n"<< files.size()<<L"\n"; //==> Breast0002.png, 159
	
	cout<<files.size()<<'\n'; //==> 0..? ==> 193!

	for (auto f : files){
		//0. Absolute Path setting
		/*
		string f_str, path_str;
		f_str.assign(f.begin(), f.end()); // wstring(str_t) to string ==> ���� �̰� ��ȯ ���� �ʿ� ���� ��..?
		path_str.assign(path.begin(), path.end()); 
		*/

		//cout<<string(path)<<endl; // C:\Users\Ryu\Desktop\180509_SampleData_CT\*.dcm ==> ? get_files_in_floder ���Ŀ� path�� ����...!
		//cout<<str_path<<endl; // �̰� ����! (get~ �Լ� ���� path�� string���� �����س���)
		//cout<<f<<endl; // CT0002.dcm

		string combpath_str = str_path + "\\" + f;
		cout<<combpath_str<<endl;
		
		//Mat ori = imread(combpath_str, 0); //2channel
		Mat ori = DcmToMat(combpath_str.c_str()); // Q. ��ä��? ==> 1ä��! (cout<<ori.channels()<<endl;)
		int rows = ori.rows;
		int cols = ori.cols;
		Mat img_copy = ori.clone();
		//imwrite("C:\\Users\\Ryu\\Desktop\\180509_SampleData_CT\\result\\test1.png", img_copy);
	
		//1. Bilateral Filtering (noise filtering)
		Mat filtered;
		bilateralFilter(img_copy, filtered, -1, 15, 15); //(src, dst, d(���͸� ������ ����), sigmaColor(�� ����), sigmaSpace(�Ÿ� ����))
		
		//2. Otsu's Thresholding
		Mat otsu;
		threshold(filtered, otsu, 0, 255, THRESH_BINARY|THRESH_OTSU); 

		//3. Morphology_preprocessing(remove outlines with erode)
		Mat pre_erode, pre_dilate;
		Mat mask = getStructuringElement(MORPH_RECT, Size(3,3), Point(1,1));
		erode(otsu, pre_erode, mask, Point(-1, -1), 7); // 6ȸ�̻� �ݺ� �� ������ �����...(0002) ==> dcm �̹����� 7ȸ�̻� �ؾ��� 0004������ ó�� �Ϸ�!
		dilate(pre_erode, pre_dilate, mask, Point(-1, -1), 7);

		//4. Floodfill (combine background to select hole in body)
		Mat hole = pre_dilate.clone();
		floodFill(hole, Point(0,0), Scalar(255));
		floodFill(hole, Point(cols-1, rows-1), Scalar(255)); 

		//5. Invert hole
		Mat hole_inv;
		bitwise_not(hole, hole_inv);

		//6. bitwise OR (combine pre(bone) and hole)
		Mat bitor = (pre_dilate | hole_inv);

		//7. overlay
		Mat back = ori.clone();  //1 channel
		Mat back3C;
		cvtColor(back, back3C, CV_GRAY2BGR); //3 channel
		//imwrite("C:\\Users\\Ryu\\Desktop\\180509_SampleData_CT\\result\\ori.png", back);
		//imwrite("C:\\Users\\Ryu\\Desktop\\180509_SampleData_CT\\result\\ori3C.png", back3C); ==> �� ���� ����! (1c to 3c)
		Mat front = bitor.clone(); //1 channel
		Overlay(back3C, front, rows, cols);
		imwrite("C:\\Users\\Ryu\\Desktop\\180509_SampleData_CT\\result\\"+f+".png", back3C);
	}
	
	return 0;
}
