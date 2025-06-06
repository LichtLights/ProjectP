#pragma once

#include "Logging/LogMacros.h"

class UObject;

PROJECTP_API DECLARE_LOG_CATEGORY_EXTERN(LogDB, Log, All);
PROJECTP_API DECLARE_LOG_CATEGORY_EXTERN(LogDBExperience, Log, All);
PROJECTP_API DECLARE_LOG_CATEGORY_EXTERN(LogDBAbilitySystem, Log, All);
PROJECTP_API DECLARE_LOG_CATEGORY_EXTERN(LogDBTeams, Log, All);

PROJECTP_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
