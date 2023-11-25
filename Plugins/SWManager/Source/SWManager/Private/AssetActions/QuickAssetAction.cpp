#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"

void UQuickAssetAction::DuplicateAssets(int32 NumOfDuplicates) // Asset �����ϱ�
{
	if (NumOfDuplicates <= 0) // ���� ������ 0���϶��
	{
		// DebugHeader.h�� ���� ShowMsgDialog�Լ��� ���� �Ʒ��� ������ ����.
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enter a VALID number"));
		return;
	}

	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 Counter = 0;

	for (const FAssetData& SelectedAssetData : SelectedAssetsData)
	{
		for (int32 i = 0; i < NumOfDuplicates; i++)
		{
			// Asset ���, �̸�����, ������ Asset�� ���
			const FString SourceAssetPath = SelectedAssetData.ObjectPath.ToString();
			const FString NewDuplicatedAssetName = SelectedAssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i + 1);
			const FString NewPathName = FPaths::Combine(SelectedAssetData.PackagePath.ToString(), NewDuplicatedAssetName);

			// SourceAssetPath��ο� �ִ� Asset�� NewPathName��η� ����
			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName)) 
			{
				UEditorAssetLibrary::SaveAsset(NewPathName, false); // NewPathName��ο� Asset�� ����
				++Counter; // ���Ŀ� ������ ��ﶧ ��� Asset���� ���� ����� �˾ƾ��ϹǷ� �� ���
			}
		}
	}

	if (Counter > 0) // ������ 0 �ʰ��� ���� ����
	{
		// DebugHeader.h�� ���� ShowNotifyInfo�Լ��� ���� �𸮾󿡵��� ���� �ϴܿ� �Ʒ��� ������ ����.
		DebugHeader::ShowNotifyInfo(TEXT("Successfully duplicated " + FString::FromInt(Counter) + " files"));
	}
}

void UQuickAssetAction::AddPrefixes() // ���ξ� �ޱ�
{
	TArray<UObject*>SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets(); // ���õ� Asset���� TArray������ �� ��´�
	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (false == IsValid(SelectedObject)) continue; // ���õ� Asset�� ������ ����

		FString* PrefixFound = PrefixMap.Find(SelectedObject->GetClass()); // PrefixMap���� �˸´� ���ξ ã�� ������ ��´�

		if (nullptr == PrefixFound || PrefixFound->IsEmpty()) // PrefixFound�� ���ٸ�
		{
			DebugHeader::Print(TEXT("Failed to find prefix for class ") + SelectedObject->GetClass()->GetName(), FColor::Red); 
			continue;
		}

		FString OldName = SelectedObject->GetName();

		if (OldName.StartsWith(*PrefixFound)) // �����̸��� �̹� �ش� ���ξ �ִ� ���
		{
			// �Ʒ� ������ ���� continue�Ͽ� ���� Asset�� �۾����� �ʰ� �Ѿ�� ���� Asset�� ���ξ� ����
			DebugHeader::Print(OldName + TEXT(" already has prefix added"), FColor::Red);
			continue;
		}

		//** Material���� Material Instance�� �����ϴ� ��� �𸮾󿡼� ���ϸ� ���� _Inst�� �ڵ����� ���δ�. ���� �տ� M_�� ���� _Inst�� �������ش�. �׷��� ���Ŀ� ���ξ� MI_�� �������� �ٴ´�.
		if (SelectedObject->IsA<UMaterialInstanceConstant>()) // ������ Asset�� Material Instance���
		{
			OldName.RemoveFromStart(TEXT("M_"));
			OldName.RemoveFromEnd(TEXT("_Inst"));
		}

		const FString NewNameWithPrefix = *PrefixFound + OldName; // '���ξ�+�����̸�' ������ ����

		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewNameWithPrefix); // ���õ� Asset�� ���ο� �̸����� �̸� ����

		++Counter; // ���Ŀ� ������ ��ﶧ ��� Asset���� �̸����� ����� �˾ƾ��ϹǷ� ���
	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully renamed " + FString::FromInt(Counter) + " assets"));
	}
}

void UQuickAssetAction::RemoveUnusedAssets() // ������� �ʴ� Asset ����
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData(); // ���õ� Asset���� TArray������ �� ��´�
	TArray<FAssetData> UnusedAssetsData; // ������ ���� Asset���� ��� TArray���� ����

	FixUpRedirectors(); // ���� ��� ����

	for (const FAssetData& SelectedAssetData : SelectedAssetsData)
	{
		// �ش� SelectedAssetsData�� ���۷����Ȱ� �ִ��� ã�´�
		TArray<FString> AssetReferencers =
			UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAssetData.ObjectPath.ToString());

		if (AssetReferencers.Num() == 0) // ���۷����Ȱ� ������
		{
			UnusedAssetsData.Add(SelectedAssetData); // UnusedAssetsData�� �ش� SelectedAssetData�� ��´�
		}
	}

	if (UnusedAssetsData.Num() == 0) // ������ �ʴ� Asset�� ���ٸ�(=Asset���� ��� ���۷����Ǿ� �ִٸ�)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No unused asset found among selected assets"), false);
		return; // ���� ����
	}

	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsData); // UnusedAssetsData�� ��� Asset���� ����. ������ Asset ���� NumOfAssetsDeleted������ ���

	if (NumOfAssetsDeleted == 0) return; // ���Ű� �� Asset�� ���� ��� ����

	DebugHeader::ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(NumOfAssetsDeleted) + TEXT(" unused assets"))); // �𸮾� ������ �����ϴܿ� ���� ����
}

void UQuickAssetAction::FixUpRedirectors()
{
	TArray<UObjectRedirector*> RedirectorsToFixArray;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")); 

	FARFilter Filter;
	Filter.bRecursivePaths = true; // subfolder�� ������ �����ϵ��� true ����
	Filter.PackagePaths.Emplace("/Game"); // � ������ ������ �� ��� ����
	Filter.ClassNames.Emplace("ObjectRedirector"); // ���� �ϱ⸦ ����ϴ� Ŭ������ �̸�

	TArray<FAssetData> OutRedirectors; // ����� ������ TArry ����

	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectors); // Filter�� ����� OutRedirectors�� ��´�

	for (const FAssetData& RedirectorData : OutRedirectors)
	{
		if (UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorData.GetAsset())) // Redirect �Ұ� �ִٸ�
		{
			RedirectorsToFixArray.Add(RedirectorToFix); // RedirectorsToFixArray�� RedirectorToFix ����
		}
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	AssetToolsModule.Get().FixupReferencers(RedirectorsToFixArray);
}
