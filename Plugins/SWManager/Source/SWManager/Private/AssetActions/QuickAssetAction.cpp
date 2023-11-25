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

void UQuickAssetAction::AddPrefixes() // 접두어 달기
{
	TArray<UObject*>SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets(); // 선택된 Asset들을 TArray변수에 다 담는다
	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (false == IsValid(SelectedObject)) continue; // 선택된 Asset이 없으면 리턴

		FString* PrefixFound = PrefixMap.Find(SelectedObject->GetClass()); // PrefixMap에서 알맞는 접두어를 찾아 변수에 담는다

		if (nullptr == PrefixFound || PrefixFound->IsEmpty()) // PrefixFound가 없다면
		{
			Print(TEXT("Failed to find prefix for class ") + SelectedObject->GetClass()->GetName(), FColor::Red); 
			continue;
		}

		FString OldName = SelectedObject->GetName();

		if (OldName.StartsWith(*PrefixFound)) // 기존이름에 이미 해당 접두어가 있는 경우
		{
			// 아래 문구를 띄우고 continue하여 지금 Asset은 작업하지 않고 넘어가고 다음 Asset에 접두어 적용
			Print(OldName + TEXT(" already has prefix added"), FColor::Red);
			continue;
		}

		//** Material에서 Material Instance를 생성하는 경우 언리얼에서 파일명 끝에 _Inst을 자동으로 붙인다. 따라서 앞에 M_과 끝에 _Inst를 제거해준다. 그러면 추후에 접두어 MI_가 정상적을 붙는다.
		if (SelectedObject->IsA<UMaterialInstanceConstant>()) // 선택한 Asset이 Material Instance라면
		{
			OldName.RemoveFromStart(TEXT("M_"));
			OldName.RemoveFromEnd(TEXT("_Inst"));
		}

		const FString NewNameWithPrefix = *PrefixFound + OldName; // '접두어+기존이름' 변수에 담음

		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewNameWithPrefix); // 선택된 Asset을 새로운 이름으로 이름 변경

		++Counter; // 추후에 문구를 띄울때 몇개의 Asset들이 이름변경 됬는지 알아야하므로 기록
	}

	if (Counter > 0)
	{
		ShowNotifyInfo(TEXT("Successfully renamed " + FString::FromInt(Counter) + " assets"));
	}
}
