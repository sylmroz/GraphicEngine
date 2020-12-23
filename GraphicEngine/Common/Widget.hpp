#pragma once

#include <list>
#include <memory>

#include "../Core/Subject.hpp"


namespace GraphicEngine::GUI
{
	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:
		virtual void draw() = 0;
		void addParent(std::shared_ptr<Widget> parent);
		std::shared_ptr<Widget> getParent();
		void addChildren(std::shared_ptr<Widget> children);
		std::list<std::shared_ptr<Widget>> getChildrens();

	protected:
		std::shared_ptr<Widget> m_parent;
		std::list<std::shared_ptr<Widget>> m_childrens;
	};
}