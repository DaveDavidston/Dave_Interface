// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class BALLGAME_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called when the item was hit by other component. */
	UFUNCTION()
	void OnActHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	/** Called when the item is used for shooting. */
	void Use();

	/** Called when the item is killed */
	UFUNCTION()
	void Kill();

	/** The ID that tells us which target the item matches to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MatchID;

protected:
	/** If the item has been used for shooting already. */
	bool bWasUsed = false;
};
