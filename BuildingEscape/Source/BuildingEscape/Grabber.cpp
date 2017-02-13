// Copyright Daniel V. Sandholt 2017

#include "BuildingEscape.h"
#include "Grabber.h"

// This is a Macro that does noting, but highlights the word OUT in the editor
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber(){
	/// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	/// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay(){
	Super::BeginPlay();
	PhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::PhysicsHandleComponent(){
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle){
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle detected on %s"), *GetOwner()->GetName() );
	}
}

void UGrabber::SetupInputComponent(){
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent){
		UE_LOG(LogTemp, Warning, TEXT("InputComponent detected on %s"), *GetOwner()->GetName() );

		/// Bind the input actions
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}else{
		UE_LOG(LogTemp, Error, TEXT("No InputComponent detected on %s"), *GetOwner()->GetName() );
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach(){
	/// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner() );
	
	/// Line-trace (Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
											OUT Hit,
											PlayerViewPointLocation,
											LineTraceEnd,
											FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
											TraceParameters
											);
	
	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit !=nullptr) /// nil skiftet til nullptr, kan måske slette "!= nullptr"
		UE_LOG(LogTemp, Warning, TEXT("Tracer hit: %s"), *ActorHit->GetName() );
	
	return Hit;
}

void UGrabber::Grab() {
	UE_LOG( LogTemp, Warning, TEXT( "Grab pressed" ) );

	/// Line-trace (ray-cast) and  reach any actors with physicsHandle
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if ( ActorHit ) {
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release(){
	UE_LOG(LogTemp, Warning, TEXT("Grab released") );
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ){
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	/// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation );

	FVector LineTraceEnd = PlayerViewPointLocation+PlayerViewPointRotation.Vector() * Reach;

	if ( PhysicsHandle->GrabbedComponent ) {
		PhysicsHandle->SetTargetLocation( LineTraceEnd );
	}
}

