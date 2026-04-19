#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main() {
    // 1. Загрузка 
    Mat img = imread("C:/Users/user/Desktop/markers.png");
    if (img.empty()) {
        cout << "Ошибка: не удалось загрузить файл!" << endl;
        return -1;
    }

    // ПРЕДОБРАБОТКА 
    Mat ycrcb;
    cvtColor(img, ycrcb, COLOR_BGR2YCrCb);

    vector<Mat> channels;
    split(ycrcb, channels);
    equalizeHist(channels[0], channels[0]); // Выравниваем гистограмму яркости

    Mat corrected;
    merge(channels, ycrcb);
    cvtColor(ycrcb, corrected, COLOR_YCrCb2BGR);

    // СЕГМЕНТАЦИЯ
    Mat hsv;
    cvtColor(corrected, hsv, COLOR_BGR2HSV);

    // Выбираем ЗЕЛЕНЫЙ маркер, диапазоны ( 35-85)
    Scalar lower_green(35, 100, 50);
    Scalar upper_green(85, 255, 255);

    Mat mask;
    inRange(hsv, lower_green, upper_green, mask);

    // ФИЛЬТРАЦИЯ 
    Mat result;
    img.copyTo(result, mask); 

    // ВИЗУАЛИЗАЦИЯ 
    imshow("1. Original", img);
    imshow("2. Corrected (Lec style)", corrected);
    imshow("3. Mask (Green Marker)", mask);
    imshow("4. Final Recognition", result);


    Mat lab;
    cvtColor(img, lab, COLOR_BGR2Lab);
    vector<Mat> labCh;
    split(lab, labCh);
    imshow("Lab - b channel (yellow/blue)", labCh[2]);

    cout << "Маркер успешно сегментирован. Нажмите любую клавишу..." << endl;
    waitKey(0);

    imwrite("recognized_marker.png", result);

    return 0;
}