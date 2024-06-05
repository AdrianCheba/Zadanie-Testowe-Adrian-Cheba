// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerPole.generated.h"

UCLASS()
class ZADANIETESTOWE_API APowerPole : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerPole();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* MySpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* Pole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* PolePlank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* PoleInsulatorRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* PoleInsulatorLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cables")
	float DistanceBetweenPoles;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cables")
	float CablesThickness;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cables")
	float CablesNumberOfSegments;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cables")
	float CablesGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cables")
	bool GenerateCables;


protected:
	virtual void BeginPlay() override;

private:
	void GeneratePoles();
	void AttacheComponents(UStaticMeshComponent* MeshComponent);
	void ClearComponents();
	void GenerateLines(UStaticMeshComponent* StartPoint, UStaticMeshComponent* EndPoint, float Distance);
	TArray<UStaticMeshComponent*> PoleMeshComponents;
	TArray<UStaticMeshComponent*> PoleInsulatorRightComponents;
	TArray<UStaticMeshComponent*> PoleInsulatorLeftComponents;
	TArray<UStaticMeshComponent*> OtherMeshComponents;
	TArray<class UCableComponent*> CableComponents;
};
