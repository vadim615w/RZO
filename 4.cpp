#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// Функция для расчета косинуса угла (чтобы проверить, прямой он или нет)
double getCos(Point a, Point b, Point c) {
    long long dx1 = a.x - c.x;
    long long dy1 = a.y - c.y;
    long long dx2 = b.x - c.x;
    long long dy2 = b.y - c.y;
    return (dx1 * dx2 + dy1 * dy2) / (sqrt(dx1 * dx1 + dy1 * dy1) * sqrt(dx2 * dx2 + dy2 * dy2) + 1e-10);
}

string checkForm(const vector<Point>& p, const vector<Point>& original) {
    int sz = p.size();

    if (sz == 3) return "Treugolnik";

    if (sz == 4) {
        // Считаем длины сторон
        vector<double> d;
        for (int i = 0; i < 4; i++) d.push_back(norm(p[i] - p[(i + 1) % 4]));
        sort(d.begin(), d.end());

        // Проверяем, равны ли стороны (макс / мин < 1.2)
        bool sidesEqual = d[3] / d[0] < 1.2;

        // Проверяем угол (косинус 90 градусов близок к 0)
        double cosAngle = abs(getCos(p[0], p[2], p[1]));
        bool is90 = cosAngle < 0.2;

        if (sidesEqual && is90) return "Kvadrat";
        if (sidesEqual && !is90) return "Romb";
        return "Pryamougolnik";
    }

    if (sz == 5) return "Pyatiugolnik";
    if (sz == 6) return "Shestiugolnik";

    if (sz > 6) {
        RotatedRect box = minAreaRect(original);
        float f = min(box.size.width, box.size.height) / max(box.size.width, box.size.height);
        if (f > 0.9) return "Krug";
        return "Oval";
    }
    return "Mnogougolnik";
}

int main() {
    Mat raw_img = imread("Lab4_image.png");
    if (raw_img.empty()) return -1;

    Mat work, draw = raw_img.clone();
    cvtColor(raw_img, work, COLOR_BGR2GRAY);
    GaussianBlur(work, work, Size(5, 5), 0);
    Canny(work, work, 50, 150);

    // Утолщаем линии Кэнни (лекция: морфология)
    Mat str_el = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(work, work, MORPH_CLOSE, str_el);

    vector<vector<Point>> conts;
    findContours(work, conts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < conts.size(); i++) {
        if (contourArea(conts[i]) < 500) continue;

        vector<Point> poly;
        double length = arcLength(conts[i], true);
        approxPolyDP(conts[i], poly, 0.02 * length, true);

        // Определяем название и периметр
        string name = checkForm(poly, conts[i]);
        int p_val = (int)arcLength(poly, true);

        // Поиск центра масс через моменты (как в лекции)
        Moments mnt = moments(conts[i]);
        Point center(mnt.m10 / mnt.m00, mnt.m01 / mnt.m00);

        // Рисование контуров
        drawContours(draw, conts, i, Scalar(0, 255, 0), 2);
        circle(draw, center, 3, Scalar(0, 0, 255), -1);

        // Текст с плашкой
        string label = name + " P=" + to_string(p_val);
        Size sz = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, 0);
        Point pos(center.x - sz.width / 2, center.y + sz.height / 2);

        rectangle(draw, pos + Point(-2, 2), pos + Point(sz.width + 2, -sz.height - 2), Scalar(255, 255, 255), -1);
        putText(draw, label, pos, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1);

        cout << name << " P=" << p_val << endl;
    }

    imshow("Result", draw);
    imwrite("Lab4_Result.png", draw); // Сохраняем итог
    waitKey(0);
    return 0;
}