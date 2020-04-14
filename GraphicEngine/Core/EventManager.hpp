#ifndef GRAPHIC_ENGINE_CORE_EVENT_MANAGER_HPP
#define GRAPHIC_ENGINE_CORE_EVENT_MANAGER_HPP

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
		//template <typename Subject, typename... ArgumentFunction>
		//void addSubject(Subject subject, ArgumentFunction... argFunction)
		//{
		//	_notifiers.emplace_back(std::make_shared<NotifierBase>(new Notifier(
		//		[_subject = std::move(subject), _argFunction = std::move(argFunction...)]
		//		//[&]
		//	() mutable
		//	{
		//		_subject(_argFunction()...);
		//	})));
		//}

		template <typename Subject, typename... Args>
		void addSubject(Subject&& subject, Args... args)
		{
			_notifiers.emplace_back(std::shared_ptr<NotifierBase>(new Notifier(
				[_subject = std::move(subject), _largs = std::tuple(std::forward<Args>(args)...)]
			() mutable
			{
				std::apply(std::move(_subject),std::move(_largs));
			})));
		}

		template <typename Subject>
		void addSubject(Subject& subject)
		{
			_notifiers.emplace_back(std::shared_ptr<NotifierBase>(new Notifier(
				[&]
			() mutable
			{
				subject();
			})));
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

#endif // !GRAPHIC_ENGINE_CORE_EVENT_MANAGER_HPP

