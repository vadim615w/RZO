#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    // 1. Инициализация классификаторов
    CascadeClassifier faceSvm, eyeSvm, smileSvm;

    // Загрузка каскадов с проверкой
    if (!faceSvm.load("haarcascade_frontalface_default.xml") ||
        !eyeSvm.load("haarcascade_eye.xml") ||
        !smileSvm.load("haarcascade_smile.xml")) {
        cerr << "Ошибка: не удалось загрузить один из XML файлов каскада!" << endl;
        return -1;
    }

    // 2. Открытие видеофайла
    VideoCapture stream("ZUA (Lab6).mp4"); // Укажите верное расширение файла
    if (!stream.isOpened()) return -1;

    Mat frame, gray;

    while (stream.read(frame)) {
        // Подготовка изображения (перевод в ч/б и выравнивание гистограммы для точности)
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        // Поиск лиц
        vector<Rect> faces;
        faceSvm.detectMultiScale(gray, faces, 1.15, 4, 0, Size(100, 100));

        for (const auto& fBox : faces) {
            // Отрисовка лица: Квадрат (Синий)
            rectangle(frame, fBox, Scalar(255, 0, 0), 2);
            putText(frame, "Face", Point(fBox.x, fBox.y - 10), FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 0, 0));

            // РАБОТА С ROI (Область лица)
            // Создаем мини-матрицы для поиска внутри лица
            Mat roiGray = gray(fBox);
            Mat roiColor = frame(fBox);

            // Поиск глаз внутри ROI лица
            vector<Rect> eyes;
            eyeSvm.detectMultiScale(roiGray, eyes, 1.1, 15, 0, Size(20, 20));
            for (const auto& eBox : eyes) {
                // Отрисовка глаз: Круг (Желтый)
                Point center(eBox.x + eBox.width / 2, eBox.y + eBox.height / 2);
                int radius = cvRound((eBox.width + eBox.height) * 0.25);
                circle(roiColor, center, radius, Scalar(0, 255, 255), 2);
            }

            // Поиск улыбки внутри ROI лица
            vector<Rect> smiles;
            // Увеличиваем minNeighbors (до 25-30), чтобы избежать ложных срабатываний
            smileSvm.detectMultiScale(roiGray, smiles, 1.8, 25, 0, Size(30, 30));
            for (const auto& sBox : smiles) {
                // Отрисовка улыбки: Квадрат (Зеленый)
                rectangle(roiColor, sBox, Scalar(0, 255, 0), 1);
            }
        }

        imshow("Face Analysis Suite", frame);

        // Выход при нажатии Esc
        if (waitKey(30) == 27) break;
    }

    stream.release();
    destroyAllWindows();
    return 0;
}