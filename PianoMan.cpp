// Fill out your copyright notice in the Description page of Project Settings.

#include "PianoMan.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Piano.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
APianoMan::APianoMan()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	KeyBoardAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("KeyBoardAudioComp"));

	KeyBoardAudioComponent->bAutoActivate = false;
	KeyBoardAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CheckNum = 0;
	NoBrainP = true;
	REC_Check = false;
	WaitTime_Rec = 0;
	FloorFloat = true;
	SetTimeCheck_Rec = 0;
	bKeyInOut.Init(false, 10);


	for (int i = 0; i < 10; i++)
	{
		FString sTempString = "Audio" + FString::FromInt(i);
		AudioArray.Add(CreateDefaultSubobject<UAudioComponent>(FName(*sTempString)));
		AudioArray[i]->bAutoActivate = false;
		AudioArray[i]->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}


void APianoMan::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Sound0->IsValidLowLevelFast())
	{
		KeyBoardAudioComponent->SetSound(Sound0);
	}

	for (int i = 0; i < 10; i++)
	{
		if (AudioArray[i]->IsValidLowLevelFast())
			AudioArray[i]->SetSound(soundArray1[i]);
	}
}


void APianoMan::BeginPlay()
{
	Super::BeginPlay();
	bCanBeDamaged = false;
	ESpawnActorCollisionHandlingMethod::Undefined;

	TArray<AActor*>actors;
	SaveKeyActorsCondition.Init(0, 10);

	mode = false;
	intputSheet = false;

	UWorld* currentworld = GetWorld();

	for (TActorIterator<APiano> It(currentworld); It; ++It)
	{
		KeyActorsTemp.Add(*It);
		KeyActorsFinal.Add(*It);
		KeyActorsTemp[0]->ChangedMaterial();
	}
	for (APiano* actor : KeyActorsTemp)
	{
		KeyActorsFinal[FCString::Atoi(*actor->GetName()) - 10] = actor;
	}
	SoundNum = 0;
}


void APianoMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("On_0", IE_Pressed, this, &APianoMan::Key0);
	PlayerInputComponent->BindAction("On_0", IE_Released, this, &APianoMan::Key0);
	PlayerInputComponent->BindAction("On_1", IE_Pressed, this, &APianoMan::Key1);
	PlayerInputComponent->BindAction("On_1", IE_Released, this, &APianoMan::Key1);
	PlayerInputComponent->BindAction("On_2", IE_Pressed, this, &APianoMan::Key2);
	PlayerInputComponent->BindAction("On_2", IE_Released, this, &APianoMan::Key2);
	PlayerInputComponent->BindAction("On_3", IE_Pressed, this, &APianoMan::Key3);
	PlayerInputComponent->BindAction("On_3", IE_Released, this, &APianoMan::Key3);
	PlayerInputComponent->BindAction("On_4", IE_Pressed, this, &APianoMan::Key4);
	PlayerInputComponent->BindAction("On_4", IE_Released, this, &APianoMan::Key4);
	PlayerInputComponent->BindAction("On_5", IE_Pressed, this, &APianoMan::Key5);
	PlayerInputComponent->BindAction("On_5", IE_Released, this, &APianoMan::Key5);
	PlayerInputComponent->BindAction("On_6", IE_Pressed, this, &APianoMan::Key6);
	PlayerInputComponent->BindAction("On_6", IE_Released, this, &APianoMan::Key6);
	PlayerInputComponent->BindAction("On_7", IE_Pressed, this, &APianoMan::Key7);
	PlayerInputComponent->BindAction("On_7", IE_Released, this, &APianoMan::Key7);
	PlayerInputComponent->BindAction("On_8", IE_Pressed, this, &APianoMan::Key8);
	PlayerInputComponent->BindAction("On_8", IE_Released, this, &APianoMan::Key8);
	PlayerInputComponent->BindAction("On_9", IE_Pressed, this, &APianoMan::Key9);
	PlayerInputComponent->BindAction("On_9", IE_Released, this, &APianoMan::Key9);
	PlayerInputComponent->BindAction("On_Enter", IE_Pressed, this, &APianoMan::ModeChange);
}


FString APianoMan::GetSheetMusic()
{
	return SheetMusic_Rec_Array;
}

void APianoMan::REC_OnOff(bool OnOffInput)
{
	REC_Check = OnOffInput;
	if (REC_Check)
	{
		SheetMusic_Rec_Array.Empty();
		IsFirst_ = true;
	}
}

void APianoMan::SetFloorF(bool OnOffInput)
{
	FloorFloat = OnOffInput;
}

void APianoMan::Key0()
{
	playMusic(0);
}

