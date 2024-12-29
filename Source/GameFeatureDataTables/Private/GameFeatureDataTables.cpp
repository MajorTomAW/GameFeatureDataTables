// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FGameFeatureDataTablesModule"

class FGameFeatureDataTablesModule : public IModuleInterface
{
	virtual void StartupModule() override
	{
		
	}

	virtual void ShutdownModule() override
	{
		
	}
};


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameFeatureDataTablesModule, GameFeatureDataTables)