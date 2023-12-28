#include "PLCTNodes.h"
#include "Surface/BoxColliderSurface.h"
#include "Engine/Debug/DebugDraw.h"

bool PLCTSampleSurface::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    if (id == 1) // Points collection output
    {
        PLCTSurfaceList* surfaces;

        VisjectGraphBox box = node.Boxes[0];
        PLCTGraphNode* connectedNode;
        ScriptingObject* object;

        if (!GetObjectFromInputBox(box, connectedNode, volume, object))
            return false;

        if (!object->Is<PLCTSurfaceList>())
            return false;

        surfaces = (PLCTSurfaceList*) object;
        SurfaceSampler* sampler = New<SurfaceSampler>();
        sampler->Configure(Settings);
        sampler->SetSurfaceList(surfaces);

        PLCTPointsContainer* points = sampler->SampleXZ();
        output = Variant(points);
        Delete(sampler);
        return true;
    }

    return false;
}

bool PLCTGetBoxColliderSurfaces::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    BoxColliderSurface* baseInstance = New<BoxColliderSurface>();
    PLCTSurfaceList* surfaces = volume->FindAllSurfaces(baseInstance);
    Delete(baseInstance);

    output = Variant(surfaces);
    return true;
}

bool PLCTDebugDrawPoints::Execute(PLCTGraphNode& node, PLCTVolume* volume)
{
    PLCTPointsContainer* points;

    VisjectGraphBox box = node.Boxes[0];
    PLCTGraphNode* connectedNode;
    ScriptingObject* object;

    if (!GetObjectFromInputBox(box, connectedNode, volume, object))
        return false;

    if (!object->Is<PLCTPointsContainer>())
        return false;

    points = (PLCTPointsContainer*) object;
    for (int i = 0; i < points->GetPoints().Count(); i++)
    {
        DebugDraw::DrawSphere(BoundingSphere(points->GetPoints()[i]->GetTransform().Translation, 2), PointColor, 20.0f);
    }

    return true;
}
