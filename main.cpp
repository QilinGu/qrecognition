#include "mainwindow.h"
#include <QApplication>

#include <string>
#include <iostream>

#include <opencv/cv.h>
#include <opencv2/xobjdetect.hpp>

#include "classifier.h"

#include <QDebug>
#include <QImage>

using namespace std;
using namespace caffe;

// take number image type number (from cv::Mat.type()), get OpenCV's enum string.
string getImgType(int imgTypeInt)
{
    int numImgTypes = 35; // 7 base types, with five channel options each (none or C1, ..., C4)

    int enum_ints[] =       {CV_8U,  CV_8UC1,  CV_8UC2,  CV_8UC3,  CV_8UC4,
                             CV_8S,  CV_8SC1,  CV_8SC2,  CV_8SC3,  CV_8SC4,
                             CV_16U, CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
                             CV_16S, CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
                             CV_32S, CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
                             CV_32F, CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
                             CV_64F, CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4};

    string enum_strings[] = {"CV_8U",  "CV_8UC1",  "CV_8UC2",  "CV_8UC3",  "CV_8UC4",
                             "CV_8S",  "CV_8SC1",  "CV_8SC2",  "CV_8SC3",  "CV_8SC4",
                             "CV_16U", "CV_16UC1", "CV_16UC2", "CV_16UC3", "CV_16UC4",
                             "CV_16S", "CV_16SC1", "CV_16SC2", "CV_16SC3", "CV_16SC4",
                             "CV_32S", "CV_32SC1", "CV_32SC2", "CV_32SC3", "CV_32SC4",
                             "CV_32F", "CV_32FC1", "CV_32FC2", "CV_32FC3", "CV_32FC4",
                             "CV_64F", "CV_64FC1", "CV_64FC2", "CV_64FC3", "CV_64FC4"};

    for(int i=0; i<numImgTypes; i++)
    {
        if(imgTypeInt == enum_ints[i]) return enum_strings[i];
    }
    return "unknown image type";
}

void testPredict() {
    string work_dir = "/home/gkirg/projects/compvis/caffe/gtsrb/finetuning/";

    string model_file   = work_dir + "resnet/ResNet-152-deploy_ft96.prototxt";
    string trained_file = work_dir + "snapshots/resnet-152_ft96_train_iter_9001.caffemodel";
    string mean_file    = work_dir + "resnet/ResNet_mean.binaryproto";
//    string label_file   = work_dir + "gtsrb_labels.txt";

    Classifier classifier(model_file, trained_file, mean_file);

    /* Load images */
    int batch_size = 1;
    int topN = 3;
    vector<cv::Mat> imgs;
    for (int i = 0; i < batch_size; ++i) {
        string img_file = "/home/gkirg/projects/compvis/analyze/own_data/0000" + to_string(i+1) + ".ppm";

//        cv::Mat img = cv::imread(img_file, -1);
        cv::VideoCapture cap(img_file.c_str());
        cv::Mat img;
        cap.read(img);

        CHECK(!img.empty()) << "Unable to decode image " << img_file;
        imgs.push_back(img);
    }

    auto predictions = classifier.classify(imgs, topN);

    /* Print the topN predictions for every image */
    for (unsigned int j = 0; j < predictions.size(); ++j) {
        for (int i = 0; i < topN; ++i) {
            Prediction p = predictions[j][i];
            cout << "img " << j << ": "
                 << std::fixed << std::setprecision(4) << p.second << " - " << p.first << endl;
        }
        cout << endl;
    }
}

void testDetect() {
//    img 0: 0.9933 - 38
//    img 0: 0.0031 - 34
//    img 0: 0.0029 - 39

//    img 1: 0.7361 - 38
//    img 1: 0.1314 - 34
//    img 1: 0.1292 - 39

//    img 2: 0.8269 - 2
//    img 2: 0.0862 - 0
//    img 2: 0.0308 - 39
}

void trainDetector() {

//    cv::String pos_list("/home/gkirg/projects/TSRDApp/TSRDApp/train/gtsrb_imageset.txt");
//    cv::String neg_list("/home/gkirg/projects/TSRDApp/TSRDApp/train/bg_imageset.txt");
//    ifstream p(pos_list);
//    ifstream n(neg_list);

//    vector<cv::String> pos_files;
//    string buf;
//    while(getline(p, buf)) {
//        pos_files.push_back(cv::String(buf));
//    }

//    vector<cv::String> neg_files;
//    while(getline(n, buf)) {
//        neg_files.push_back(cv::String(buf));
//    }

//    cout << pos_files.size() << " " << neg_files.size() << endl;

    auto neg_dir = "/media/Documents/datasets/bts/BelgiumTS/NonTSImages/NonTSImages/TestingBG/"; //583
    auto pos_dir = "/media/Documents/datasets/GTSRB/temp/"; //1003

    auto dt = cv::xobjdetect::WBDetector::create();
    dt->train(pos_dir, neg_dir);

    cv::FileStorage fs(cv::String("/home/gkirg/projects/TSRDApp/TSRDApp/train/dt1.xml"), cv::FileStorage::WRITE);
    dt->write(fs);

}

void testBackend() {
    string ifile = "/home/gkirg/projects/compvis/analyze/own_data/00001.ppm";
    string vfile = "/media/Videos/Golodnye_igry_Soika_2.avi";
    string tifile = "/media/Documents/datasets/GTSRB/Final_Training/Images/00002/00000_00028.ppm";
    cv::VideoCapture cap(ifile.c_str());

    cv::Mat i1 = cv::imread(ifile.c_str());
    cv::Mat c1;

    cap.read(c1);

    qDebug() << "i1 size " << i1.cols << i1.rows;
    qDebug() << "c1 size " << c1.cols << c1.rows;

    auto diff = c1 != i1;
    auto countdiff = cv::countNonZero(diff);
    qDebug() << "Diff is " << countdiff;

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    testBackend();
//    testPredict();
//    trainDetector();

    return a.exec();
//    return 0;
}
