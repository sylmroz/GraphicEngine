#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace GraphicEngine::GUI
{
	template<typename Val, typename Boundary>
	class Slider : public Widget
	{
	public:
		Slider(std::string label, Val initialValue, Boundary minValue, Boundary maxValue):
			Widget{ label },
			value{ initialValue }, m_minValue{ minValue }, m_maxValue{ maxValue }
		{}

		void onSliderChange(std::function<void(float)> callback)
		{
			m_sliderValueChange.subscribe(callback);
		}

		Val value{};
	protected:
		Core::Subject<Val> m_sliderValueChange;
		Boundary m_minValue;
		Boundary m_maxValue;
	};

	class SliderFloat : public Slider<float, float>
	{
	public:
		SliderFloat(std::string label, float initialValue, float minValue, float maxValue) :
			Slider<float, float>{ label, initialValue, minValue, maxValue }
		{}

		// Inherited via Widget
		virtual void draw() override;
	};

	class SliderFloat2 : public Slider<glm::vec2, float>
	{
	public:
		SliderFloat2(std::string label, glm::vec2 initialValue, float minValue, float maxValue) :
			Slider<glm::vec2, float>{ label, initialValue, minValue, maxValue }
		{}

		// Inherited via Widget
		virtual void draw() override;
	};

	class SliderFloat3 : public Slider<glm::vec3, float>
	{
	public:
		SliderFloat3(std::string label, glm::vec3 initialValue, float minValue, float maxValue) :
			Slider<glm::vec3, float>{ label, initialValue, minValue, maxValue }
		{}

		// Inherited via Widget
		virtual void draw() override;
	};

	class SliderFloat4 : public Slider<glm::vec4, float>
	{
	public:
		SliderFloat4(std::string label, glm::vec4 initialValue, float minValue, float maxValue) :
			Slider<glm::vec4, float>{ label, initialValue, minValue, maxValue }
		{}

		// Inherited via Widget
		virtual void draw() override;
	};
}