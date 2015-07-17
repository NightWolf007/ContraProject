// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "ContraPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerStates : uint8
{
	PS_USUAL UMETA(DisplayName="Usual"),
	// PS_MIDAIR UMETA(DisplayName="MidAir"),
	PS_DEFEATED UMETA(DisplayName="Defeated"),
	PS_LYING UMETA(DisplayName="Lying"),
	PS_AIMING_UP UMETA(DisplayName="AimingUp"),
	PS_AIMING_DOWN UMETA(DisplayName="AimingUp")
};

UCLASS()
class CONTRAPROJECT_API AContraPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AContraPlayer();
	
	// Called every frame
	void UpdateAnimation();

	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void Move(float AxisValue);
	void Aim(float AxisValue);
	void Jump() override;
	
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

	UPROPERTY(BlueprintReadOnly, Category=State)
		EPlayerStates state;

	bool is_jumping = false;
};