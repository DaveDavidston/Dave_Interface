// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

class AItem;

UCLASS()
class BALLGAME_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Constructor */
	ATarget();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Triggered when a target was hit by an item. */
	UFUNCTION(BlueprintImplementableEvent)
	void HitByItem();

	/** The ID that tells us which item the target matches to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MatchID; 
};
