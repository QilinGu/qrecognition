#include "opennetdialog.h"
#include "ui_opennetdialog.h"

#include <QFileDialog>

using namespace std;

OpenNetDialog::OpenNetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::openNetDialog)
    , isModelOpened_(false)
    , isWeightsOpened_(false)
    , filePaths_()
{
    ui->setupUi(this);

    connect(ui->pushButtonOpenModel, &QPushButton::clicked, this, &OpenNetDialog::openModelClicked);
    connect(ui->pushButtonOpenWeights, &QPushButton::clicked, this, &OpenNetDialog::openWeightsClicked);
    connect(ui->pushButtonOpenMeanImage, &QPushButton::clicked, this, &OpenNetDialog::openMeanImageClicked);

    filePaths_.resize(4);
    QDir::setCurrent(QDir::homePath());
}

OpenNetDialog::~OpenNetDialog()
{
    delete ui;
}

bool OpenNetDialog::changeDir(const QString &path) {
    if (!path.isNull()) {
        QString dir_path = path.left( path.lastIndexOf('/') + 1 );
        QDir dir(dir_path);
        if (dir.isReadable()) {
            QDir::setCurrent(dir.absolutePath());
            return true;
        }
    }
    return false;
}

QString OpenNetDialog::getFilename(const QString &filepath) {
    int name_length = filepath.length() - filepath.lastIndexOf('/') - 1;
    return filepath.right( name_length );
}

bool OpenNetDialog::isFilesChoosed() {
    return isModelOpened_ && isWeightsOpened_;
}

vector<QString> OpenNetDialog::filePaths() {
    return filePaths_;
}

void OpenNetDialog::openModelClicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,
        "Open Model", QDir::currentPath(), "(*.prototxt)");

    if ( changeDir(filepath) ) {
        ui->lineEditModelFile->setText( getFilename(filepath) );

        filePaths_[0] = filepath;
        isModelOpened_ = true;
    } else {
        ui->lineEditModelFile->setText("Necessary");
        isModelOpened_ = false;
    }
}

void OpenNetDialog::openWeightsClicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,
        "Open Trained Net", QDir::currentPath(), "(*.caffemodel)");

    if ( changeDir(filepath) ) {
        ui->lineEditWeightsFile->setText( getFilename(filepath) );

        filePaths_[1] = filepath;
        isWeightsOpened_ = true;
    } else {
        ui->lineEditWeightsFile->setText("Necessary");
        isWeightsOpened_ = false;
    }
}

void OpenNetDialog::openMeanImageClicked()
{

    QString filepath = QFileDialog::getOpenFileName(this,
        "Open Mean Image File", QDir::currentPath(), "(*.binaryproto)");

    if (!filepath.isEmpty()) {
        changeDir(filepath);
        ui->lineEditMeanFile->setText( getFilename(filepath) );

        filePaths_[2] = filepath;
    } else {
        ui->lineEditMeanFile->setText("Recommended");
    }
}
