#include "PLCTVolume.h"
#include "Engine/Core/Log.h"
#include "Engine/Level/Scene/Scene.h"
#include "Engine/Level/Actor.h"
#include "Engine/Level/Level.h"
#include "Engine/PLCT/PLCTSurface.h"

PLCTVolume::PLCTVolume(const SpawnParams& params)
    : BoxVolume(params)
{
    LOG(Warning, "Created new PLCT volume.");
}

bool PLCTVolume::FindFirstSurface(PLCTSurface* surface)
{
    for (int sceneIdx = -1; sceneIdx < Level::Scenes.Count(); sceneIdx++)
    {
        Scene* readingScene = sceneIdx == -1 ? this->GetScene() : Level::Scenes[sceneIdx];
        if (readingScene == this->GetScene() && sceneIdx != -1)
            continue;

        Array<Actor*> sceneActors = readingScene->GetChildren<Actor>();
        for (int actorIdx = 0; actorIdx < sceneActors.Count(); actorIdx++)
        {
            Actor* actor = sceneActors[actorIdx];
            ContainmentType contains = this->GetOrientedBox().Contains(actor->GetSphere());
            if (contains == ContainmentType::Disjoint)
            {
                continue;
            }

            if (surface->CheckActorMatchesAndSet(actor))
            {
                surface->SetVolume(this);
                return true;
            }
        }
    }

    return false;
}
