// Copyright Matias Nehuen Falcon 2021


#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "OpenDoorRight.h"

// Sets default values for this component's properties
UOpenDoorRight::UOpenDoorRight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorRight::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has OpenDoor component but no PressurePlate"), *GetOwner()->GetName());
	}

}


// Called every frame
void UOpenDoorRight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	//UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), GetOwner()->GetActorRotation().Yaw);
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		/*if (GetWorld()->GetTimeSeconds() - DoorLastOpened >= 2.0f)
		{
			CloseDoor(DeltaTime);
		}*/
	}
}

void UOpenDoorRight::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * 0.5f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoorRight::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * 0.5f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

