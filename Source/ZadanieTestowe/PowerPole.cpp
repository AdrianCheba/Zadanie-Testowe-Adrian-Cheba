// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerPole.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CableComponent.h"
#include "Components/SceneComponent.h"

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

	PolePlank = CreateDefaultSubobject<UStaticMeshComponent>("PolePlank");
	PolePlank->SetupAttachment(RootComponent, FName(TEXT("AllSecket")));

	PoleInsulatorRight = CreateDefaultSubobject<UStaticMeshComponent>("PoleInsulatorRight");
	PoleInsulatorRight->SetupAttachment(RootComponent, FName(TEXT("AllSecket")));

	PoleInsulatorLeft = CreateDefaultSubobject<UStaticMeshComponent>("PoleInsulatorLeft");
	PoleInsulatorLeft->SetupAttachment(RootComponent, FName(TEXT("AllSecket")));


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
	GeneratePoles();
}

void APowerPole::GeneratePoles()
{
	if (!MySpline || !Pole)
		return;

	ClearComponents();
	
	const int NumberOfMeshes = FMath::FloorToInt(MySpline->GetSplineLength() / DistanceBetweenPoles);

	for (int i = 0; i <= NumberOfMeshes; i++)
	{
		const float Distance = i * DistanceBetweenPoles;
		const FVector Location = MySpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		FRotator Rotation = MySpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);

		UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
		MeshComponent->SetStaticMesh(Pole->GetStaticMesh());
		MeshComponent->SetWorldLocationAndRotation(Location, Rotation);
		MeshComponent->SetupAttachment(RootComponent);
		MeshComponent->RegisterComponent();
		PoleMeshComponents.Add(MeshComponent);
		AttacheComponents(MeshComponent);	

		if (i > 0) 
		{
			FVector StartSocketLocation = MeshComponent->DoesSocketExist(FName("InsulatorLeftSocket")) ? MeshComponent->GetSocketLocation(FName("InsulatorLeftSocket")) : MeshComponent->GetComponentLocation();
			FVector EndSocketLocation = PoleMeshComponents[i - 1]->DoesSocketExist(FName("InsulatorRightSocket")) ? PoleMeshComponents[i - 1]->GetSocketLocation(FName("InsulatorRightSocket")) : PoleMeshComponents[i - 1]->GetComponentLocation();


			UCableComponent* CableComponent = NewObject<UCableComponent>(this);
			CableComponent->SetupAttachment(Pole);

			CableComponent->SetAttachEndToComponent(MeshComponent, FName("InsulatorLeftSocket"));
			CableComponent->SetAttachEndToComponent(PoleMeshComponents[i-1], FName("InsulatorRightSocket"));
			CableComponent->CableWidth = 1;

			FVector StartLocation = MeshComponent->GetSocketLocation(FName("InsulatorLeftSocket"));
			FVector EndLocation = PoleMeshComponents[i - 1]->GetSocketLocation(FName("InsulatorRightSocket"));
			CableComponent->SetWorldLocation(StartSocketLocation);
			CableComponent->EndLocation = (EndSocketLocation - StartSocketLocation) / Distance;
			CableComponent->RegisterComponent();

			CableComponents.Add(CableComponent);
		}
	}
}

void APowerPole::AttacheComponents(UStaticMeshComponent* MeshComponent)
{
	UStaticMeshComponent* PlankComponent = NewObject<UStaticMeshComponent>(this);
	PlankComponent->SetStaticMesh(PolePlank->GetStaticMesh());
	PlankComponent->SetupAttachment(MeshComponent, FName(TEXT("AllSecket")));
	PlankComponent->RegisterComponent();
	OtherMeshComponents.Add(PlankComponent);

	UStaticMeshComponent* InsulatorRightComponent = NewObject<UStaticMeshComponent>(this);
	InsulatorRightComponent->SetStaticMesh(PoleInsulatorRight->GetStaticMesh());
	InsulatorRightComponent->SetupAttachment(MeshComponent, FName(TEXT("AllSecket")));
	InsulatorRightComponent->RegisterComponent();
	OtherMeshComponents.Add(InsulatorRightComponent);

	UStaticMeshComponent* InsulatorLeftComponent = NewObject<UStaticMeshComponent>(this);
	InsulatorLeftComponent->SetStaticMesh(PoleInsulatorLeft->GetStaticMesh());
	InsulatorLeftComponent->SetupAttachment(MeshComponent, FName(TEXT("AllSecket")));
	InsulatorLeftComponent->RegisterComponent();
	OtherMeshComponents.Add(InsulatorLeftComponent);
}

void APowerPole::ClearComponents()
{
	for (UStaticMeshComponent* Component : PoleMeshComponents)
	{
		if (Component)
		{
			Component->DestroyComponent();
		}
	}
	PoleMeshComponents.Empty();
	
	for (UStaticMeshComponent* Component2 : OtherMeshComponents)
	{
		if (Component2)
		{
			Component2->DestroyComponent();
		}
	}
	OtherMeshComponents.Empty();
	
	for (UCableComponent* Component5 : CableComponents)
	{
		if (Component5)
		{
			Component5->DestroyComponent();
		}
	}
	CableComponents.Empty();
}

