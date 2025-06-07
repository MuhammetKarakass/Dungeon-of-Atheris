// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ABaseAIController::ABaseAIController()
{
	Blackboard=CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	BehaviorTree=CreateDefaultSubobject<UBehaviorTree>("BehaviorTree");
}
