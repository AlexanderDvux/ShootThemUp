#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBaseWeapon();

	virtual void StartFire();
	virtual void StopFire();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent *WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

   
    
   
	virtual void BeginPlay() override;
    virtual void MakeShot();
    APlayerController *GetPlayerController() const;
    bool GetPlayerViewPoint(FVector &ViewLocation,FRotator &ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;
    virtual bool GetTraceData(FVector &InTraceStart,FVector &InTraceEnd) const;
    void MakeHit(FHitResult &HitResult,const FVector &TraceStart,const FVector &TraceEnd);

  
};
