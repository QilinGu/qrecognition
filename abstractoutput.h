#pragma once

#include <vector>
#include <string>

#include <QObject>

#include <opencv/cv.hpp>

class AbstractOutput : public QObject
{
    Q_OBJECT
public:
    explicit AbstractOutput(QObject *parent = nullptr);
    virtual ~AbstractOutput();

    /* Load labels from file for more pleasant output.
     * @param labels_file file to read labels from.
     * @param num_labels is needed to verify labels count in labels_file.
     * @return success status */
    virtual bool loadLabels(const std::string &labels_file, int num_labels);

    /* Overloaded function for convenience.
     * Load labels vector for more pleasant output.
     * @param labels labels for output. */
    virtual void loadLabels(const std::vector<std::string> &labels);

    /* Output predictions.
     * @param predictions data to output. */
    virtual void output(const std::vector<std::pair<int, float> > &predictions) = 0;

    /* Output bounding boxes.
     * @param boxes data to output. */
    virtual void output(const std::vector<cv::Rect> &boxes) = 0;

    /* Output bounding boxes and vector of predictions for each from them.
     * @param boxes data to output.
     * @param predictions data to output. */
    virtual void output(const std::vector<cv::Rect> &boxes,
                        const std::vector<std::vector<std::pair<int, float> > > &predictions) = 0;


    /* Update graphic output. */
    virtual void update() = 0;

    /* Update size of the output overlay.
     * Usually it should be equal to processing image size.
     * If overlay_size equal cv::Size() then graphic output will be disabled.
     * @param overlay_size new size of the output. */
    virtual void updateOutputSize(cv::Size overlay_size) = 0;

signals:
    void outputReady(const QPixmap &overlay);

protected:
    std::vector<std::string> labels_;
    bool is_labels_ = false;
};
