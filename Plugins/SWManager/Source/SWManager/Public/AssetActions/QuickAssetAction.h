#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "QuickAssetAction.generated.h"

/** ��Ŭ�� AssetAction�� ���ο� ��� �߰��ϱ�
 * 
 */
UCLASS()
class SWMANAGER_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
	void TestFunc();
};
