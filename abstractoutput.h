#pragma once

#include <vector>
#include <string>

#include <opencv/cv.hpp>

class AbstractOutput
{
public:
    /* Returns true if loading was successful.
     * @num_labels is needed to verify labels count in @labels_file. */
    virtual bool loadLabels(const std::string &labels_file, int num_labels);
    virtual void loadLabels(const std::vector<std::string> &labels);

    virtual void output(const std::vector<std::pair<int, float> > &predictions) = 0;
    virtual void output(const std::vector<cv::Rect> &boxes) = 0;
    virtual void output(const std::vector<cv::Rect> &boxes,
                        const std::vector<std::vector<std::pair<int, float> > > &predictions) = 0;

    virtual void update(cv::Size new_img_size) = 0;
    virtual void update(cv::Size orig_img_size, cv::Size new_img_size) = 0;

    virtual ~AbstractOutput();

protected:
    std::vector<std::string> labels_;
    bool is_labels_ = false;
};
