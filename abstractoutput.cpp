#include "abstractoutput.h"

#include <iostream>
#include <fstream>

bool AbstractOutput::loadLabels(const std::string &labels_file, int num_labels) {
    std::ifstream ifile(labels_file);

    if (ifile.is_open()) {
        for (std::string line; std::getline(ifile, line); ) {
            labels_.push_back(line);
        }

        is_labels_ = (labels_.size() == num_labels);
        return is_labels_;
    }
    return false;
}

void AbstractOutput::loadLabels(const std::vector<std::string> &labels) {
    labels_ = labels;
    is_labels_ = true;
}

AbstractOutput::~AbstractOutput() {}
