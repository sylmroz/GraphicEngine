#include "pch.h"
#include "../GraphicEngine/Common/Vertex.hpp"

using namespace GraphicEngine::Common;

TEST(VertexSizeOffsetStrideTest, IsBasicValuesCorrect)
{
	auto sizeAndOffset = VertexP::getSizeAndOffsets();
	EXPECT_EQ(sizeAndOffset[0].first, 3);
	EXPECT_EQ(sizeAndOffset[0].second, 0);
	uint32_t stride = VertexP::getStride();
	EXPECT_EQ(stride, 12);
}

TEST(VertexSizeOffsetStrideTest, IsPositionColorTexCoordCorrect)
{
	auto sizeAndOffset = VertexPCTc::getSizeAndOffsets();

	EXPECT_EQ(sizeAndOffset.size(), 3);
	EXPECT_EQ(sizeAndOffset[0].first, 3);
	EXPECT_EQ(sizeAndOffset[0].second, 0);
	EXPECT_EQ(sizeAndOffset[1].first, 3);
	EXPECT_EQ(sizeAndOffset[1].second, 3*4);
	EXPECT_EQ(sizeAndOffset[2].first, 2);
	EXPECT_EQ(sizeAndOffset[2].second, 2 * 3 * 4);
	uint32_t stride = VertexPCTc::getStride();
	EXPECT_EQ(stride, (3 + 3 + 2) * 4);
}