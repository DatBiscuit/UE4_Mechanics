// Fill out your copyright notice in the Description page of Project Settings.


//A way to print to the screen
#define print(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Green,text)
#define printf(text, fstring) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, FString::Printf(TEXT(text), fstring))


#include "CustomTriggerBox.h"
#include "DrawDebugHelpers.h"	

ACustomTriggerBox::ACustomTriggerBox() {

	//Dynamically adding a runtime code to the box
	OnActorBeginOverlap.AddDynamic(this, &ACustomTriggerBox::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACustomTriggerBox::OnOverlapEnd);

}


void ACustomTriggerBox::BeginPlay() {
	Super::BeginPlay();

	//Makes the box visible for debuging purposes
	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);

}


//Occurs when an actor is overlapped with the box
void ACustomTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) {
	if (OtherActor && (OtherActor != this)) {
		print("Overlap begin");
		printf("Overlapped actor = %s", *OverlappedActor->GetName());
	}
}

//Occurs when the actor is not overlapping with the box
void ACustomTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) {
	if (OtherActor && (OtherActor != this)) {
		print("Overlap end");
		printf("Overlapped actor = %s", *OtherActor->GetName());
	}
}