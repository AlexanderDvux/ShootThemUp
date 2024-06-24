// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUWeaponComponent.h"
#include "Weapons/STUBaseWeapon.h"
#include "GameFramework/Character.h"

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}




void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
    SpawnWeapons();

}

void USTUWeaponComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::StartFire()
{
    if (!CurrentWeapon)
    {
        return;
    }
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
    {
        return;
    }
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::SpawnWeapons()
{   
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!GetWorld() || !Character)
    {
        return;
    }
    for (auto WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon)
        {
            continue;
        }
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }

}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon *Weapon, USceneComponent *SceneComponent,
                                               const FName &SocketName)
{
    if (!Weapon ||!SceneComponent)
    {
        return;
    }
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }
    if (CurrentWeapon)
    {
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

}



