#pragma once

#include "abstractdetector.h"

class AbstractDetectorBuilder
{
public:
    virtual AbstractDetector *buildDetector() = 0;
    virtual ~AbstractDetectorBuilder() {}
};
