#include "PLCTVolume.h"
#include "Engine/Core/Log.h"
#include "Engine/Threading/JobSystem.h"
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
    CHECK_RETURN(surface, false);
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

void PLCTVolume::GenerateThread(int32 id)
{
    PLCTGraph* graph = Graph.Get();

    CHECK(graph);
    graph->RunGeneration(this);
}

bool PLCTVolume::Generate()
{
    if (!(Graph && !Graph->WaitForLoaded()))
    {
        LOG(Warning, "Graph failed to load on PLCT volume!");
        return false;
    }

    if (RuntimeCache)
    {
        SAFE_DELETE(RuntimeCache);
    }
    RuntimeCache = New<PLCTPropertyStorage>();
    CHECK_RETURN(RuntimeCache, false);

    GenerateThread(0);
    /*Function<void (int32)> action(GenerateThread);
    int64 waitID = JobSystem::Dispatch(action);*/

    return true;
}

bool PLCTVolume::FindFirstSurface(PLCTSurface* surface)
{
    return FindSurfaceAtIndex(surface, 0);
}

PLCTSurfaceList* PLCTVolume::FindAllSurfaces(PLCTSurface* baseInstance)
{
    bool foundAnySurface = false;
    CHECK_RETURN(baseInstance, nullptr);
    PLCTSurfaceList* surfaces = New<PLCTSurfaceList>();    

    int index = 0;
    while (true)
    {
        PLCTSurface* surface = (PLCTSurface*) ScriptingObject::NewObject(baseInstance->GetTypeHandle());
        if (!FindSurfaceAtIndex(surface, index++))
        {
            SAFE_DELETE(surface);
            surface = nullptr;
            break;
        }

        foundAnySurface = true;
        surfaces->GetSurfaces().Add(surface);
    }

    if (!foundAnySurface)
    {
        SAFE_DELETE(surfaces);
        return nullptr;
    }

    return surfaces;
}
