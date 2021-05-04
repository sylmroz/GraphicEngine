#pragma once

namespace GraphicEngine::Engines::Graphic::Shaders
{
	struct Time
	{
		Time() = default;
		Time(float timestamp) : timestamp{ timestamp } {}
		float timestamp;
	};
}