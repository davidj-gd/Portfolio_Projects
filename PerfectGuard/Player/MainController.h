#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "MainController.generated.h"

UCLASS()
class CS_API AMainController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public: 
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	
};
