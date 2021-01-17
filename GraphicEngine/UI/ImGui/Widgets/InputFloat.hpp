#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

#include <glm/vec4.hpp>

namespace GraphicEngine::GUI
{
	class InputScalar : public Widget
	{
	public:
		InputScalar(std::string label, float initialValue, float step);
		// Inherited via Widget
		virtual void draw() override;

		void onInputScalarEdit(std::function<void(float)> callback);

		float value{};
	private:
		Core::Subject<float> m_inputScalarSubject;
		float step{};
		std::string label;
	};

	class InputScalar4 : public Widget
	{
	public:
		InputScalar4(std::string label, glm::vec4 initialValue, float step);
		// Inherited via Widget
		virtual void draw() override;

		void onInputScalarEdit(std::function<void(glm::vec4)> callback);

		glm::vec4 value{};
	private:	
		Core::Subject<glm::vec4> m_inputScalarSubject;
		float step{};
		std::string label;
	};
}