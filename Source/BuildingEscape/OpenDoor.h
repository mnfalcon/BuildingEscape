// Copyright Matias Nehuen Falcon 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

	float TotalMassOfActors() const;

	void PressSlab(float DeltaTime);
	void UnpressSlab(float DeltaTime);

private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	float CurrentYaw;
	float InitialYaw;


	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 6.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float MassRequiredToOpen = 70.f;

	UPROPERTY(EditAnywhere)
	AActor* Slab;

	UPROPERTY(EditAnywhere)
	float SlabTargetPitch = -10.f;
	float SlabCurrentPitch;
	float SlabInitialPitch;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;



};
