#pragma once

#include "abstractclassifierbuilder.h"
#include "opennetdialog.h"

class CaffeNetBuilder : public AbstractClassifierBuilder
{
public:
    CaffeNetBuilder();

    /* Returns nullptr if build was not succesful. */
    AbstractClassifier *buildClassifier() override;

private:
    OpenNetDialog dialog_;
};
