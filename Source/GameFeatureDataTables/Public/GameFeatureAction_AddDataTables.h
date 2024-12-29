// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddDataTables.generated.h"

class UCompositeDataTable;

USTRUCT()
struct GAMEFEATUREDATATABLES_API FGameFeatureDataTableEntry
{
	GENERATED_BODY()

	FGameFeatureDataTableEntry() {}

	/** The composite data table to add additional data tables to */
	UPROPERTY(EditAnywhere, Category = "Data Tables", meta = (MetaClass = "/Script/Engine.CompositeDataTable"))
	TSoftObjectPtr<UCompositeDataTable> CompositeTable;

	/** List of data tables to add to the composite data table */
	UPROPERTY(EditAnywhere, Category = "Data Tables", meta = (MetaClass = "/Script/Engine.DataTable"))
	TArray<TSoftObjectPtr<UDataTable>> DataTables;
};

/**
 * Adds data tables to an existing composite data table.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Data Tables"))
class UGameFeatureAction_AddDataTables : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UGameFeatureAction_AddDataTables();
	
	//~ Begin UGameFeatureAction Interface
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureUnregistering() override;
	
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
	//~ End UGameFeatureAction Interface

	//~ Begin UObject Interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~ End UObject Interface

	/** If true, we should load the data tables at registration time instead of activation time */
	virtual bool ShouldPreloadAtRegistration();

	/** List of data tables to add to the composite data table when this game feature is enabled */
	UPROPERTY(EditAnywhere, Category = "Data Tables", meta = (TitleProperty = "{CompositeTable}"))
	TArray<FGameFeatureDataTableEntry> DataTableList;

	/**
	 * If true, this will preload the data tables when the feature is registered in the editor,
	 * but be aware that this can cause validation errors if the composite table is in a bad package.
	 */
	UPROPERTY(EditAnywhere, Category = "Data Tables")
	bool bPreloadInEditor;

	/** If true, this will preload the data tables when the feature is registered whilst a commandlet is running */
	UPROPERTY(EditAnywhere, Category = "Data Tables")
	bool bPreloadInCommandlets;
};
