#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

namespace GraphicEngine::GUI
{
	class ComboBox : public Widget
	{
	public:
		ComboBox(std::string label, std::vector<std::string> labels, int selectedItemIndex = 0);
		// Inherited via Widget
		virtual void draw() override;

		void setLabels(std::vector<std::string> labels);

		void onItemSelected(std::function<void(int)> callback);
	private:
		int selectedItemIndex;
		std::vector<std::string> labels;
		std::string label;
		Core::Subject<int> m_selectedItemEventListener;
	};
}
