// Copyright Matias Nehuen Falcon 2021


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
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

	FindAudioComponent();
	FindPressurePlate();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if (Slab)
	{
		SlabInitialZ = Slab->GetActorLocation().Z;
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	bCloseDoorSound = false;
	if (AudioComponent)
	{
		if (!bOpenDoorSound) 
		{
			AudioComponent->Play();
			bOpenDoorSound = true;
		}
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

	bOpenDoorSound = false;
	if (AudioComponent)
	{
		if (!bCloseDoorSound)
		{
			AudioComponent->Play();
			bCloseDoorSound = true;
		}
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; } // Protecting from null pointer
	PressurePlate->GetOverlappingActors(OUT_PARAMETER OverlappingActors);

	for (int32 i = 0; i < OverlappingActors.Num(); i++)
	{
		TotalMass += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::PressSlab(float DeltaTime)
{
	SlabCurrentZ = FMath::Lerp(SlabCurrentZ, SlabTargetZ, DeltaTime * SlabPressSpeed);
	FVector SlabLocation = Slab->GetActorLocation();
	SlabLocation.Z = SlabCurrentZ;
	Slab->SetActorLocation(SlabLocation);
}

void UOpenDoor::UnpressSlab(float DeltaTime)
{
	SlabCurrentZ = FMath::Lerp(SlabCurrentZ, SlabInitialZ, DeltaTime * SlabPressSpeed);
	FVector SlabLocation = Slab->GetActorLocation();
	SlabLocation.Z = SlabCurrentZ;
	Slab->SetActorLocation(SlabLocation);
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No audio component found on %s!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has OpenDoor component but no PressurePlate"), *GetOwner()->GetName());
	}
}