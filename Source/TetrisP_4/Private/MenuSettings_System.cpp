#include "MenuSettings_System.h"
#include "PlayerPawn.h"
#include "TetrisSystems/TetrisTimeManager.h"


UMenuSettings_System::UMenuSettings_System()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMenuSettings_System::SetDifficulty(int dif)
{
	UTetrisTimeManager* TTM = Cast<APlayerPawn>(GetOwner())->Tetris->TetrisTime;
	if (dif < TTM->Delays.Num())
	{
		TTM->StartSpeedIndex = dif;
		return;
	}


}

void UMenuSettings_System::BindMaterialForColorSettings(FString name, UMaterialInstanceDynamic* Mater)
{
	TArray<UMaterialInstanceDynamic*>* Materials;
	Materials = ColorSettingsMaterials.Find(name);
	if (Materials)
	{
		Materials->Add(Mater);
		return;
	}
	else
	{
		ColorSettingsMaterials.Add(name, TArray<UMaterialInstanceDynamic*>()).Add(Mater);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, name );
	}
}

void UMenuSettings_System::BindMeshMaterialForColorSettings(FString name, UStaticMeshComponent* SMC)
{
	UMaterialInstanceDynamic* Mater = UMaterialInstanceDynamic::Create(SMC->GetMaterial(0), SMC->GetOwner());
	SMC->SetMaterial(0, Mater);
	TArray<UMaterialInstanceDynamic*>* Materials;
	Materials = ColorSettingsMaterials.Find(name);
	if (Materials)
	{
		Materials->Add(Mater);
		return;
	}
	else
	{
		ColorSettingsMaterials.Add(name, TArray<UMaterialInstanceDynamic*>()).Add(Mater);
	}
}

void UMenuSettings_System::ChangeColorPreset()
{
	++MenuPreset.CurrentColorPreset;
	if (MenuPreset.CurrentColorPreset >= MenuPreset.ColorPresets.Num())
	{
		MenuPreset.CurrentColorPreset = 0;
	}
	
	//Setting Dynamic
	ChangeMaterialColor("Blocks:Empty", MenuPreset.ColorPresets[MenuPreset.CurrentColorPreset][0]);
	ChangeMaterialColor("Blocks:Filled", MenuPreset.ColorPresets[MenuPreset.CurrentColorPreset][1]);
	ChangeMaterialColor("TV:Empty", MenuPreset.ColorPresets[MenuPreset.CurrentColorPreset][2]);
	ChangeMaterialColor("TV:Filled", MenuPreset.ColorPresets[MenuPreset.CurrentColorPreset][3]);
	ChangeMaterialColor("TV:Box", MenuPreset.ColorPresets[MenuPreset.CurrentColorPreset][4]);

	//Setting Static Boxes 1
	for (int i = 0; i < 4; ++i)
	ChangeStaticMaterialColor(i, MenuPreset.ColorPresets[MenuPreset.CurrentColorPreset][5]);

	//Setting Static Boxes 2
	for(int i = 4; i < 10; ++i)
	ChangeStaticMaterialColor(i, MenuPreset.ColorPresets[MenuPreset.CurrentColorPreset][6]);



}

void UMenuSettings_System::AddColorPreset(TArray<FLinearColor> tal)
{
	if (tal.Num() == COLOR_PRESET_OBJECTS_NUM)
	{
		MenuPreset.ColorPresets.Add(tal);
		UE_LOG(Log_MenuSettings, Display, TEXT("Added Color Preset "));

	}
	else
	{
		UE_LOG(Log_MenuSettings, Warning, TEXT("Cant Add New Color Preset : tal.Num() is not equal to COLOR_PRESET_OBJ_NUM"));
	}
}

void UMenuSettings_System::ChangeMaterialColor(FString&& MaterialName, FLinearColor Color)
{
	TArray<UMaterialInstanceDynamic*>* MaterialsDyn = ColorSettingsMaterials.Find(MaterialName);
	if (MaterialsDyn)
	{
		int count = 0;
		for (int i = 0; i < MaterialsDyn->Num(); ++i)
		{
			(*MaterialsDyn)[i]->SetVectorParameterValue("BaseColor", Color);
		}
		return;
	}
}

void UMenuSettings_System::ChangeStaticMaterialColor(int SM_index, FLinearColor Color)
{
	if (SM_index >= SM_ColorSettings.Num())
	{
		return;
	}

	SM_ColorSettings[SM_index]->SetVectorParameterValue("BaseColor", Color);
}

void UMenuSettings_System::BeginPlay()
{
	Super::BeginPlay();	

	UMaterialInstanceDynamic* DynMat;
	for (int i = 0; i < SM_Actors.Num(); ++i)
	{
		if (!SM_Actors[i])
			continue;
		DynMat = UMaterialInstanceDynamic::Create(SM_Actors[i]->GetStaticMeshComponent()->GetMaterial(0),SM_Actors[i]);
		SM_Actors[i]->GetStaticMeshComponent()->SetMaterial(0, DynMat);
		if (DynMat)
		{
			SM_ColorSettings.Add(DynMat);
		}

	}
}


