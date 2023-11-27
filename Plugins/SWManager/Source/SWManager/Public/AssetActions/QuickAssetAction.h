#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"

#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"	
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Engine/Texture.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"

#include "QuickAssetAction.generated.h"

/** 우클릭 AssetAction에 새로운 기능 추가하기
 *  - Asset 복제
 *	- Asset 접두어 붙이기
 */
UCLASS()
class SWMANAGER_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
	void DuplicateAssets(int32 NumOfDuplicates); // Asset 복제

	UFUNCTION(CallInEditor)
	void AddPrefixes(); // 접두어 달기

	UFUNCTION(CallInEditor)
	void RemoveUnusedAssets(); // 사용하지 않는 Asset 제거

private:
	void FixUpRedirectors(); // Fix Up Redirectors 

	TMap<UClass*, FString>PrefixMap =
	{
		{UBlueprint::StaticClass(),TEXT("BP_")},
		{UStaticMesh::StaticClass(),TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstanceConstant::StaticClass(),TEXT("MI_")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MF_")},
		{UParticleSystem::StaticClass(), TEXT("PS_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("SW_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
		{UUserWidget::StaticClass(), TEXT("WBP_")},
		{USkeletalMesh::StaticClass(),TEXT("SKM_")},
		{USkeletalMeshComponent::StaticClass(), TEXT("SK_")},
		{USkeleton::StaticClass(),TEXT("Skel_")},
		{UPhysicsAsset::StaticClass(),TEXT("PHY_")},
		{UNiagaraSystem::StaticClass(), TEXT("NS_")},
		{UNiagaraEmitter::StaticClass(), TEXT("NE_")}
	};
};
