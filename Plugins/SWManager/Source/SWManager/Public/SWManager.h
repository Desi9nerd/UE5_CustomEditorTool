#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSWManagerModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

#pragma region ContentBrowserMenuExtention

	void InitCBMenuExtention();

	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);

	void AddCBMenuEntry(class FMenuBuilder& MenuBuilder);

	void OnDeleteUnsuedAssetButtonClicked();

#pragma endregion
};
