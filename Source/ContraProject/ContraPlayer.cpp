// Fill out your copyright notice in the Description page of Project Settings.

#include "ContraProject.h"
#include "ContraPlayer.h"
#include "PaperFlipbookComponent.h"

// Sets default values
AContraPlayer::AContraPlayer()
{
	// Setup the assets
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> AimRunningUpAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> AimRunningDownAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> StandAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> JumpAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> DefeatAnimationAsset;
		FConstructorStatics()
			: RunningAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Running.Running")),
			AimRunningUpAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/AimRunningUp.AimRunningUp")),
			AimRunningDownAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/AimRunningDown.AimRunningDown")),
			IdleAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Idle.Idle")),
			StandAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Stand.Stand")),
			JumpAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Jump.Jump")),
			DefeatAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Defeat.Defeat"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RunningAnimation = ConstructorStatics.RunningAnimationAsset.Get();
	AimRunningUpAnimation = ConstructorStatics.AimRunningUpAnimationAsset.Get();
	AimRunningDownAnimation = ConstructorStatics.AimRunningDownAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	StandAnimation = ConstructorStatics.StandAnimationAsset.Get();
	JumpAnimation = ConstructorStatics.JumpAnimationAsset.Get();
	DefeatAnimation = ConstructorStatics.DefeatAnimationAsset.Get();

	state = EPlayerStates::PS_USUAL;
	GetSprite()->SetFlipbook(IdleAnimation);
	GetSprite()->SetRelativeScale3D(FVector(4.5, 1, 4.5));


	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(90);
	GetCapsuleComponent()->SetCapsuleRadius(68);
	GetCapsuleComponent()->SetRelativeLocation(FVector(-25, 0, 0));


 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->AttachTo(RootComponent);
	camera->SetRelativeLocation(FVector(0, 200, 0));
	camera->SetRelativeRotation(FRotator(0, -90, 0));
	camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	camera->OrthoWidth = 2048;

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

// Called every frame
void AContraPlayer::UpdateAnimation()
{
	FVector velocity = GetVelocity();

	if (EPlayerStates::PS_MIDAIR == state && velocity.Z != 0) {
		GetSprite()->SetFlipbook(JumpAnimation);
		return;
	}

	if (velocity.X != 0) {
		if (EPlayerStates::PS_AIMING_UP == state)
			GetSprite()->SetFlipbook(AimRunningUpAnimation);
		else if (EPlayerStates::PS_AIMING_DOWN == state)
			GetSprite()->SetFlipbook(AimRunningDownAnimation);
		else
			GetSprite()->SetFlipbook(RunningAnimation);
	}
	else
		GetSprite()->SetFlipbook(IdleAnimation);
}

void AContraPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateAnimation();
}

// Called to bind functionality to input
void AContraPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AContraPlayer::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("AimUp", IE_Pressed, this, &AContraPlayer::AimRunUp);
	InputComponent->BindAction("AimUp", IE_Released, this, &AContraPlayer::Run);
	InputComponent->BindAction("AimDown", IE_Pressed, this, &AContraPlayer::AimRunDown);
	InputComponent->BindAction("AimDown", IE_Released, this, &AContraPlayer::Run);

	InputComponent->BindAxis("Move", this, &AContraPlayer::Move);
}

// Input events handlers
void AContraPlayer::Move(float AxisValue)
{
	AddMovementInput(FVector(1, 0, 0), AxisValue);
	if(AxisValue != 0)
		if(AxisValue > 0)
			GetSprite()->SetRelativeRotation(FQuat(FRotator(0, 0, 0)));
		else
			GetSprite()->SetRelativeRotation(FQuat(FRotator(0, 180, 0)));
}

void AContraPlayer::AimRunUp()
{
	state = EPlayerStates::PS_AIMING_UP;
}

void AContraPlayer::AimRunDown()
{
	state = EPlayerStates::PS_AIMING_DOWN;
}

void AContraPlayer::Run()
{
	state = EPlayerStates::PS_USUAL;
}

void AContraPlayer::Jump()
{
	UE_LOG(LogClass, Log, TEXT("NYAN"));
	state = EPlayerStates::PS_MIDAIR;
	Super::Jump();
}