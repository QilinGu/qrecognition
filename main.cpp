#include "mainwindow.h"
#include <QApplication>

#include <string>
#include <iostream>

#include <opencv/cv.h>
#include <opencv2/xobjdetect.hpp>

#include "classifier.h"

using namespace std;
using namespace caffe;

void testPredict() {
    string work_dir = "/home/gkirg/projects/compvis/caffe/gtsrb/finetuning/";

    string model_file   = work_dir + "resnet/ResNet-152-deploy_ft96.prototxt";
    string trained_file = work_dir + "snapshots/resnet-152_ft96_train_iter_9001.caffemodel";
    string mean_file    = work_dir + "resnet/ResNet_mean.binaryproto";
//    string label_file   = work_dir + "gtsrb_labels.txt";

    Classifier classifier(model_file, trained_file, mean_file);

    /* Load images */
    int batch_size = 3;
    int topN = 3;
    vector<cv::Mat> imgs;
    for (int i = 0; i < batch_size; ++i) {
        string img_file = "/home/gkirg/projects/compvis/analyze/own_data/0000" + to_string(i+1) + ".ppm";
        cv::Mat img = cv::imread(img_file, -1);
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    trainDetector();

    return a.exec();
    return 0;
}
