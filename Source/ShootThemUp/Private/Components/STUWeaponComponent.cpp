
#include "Components/STUWeaponComponent.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapons/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentWeaponIndex = 0;
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
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
    if (!CanFire())
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
    if (!CanEquip())
    {
        return;
    }
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
    if (!Weapon || !SceneComponent)
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
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage *Animation)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }
    Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
    if (!EquipAnimMontage)
    {
        return;
    }
    const auto NotifyEvents = EquipAnimMontage->Notifies;
    for (auto NotifyEvent : NotifyEvents)
    {
        auto EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify);
        if (EquipFinishedNotify)
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent *MeshComponent)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh())
    {
        return;
    }
    UE_LOG(LogWeaponComponent, Display, TEXT("Equip Finished"));
    EquipAnimInProgress = false;
}

bool USTUWeaponComponent::CanEquip() const
{

    return !EquipAnimInProgress;
}

bool USTUWeaponComponent::CanFire() const
{

    return CurrentWeapon && !EquipAnimInProgress;
}
