#include "Weapons/STULauncherWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/STUProjectile.h"

void ASTULauncherWeapon::MakeShot()
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

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass,SpawnTransform);
    if (!Projectile)
    {
        return;
    }
    Projectile->SetShotDirection(Direction);
    Projectile->SetOwner(GetOwner());
    Projectile->FinishSpawning(SpawnTransform);


}

void ASTULauncherWeapon::StartFire()
{
    MakeShot();

}
