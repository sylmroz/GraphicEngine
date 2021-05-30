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
			//double e1 = 1.75 * glm::perlin(glm::vec2(speed * x, speed * y)) + 0.5 * glm::perlin(glm::vec2(speed * 2 * x, speed * 2 * y)) + 0.25 * glm::perlin(glm::vec2(4 * speed * x, 4 * speed * y)) + 0.15 * glm::perlin(glm::vec2(8 * speed * x, 8 * speed * y));
			double e1 = 1.0 * glm::perlin(glm::vec2(speed * 2 * x, speed * 2 * y)) + 0.5 * glm::perlin(glm::vec2(4 * speed * x, 4 * speed * y)) + 0.25 * glm::perlin(glm::vec2(8 * speed * x, 8 * speed * y));
			e1 /= (1.0 + 0.5 + 0.25);
			e1 *= glm::perlin(glm::vec2(speed * x, speed * y));
			e1 = 255 * (e1 * 0.5 + 0.5);
			//double e2 = 1.75 * glm::perlin(glm::vec2(speed * x2, speed * y2)) + 0.5 * glm::perlin(glm::vec2(speed * 2 * x2, speed * 2 * y2)) + 0.25 * glm::perlin(glm::vec2(4 * speed * x2, 4 * speed * y2)) + 0.15 * glm::perlin(glm::vec2(8 * speed * x2, 8 * speed * y2));
			//e2 /= (1.75 + 0.5 + 0.25 + 0.15);
			double e2 = 1.0 * glm::perlin(glm::vec2(speed * 2 * x2, speed * 2 * y2)) + 0.5 * glm::perlin(glm::vec2(4 * speed * x2, 4 * speed * y2)) + 0.25 * glm::perlin(glm::vec2(8 * speed * x2, 8 * speed * y2));
			e2 /= (1.0 + 0.5 + 0.25);
			e2 *= glm::perlin(glm::vec2(speed * x2, speed * y2));
			e2 = 255 * (e2 * 0.5 + 0.5);
			mat.at<cv::Vec4b>(i, j) = cv::Vec4b(e2, 0.0f, e1, 0.0f);
		}
		});
	return mat;
}