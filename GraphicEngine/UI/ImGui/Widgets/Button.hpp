#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

namespace GraphicEngine::GUI
{
	class Button : public Widget
	{
	public:
		Button(std::string label);

		// Inherited via Widget
		virtual void draw() override;

		void onClicked(std::function<void(void)> callback);
	private:
		std::string label;
        Core::Subject<> m_clickedSubject;
	};
}