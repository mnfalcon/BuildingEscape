// Copyright Matias Nehuen Falcon 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoorRight.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoorRight : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoorRight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

private:
	UPROPERTY(EditAnywhere)
		float TargetYaw = -90.f;
	float CurrentYaw;
	float InitialYaw;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpens;

	float DoorLastOpened = 0.f;
	float DoorCloseDelay = 20.f;
		
};
