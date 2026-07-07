
#include "GameModeMain.h"
#include "../Player/MainCharacter.h"
#include "../Player/MainController.h"

AGameModeMain::AGameModeMain()
{
	DefaultPawnClass = AMainCharacter::StaticClass();
	PlayerControllerClass = AMainController::StaticClass();
}
