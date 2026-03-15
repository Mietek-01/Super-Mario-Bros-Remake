#include "Scen.h"
#include "../MarioGame.h"

using namespace std;

/// Statics

const size_t Scene::sTileSize = 720 / 15; /// Window height divided by vertical tile count
float Scene::sDurationScene = 0.0f;
float Scene::sFrameTime = 0.0f;

Scene::Scene(std::string pNameBackground) {
    sDurationScene = 0.0f;
    mBackground.setTexture(MarioGame::sTextureManager[pNameBackground]);
}
