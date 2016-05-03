#include "opennetdialog.h"
#include "ui_opennetdialog.h"

#include <QFileDialog>

using namespace std;

OpenNetDialog::OpenNetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::openNetDialog)
    , isModelOpened(false)
    , isWeightsOpened(false)
    , filePaths_()
{
    ui->setupUi(this);

    connect(ui->pushButtonOpenModel, &QPushButton::clicked, this, &OpenNetDialog::openModelClicked);
    connect(ui->pushButtonOpenWeights, &QPushButton::clicked, this, &OpenNetDialog::openWeightsClicked);
    connect(ui->pushButtonOpenMeanImage, &QPushButton::clicked, this, &OpenNetDialog::openMeanImageClicked);
    connect(ui->pushButtonOpenLabels, &QPushButton::clicked, this, &OpenNetDialog::openLabelsClicked);

    filePaths_.resize(4);
}

OpenNetDialog::~OpenNetDialog()
{
    delete ui;
}

bool OpenNetDialog::isFilesChoosed() {
    return isModelOpened && isWeightsOpened;
}

vector<QString> OpenNetDialog::filePaths() {
    return filePaths_;
}

void OpenNetDialog::openModelClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Model"), QDir::homePath(), tr("(*.prototxt)"));

    if (!filename.isEmpty()) {
        filePaths_[0] = filename;
        isModelOpened = true;
    }
}

void OpenNetDialog::openWeightsClicked()
{

    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Trained Net"), QDir::homePath(), tr("(*.caffemodel)"));

    if (!filename.isEmpty()) {
        filePaths_[1] = filename;
        isWeightsOpened = true;
    }
}

void OpenNetDialog::openMeanImageClicked()
{

    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Mean Image File"), QDir::homePath(), tr("(*.binaryproto)"));

    if (!filename.isEmpty()) {
        filePaths_[2] = filename;
    }
}

void OpenNetDialog::openLabelsClicked()
{

    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Labels File"), QDir::homePath(), tr("(*.txt)"));

    if (!filename.isEmpty()) {
        filePaths_[3] = filename;
    }
}
