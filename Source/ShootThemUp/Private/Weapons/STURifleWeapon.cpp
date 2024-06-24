// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld())
    {
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 3.0f);
       // UE_LOG(LogBaseWeapon, Display, TEXT("Bone %s"), *HitResult.BoneName.ToString());
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }


}

void ASTURifleWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);

}


void ASTURifleWeapon::MakeDamage(const FHitResult &HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor)
    {
        return;
    }
    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}



bool ASTURifleWeapon::GetTraceData(FVector &InTraceStart, FVector &InTraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    InTraceStart = ViewLocation; // SocketTransform.GetLocation();
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection =
        FMath::VRandCone(ViewRotation.Vector(), HalfRad); // SocketTransform.GetRotation().GetForwardVector();
    InTraceEnd = InTraceStart + ShootDirection * TraceMaxDistance;

    return true;
}
