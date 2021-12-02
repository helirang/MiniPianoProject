// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PianoMan.generated.h"

class USoundCue;
class UCameraComponent;
class APiano;

UCLASS()
class MINIPROJECT_PIANO_API APianoMan : public APawn
{
	GENERATED_BODY()

	public:
	// Sets default values for this pawn's properties
	APianoMan();
	FString KeyName;
	bool OnOff;
	TArray<AActor*> actors;
	TArray<APiano*> KeyActorsTemp;
	TArray<APiano*> KeyActorsFinal;

	TArray<int32> SaveKeyActorsCondition;
	AActor* KeyBoard;
	int32 SoundNum;
	TArray<bool> bKeyInOut;
	TArray<UAudioComponent*> AudioArray;

	TArray<FString> SheetMusic_Array;
	float WaitTime;
	float SetTimeCheck;
	int CheckNum;
	int PressNum;
	bool startMusc;
	bool NoBrainP;
	float WaitTime_Rec;
	float SetTimeCheck_Rec;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere)
	TArray<APiano*> KeyActorsFinalBP;

public:
	// Called every frame
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	bool REC_Check;
	bool IsFirst_;
	bool FloorFloat;

	UFUNCTION(BlueprintCallable, Category = "SheetMusic")
	FString GetSheetMusic();
	FString SheetMusic_Rec_Array;
	UFUNCTION(BlueprintCallable, Category = "SheetMusic")
	void REC_OnOff(bool OnOffInput);
	UFUNCTION(BlueprintCallable, Category = "SheetMusic")
	void SetFloorF(bool OnOffInput);


	void Key0();
	void Key1();
	void Key2();
	void Key3();
	void Key4();
	void Key5();
	void Key6();
	void Key7();
	void Key8();
	void Key9();
	void ModeChange();

	void KeyBoardPressInOut(int32 OnOffNumberCheck);

	UFUNCTION()
	void playMusic(int32 OnOffNumber);

	UFUNCTION(BlueprintCallable, Category = "Play_SheetMusic")
	void Play_SheetMusic_Setting(FString Music_Array);

	bool intputSheet;

	UFUNCTION(BlueprintCallable, Category = "Play_IntputMode")
	void Play_intputSheet_Setting(bool OnOffInput);

	UPROPERTY(EditAnywhere, Category = "Sound")
	bool mode;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* Sound0;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundCue*> soundArray1;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	UAudioComponent* KeyBoardAudioComponent;
};
