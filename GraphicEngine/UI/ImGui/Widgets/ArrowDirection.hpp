#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <imGuIZMOquat.h>

namespace GraphicEngine::GUI
{
	template <typename Vec>
	class ArrowDirection : public Widget
	{
	public:
		ArrowDirection() = default;
		ArrowDirection(std::string label, Vec initialDirection, glm::vec4 arrowColor = glm::vec4{ 0.2f, 0.4f, 0.42f, 1.0f }):
			Widget{ label }, direction{ initialDirection }, arrowColor{ arrowColor }
		{
		}
		// Inherited via Widget
		virtual void draw() override
		{
			imguiGizmo::setDirectionColor(ImVec4(arrowColor.r, arrowColor.g, arrowColor.b, arrowColor.a));
			if (ImGui::gizmo3D(label.c_str(), m_direction))
			{
				copyTempDirToOriginalDir();
				m_directionChangeLisener.notify(direction);
			}
			imguiGizmo::restoreDirectionColor();
		}

		template <typename Callback>
		void subscribe(Callback callback)
		{
			m_directionChangeLisener.subscribe(callback);
		}
		// Properties
		Vec direction{};
		glm::vec4 arrowColor{};
	protected:
		virtual void copyTempDirToOriginalDir() = 0;

		glm::vec3 m_direction{};
		Core::Subject<Vec> m_directionChangeLisener;
	};

	class ArrowDirection3D : public ArrowDirection<glm::vec3>
	{
	public:
		ArrowDirection3D() = default;
		ArrowDirection3D(std::string label, glm::vec3 initialDirection, glm::vec4 arrowColor = glm::vec4{0.2f, 0.4f, 0.42f, 1.0f});

	protected:
		virtual void copyTempDirToOriginalDir() override;
	};

	class ArrowDirection2D : public ArrowDirection<glm::vec2>
	{
	public:
		ArrowDirection2D() = default;
		ArrowDirection2D(std::string label, glm::vec2 initialDirection, glm::vec4 arrowColor = glm::vec4{ 0.2f, 0.4f, 0.42f, 1.0f });

	protected:
		virtual void copyTempDirToOriginalDir() override;
	};
}