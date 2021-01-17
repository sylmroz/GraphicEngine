#pragma once

#include "../../Widgets/Button.hpp"
#include "../../Widgets/ColorEdit.hpp"
#include "../../Widgets/TabBarItem.hpp"
#include <imgui.h>
#include <string>
#include "../../../../Engines/Graphic/Shaders/Models/Light.hpp"

namespace GraphicEngine::GUI
{
	class LightColorComponent : public Widget
	{
	public:
		LightColorComponent(GraphicEngine::Engines::Graphic::Shaders::LightColor lightColor);
		void onLightColor(std::function<void(Engines::Graphic::Shaders::LightColor)> callback);
		void setCurrentColors(Engines::Graphic::Shaders::LightColor lightColor);\
		// Inherited via Widget
		virtual void draw() override;
	private:
		GraphicEngine::Engines::Graphic::Shaders::LightColor lightColor;
		Core::Subject<Engines::Graphic::Shaders::LightColor> m_lightColorSubject;

		std::shared_ptr<ColorEdit> m_diffuseColorEdit;
		std::shared_ptr<ColorEdit> m_specularColorEdit;
		std::shared_ptr<ColorEdit> m_ambientColorEdit;
	};

	template <typename LightType>
	class LightComponent : public Widget
	{
	public:
		LightComponent(std::string label) :
			label{ label }
		{
			m_addLightButton = std::make_shared<Button>("Add light");
			m_deleteLightButton = std::make_shared<Button>("Delete light");
			m_lightColorComponent = std::make_shared<LightColorComponent>(currentLightParameters.color);
			m_lightColorComponent->onLightColor([&](Engines::Graphic::Shaders::LightColor color)
			{
				currentLightParameters.color = color;
				m_lightEditedSubject.notify(selectedItem, currentLightParameters);
			});
		}
		// Inherited via Widget
		virtual void draw() override
		{
			m_addLightButton->draw();
			ImGui::SameLine();
			m_deleteLightButton->draw();
			std::vector<std::string> labels;
			labels.resize(elementsCount);
			for (int i = 0; i < elementsCount; ++i)
			{
				labels[i] = label + std::to_string(i);
				if (ImGui::Selectable(labels[i].c_str(), i == selectedItem, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (ImGui::IsMouseDoubleClicked(0))
					{
						selectedItem = i;
						m_selectedItemSubject.notify(selectedItem);
						m_lightColorComponent->setCurrentColors(currentLightParameters.color);
						updateRestComponents();
					}
				}
			}
			drawEditTools();
			m_lightColorComponent->draw();
		}

		// Properties
		uint32_t elementsCount{ 0 };
		LightType currentLightParameters;
		int selectedItem{ -1 };

		// Event listeners
		void onSelectedItem(std::function<void(int)> callback)
		{
			m_selectedItemSubject.subscribe(callback);
		}

		void onAddLightClicked(std::function<void(void)> callback)
		{
			m_addLightButton->onClicked(callback);
		}

		void onLightEdited(std::function<void(int, LightType)> callback)
		{
			if (selectedItem > -1)
				m_lightEditedSubject.subscribe(callback);
		}

		void onDeleteLightClicked(std::function<void(int)> callback)
		{
			deleteFunction = callback;
			m_deleteLightButton->onClicked([&]()
			{
				if (selectedItem > -1)
				{
					deleteFunction(selectedItem);
					selectedItem = -1;
				}
			});
		}

	protected:
		virtual void drawEditTools() = 0;
		virtual void updateRestComponents() = 0;
		std::string label;

		Core::Subject<int> m_selectedItemSubject;
		Core::Subject<int, LightType> m_lightEditedSubject;
		std::shared_ptr<Button> m_addLightButton;
		std::shared_ptr<Button> m_deleteLightButton;
		std::shared_ptr<LightColorComponent> m_lightColorComponent;
		std::function<void(int)> deleteFunction;
	};
}