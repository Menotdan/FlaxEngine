#include "PLCTVolume.h"
#include "Engine/Core/Log.h"
#include "Engine/Level/Scene/Scene.h"
#include "Engine/Level/Actor.h"
#include "Engine/Level/Level.h"
#include "Engine/PLCT/PLCTSurface.h"
#include "Engine/PLCT/PLCTGraph.h"
#include <typeinfo>

PLCTVolume::PLCTVolume(const SpawnParams& params)
    : BoxVolume(params)
{
    LOG(Warning, "Created new PLCT volume.");
}

bool PLCTVolume::FindSurfaceAtIndex(PLCTSurface* surface, int index)
{
    int foundCounter = 0;
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
                if (foundCounter != index)
                {
                    foundCounter++;
                    continue;
                }

                surface->SetVolume(this);
                return true;
            }
        }
    }

    return false;
}

bool PLCTVolume::Generate()
{
    LOG(Warning, "1");
    if (!(Graph && !Graph->WaitForLoaded()))
    {
        LOG(Warning, "Graph failed to load on PLCT volume!");
        return false;
    }

    PLCTGraph* graph = Graph.Get();
    LOG(Warning, "2");
    graph->RunGeneration(this);
    LOG(Warning, "3");
    return false;
}

bool PLCTVolume::FindFirstSurface(PLCTSurface* surface)
{
    return FindSurfaceAtIndex(surface, 0);
}

PLCTSurfaceList* PLCTVolume::FindAllSurfaces(PLCTSurface* baseInstance)
{
    bool foundAnySurface = false;
    PLCTSurfaceList* surfaces = new PLCTSurfaceList();

    int index = 0;
    while (true)
    {
        PLCTSurface* surface = (PLCTSurface*) ScriptingObject::NewObject(baseInstance->GetTypeHandle());
        if (!FindSurfaceAtIndex(surface, index++))
        {
            Delete(surface);
            surface = nullptr;
            break;
        }

        foundAnySurface = true;
        surfaces->GetSurfaces().Add(surface);
    }

    return foundAnySurface ? surfaces : nullptr;
}
