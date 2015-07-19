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
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> DuckAnimationAsset;
		FConstructorStatics()
			: RunningAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Running.Running")),
			AimRunningUpAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/AimRunningUp.AimRunningUp")),
			AimRunningDownAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/AimRunningDown.AimRunningDown")),
			IdleAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Idle.Idle")),
			StandAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Stand.Stand")),
			JumpAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Jump.Jump")),
			DefeatAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Defeat.Defeat")),
			DuckAnimationAsset(TEXT("/Game/2dSideScroller/Animation/Ray/Duck.Duck"))
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
	DuckAnimation = ConstructorStatics.DuckAnimationAsset.Get();

	ChangeState(EPlayerStates::PS_IDLE);
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

void AContraPlayer::ChangeState(EPlayerStates nstate)
{
	UPaperFlipbook* flipbook = nullptr;

	switch (nstate) {
		case EPlayerStates::PS_IDLE :
			flipbook = IdleAnimation;
			// GetWorldTimerManager()->SetTimer(standTimerHandle, this, &AContraPlayer::Stand, 3);
		break;

		case EPlayerStates::PS_STAND :
			flipbook = StandAnimation;
		break;

		case EPlayerStates::PS_RUN :
			flipbook = RunningAnimation;
		break;

		case EPlayerStates::PS_RUN_AIM_UP :
			flipbook = AimRunningUpAnimation;
		break;

		case EPlayerStates::PS_RUN_AIM_DOWN :
			flipbook = AimRunningDownAnimation;
		break;

		case EPlayerStates::PS_JUMP :
			flipbook = JumpAnimation;
		break;

		case EPlayerStates::PS_LIE :
		break;

		case EPlayerStates::PS_DEFEAT :
			flipbook = DefeatAnimation;
		break;

		case EPlayerStates::PS_DUCK:
			flipbook = DuckAnimation;
			break;
	};

	GetSprite()->SetFlipbook(flipbook);
	state = nstate;
	// if (EPlayerStates::PS_IDLE != nstate)
	// 	GetWorldTimerManager()->ClearTimer(standTimerHandle);
}

void AContraPlayer::RequestState(EPlayerStates nstate)
{
	/*if ((EPlayerStates::PS_RUN_AIM_UP == state || EPlayerStates::PS_RUN_AIM_DOWN == state) &&
		EPlayerStates::PS_RUN == nstate)
		return;

	if (EPlayerStates::PS_JUMP == state || EPlayerStates::PS_DEFEAT == state)
		return;

	if ((EPlayerStates::PS_RUN_AIM_UP == nstate || EPlayerStates::PS_RUN_AIM_DOWN == nstate) &&
		EPlayerStates::PS_IDLE == state)
		return;

	if (EPlayerStates::PS_IDLE == state && EPlayerStates::PS_RUN_AIM_DOWN == nstate)
		nstate = EPlayerStates::PS_DUCK;*/

	if ((EPlayerControls::KEY_LEFT == state || EPlayerControls::KEY_RIGHT == state) &&
		EPlayerControls::KEY_LEFT == nstate)
		return;

	ChangeState(nstate);
}

void AContraPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AContraPlayer::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("Move", this, &AContraPlayer::Move);
	InputComponent->BindAxis("Aim", this, &AContraPlayer::Aim);
}

// Input events handlers
void AContraPlayer::Move(float AxisValue)
{
	AddMovementInput(FVector(1, 0, 0), AxisValue);
	if (AxisValue == 0) 
	{
		RequestState(EPlayerStates::PS_IDLE);
		return;
	}

	
	if (AxisValue > 0)
	{
		RequestState(EPlayerControls::KEY_RIGHT);
		GetSprite()->SetRelativeRotation(FQuat(FRotator(0, 0, 0)));
	}
	else
	{
		RequestState(EPlayerControls::KEY_LEFT);
		GetSprite()->SetRelativeRotation(FQuat(FRotator(0, 180, 0)));
	}

}

void AContraPlayer::Aim(float AxisValue)
{
	if (AxisValue == 0) {
		RequestState((GetVelocity().X != 0) ? EPlayerStates::PS_RUN : EPlayerStates::PS_IDLE);
		return;
	}
	if (AxisValue > 0)
		RequestState(EPlayerStates::PS_RUN_AIM_UP);
	else if (AxisValue < 0)
		RequestState(EPlayerStates::PS_RUN_AIM_DOWN);
}

void AContraPlayer::Jump()
{
	RequestState(EPlayerStates::PS_JUMP);
	Super::Jump();
}

void AContraPlayer::Landed(const FHitResult& Hit)
{
	ChangeState(EPlayerStates::PS_IDLE);
	Super::Landed(Hit);
}

void AContraPlayer::Kill()
{
	RequestState(EPlayerStates::PS_DEFEAT);
}

void AContraPlayer::Stand()
{
	RequestState(EPlayerStates::PS_STAND);
}

void AContraPlayer::Duck()
{
	RequestState(EPlayerStates::PS_DUCK);
}
