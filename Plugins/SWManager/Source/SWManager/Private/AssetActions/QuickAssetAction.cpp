#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"

void UQuickAssetAction::DuplicateAssets(int32 NumOfDuplicates) // Asset 복제하기
{
	if (NumOfDuplicates <= 0) // 복사 개수가 0이하라면
	{
		// DebugHeader.h에 만든 ShowMsgDialog함수를 콜해 아래의 문구를 띄운다.
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Please enter a VALID number"));
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
				++Counter; // 추후에 문구를 띄울때 몇개의 Asset들이 복제 됬는지 알아야하므로 수 기록
			}
		}
	}

	if (Counter > 0) // 복제가 0 초과면 문구 띄우기
	{
		// DebugHeader.h에 만든 ShowNotifyInfo함수를 콜해 언리얼에디터 우측 하단에 아래의 문구를 띄운다.
		DebugHeader::ShowNotifyInfo(TEXT("Successfully duplicated " + FString::FromInt(Counter) + " files"));
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
			DebugHeader::Print(TEXT("Failed to find prefix for class ") + SelectedObject->GetClass()->GetName(), FColor::Red); 
			continue;
		}

		FString OldName = SelectedObject->GetName();

		if (OldName.StartsWith(*PrefixFound)) // 기존이름에 이미 해당 접두어가 있는 경우
		{
			// 아래 문구를 띄우고 continue하여 지금 Asset은 작업하지 않고 넘어가고 다음 Asset에 접두어 적용
			DebugHeader::Print(OldName + TEXT(" already has prefix added"), FColor::Red);
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
		DebugHeader::ShowNotifyInfo(TEXT("Successfully renamed " + FString::FromInt(Counter) + " assets"));
	}
}

void UQuickAssetAction::RemoveUnusedAssets() // 사용하지 않는 Asset 제거
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData(); // 선택된 Asset들을 TArray변수에 다 담는다
	TArray<FAssetData> UnusedAssetsData; // 사용되지 않은 Asset들을 담는 TArray변수 선언

	FixUpRedirectors(); // 파일 경로 관련

	for (const FAssetData& SelectedAssetData : SelectedAssetsData)
	{
		// 해당 SelectedAssetsData에 레퍼런스된게 있는지 찾는다
		TArray<FString> AssetReferencers =
			UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAssetData.ObjectPath.ToString());

		if (AssetReferencers.Num() == 0) // 레퍼런스된게 없으면
		{
			UnusedAssetsData.Add(SelectedAssetData); // UnusedAssetsData에 해당 SelectedAssetData을 담는다
		}
	}

	if (UnusedAssetsData.Num() == 0) // 사용되지 않는 Asset이 없다면(=Asset들이 모두 레퍼런스되어 있다면)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No unused asset found among selected assets"), false);
		return; // 리턴 종료
	}

	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsData); // UnusedAssetsData에 담긴 Asset들을 제거. 제거한 Asset 수를 NumOfAssetsDeleted변수에 기록

	if (NumOfAssetsDeleted == 0) return; // 제거가 된 Asset이 없는 경우 리턴

	DebugHeader::ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(NumOfAssetsDeleted) + TEXT(" unused assets"))); // 언리얼 에디터 우측하단에 문구 띄우기
}

void UQuickAssetAction::FixUpRedirectors()
{
	TArray<UObjectRedirector*> RedirectorsToFixArray;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")); 

	FARFilter Filter;
	Filter.bRecursivePaths = true; // subfolder에 접근이 가능하도록 true 설정
	Filter.PackagePaths.Emplace("/Game"); // 어떤 폴더에 접근할 지 경로 설정
	Filter.ClassNames.Emplace("ObjectRedirector"); // 필터 하기를 희망하는 클래스의 이름

	TArray<FAssetData> OutRedirectors; // 결과를 저장할 TArry 변수

	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectors); // Filter한 결과를 OutRedirectors에 담는다

	for (const FAssetData& RedirectorData : OutRedirectors)
	{
		if (UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorData.GetAsset())) // Redirect 할게 있다면
		{
			RedirectorsToFixArray.Add(RedirectorToFix); // RedirectorsToFixArray에 RedirectorToFix 담음
		}
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	AssetToolsModule.Get().FixupReferencers(RedirectorsToFixArray);
}
