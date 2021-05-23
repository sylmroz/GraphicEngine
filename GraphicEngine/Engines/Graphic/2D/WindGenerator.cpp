#include "WindGenerator.hpp"
#include <random>
#include <glm\glm.hpp>
#include <glm\gtc\noise.hpp>
#include <chrono>
#include <random>

cv::Mat GraphicEngine::Engines::Graphic::WindGenerator::generate(int dim, float speed)
{
	cv::Mat mat(dim, dim, CV_32FC4);
	cv::parallel_for_(cv::Range(0, dim * dim), [&](cv::Range range) {
		for (int r = range.start; r < range.end; ++r)
		{
			int i = r / dim;
			int j = r % dim;
			double x = (double)i / dim - 0.5;
			double y = (double)j / dim - 0.5;

			double x2 = (double)i / dim;
			double y2 = (double)j / dim;
			double e1 = /*0.75 * glm::perlin(glm::vec2(speed * x, speed * y)) +0.5 * glm::perlin(glm::vec2(speed * 9 * x, speed * 9 * y)) + 0.25 * glm::perlin(glm::vec2(18 * speed * x, 18 * speed * y)) + 0.15 **/ glm::perlin(glm::vec2(45 * speed * x, 45 * speed * y));
			//e1 /= (0.75 + 0.5 + 0.25 + 0.15);
			//e1 = (e1 * 0.5 + 0.5);
			double e2 = /*0.75 * glm::perlin(glm::vec2(speed * x2, speed * y2)) +0.5 * glm::perlin(glm::vec2(speed * 9 * x2, speed * 9 * y2)) + 0.25 * glm::perlin(glm::vec2(18 * speed * x2, 18 * speed * y2)) + 0.15 **/  glm::perlin(glm::vec2(45 * speed * x2, 45 * speed * y2));
			//e2 /= (0.75 + 0.5 + 0.25 + 0.15);
			//e2 = (e2 * 0.5 + 0.5);
			mat.at<cv::Vec4f>(i, j) = cv::Vec4f(e2, 0.0f, e1, 0.0f);
		}
		});
	/*for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			double x = (double)i / dim - 0.5;
			double y = (double)j / dim - 0.5;

			double x2 = (double)i / dim;
			double y2 = (double)j / dim;
			double e1 = 0.75 * glm::perlin(glm::vec2(speed * x, speed * y)) +0.5 * glm::perlin(glm::vec2(speed * 9 * x, speed * 9 * y)) + 0.25 * glm::perlin(glm::vec2(18 * speed * x, 18 * speed * y)) + 0.15 * glm::perlin(glm::vec2(45 * speed * x, 45 * speed * y));
			e1 /= (0.75 + 0.5 + 0.25 + 0.15);
			e1 = (e1 * 0.5 + 0.5);
			double e2 = 0.75 * glm::perlin(glm::vec2(speed * x2, speed * y2)) +0.5 * glm::perlin(glm::vec2(speed * 9 * x2, speed * 9 * y2)) + 0.25 * glm::perlin(glm::vec2(18 * speed * x2, 18 * speed * y2)) + 0.15 * glm::perlin(glm::vec2(45 * speed * x2, 45 * speed * y2));
			e2 /= (0.75 + 0.5 + 0.25 + 0.15);
			e2 = (e2 * 0.5 + 0.5);
			mat.at<cv::Vec4f>(i, j) = cv::Vec4f(e1, 0.0f, e2, 0.0f);
		}
	}*/
	return mat;
}