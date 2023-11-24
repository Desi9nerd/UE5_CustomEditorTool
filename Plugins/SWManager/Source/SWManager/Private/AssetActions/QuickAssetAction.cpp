#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"

void UQuickAssetAction::DuplicateAssets(int32 NumOfDuplicates) // Asset �����ϱ�
{
	if (NumOfDuplicates <= 0) // ���� ������ 0���϶��
	{
		// �߸���ٴ� ������ ���� �����Ѵ�
		Print(TEXT("Please enter a VALID number"), FColor::Red);
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
				++Counter;
			}
		}
	}

	if (Counter > 0) // ������ 0 �ʰ��� ���� ����
	{
		Print(TEXT("Successfully duplicated " + FString::FromInt(Counter) + " files"), FColor::Green);
	}
}