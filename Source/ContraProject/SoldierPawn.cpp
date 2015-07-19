// Fill out your copyright notice in the Description page of Project Settings.

#include "ContraProject.h"
#include "SoldierPawn.h"
#include "PaperFlipbookComponent.h"


// Sets default values
ASoldierPawn::ASoldierPawn()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;

		FConstructorStatics()
			: RunAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Soldier/Run.Run")),
			IdleAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Soldier/Idle.Idle"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RunAnimation = ConstructorStatics.RunAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();

	GetSprite()->SetFlipbook(IdleAnimation);
	GetSprite()->SetRelativeScale3D(FVector(4.5, 1, 4.5));


	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(90);
	GetCapsuleComponent()->SetCapsuleRadius(43);
	GetCapsuleComponent()->SetRelativeLocation(FVector(-25, 0, 0));

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, -1, 0));

	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
}

// Called when the game starts or when spawned
void ASoldierPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoldierPawn::UpdateAnimation()
{
	FVector velocity = GetVelocity();

	if (velocity.X != 0) 
		GetSprite()->SetFlipbook(RunAnimation);
	else
		GetSprite()->SetFlipbook(IdleAnimation);
}

void ASoldierPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateAnimation();
}


// Called to bind functionality to input
void ASoldierPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

