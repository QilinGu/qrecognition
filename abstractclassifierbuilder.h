#pragma once

#include "abstractclassifier.h"

class AbstractClassifierBuilder
{
public:
    virtual AbstractClassifier *buildClassifier() = 0;
};
