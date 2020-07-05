#pragma once

#include "../Core/Configuration.hpp"
#include "../Core/Subject.hpp"
#include "../Core/Input/Mouse/MouseEventProxy.hpp"

#include <exception>
#include <utility>

namespace GraphicEngine
{
	class Window
	{
	public:
		Window(std::shared_ptr<Core::Configuration> cfg) :m_cfg(cfg) {}
		virtual void init()
		{
			m_width = m_cfg->getProperty<int>("window:width");
			m_height = m_cfg->getProperty<int>("window:height");
			m_title = m_cfg->getProperty<std::string>("window:title");
			try
			{
				initialize();
				m_resizeSubject.subscribe([&](size_t width, size_t height)
					{
						setWidth(width);
						setHeight(height);
					}
				);
			}

			catch (std::exception e)
			{
				throw e;
			}
		}

		virtual void swapBuffer() = 0;

		virtual void poolEvents() = 0;

		virtual std::pair<uint32_t, uint32_t> getFrameBufferSize() = 0;

		virtual void addResizeCallbackListener(std::function<void(size_t, size_t)> resizeListener)
		{
			m_resizeSubject.subscribe(std::move(resizeListener));
		}

		virtual bool windowShouldBeClosed() { return m_shouldClose; }

		size_t getWidth() const { return m_width; }
		void setWidth(size_t width) { m_width = width; }

		size_t getHeight() const { return m_height; }
		void setHeight(size_t height) { m_height = height; }

		virtual void initialize() = 0;

	protected:
		size_t m_width{ 0 };
		size_t m_height{ 0 };
		std::string m_title;

		bool m_shouldClose{ false };

		Core::Subject<size_t, size_t> m_resizeSubject;

		std::shared_ptr<Core::Configuration> m_cfg;
	};
}
