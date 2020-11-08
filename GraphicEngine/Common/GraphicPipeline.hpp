#pragma once

namespace GraphicEngine
{
	class GraphicPipeline
	{
	public:
		virtual void compile() = 0;
		virtual void bind() = 0;
		virtual void draw() = 0;
	};
}