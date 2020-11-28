#pragma once

#include "../../../UtilityLib/UtilityLib/Utility/uuid.hpp"

namespace GraphicEngine::Core::Utils
{
	template <typename Implementation>
	struct UniqueIdentifier
	{
		UtilityLib::uuid id = UtilityLib::uuid::v4();

		bool operator<(UniqueIdentifier<Implementation>& identifier)
		{
			return id < identifier.id;
		}
		bool operator>(UniqueIdentifier<Implementation>& identifier)
		{
			return id > identifier.id;
		}
		bool operator<=(UniqueIdentifier<Implementation>& identifier)
		{
			return id <= identifier.id;
		}
		bool operator>=(UniqueIdentifier<Implementation>& identifier)
		{
			return id >= identifier.id;
		}
		bool operator==(UniqueIdentifier<Implementation>& identifier)
		{
			return id == identifier.id;
		}
		bool operator!=(UniqueIdentifier<Implementation>& identifier)
		{
			return id != identifier.id;
		}

		std::string toString()
		{
			return id.toString();
		}
	};
}