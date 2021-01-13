#pragma once

#include "../../../Common/Widget.hpp"
#include "../../../Core/Subject.hpp"

namespace GraphicEngine::GUI
{
	class Checkbox : public Widget
	{
	public:
		Checkbox(std::string label, bool initialChecked);
		// Inherited via Widget
		virtual void draw() override;

		template <typename Callback>
		void subscribe(Callback callback)
		{
			m_checkboxEventListener.subscribe(callback);
		}

	private:
		bool checked;
		std::string label;
		Core::Subject<bool> m_checkboxEventListener;
	};
}