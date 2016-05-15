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

private:
    Ui::openNetDialog *ui;
    bool isModelOpened_;
    bool isWeightsOpened_;

    std::vector<QString> filePaths_;

    bool changeDir(const QString &path);
    QString getFilename(const QString &filepath);
};
