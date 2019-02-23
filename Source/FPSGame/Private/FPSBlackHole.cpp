// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	ActorsDestroyedComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ActorsDestroyedComponent"));
	ActorsDestroyedComponent->SetSphereRadius(100);
	ActorsDestroyedComponent->SetupAttachment(MeshComponent);

	ActorsDestroyedComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapActorsDestroyed);

	PullingForseComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PullingForseComponent"));
	PullingForseComponent->SetSphereRadius(3000);
	PullingForseComponent->SetupAttachment(MeshComponent);


}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSBlackHole::OverlapActorsDestroyed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> ComponentsInRange;

	PullingForseComponent->GetOverlappingComponents(ComponentsInRange);
	for (int i = 0; i < ComponentsInRange.Num(); i++)
	{
		UPrimitiveComponent* PrimitiveComponent = ComponentsInRange[i];

		if (PrimitiveComponent && PrimitiveComponent->IsSimulatingPhysics())
		{
			const float SphereRadius = PullingForseComponent->GetScaledSphereRadius();
			const float ForseStrength = -2000; // minus is pulling; plus is pushing

			PrimitiveComponent->AddRadialForce(this->GetActorLocation(), SphereRadius, ForseStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

