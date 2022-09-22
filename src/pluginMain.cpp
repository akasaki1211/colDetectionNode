#include "colDetectionNode.h"
#include "colDetectionMtxNode.h"

#include <maya/MFnPlugin.h>

#define colDetectionNodeName "colDetectionNode"
#define colDetectionMtxNodeName "colDetectionMtxNode"

MStatus initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj, "Hiroyuki Akasaki", "1.0", "Any");

    status = plugin.registerNode(colDetectionNodeName, colDetectionNode::s_id, colDetectionNode::creator, colDetectionNode::initialize);
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    status = plugin.registerNode(colDetectionMtxNodeName, colDetectionMtxNode::s_id, colDetectionMtxNode::creator, colDetectionMtxNode::initialize);
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

    status = plugin.deregisterNode(colDetectionMtxNode::s_id);
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}