#pragma once

#include <vector>

#include <QDialog>
#include <QString>

namespace Ui {
class openNetDialog;
}

class OpenNetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenNetDialog(QWidget *parent = 0);
    ~OpenNetDialog();

    std::vector<QString> filePaths();
    bool isFilesChoosed();

private slots:
    void openModelClicked();
    void openWeightsClicked();
    void openMeanImageClicked();
    void openLabelsClicked();

private:
    Ui::openNetDialog *ui;
    bool isModelOpened;
    bool isWeightsOpened;

    std::vector<QString> filePaths_;
};
