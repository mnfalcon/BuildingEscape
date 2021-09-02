// Copyright Matias Nehuen Falcon 2021


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT_PARAMETER

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has OpenDoor component but no PressurePlate"), *GetOwner()->GetName());
	}

	if (Slab)
	{
		SlabInitialPitch = Slab->GetActorLocation().Z;
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	//UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), GetOwner()->GetActorRotation().Yaw);
	if (TotalMassOfActors() > MassRequiredToOpen)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (Slab)
	{
		PressSlab(DeltaTime);
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (Slab)
	{
		UnpressSlab(DeltaTime);
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT_PARAMETER OverlappingActors);

	for (int32 i = 0; i < OverlappingActors.Num(); i++)
	{
		TotalMass += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::PressSlab(float DeltaTime)
{
	SlabCurrentPitch = FMath::Lerp(SlabCurrentPitch, SlabTargetPitch, DeltaTime * 1.f);
	FVector SlabLocation = Slab->GetActorLocation();
	SlabLocation.Z = SlabCurrentPitch;
	Slab->SetActorLocation(SlabLocation);
}

void UOpenDoor::UnpressSlab(float DeltaTime)
{
	SlabCurrentPitch = FMath::Lerp(SlabCurrentPitch, SlabInitialPitch, DeltaTime * 1.f);
	FVector SlabLocation = Slab->GetActorLocation();
	SlabLocation.Z = SlabCurrentPitch;
	Slab->SetActorLocation(SlabLocation);
}