void APianoMan::Key1()
{
	playMusic(1);
}
void APianoMan::Key2()
{
	playMusic(2);
}
void APianoMan::Key3()
{
	playMusic(3);
}
void APianoMan::Key4()
{
	playMusic(4);
}
void APianoMan::Key5()
{
	playMusic(5);
}
void APianoMan::Key6()
{
	playMusic(6);
}
void APianoMan::Key7()
{
	playMusic(7);
}
void APianoMan::Key8()
{
	playMusic(8);
}
void APianoMan::Key9()
{
	playMusic(9);
}



void APianoMan::Play_SheetMusic_Setting(FString Music_Array)
{
	SheetMusic_Array.Empty();
	Music_Array.ParseIntoArray(SheetMusic_Array, TEXT("/"));
	startMusc = true;
	SetActorTickEnabled(true);
}

void APianoMan::Play_intputSheet_Setting(bool OnOffInput)
{
	intputSheet = OnOffInput;
}

void APianoMan::ModeChange()
{
	mode = !mode;

	UE_LOG(LogTemp, Error, TEXT("modeChange"));
}

void APianoMan::KeyBoardPressInOut(int32 OnOffNumberCheck)
{
	FString nNumName = "bOnOff" + FString::FromInt(OnOffNumberCheck);
	bKeyInOut[OnOffNumberCheck] = !bKeyInOut[OnOffNumberCheck];

	if (bKeyInOut[OnOffNumberCheck])
	{
		KeyBoardAudioComponent->SetIntParameter(FName(FName(*nNumName)), bKeyInOut[OnOffNumberCheck]);
	}
	else
	{
		KeyBoardAudioComponent->SetIntParameter(FName(FName(*nNumName)), bKeyInOut[OnOffNumberCheck]);
	}
}

void APianoMan::playMusic(int32 OnOffNumber)
{
	if (!intputSheet)
	{
		switch (SaveKeyActorsCondition[OnOffNumber])
		{
		case 0:
			SaveKeyActorsCondition[OnOffNumber] = 1;
			(Cast<APiano>(KeyActorsFinal[OnOffNumber]))->ChangedMaterial();

			if (mode)
			{
				KeyBoardAudioComponent->SetIntParameter(FName("SoundPitchSwPa"), OnOffNumber);
				KeyBoardAudioComponent->Play();
			}
			else
			{
				AudioArray[OnOffNumber]->Play();
			}

			if (REC_Check)
			{
				if (IsFirst_)
				{
					SetTimeCheck_Rec = GetWorld()->GetTimeSeconds();
				}

				WaitTime_Rec = GetWorld()->GetTimeSeconds() - SetTimeCheck_Rec;

				if (FloorFloat)
				{
					WaitTime_Rec = FMath::FloorToFloat(WaitTime_Rec * 100) / 100;
				}

				if (IsFirst_)
				{
					SheetMusic_Rec_Array += FString::SanitizeFloat(WaitTime_Rec);
					IsFirst_ = false;

				}
				else
				{
					SheetMusic_Rec_Array += "/" + FString::SanitizeFloat(WaitTime_Rec);
				}

				SheetMusic_Rec_Array += "/" + FString::FromInt(OnOffNumber);
				SetTimeCheck_Rec = GetWorld()->GetTimeSeconds();
			}

			break;
		case 1:
			SaveKeyActorsCondition[OnOffNumber] = 0;
			(Cast<APiano>(KeyActorsFinal[OnOffNumber]))->ChangedMaterial();
			break;
		}
	}
}

void APianoMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (startMusc)
	{
		if (SheetMusic_Array.Num() >= 2)
		{
			if (NoBrainP)
			{
				WaitTime = FCString::Atof(*SheetMusic_Array[CheckNum]);
				UE_LOG(LogTemp, Error, TEXT("waitTime : %f "), WaitTime);

				SetTimeCheck = WaitTime + GetWorld()->GetTimeSeconds();
				UE_LOG(LogTemp, Error, TEXT("NowTime : %f "), GetWorld()->GetTimeSeconds());
				UE_LOG(LogTemp, Error, TEXT("SetTimeCheck : %f "), SetTimeCheck);
				NoBrainP = !NoBrainP;
				CheckNum += 1;
			}
			else
			{
				if (SetTimeCheck <= GetWorld()->GetTimeSeconds())
				{
					if (0 <= FCString::Atoi(*SheetMusic_Array[CheckNum]) && FCString::Atoi(*SheetMusic_Array[CheckNum]) <= 9)
					{
						playMusic(FCString::Atoi(*SheetMusic_Array[CheckNum]));
						playMusic(FCString::Atoi(*SheetMusic_Array[CheckNum]));
						NoBrainP = !NoBrainP;
						CheckNum += 1;
						if (SheetMusic_Array.Num() <= CheckNum)
						{
							SetActorTickEnabled(false);
							startMusc = false;
							CheckNum = 0;
							SheetMusic_Array.Empty();
						}
					}
				}
			}
		}
	}
}

