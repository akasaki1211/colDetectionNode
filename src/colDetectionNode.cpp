#include "colDetectionNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MVector.h>

MTypeId colDetectionNode::s_id(0x7c001);

MObject colDetectionNode::s_p0;
MObject colDetectionNode::s_p;
MObject colDetectionNode::s_r;
MObject colDetectionNode::s_d;
MObject colDetectionNode::s_iter;

MObject colDetectionNode::s_sphereCollider; 
MObject colDetectionNode::s_sphereColPos;
MObject colDetectionNode::s_sphereColRad;

MObject colDetectionNode::s_capsuleCollider;
MObject colDetectionNode::s_capsuleColPosA;
MObject colDetectionNode::s_capsuleColPosB;
MObject colDetectionNode::s_capsuleColRadA;
MObject colDetectionNode::s_capsuleColRadB;

MObject colDetectionNode::s_infinitePlaneCollider;
MObject colDetectionNode::s_infinitePlaneColPos;
MObject colDetectionNode::s_infinitePlaneColNor;

MObject colDetectionNode::s_enableGroundCol;
MObject colDetectionNode::s_groundHeight;

MObject colDetectionNode::s_output;

void* colDetectionNode::creator()
{
    return new colDetectionNode;
};

colDetectionNode::SchedulingType colDetectionNode::schedulingType() const
{
    return kParallel;
}

