# BuildingEscape
A project used to learn. Basic functionalities include setting some actors' location and rotation, and grabbing a physics component. Made purely in C++.

## Grabber.cpp
This should be a component on the player's pawn.
### How to Grab a physics component
The first thing we need is a line trace, to know which object is being looked at. This is acomplished by the GetFirstPhysicsBodyInReach() function. The Hit variable is an out parameter where we save the FHitResult. 
The LineTraceSingleByObjectType (read docs https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Engine/UWorld/LineTraceSingleByObjectType/) uses two things we need beforehand:
- GetLineTraceEnd(): this returns an FVector that starts on the player's camera and it's length is determined by the LineReach variable.
- GetPlayerViewPointLocation(): this value is also used when getting the line trace. Self explanatory. Read docs https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/GameFramework/APlayerController/GetPlayerViewPoint/
- Also keep in mind the collision channel.

The next thing is a PhysicsHandle, which we get with the FindPhysicsHandle() function. After that, the grab function is pretty simple. Using whatever the GetFirstPhysicsBodyInReach() gets us, we define the component to grab and the actor that was hit. If there's no actor hit because nothing was hit, then nothing would happen because there is protection from a null pointer in place. If it is an actor in the physics channel, then we call the UPhysicsHandleComponent::GrabComponentAtLocation() with the component to grab, the name (using NAME_None in my case), and the grab location. 

Now the physics object is attached to the PhysicsHandle. The PhysicsHandle is a component the player's pawn should have for this to work (otherwise, nothing will happen). To release the component you simply call the UPhysicsHandleComponent::ReleaseComponent() function.

## OpenDoor.cpp
This should be a component in the door we want to open.
### How to open a door using C++
For this we use a ATriggerVolume (PressurePlate) which should be defined on the level in the editor. You can define any condition you want for the door to open. Maybe you could use a lever which you move also with C++ using similar methods to the ones used here, and setting it to open based on the lever's position. But lets continue.
The OpenDoor() and CloseDoor() functions are quite similar. What do we use? 
We need:
- The door's current angle
- The door's target angle (at which angle the door completely open)
- The door's initial angle (we get it on BeginPlay())
With those three values we use the FMath::Lerp() https://docs.unrealengine.com/4.27/en-US/API/Runtime/Core/Math/FMath/Lerp/
Notice we are passing DeltaTime multiplied by another float. We pass in DeltaTime because otherwise the speed would vary with the hardware the game is being run on.
After that we set the door's rotation (GetOwner()->SetActorRotation()). Notice we get the owner becuase OpenDoor is a child component. You can also see PressSlab() function to see it's quite similar if we want to move an object instead of rotating it.

On the TickComponent we check for the total mass of the actors that are overlapping the TriggerVolume. This involves iterating through actors. For this we use a TArray for the AActor class and we leave it uninitialized. After that we call the ATriggerVolume::GetOverlappingActors() which stores every actor on the TriggerVolume on the TArray we created.

__Note: the physics object we place on the TriggerVolume, must have the option "generates overlap events" (or something like that) on it's collision properties.__

After that is a simple for loop where we add to the total mass the mass of each actor's component. If you're using actors that have several UPrimitiveComponents maybe you'll have to do double loop. But i'm not so sure about this.

And that's it.
