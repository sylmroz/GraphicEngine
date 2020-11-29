#pragma once

#include "../../../UtilityLib/UtilityLib/Utility/uuid.hpp"

namespace GraphicEngine::Core::Utils
{
	struct UniqueIdentifier
	{
		UtilityLib::uuid uniqueIdentifier = UtilityLib::uuid::v4();

		bool operator<(UniqueIdentifier& identifier)
		{
			return uniqueIdentifier < identifier.uniqueIdentifier;
		}
		bool operator>(UniqueIdentifier& identifier)
		{
			return uniqueIdentifier > identifier.uniqueIdentifier;
		}
		bool operator<=(UniqueIdentifier& identifier)
		{
			return uniqueIdentifier <= identifier.uniqueIdentifier;
		}
		bool operator>=(UniqueIdentifier& identifier)
		{
			return uniqueIdentifier >= identifier.uniqueIdentifier;
		}
		bool operator==(UniqueIdentifier& identifier)
		{
			return uniqueIdentifier == identifier.uniqueIdentifier;
		}
		bool operator!=(UniqueIdentifier& identifier)
		{
			return uniqueIdentifier != identifier.uniqueIdentifier;
		}

		std::string toString()
		{
			return uniqueIdentifier.toString();
		}
	};
}