// Copyright Daniel V. Sandholt 2017

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT // This is a Macro that does noting,
			// but highlights the word OUT in the editor


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
															   OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	// Log player view point
	//	UE_LOG(LogTemp, Warning,
	//		   TEXT("Location: %s, Rotation: %s"),
	//		   *PlayerViewPointLocation.ToString(),
	//		   *PlayerViewPointRotation.ToString()
	//		   );
	
	FVector LineTraceEnd = PlayerViewPointLocation
	+ PlayerViewPointRotation.Vector() * Reach;
	
	// Draw a red trace in world to visualice
	DrawDebugLine(GetWorld(),
				  PlayerViewPointLocation,
				  LineTraceEnd,
				  FColor(255, 0, 0),//FLinearColor(255, 0, 0),
				  false, 0.f,
				  0,
				  10.f
				  );
	
	// Ray-cast out to reach distance
	
	// See what we hit
}

