#include "caffenetbuilder.h"

#include <string>

#include <QString>

#include "classifier.h"

CaffeNetBuilder::CaffeNetBuilder()
    : dialog_()
{

}

AbstractClassifier *CaffeNetBuilder::buildClassifier() {
    dialog_.exec();

    if (dialog_.result() == QDialog::Accepted && dialog_.isFilesChoosed()) {
        auto paths = dialog_.filePaths();
        assert(paths.size() >= 3);
        std::string model = paths[0].toStdString();
        std::string weights = paths[1].toStdString();
        std::string mean_img = paths[2].toStdString();

        return new Classifier(model, weights, mean_img);
    } else {
        return nullptr;
    }
}
