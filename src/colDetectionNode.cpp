#include "colDetectionNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MVector.h>

MTypeId colDetectionNode::s_id(0x7c001);
MObject colDetectionNode::s_p0;
MObject colDetectionNode::s_p;
MObject colDetectionNode::s_r;
MObject colDetectionNode::s_d;
MObject colDetectionNode::s_iter;
MObject colDetectionNode::s_sphereCol_p;
MObject colDetectionNode::s_sphereCol_r;
MObject colDetectionNode::s_enableGroundCol;
MObject colDetectionNode::s_groundHeight;
MObject colDetectionNode::s_output;

void* colDetectionNode::creator()
{
    return new colDetectionNode;
};

MStatus colDetectionNode::initialize()
{
    MFnNumericAttribute nAttr;
    MObject x, y, z;

    //s_p0 = nAttr.create("parent", "pr", MFnNumericData::k3Double, 0.0);
    //nAttr.setKeyable(true);
    x = nAttr.create("parentX", "prx", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("parentY", "pry", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("parentZ", "prz", MFnNumericData::kDouble, 0.0);
    s_p0 = nAttr.create("parent", "pr", x, y, z);
    nAttr.setKeyable(true);

    x = nAttr.create("inputX", "inx", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("inputY", "iny", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("inputZ", "inz", MFnNumericData::kDouble, 0.0);
    s_p = nAttr.create("input", "in", x, y, z);
    nAttr.setKeyable(true);

    s_r = nAttr.create("radius", "rad", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_d = nAttr.create("distance", "dist", MFnNumericData::kDouble, 1.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_iter = nAttr.create("iterations", "iter", MFnNumericData::kLong, 3);
    nAttr.setKeyable(true);
    nAttr.setMin(0);
    nAttr.setMax(10);

    x = nAttr.create("sphereColInputX", "scinx", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("sphereColInputY", "sciny", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("sphereColInputZ", "scinz", MFnNumericData::kDouble, 0.0);
    s_sphereCol_p = nAttr.create("sphereColInput", "scin", x, y, z);
    nAttr.setKeyable(true);

    s_sphereCol_r = nAttr.create("sphereCol_radius", "scrad", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_enableGroundCol = nAttr.create("enableGroundCol", "gCol", MFnNumericData::kBoolean, false);
    nAttr.setKeyable(true);

    s_groundHeight = nAttr.create("groundHeight", "gHeight", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);

    x = nAttr.create("outputX", "outx", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("outputY", "outy", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("outputZ", "outz", MFnNumericData::kDouble, 0.0);
    s_output = nAttr.create("output", "out", x, y, z);
    nAttr.setStorable(false);
    nAttr.setWritable(false);

    addAttribute(s_p0);
    addAttribute(s_p);
    addAttribute(s_r);
    addAttribute(s_d);
    addAttribute(s_iter);
    addAttribute(s_sphereCol_p);
    addAttribute(s_sphereCol_r);
    addAttribute(s_enableGroundCol);
    addAttribute(s_groundHeight);
    addAttribute(s_output);
    
    attributeAffects(s_p0, s_output);
    attributeAffects(s_p, s_output);
    attributeAffects(s_r, s_output);
    attributeAffects(s_d, s_output);
    attributeAffects(s_iter, s_output);
    attributeAffects(s_sphereCol_p, s_output);
    attributeAffects(s_sphereCol_r, s_output);
    attributeAffects(s_enableGroundCol, s_output);
    attributeAffects(s_groundHeight, s_output);

    return MS::kSuccess;
}

MStatus colDetectionNode::compute(const MPlug& plug, MDataBlock& data)
{
    if (plug == s_output)
    {
        // input values
        MVector p0 = data.inputValue(s_p0).asVector();
        MVector p = data.inputValue(s_p).asVector();
        double r = data.inputValue(s_r).asDouble();
        double d = data.inputValue(s_d).asDouble();

        long iter = data.inputValue(s_iter).asLong();

        MVector sphereCol_p = data.inputValue(s_sphereCol_p).asVector();
        double sphereCol_r = data.inputValue(s_sphereCol_r).asDouble();

        bool enableGroundCol = data.inputValue(s_enableGroundCol).asBool();
        double groundHeight = data.inputValue(s_groundHeight).asDouble();

        
        // detection
        for (int i = 0; i < iter; i++)
        {
            //sphere collision
            if ((p - sphereCol_p).length() < (sphereCol_r + r))
            {
                p = sphereCol_p + ((p - sphereCol_p).normal() * (sphereCol_r + r));
            }

            //ground collision
            if (enableGroundCol)
            {
                p[1] = p[1] < (groundHeight + r) ? (groundHeight + r) : p[1];
            }

            //keep length
            p = p0 + ((p - p0).normal() * d);
        }
        
        // set output values
        MDataHandle outputHandle = data.outputValue(s_output);
        outputHandle.set(p[0], p[1], p[2]);
        
        data.setClean(plug);

        return MS::kSuccess;
    };

    return MS::kUnknownParameter;
}