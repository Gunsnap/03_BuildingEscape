
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnOpenRequest );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY( BlueprintAssignable )
		FOnOpenRequest OnOpenRequest;
	
private:
	void OpenDoor();
	void CloseDoor();

	// Returns mass in kg
	float GetTotalMassOffActorsOnPlate();
	
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = -90.0f;
	
	UPROPERTY(VisibleAnywhere)
	float DoorCloseDelay = 0.25f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	
	float LastDoorOpenTime;
	
	AActor* Owner = nullptr;
};
