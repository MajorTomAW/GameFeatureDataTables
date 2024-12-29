// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameFeatureAction_AddDataTables.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include "GameFeaturesSubsystemSettings.h"
#include "Engine/AssetManager.h"
#include "Engine/CompositeDataTable.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddDataTables)

#define LOCTEXT_NAMESPACE "GameFeatures"

UGameFeatureAction_AddDataTables::UGameFeatureAction_AddDataTables()
{
	bPreloadInCommandlets = false;
	bPreloadInEditor = false;
}

void UGameFeatureAction_AddDataTables::OnGameFeatureRegistering()
{
	Super::OnGameFeatureRegistering();

	if (ShouldPreloadAtRegistration())
	{
		for (const FGameFeatureDataTableEntry& Entry : DataTableList)
		{
			UCompositeDataTable* Composite = Entry.CompositeTable.LoadSynchronous();
			
			if (Composite == nullptr)
			{
				continue;
			}

			for (const TSoftObjectPtr<UDataTable>& DataTablePtr : Entry.DataTables)
			{
				UDataTable* DataTable = DataTablePtr.LoadSynchronous();
				if (DataTable)
				{
					Composite->AddParentTable(DataTable);
				}
			}
		}
	}
}

void UGameFeatureAction_AddDataTables::OnGameFeatureUnregistering()
{
	Super::OnGameFeatureUnregistering();

	if (ShouldPreloadAtRegistration())
	{
		for (const FGameFeatureDataTableEntry& Entry : DataTableList)
		{
			UCompositeDataTable* Composite = Entry.CompositeTable.LoadSynchronous();
			
			if (Composite == nullptr)
			{
				continue;
			}

			for (const TSoftObjectPtr<UDataTable>& TablePtr : Entry.DataTables)
			{
				UDataTable* DataTable = TablePtr.LoadSynchronous();
				if (DataTable)
				{
					Composite->RemoveParentTable(DataTable);
				}
			}
		}
	}
}

void UGameFeatureAction_AddDataTables::OnGameFeatureActivating()
{
	Super::OnGameFeatureActivating();

	if (ShouldPreloadAtRegistration())
	{
		// This already happened at registration time
		return;
	}

	for (const FGameFeatureDataTableEntry& Entry : DataTableList)
	{
		UCompositeDataTable* Composite = Entry.CompositeTable.LoadSynchronous();

		if (Composite == nullptr)
		{
			continue;
		}

		for (const TSoftObjectPtr<UDataTable>& TablePtr : Entry.DataTables)
		{
			UDataTable* DataTable = TablePtr.LoadSynchronous();
			if (DataTable)
			{
				Composite->AddParentTable(DataTable);
			}
		}
	}
}

void UGameFeatureAction_AddDataTables::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	
	if (ShouldPreloadAtRegistration())
	{
		// This already happened at unregistration
		return;
	}

	for (const FGameFeatureDataTableEntry& Entry : DataTableList)
	{
		UCompositeDataTable* Composite = Entry.CompositeTable.LoadSynchronous();

		if (Composite == nullptr)
		{
			continue;
		}

		for (const TSoftObjectPtr<UDataTable>& TablePtr : Entry.DataTables)
		{
			UDataTable* DataTable = TablePtr.LoadSynchronous();
			if (DataTable)
			{
				Composite->RemoveParentTable(DataTable);
			}
		}
	}
}

bool UGameFeatureAction_AddDataTables::ShouldPreloadAtRegistration()
{
	if (IsRunningCommandlet())
	{
		return bPreloadInCommandlets;
	}

	return GIsEditor && bPreloadInEditor;
}

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddDataTables::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsInitialized())
	{
		for (const FGameFeatureDataTableEntry& Entry : DataTableList)
		{
			for (const TSoftObjectPtr<UDataTable>& DataTablePtr : Entry.DataTables)
			{
				if (!DataTablePtr.IsNull())
				{
					AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, DataTablePtr.ToSoftObjectPath().GetAssetPath());
					AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, DataTablePtr.ToSoftObjectPath().GetAssetPath());
				}
			}
		}
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddDataTables::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 EntryIdx = 0;
	for (const FGameFeatureDataTableEntry& Entry : DataTableList)
	{
		if (Entry.CompositeTable.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("CompositeTableIsNull", "CompositeTable is null at index {0} in DataTableList"), FText::AsNumber(EntryIdx)));
		}

		int32 TableIdx = 0;
		for (const TSoftObjectPtr<UDataTable>& DataTable : Entry.DataTables)
		{
			if (DataTable.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("DataTableIsNull", "DataTable is null at index {0} in DataTableList[{1}]"), FText::AsNumber(TableIdx), FText::AsNumber(EntryIdx)));
			}
			++TableIdx;
		}

		++EntryIdx;
	}

	return Result;
}
#endif

#undef LOCTEXT_NAMESPACE