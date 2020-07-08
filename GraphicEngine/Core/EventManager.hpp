#pragma once

#include "Subject.hpp"

namespace GraphicEngine::Core
{
	class EventManager
	{
		class NotifierBase
		{
		public:
			virtual ~NotifierBase() = default;
			virtual void notify() = 0;
		};

		template <typename Subject>
		class Notifier : public NotifierBase
		{
		public:
			explicit Notifier(Subject&& subject)
				: _subject(std::forward<Subject>(subject))
			{
			}
			
			virtual void notify() override
			{
				_subject();
			}

		private:
			Subject _subject;
		};
	public:

		template <typename Subject, typename... Args>
		void addSubject(Subject&& subject, Args... args)
		{
			_notifiers.emplace_back(std::shared_ptr<NotifierBase>(new Notifier(
				[_subject = std::move(subject), _largs = std::tuple(std::forward<Args>(args)...)]
			() mutable
			{
				std::apply(std::move(_subject), std::move(_largs));
			})));
		}

		template <typename Subject>
		void addSubject(Subject& subject)
		{
			std::shared_ptr<NotifierBase> instance = std::make_shared<Notifier>([&]
			() mutable
				{
					subject();
				}
			);
			_notifiers.emplace_back(instance);
		}

		void call()
		{
			for(auto& notifier : _notifiers)
			{
				notifier->notify();
			}
		}
	private:
		std::vector<std::shared_ptr<NotifierBase>> _notifiers;
	};
}
