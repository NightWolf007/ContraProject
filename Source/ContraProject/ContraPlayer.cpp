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

	int HitBoxHeight  = 90;
	int HitBoxRadius = 68;

	switch (nstate) {
		case EPlayerStates::PS_IDLE :
			flipbook = IdleAnimation;
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

		case EPlayerStates::PS_JUMP:
			HitBoxHeight = 74;
			flipbook = JumpAnimation;
		break;

		case EPlayerStates::PS_DEFEAT :
			flipbook = DefeatAnimation;
		break;

		case EPlayerStates::PS_DUCK :
			HitBoxHeight = 50;
			HitBoxRadius = 40;
			flipbook = DuckAnimation;
		break;
	};

	GetCapsuleComponent()->SetCapsuleHalfHeight(HitBoxHeight);
	GetCapsuleComponent()->SetCapsuleRadius(HitBoxRadius);
	GetSprite()->SetFlipbook(flipbook);
	state = nstate;
}

void AContraPlayer::UpdateState()
{

	if (EPlayerStates::PS_DEFEAT == state) return;

	if (jump) {
		ChangeState(EPlayerStates::PS_JUMP);
		return;
	}

	EPlayerStates nstate;

	if (xVal != 0)
		if (yVal > 0)
			nstate = EPlayerStates::PS_RUN_AIM_UP;
		else if (yVal < 0)
			nstate = EPlayerStates::PS_RUN_AIM_DOWN;
		else
			nstate = EPlayerStates::PS_RUN;
	else
		if (yVal > 0)
			nstate = EPlayerStates::PS_IDLE;
		else if (yVal < 0)
			nstate = EPlayerStates::PS_DUCK;
		else
			nstate = EPlayerStates::PS_IDLE;

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
	xVal = AxisValue;
	UpdateState();

	AddMovementInput(FVector(1, 0, 0), AxisValue);
	if(AxisValue > 0)
		GetSprite()->SetRelativeRotation(FQuat(FRotator(0, 0, 0)));
	else
		GetSprite()->SetRelativeRotation(FQuat(FRotator(0, 180, 0)));
}

void AContraPlayer::Aim(float AxisValue)
{
	yVal = AxisValue;
	UpdateState();
}

void AContraPlayer::Jump()
{
	jump = true;
	UpdateState();
	Super::Jump();
}

void AContraPlayer::Landed(const FHitResult& Hit)
{
	jump = false;
	UpdateState();
	Super::Landed(Hit);
}

void AContraPlayer::Kill()
{
	ChangeState(EPlayerStates::PS_DEFEAT);
}

void AContraPlayer::Stand()
{
	ChangeState(EPlayerStates::PS_STAND);
}