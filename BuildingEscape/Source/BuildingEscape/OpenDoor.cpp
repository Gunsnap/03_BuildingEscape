// Copyright Daniel V. Sandholt 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();
	Owner = GetOwner();

	if( !PressurePlate ) UE_LOG(LogTemp, Error, TEXT("%s missing a PressurePlate"), *GetOwner()->GetName());
}

// Åbner døren
void UOpenDoor::OpenDoor(){
	///Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	OnOpenRequest.Broadcast();
}

// Lukker døren
void UOpenDoor::CloseDoor(){
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll trigger
	if( GetTotalMassOffActorsOnPlate() > 30.0f ) { // TODO make into parameter
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	auto TimeNow = GetWorld()->GetTimeSeconds();

	if( LastDoorOpenTime + DoorCloseDelay < TimeNow )
		CloseDoor();

}

float UOpenDoor::GetTotalMassOffActorsOnPlate() {
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if( !PressurePlate ) return 0.f;
	PressurePlate->GetOverlappingActors( OUT OverlappingActors );
	
	for( const auto* Actor : OverlappingActors ) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}