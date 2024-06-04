// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerPole.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APowerPole::APowerPole()
{
	PrimaryActorTick.bCanEverTick = true;

	MySpline = CreateDefaultSubobject<USplineComponent>("MySpline");
	MySpline->SetupAttachment(RootComponent);
	MySpline->bDrawDebug = true;

	RootComponent = MySpline;

	Pole = CreateDefaultSubobject<UStaticMeshComponent>("Pole");
	Pole->SetupAttachment(RootComponent);
		
	
	DistanceBetweenPoles = 100.0f;
}

void APowerPole::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerPole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerPole::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateMeshes();
}

void APowerPole::GenerateMeshes()
{
	if (!MySpline || !Pole)
		return;

	for (UStaticMeshComponent* MeshComponent : MeshComponents)
	{
		if (MeshComponent)
		{
			MeshComponent->DestroyComponent();
		}
	}
	MeshComponents.Empty();

	const int32 NumberOfMeshes = FMath::FloorToInt(MySpline->GetSplineLength() / DistanceBetweenPoles);

	for (int32 i = 0; i <= NumberOfMeshes; ++i)
	{
		const float Distance = i * DistanceBetweenPoles;
		const FVector Location = MySpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	//	const FRotator Rotation = MySpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
		MeshComponent->SetStaticMesh(Pole->GetStaticMesh());
		MeshComponent->SetWorldLocation(Location);
		MeshComponent->SetupAttachment(RootComponent);
		MeshComponent->RegisterComponent();
		MeshComponents.Add(MeshComponent);
	}
}

