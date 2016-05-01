#include "mainwindow.h"
#include <QApplication>

#include <string>

#include <opencv/cv.h>

#include "classifier.h"

using namespace std;
using namespace caffe;

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    string work_dir = "/home/gkirg/projects/compvis/caffe/gtsrb/finetuning/";

    string model_file   = work_dir + "resnet/ResNet-152-deploy_ft96.prototxt";
    string trained_file = work_dir + "snapshots/resnet-152_ft96_train_iter_9001.caffemodel";
    string mean_file    = work_dir + "resnet/ResNet_mean.binaryproto";
    string label_file   = work_dir + "gtsrb_labels.txt";

    Classifier classifier(model_file, trained_file, mean_file, label_file);

    string img_file = "/home/gkirg/projects/compvis/analyze/own_data/00005.ppm";

    cv::Mat img = cv::imread(img_file, -1);
    CHECK(!img.empty()) << "Unable to decode image " << img_file;
    auto predictions = classifier.Classify(img, 5);

    /* Print the top N predictions. */
    for (size_t i = 0; i < predictions.size(); ++i) {
        Prediction p = predictions[i];
        cout << std::fixed << std::setprecision(4) << p.second << " - " << p.first << endl;
    }

//    return a.exec();
    return 0;
}
