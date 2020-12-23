#pragma once

#include "Widget.hpp"

namespace GraphicEngine::Common
{
	class UI
	{
	protected:
		struct EngineBackend
		{
			virtual void initialize() = 0;
			virtual void nextFrame() = 0;
			virtual void shutdown() = 0;

			virtual ~EngineBackend() = default;
		};

		template <typename RenderingEngineBackendImpl, typename... Args>
		struct RenderEngineBackend : EngineBackend
		{
			void renderData(Args... args)
			{
				static_cast<RenderingEngineBackendImpl*>(this)->renderData(args...);
			}
		};

	public:
		UI& addBackend(std::shared_ptr<EngineBackend> renderingEngineBackend)
		{
			m_renderingEnginebackend = renderingEngineBackend;
			
			return *this;
		}

		UI& addWidow(std::shared_ptr<EngineBackend> windowBackend)
		{
			m_windowBackend = windowBackend;
			return *this;
		}

		virtual UI& initialize()
		{
			m_windowBackend->initialize();
			m_renderingEnginebackend->initialize();
			return *this;
		}

		virtual void nextFrame()
		{
			m_renderingEnginebackend->nextFrame();
			m_windowBackend->nextFrame();
		}

		void addWidget(std::shared_ptr<GUI::Widget> widget)
		{
			m_widgets.push_back(widget);
		}

		virtual void drawUi() = 0;
		virtual bool isKeyboardBusy() = 0;
		virtual bool isMouseButtonBusy() = 0;

		virtual void shutdown()
		{
			m_renderingEnginebackend->shutdown();
			m_windowBackend->shutdown();
		}

		virtual ~UI() = default;
	protected:
		std::shared_ptr<EngineBackend> m_windowBackend;
		std::shared_ptr<EngineBackend> m_renderingEnginebackend;

		std::list<std::shared_ptr<GUI::Widget>> m_widgets;

	private:
	};
}