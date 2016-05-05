#include "mainwindow.h"
#include <QApplication>

#include <string>

#include <opencv/cv.h>

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

    auto predictions = classifier.Classify(imgs, topN);

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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
