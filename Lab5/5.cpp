#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

void processShape(Mat& canvas, const vector<Point>& figure) {
    double s = contourArea(figure);
    if (s < 300) return; // Игнорируем шум

    double p = arcLength(figure, true);
    double roundness = (4 * CV_PI * s) / (p * p);

    Rect bounds = boundingRect(figure);
    Point labelPos(bounds.x, bounds.y - 5); 

    // Определение круга 
    if (roundness > 0.7) {
        rectangle(canvas, bounds, Scalar(50, 50, 255), 2);
        putText(canvas, "Circle", labelPos, FONT_ITALIC, 0.5, Scalar(50, 50, 255), 1);
        return;
    }

    //Определение многоугольников
    vector<Point> poly;
    approxPolyDP(figure, poly, 0.07 * p, true);

    size_t nodes = poly.size();

    if (nodes == 3) {
        rectangle(canvas, bounds, Scalar(255, 100, 0), 2);
        putText(canvas, "Triangle", labelPos, FONT_ITALIC, 0.5, Scalar(255, 100, 0), 1);
    }
    else if (nodes == 4) {
        RotatedRect rRect = minAreaRect(figure);
        float sideA = rRect.size.width;
        float sideB = rRect.size.height;
        float factor = min(sideA, sideB) / max(sideA, sideB);

        if (factor > 0.85) {
            rectangle(canvas, bounds, Scalar(100, 255, 100), 2);
            putText(canvas, "Square", labelPos, FONT_ITALIC, 0.5, Scalar(100, 255, 100), 1);
        }
    }
}

int main() {
    VideoCapture stream("Motion abstract geometric shapes (Lab5).mkv");
    if (!stream.isOpened()) return -1;

    Mat raw, processed;
    const Size morphSize(3, 3);

    while (stream.read(raw)) {
        cvtColor(raw, processed, COLOR_BGR2GRAY);
        threshold(processed, processed, 230, 255, THRESH_BINARY_INV);

        Mat element = getStructuringElement(MORPH_RECT, morphSize);
        morphologyEx(processed, processed, MORPH_CLOSE, element);
        morphologyEx(processed, processed, MORPH_OPEN, element);

        vector<vector<Point>> elements;
        findContours(processed, elements, RETR_TREE, CHAIN_APPROX_SIMPLE);

        for (const auto& item : elements) {
            processShape(raw, item);
        }

        imshow("Output", raw);
        if (waitKey(30) == 27) break;
    }

    stream.release();
    destroyAllWindows();
    return 0;
}
