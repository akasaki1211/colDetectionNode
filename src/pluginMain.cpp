#include "colDetectionNode.h"

#include <maya/MFnPlugin.h>

#define colDetectionNodeName "colDetectionNode"

MStatus initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj, "Hiroyuki Akasaki", "1.0", "Any");

    status = plugin.registerNode(colDetectionNodeName, colDetectionNode::s_id, colDetectionNode::creator, colDetectionNode::initialize);
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    return status;
}

MStatus uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj);

    status = plugin.deregisterNode(colDetectionNode::s_id);
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}