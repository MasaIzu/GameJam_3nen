#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "EffectMakeScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // éüÇÃÉVÅ[ÉìÇê∂ê¨
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAMEPLAY") {
        newScene = new GameScene();
    }
    else if (sceneName == "EffectMakeScene") {
        newScene = new MakeEffectScene();
    }

    return newScene;
}
