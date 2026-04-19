#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main() {

    Mat canvas(600, 800, CV_8UC3, Scalar(30, 30, 30));

    // Параметры анимации текста
    int tx = 150;
    int ty = 150;
    int state = 0;
    int step = 4; // Скорость 

    string my_name = "Vadim Dobrovolskiy ";

    cout << "Эск чтобы выйти" << endl;

    while (true) {
        canvas = Scalar(30, 30, 30);

        // ЛИНИЯ 
        line(canvas, Point(50, 550), Point(250, 450), Scalar(180, 100, 255), 3);


        // Синий прямоугольник
        rectangle(canvas, Point(50, 50), Point(200, 200), Scalar(255, 100, 50), 2);

        // ОКРУЖНОСТЬ
        circle(canvas, Point(650, 450), 70, Scalar(0, 140, 255), -1);

        // ЭЛЛИПС повернут на 30
        ellipse(canvas, Point(400, 300), Size(150, 80), 30, 0, 360, Scalar(100, 255, 100), 2);

        // Пятиугольник
        vector<Point> pts;
        pts.push_back(Point(500, 50));
        pts.push_back(Point(700, 80));
        pts.push_back(Point(750, 200));
        pts.push_back(Point(600, 250));
        pts.push_back(Point(520, 180));


        polylines(canvas, pts, true, Scalar(0, 255, 255), 3);

        // АНИМАЦИЯ ТЕКСТА 
        if (state == 0) { // Идем вправо
            tx += step;
            if (tx >= 500) state = 1;
        }
        else if (state == 1) { // Идем вниз
            ty += step;
            if (ty >= 450) state = 2;
        }
        else if (state == 2) { // Идем влево
            tx -= step;
            if (tx <= 150) state = 3;
        }
        else if (state == 3) { // Идем вверх
            ty -= step;
            if (ty <= 150) state = 0;
        }

        // Текст
        putText(canvas, my_name, Point(tx, ty),
            FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255), 2);

        // Результат
        imshow("Graphics Assignment", canvas);

        // ESC
        if (waitKey(20) == 27) break;
    }

    // Сохранение 
    imwrite("student_work_final.jpg", canvas);

    destroyAllWindows();
    return 0;
}
