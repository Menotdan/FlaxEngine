#include "PLCTNodes.h"
#include "Surface/BoxColliderSurface.h"

bool PLCTSampleSurface::GetOutputBox(PLCTGraphNode& node, PLCTVolume* volume, int id, Variant& output)
{
    if (id == 1) // Points collection output
    {
        PLCTSurfaceList* surfaces;

        VisjectGraphBox box = node.Boxes[0];
        VisjectGraphBox* connectedBox;
        PLCTGraphNode* connectedNode;

        if (!GetInputBox(box, connectedNode, connectedBox))
            return false;
        
        Variant connectedOutput;
        if (!connectedNode->Instance->GetOutputBox(node, volume, connectedBox->ID, connectedOutput))
            return false;

        ScriptingObject* object = connectedOutput.AsObject;
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
    VisjectGraphBox* connectedBox;
    PLCTGraphNode* connectedNode;

    if (!GetInputBox(box, connectedNode, connectedBox))
        return false;

    Variant output;
    if (!connectedNode->Instance->GetOutputBox(node, volume, connectedBox->ID, output))
        return false;

    ScriptingObject* object = output.AsObject;
    if (!object->Is<PLCTPointsContainer>())
        return false;

    points = (PLCTPointsContainer*) object;
    for (int i = 0; i < points->GetPoints().Count(); i++)
    {
        LOG(Warning, "point: {0}", points->GetPoints()[i]->GetTransform());
    }

    return true;
}
