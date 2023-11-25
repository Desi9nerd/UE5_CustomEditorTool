#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"

void UQuickAssetAction::DuplicateAssets(int32 NumOfDuplicates) // Asset 복제하기
{
	if (NumOfDuplicates <= 0) // 복사 개수가 0이하라면
	{
		// DebugHeader.h에 만든 ShowMsgDialog함수를 콜해 아래의 문구를 띄운다.
		ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enter a VALID number"));
		return;
	}

	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 Counter = 0;

	for (const FAssetData& SelectedAssetData : SelectedAssetsData)
	{
		for (int32 i = 0; i < NumOfDuplicates; i++)
		{
			// Asset 경로, 이름짓기, 복제될 Asset의 경로
			const FString SourceAssetPath = SelectedAssetData.ObjectPath.ToString();
			const FString NewDuplicatedAssetName = SelectedAssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i + 1);
			const FString NewPathName = FPaths::Combine(SelectedAssetData.PackagePath.ToString(), NewDuplicatedAssetName);

			// SourceAssetPath경로에 있는 Asset을 NewPathName경로로 복제
			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName)) 
			{
				UEditorAssetLibrary::SaveAsset(NewPathName, false); // NewPathName경로에 Asset을 저장
				++Counter;
			}
		}
	}

	if (Counter > 0) // 복제가 0 초과면 문구 띄우기
	{
		// DebugHeader.h에 만든 ShowNotifyInfo함수를 콜해 언리얼에디터 우측 하단에 아래의 문구를 띄운다.
		ShowNotifyInfo(TEXT("Successfully duplicated " + FString::FromInt(Counter) + " files"));
	}
}