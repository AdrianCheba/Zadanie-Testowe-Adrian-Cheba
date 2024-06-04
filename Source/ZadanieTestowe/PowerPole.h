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
	float DistanceBetweenPoles;

protected:
	virtual void BeginPlay() override;

private:
	void GenerateMeshes();
	TArray<UStaticMeshComponent*> MeshComponents;

};
