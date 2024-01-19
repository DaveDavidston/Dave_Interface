// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShootingCharacter.generated.h"

class UBillboardComponent;
class AItem;
class UPhysicsHandleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemSwitched, AItem*, Item);

UCLASS()
class BALLGAME_API AShootingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Constructor */
	AShootingCharacter();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	
public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	/** Called when we press left mouse button. */
	void StartShoot();

	/** Called when we release left mouse button. */
	void StopShoot();

	/** Shoots the item in direction of the forward vector of the item. */
	void ShootItem();

	/** Spawns a new item and grabs it, meaning attaches it to the physics handle. */
	void SpawnAndGrabItem();

	/** Discards the current item. */
	void DiscardItem();

	/** Switch the item we are throwing. */
	UFUNCTION(BlueprintCallable)
	void SwitchItem(TSubclassOf<AItem> NewItemClass); 

	/** Called when the shooting strength is updated. */
	UFUNCTION(BlueprintImplementableEvent)
	void ShootingStrengthUpdated(float CurrentShootingTime);
	
	/** The blueprint class of the item we are spawning. */
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<AItem> CurrentItemClass;

	/** The blueprint class of the item we are spawning. */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> DefaultItemClass;

private: 
	/** Stores a reference to the item we are currently holding. If nullptr, we are not holding a item. */
	AItem* Item;

	/** The billboard component we use to determine the location and rotation of the item we are holding. */
	UBillboardComponent* Billboard;
	
	/** The physics handle where we attach the item: */
	UPhysicsHandleComponent* PhysicsHandle;
	
public: 
	/** A multiplier to tweak the impulse the item is thrown with. */
	UPROPERTY(EditAnywhere)
	float ForceMultiplier = 3.f;

	/** The time to respawn the item in seconds. */
	UPROPERTY(EditAnywhere)
	float RespawnTime = 0.2f;

	
private:

	/** The strength of the impulse we shoot the item with */
	float ShootingStrength = 35000; 
	
	/** If the player is currently shooting, meaning they have the left mouse button clicked. */
	bool bIsShooting = false; 

	/** Fired when a item was shot. */
	UPROPERTY(BlueprintAssignable)
	FItemShot OnItemShot;

	/** Fired when item was switched. */
	UPROPERTY(BlueprintAssignable)
	FItemSwitched OnItemSwitched; 
	
};
