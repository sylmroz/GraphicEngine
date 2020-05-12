#pragma once

#include <any>
#include <exception>
#include <vector>

namespace GraphicEngine::Core
{
	class ServiceManager
	{
	public:
		template <typename  S>
		S& addService()
		{
			try
			{
				S& service = getService<S>();
				return service;
			}

			catch (const std::runtime_error& e)
			{
				return _baseSubjects.emplace_back(std::any(S()));
			}
		}

		template <typename  S>
		S& addService(S&& service)
		{
			try
			{
				S& service = getService<S>();
				return service;
			}

			catch (const std::runtime_error& e)
			{
				return _baseSubjects.emplace_back(std::any(subject));
			}
		}

		template <typename S>
		S& getService()
		{
			for (auto& b : _baseSubjects)
			{
				try
				{
					S& s = std::any_cast<S&>(b);
					return s;
				}
				catch (const std::bad_any_cast& e)
				{
				}
			}
			throw std::runtime_error("Service does not exist!");
		}
	private:

		std::vector<std::any> _baseSubjects;
	};
}