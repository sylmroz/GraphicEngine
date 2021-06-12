#include "WindGenerator.hpp"
#include <random>
#include <glm\glm.hpp>
#include <glm\gtc\noise.hpp>
#include <chrono>
#include <random>

cv::Mat GraphicEngine::Engines::Graphic::WindGenerator::generate(int dim, float speed)
{
	cv::Mat mat(dim, dim, CV_8UC4);
	cv::parallel_for_(cv::Range(0, dim * dim), [&](cv::Range range) {
		for (int r = range.start; r < range.end; ++r)
		{
			int i = r / dim;
			int j = r % dim;
			double x = (double)i / dim - 0.5;
			double y = (double)j / dim - 0.5;

			double x2 = (double)i / dim;
			double y2 = (double)j / dim;
			double e1 = glm::perlin(glm::vec2(speed * 2 * x + 0.25, speed * 2 * y + 0.15)) + glm::perlin(glm::vec2(4 * speed * x + 0.5, 4 * speed * y - 0.215));
			e1 /= (2.0);
			e1 *= glm::perlin(glm::vec2(6*speed * x, 6*speed * y));
			e1 = 255 * (e1 * 0.5 + 0.5);
			double e2 = glm::perlin(glm::vec2(speed * 2 * x2 + 0.05, speed * 2 * y2 - 0.7515)) + glm::perlin(glm::vec2(4 * speed * x2 + 0.025, 4 * speed * y2 - 0.82));
			e2 /= (2.0);
			e2 *= glm::perlin(glm::vec2(6* speed * x2 - 0.2122, 6*speed * y2 + 0.2212));
			e2 = 255 * (e2 * 0.5 + 0.5);
			mat.at<cv::Vec4b>(i, j) = cv::Vec4b(e2, 0.0f, e1, 0.0f);
		}
		});
	return mat;
}