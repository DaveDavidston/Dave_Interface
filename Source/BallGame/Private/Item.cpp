#include "Item.h"
#include "Target.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// We bind OnActHit() on our OnActorHit event, so the function is called when the event is triggered. 
	OnActorHit.AddDynamic(this, &AItem::OnActHit);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::OnActHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bWasUsed) return;

	// We check if the item was acutally hitting a target.
	if (ATarget* Target = Cast<ATarget>(OtherActor))
	{
		Destroy();

		// The hit item matches the target, so we want to call the hit by item event on it and then destroy it. 
		if (MatchID == Target->MatchID)
		{
			Target->HitByItem();
			Target->Destroy();
		}
	}
}

void AItem::Use()
{
	bWasUsed = true;

	// Set timer to kill item 5 seconds after it is used
	FTimerHandle TimerHandle_Destroy;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Destroy, this, &AItem::Kill, 5.f, false);
}

// Just a wrapper basically because Destroy() needs params and the timer in Use() would be more difficult to setup then
void AItem::Kill()
{
	Destroy();
}
