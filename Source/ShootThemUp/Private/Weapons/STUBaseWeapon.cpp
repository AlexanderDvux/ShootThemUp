#include "Weapons/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFrameWork/Character.h"
#include "GameFrameWork/Controller.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{
   
}

void ASTUBaseWeapon::StopFire()
{
    
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
}

void ASTUBaseWeapon::MakeShot()
{
  
}

APlayerController *ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
    {
        return nullptr;
    }
    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const
{
    const APlayerController *Controller = GetPlayerController();

    if (!Controller)
    {
        return false;
    }
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector &InTraceStart, FVector &InTraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
   if( !GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
       return false;
    }

    InTraceStart = ViewLocation;              // SocketTransform.GetLocation();
    const FVector ShootDirection = ViewRotation.Vector(); // SocketTransform.GetRotation().GetForwardVector();
    InTraceEnd = InTraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult & HitResult, const FVector &TraceStart, const FVector &TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility,
                                         CollisionParams);
}