MStatus colDetectionNode::initialize()
{
    MFnNumericAttribute nAttr;
    MFnCompoundAttribute cmpAttr;
    MObject x, y, z;

    // input attributes
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

    // collider attributes
    // sphereCollider
    x = nAttr.create("sphereColInputX", "scinx", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("sphereColInputY", "sciny", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("sphereColInputZ", "scinz", MFnNumericData::kDouble, 0.0);
    s_sphereColPos = nAttr.create("sphereColInput", "scin", x, y, z);
    nAttr.setKeyable(true);

    s_sphereColRad = nAttr.create("sphereColRadius", "scrad", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_sphereCollider = cmpAttr.create("sphereCollider", "sc");
    cmpAttr.setArray(true);
    cmpAttr.addChild(s_sphereColPos);
    cmpAttr.addChild(s_sphereColRad);
    cmpAttr.setReadable(true);
    cmpAttr.setUsesArrayDataBuilder(true);

    // capsuleCollider
    x = nAttr.create("capsuleColInputAX", "ccinax", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("capsuleColInputAY", "ccinay", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("capsuleColInputAZ", "ccinaz", MFnNumericData::kDouble, 0.0);
    s_capsuleColPosA = nAttr.create("capsuleColInputA", "ccina", x, y, z);
    nAttr.setKeyable(true);

    x = nAttr.create("capsuleColInputBX", "ccinbx", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("capsuleColInputBY", "ccinby", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("capsuleColInputBZ", "ccinbz", MFnNumericData::kDouble, 0.0);
    s_capsuleColPosB = nAttr.create("capsuleColInputB", "ccinb", x, y, z);
    nAttr.setKeyable(true);

    s_capsuleColRadA = nAttr.create("capsuleColRadiusA", "ccrada", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_capsuleColRadB = nAttr.create("capsuleColRadiusB", "ccradb", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_capsuleCollider = cmpAttr.create("capsuleCollider", "cc");
    cmpAttr.setArray(true);
    cmpAttr.addChild(s_capsuleColPosA);
    cmpAttr.addChild(s_capsuleColPosB);
    cmpAttr.addChild(s_capsuleColRadA);
    cmpAttr.addChild(s_capsuleColRadB);
    cmpAttr.setReadable(true);
    cmpAttr.setUsesArrayDataBuilder(true);

    // infinitePlaneCollider
    x = nAttr.create("infinitePlaneColInputX", "pcinx", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("infinitePlaneColInputY", "pciny", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("infinitePlaneColInputZ", "pcinz", MFnNumericData::kDouble, 0.0);
    s_infinitePlaneColPos = nAttr.create("infinitePlaneColInput", "pcin", x, y, z);
    nAttr.setKeyable(true);

    x = nAttr.create("infinitePlaneColNormalX", "pcnorx", MFnNumericData::kDouble, 0.0);
    y = nAttr.create("infinitePlaneColNormalY", "pcnory", MFnNumericData::kDouble, 0.0);
    z = nAttr.create("infinitePlaneColNormalZ", "pcnorz", MFnNumericData::kDouble, 0.0);
    s_infinitePlaneColNor = nAttr.create("infinitePlaneColNormal", "pcnor", x, y, z);
    nAttr.setKeyable(true);

    s_infinitePlaneCollider = cmpAttr.create("infinitePlaneCollider", "pc");
    cmpAttr.setArray(true);
    cmpAttr.addChild(s_infinitePlaneColPos);
    cmpAttr.addChild(s_infinitePlaneColNor);
    cmpAttr.setReadable(true);
    cmpAttr.setUsesArrayDataBuilder(true);
    
    // groundCollider
    s_enableGroundCol = nAttr.create("enableGroundCol", "gCol", MFnNumericData::kBoolean, false);
    nAttr.setKeyable(true);

    s_groundHeight = nAttr.create("groundHeight", "gHeight", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);

    // output attribute
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
    addAttribute(s_sphereColPos);
    addAttribute(s_sphereColRad);
    addAttribute(s_sphereCollider);
    addAttribute(s_capsuleColPosA);
    addAttribute(s_capsuleColPosB);
    addAttribute(s_capsuleColRadA);
    addAttribute(s_capsuleColRadB);
    addAttribute(s_capsuleCollider);
    addAttribute(s_infinitePlaneColPos);
    addAttribute(s_infinitePlaneColNor);
    addAttribute(s_infinitePlaneCollider);
    addAttribute(s_enableGroundCol);
    addAttribute(s_groundHeight);
    addAttribute(s_output);
    
    attributeAffects(s_p0, s_output);
    attributeAffects(s_p, s_output);
    attributeAffects(s_r, s_output);
    attributeAffects(s_d, s_output);
    attributeAffects(s_iter, s_output);
    attributeAffects(s_sphereColPos, s_output);
    attributeAffects(s_sphereColRad, s_output);
    attributeAffects(s_sphereCollider, s_output);
    attributeAffects(s_capsuleColPosA, s_output);
    attributeAffects(s_capsuleColPosB, s_output);
    attributeAffects(s_capsuleColRadA, s_output);
    attributeAffects(s_capsuleColRadB, s_output);
    attributeAffects(s_capsuleCollider, s_output);
    attributeAffects(s_infinitePlaneColPos, s_output);
    attributeAffects(s_infinitePlaneColNor, s_output);
    attributeAffects(s_infinitePlaneCollider, s_output);
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

        MArrayDataHandle sphereColArrayHandle = data.inputArrayValue(s_sphereCollider);
        unsigned int scCount = sphereColArrayHandle.elementCount();
        
        MArrayDataHandle capsuleColArrayHandle = data.inputArrayValue(s_capsuleCollider);
        unsigned int ccCount = capsuleColArrayHandle.elementCount();
        
        MArrayDataHandle iPlaneColArrayHandle = data.inputArrayValue(s_infinitePlaneCollider);
        unsigned int pcCount = iPlaneColArrayHandle.elementCount();
        
        bool enableGroundCol = data.inputValue(s_enableGroundCol).asBool();
        double groundHeight = data.inputValue(s_groundHeight).asDouble();

        // detection
        MVector sphereCol_p;
        double sphereCol_r;
        MVector capsuleCol_pA;
        MVector capsuleCol_pB;
        double capsuleCol_rA;
        double capsuleCol_rB;
        MVector iPlaneCol_p;
        MVector iPlaneCol_n;

        for (int i = 0; i < iter; i++)
        {
            //sphere collision
            for (unsigned int i = 0; i < scCount; i++) {
                sphereColArrayHandle.jumpToElement(i);
                MDataHandle sphereCollider = sphereColArrayHandle.inputValue();
                sphereCol_p = sphereCollider.child(s_sphereColPos).asVector();
                sphereCol_r = sphereCollider.child(s_sphereColRad).asDouble();
                
                if ((p - sphereCol_p).length() < (sphereCol_r + r))
                {
                    p = sphereCol_p + ((p - sphereCol_p).normal() * (sphereCol_r + r));
                };
            };

            //capsule collision
            for (unsigned int i = 0; i < ccCount; i++) {
                capsuleColArrayHandle.jumpToElement(i);
                MDataHandle capsuleCollider = capsuleColArrayHandle.inputValue();
                capsuleCol_pA = capsuleCollider.child(s_capsuleColPosA).asVector();
                capsuleCol_rA = capsuleCollider.child(s_capsuleColRadA).asDouble();
                capsuleCol_pB = capsuleCollider.child(s_capsuleColPosB).asVector();
                capsuleCol_rB = capsuleCollider.child(s_capsuleColRadB).asDouble();
                
                double h = (capsuleCol_pB - capsuleCol_pA).length();
                MVector ab = (capsuleCol_pB - capsuleCol_pA).normal();

                double t = ab * (p - capsuleCol_pA);
                double ratio = t / h;
                if (ratio <= 0)
                {
                    if ((p - capsuleCol_pA).length() < (capsuleCol_rA + r))
                    {
                        p = capsuleCol_pA + ((p - capsuleCol_pA).normal() * (capsuleCol_rA + r));
                    };
                }
                else if (ratio >= 1)
                {
                    if ((p - capsuleCol_pB).length() < (capsuleCol_rB + r))
                    {
                        p = capsuleCol_pB + ((p - capsuleCol_pB).normal() * (capsuleCol_rB + r));
                    };
                }
                else
                {
                    MVector q = capsuleCol_pA + (ab * t);
                    double rad = capsuleCol_rA * (1.0 - ratio) + capsuleCol_rB * ratio;
                    if ((p - q).length() < (rad + r))
                    {
                        p = q + ((p - q).normal() * (rad + r));
                    };
                };
            };

            //infinite plane collision
            for (unsigned int i = 0; i < pcCount; i++) {
                iPlaneColArrayHandle.jumpToElement(i);
                MDataHandle iPlaneCollider = iPlaneColArrayHandle.inputValue();
                iPlaneCol_p = iPlaneCollider.child(s_infinitePlaneColPos).asVector();
                iPlaneCol_n = iPlaneCollider.child(s_infinitePlaneColNor).asVector();
                
                double distancePointPlane = iPlaneCol_n * (p - iPlaneCol_p);
                if (distancePointPlane - r < 0)
                {
                    p = p - (iPlaneCol_n * (distancePointPlane - r));
                };
            };
            
            //ground collision
            if (enableGroundCol)
            {
                p[1] = p[1] < (groundHeight + r) ? (groundHeight + r) : p[1];
            };

            //keep length
            p = p0 + ((p - p0).normal() * d);
        };
        
        // set output values
        MDataHandle outputHandle = data.outputValue(s_output);
        outputHandle.set(p[0], p[1], p[2]);
        
        data.setClean(plug);

        return MS::kSuccess;
    };

    return MS::kUnknownParameter;
}