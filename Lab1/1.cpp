#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image = imread("C:/Users/user/Desktop/IMG_4256.PNG");

    if (image.empty())
    {
        cout << "Ошибка открытия файла" << endl;
        return -1;
    }

    imshow("Original", image);

    // Изменение размера
    Mat img_resized;
    resize(image, img_resized, Size(), 0.5, 0.5);
    imshow("Razmer", img_resized);

    // Поворот
    Mat img_rotated;
    Point2f center(image.cols / 2.0, image.rows / 2.0);
    Mat rot = getRotationMatrix2D(center, 45, 1.0);
    warpAffine(image, img_rotated, rot, image.size());
    imshow("Povorot", img_rotated);

    // Серое
    Mat img_gray;
    cvtColor(image, img_gray, COLOR_BGR2GRAY);
    imshow("Gray", img_gray);

    // Поворот 90
    Mat img_rot90;
    rotate(image, img_rot90, ROTATE_90_CLOCKWISE);
    imshow("Povorot90", img_rot90);

    // Размытие
    Mat img_blur;
    GaussianBlur(image, img_blur, Size(15, 15), 0);
    imshow("Blur", img_blur);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
