
#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"
#include "Engine/World.h"
#include "TimerManager.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent,All,All)

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
    OnHealthChanged.Broadcast(CurrentHealth);
    AActor *Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);     
    }
}


void USTUHealthComponent::OnTakeAnyDamageHandle(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                              AController *InstigatedBy, AActor *DamageCauser)
{
    if (Damage <= 0.0f ||IsDead())
    {
        return;
    }

    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth);
    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal && GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime,
                                               true, HealDelay);
    }


   // UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);
    /*if (DamageType)
    {
        if (DamageType->IsA<USTUFireDamageType>())
        {
            UE_LOG(LogHealthComponent,Display,TEXT("so hot"))

        }
        else if (DamageType->IsA<USTUIceDamageType>())
        {
            UE_LOG(LogHealthComponent, Display, TEXT("so cold"))
        }
        else
        {
            UE_LOG(LogHealthComponent, Display, TEXT("none"))     
        }
    }*/
}

void USTUHealthComponent::HealUpdate()
{
    CurrentHealth = FMath::Min(CurrentHealth + HealModifier, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth);
    if (CurrentHealth == MaxHealth && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}
