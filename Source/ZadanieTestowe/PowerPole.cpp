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
	CablesThickness = 1.0f;
	CablesNumberOfSegments = 10.0f;
	CablesNumberOfSegments = 1.0f;
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
			if(GenerateCables)
			GenerateLines(MeshComponent, PoleMeshComponents[i - 1], Distance);
	}
}

void APowerPole::GenerateLines(UStaticMeshComponent* StartPoint, UStaticMeshComponent* EndPoint, float Distance)
{
	FVector StartSocketLocation = StartPoint->DoesSocketExist(FName("InsulatorRightSocket")) ? StartPoint->GetSocketLocation(FName("InsulatorRightSocket")) : StartPoint->GetComponentLocation();
	FVector EndSocketLocation = EndPoint->DoesSocketExist(FName("InsulatorRightSocket")) ? EndPoint->GetSocketLocation(FName("InsulatorRightSocket")) : EndPoint->GetComponentLocation();


	UCableComponent* CableComponentRight = NewObject<UCableComponent>(this);
	CableComponentRight->SetupAttachment(Pole);

	CableComponentRight->SetAttachEndToComponent(StartPoint, FName("InsulatorRightSocket"));
	CableComponentRight->SetAttachEndToComponent(EndPoint, FName("InsulatorRightSocket"));
	CableComponentRight->CableWidth = CablesThickness;
	CableComponentRight->NumSegments = CablesNumberOfSegments;
	CableComponentRight->CableGravityScale = CablesGravityScale;

	FVector StartLocation = StartPoint->GetSocketLocation(FName("InsulatorRightSocket"));
	FVector EndLocation = EndPoint->GetSocketLocation(FName("InsulatorRightSocket"));
	CableComponentRight->SetWorldLocation(StartSocketLocation);
	CableComponentRight->EndLocation = (EndSocketLocation - StartSocketLocation) / Distance;
	CableComponentRight->RegisterComponent();

	CableComponents.Add(CableComponentRight);

	StartSocketLocation = StartPoint->DoesSocketExist(FName("InsulatorLeftSocket")) ? StartPoint->GetSocketLocation(FName("InsulatorLeftSocket")) : StartPoint->GetComponentLocation();
	EndSocketLocation = EndPoint->DoesSocketExist(FName("InsulatorLeftSocket")) ? EndPoint->GetSocketLocation(FName("InsulatorLeftSocket")) : EndPoint->GetComponentLocation();

	UCableComponent* CableComponentLeft = NewObject<UCableComponent>(this);
	CableComponentLeft->SetupAttachment(Pole);

	CableComponentLeft->SetAttachEndToComponent(StartPoint, FName("InsulatorLeftSocket"));
	CableComponentLeft->SetAttachEndToComponent(EndPoint, FName("InsulatorLeftSocket"));
	CableComponentLeft->CableWidth = CablesThickness;
	CableComponentLeft->NumSegments = CablesNumberOfSegments;
	CableComponentLeft->CableGravityScale = CablesGravityScale;

	StartLocation = StartPoint->GetSocketLocation(FName("InsulatorLeftSocket"));
	EndLocation = EndPoint->GetSocketLocation(FName("InsulatorLeftSocket"));
	CableComponentLeft->SetWorldLocation(StartSocketLocation);
	CableComponentLeft->EndLocation = (EndSocketLocation - StartSocketLocation) / Distance;
	CableComponentLeft->RegisterComponent();
	CableComponents.Add(CableComponentLeft);
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




