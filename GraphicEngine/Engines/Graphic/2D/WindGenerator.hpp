#pragma once

#include <opencv2\opencv.hpp>

namespace GraphicEngine::Engines::Graphic
{
    class WindGenerator
    {
    public:
	    static cv::Mat generate(int dim, float speed);
    };
}