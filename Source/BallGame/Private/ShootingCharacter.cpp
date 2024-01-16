// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingCharacter.h"
#include <Runtime/Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h>
#include "Components/BillboardComponent.h"
#include "Item.h"
#include "Components/SphereComponent.h"


AShootingCharacter::AShootingCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AShootingCharacter::BeginPlay()
{
	Super::BeginPlay();

	Billboard = this->FindComponentByClass<UBillboardComponent>();
	PhysicsHandle = this->FindComponentByClass<UPhysicsHandleComponent>();

	CurrentItemClass = DefaultItemClass;
	SpawnAndGrabItem();
}

void AShootingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We check if the components we use in the following are null, if yes we stop executing the function.
	// We do this because we access them in the following, if we access them when they are null however, we would get an exception. 
	if (Billboard == nullptr && PhysicsHandle == nullptr) return;

	// Update the target of the physics handle to the billboard position and rotation. 
	PhysicsHandle->SetTargetLocationAndRotation(Billboard->GetComponentLocation(), Billboard->GetComponentRotation());

	// We need to wake the physics handle when it is done interpolating to the target. Otherwise it will get "stuck". 
	if (PhysicsHandle->GetGrabbedComponent() && !PhysicsHandle->GetGrabbedComponent()->RigidBodyIsAwake())
	{
		PhysicsHandle->GetGrabbedComponent()->SetSimulatePhysics(true);
	}
}

void AShootingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind start and stop shooting functions to our input events. 
	InputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AShootingCharacter::StartShoot);
	InputComponent->BindAction("PrimaryAction", IE_Released, this, &AShootingCharacter::StopShoot);
}

void AShootingCharacter::StartShoot()
{
	bIsShooting = true;
}

void AShootingCharacter::StopShoot()
{
	if (Item) ShootItem();
	bIsShooting = false;

	// Set timer to spawn new item
	FTimerHandle FuzeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &AShootingCharacter::SpawnAndGrabItem, RespawnTime,
	                                       false);
}

void AShootingCharacter::SpawnAndGrabItem()
{
	// Calc spawn position and spawn rotation; set spawn params
	const FVector SpawnPos = Billboard->GetComponentLocation();
	const FRotator SpawnRot = Billboard->GetComponentRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn a new item
	Item = GetWorld()->SpawnActor<AItem>(CurrentItemClass, SpawnPos, SpawnRot, SpawnParams);

	if (Item)
	{
		// Ensure simulation is running for all bodies in this component
		UPrimitiveComponent* ItemCollider = Item->FindComponentByClass<UPrimitiveComponent>();

		// Attach item to physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(ItemCollider, "", Item->GetActorLocation(),
		                                                   Item->GetActorRotation());
	}
}

void AShootingCharacter::DiscardItem()
{
	if (!Item) return;

	Item->Destroy();
	Item = nullptr;
}

void AShootingCharacter::SwitchItem(TSubclassOf<AItem> NewItemClass)
{
	// First check if we really need to switch now
	if (bIsShooting) return;
	if (CurrentItemClass == NewItemClass) return; 

	// If we do need to switch, switch!
	CurrentItemClass = NewItemClass;
	DiscardItem();
	SpawnAndGrabItem();

	// Trigger the OnItemSwitched event. We pass the current item, so we can use it somewhere else
	OnItemSwitched.Broadcast(Item);
}

void AShootingCharacter::ShootItem()
{
	// Detach the item from the physics handle
	PhysicsHandle->ReleaseComponent();

	// Retrieve item mesh 
	UStaticMeshComponent* ItemMesh = Item->FindComponentByClass<UStaticMeshComponent>();

	// Apply an impulse to the item so it is thrown away by the physics system
	ItemMesh->AddImpulse(
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector() * ShootingStrength *
		ForceMultiplier);

	// Broadcast the OnItemShot delegate, so we can bind to it in blueprint. 
	OnItemShot.Broadcast();

	// We also call Use() on our Item so we can let it know it is used. 
	Item->Use();

	// Item variable now stores nullptr because we are not holding the item anymore
	Item = nullptr;
}
