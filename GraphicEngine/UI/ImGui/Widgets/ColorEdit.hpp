#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

#include <glm/vec4.hpp>

namespace GraphicEngine::GUI
{
	class ColorEdit : public Widget
	{
	public:
		ColorEdit() = default;
		ColorEdit(std::string label, glm::vec4 initialColor);
		// Inherited via Widget
		virtual void draw() override;

		template <typename Callback>
		void subscribe(Callback callback)
		{
			m_colorSelectorEventListener.subscribe(callback);
		}
		// Properties
		glm::vec4 color{};
	private:
		Core::Subject<glm::vec4> m_colorSelectorEventListener;
		std::string label;
	};
}