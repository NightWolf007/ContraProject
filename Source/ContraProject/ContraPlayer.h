// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "ContraPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerStates : uint8
{
	PS_IDLE UMETA(DisplayName="Idle"),
	PS_STAND UMETA(DisplayName="Stand"),
	PS_AIM_UP UMETA(DisplayName = "StandAimUp"),

	PS_RUN UMETA(DisplayName="Run"),
	PS_RUN_AIM_UP UMETA(DisplayName="AimUp"),
	PS_RUN_AIM_DOWN UMETA(DisplayName="AimDown"),

	PS_JUMP UMETA(DisplayName="Jump"),
	PS_DEFEAT UMETA(DisplayName = "Defeated"),
	PS_DUCK UMETA(DisplayName = "Duck")
};

UCLASS()
class CONTRAPROJECT_API AContraPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AContraPlayer();
	
	// Called every frame
	void UpdateState();

	// virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void ChangeState(EPlayerStates nstate);

	void Move(float AxisValue);
	void Aim(float AxisValue);
	void Duck();
	void Jump() override;
	void Landed(const FHitResult& Hit) override;
	void Kill();
	void Stand();
	
protected:
	UPROPERTY(EditAnywhere)
		class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
		class UPaperFlipbook* RunningAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
		class UPaperFlipbook* AimRunningUpAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
		class UPaperFlipbook* AimRunningDownAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
		class UPaperFlipbook* IdleAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
		class UPaperFlipbook* StandAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
		class UPaperFlipbook* JumpAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
		class UPaperFlipbook* DefeatAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* DuckAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* AimUpAnimation;

	UPROPERTY(BlueprintReadOnly, Category=State)
		EPlayerStates state;

	int8 xVal;
	int8 yVal;
	bool jump;

	FTimerHandle standTimerHandle;
};