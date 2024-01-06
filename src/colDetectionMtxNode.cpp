#include "colDetectionMtxNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>

MTypeId colDetectionMtxNode::s_id(0x7c002);

MObject colDetectionMtxNode::s_p0;
MObject colDetectionMtxNode::s_p;
MObject colDetectionMtxNode::s_r;
MObject colDetectionMtxNode::s_iter;

MObject colDetectionMtxNode::s_sphereCollider; 
MObject colDetectionMtxNode::s_sphereColMtx;
MObject colDetectionMtxNode::s_sphereColRad;

MObject colDetectionMtxNode::s_capsuleCollider;
MObject colDetectionMtxNode::s_capsuleColMtxA;
MObject colDetectionMtxNode::s_capsuleColMtxB;
MObject colDetectionMtxNode::s_capsuleColRadA;
MObject colDetectionMtxNode::s_capsuleColRadB;

MObject colDetectionMtxNode::s_infinitePlaneCollider;
MObject colDetectionMtxNode::s_infinitePlaneColMtx;

MObject colDetectionMtxNode::s_enableGroundCol;
MObject colDetectionMtxNode::s_groundHeight;

MObject colDetectionMtxNode::s_output;

void* colDetectionMtxNode::creator()
{
    return new colDetectionMtxNode;
};

colDetectionMtxNode::SchedulingType colDetectionMtxNode::schedulingType() const
{
    return kParallel;
}

MStatus colDetectionMtxNode::initialize()
{
    MFnNumericAttribute nAttr;
    MFnCompoundAttribute cmpAttr;
    MFnMatrixAttribute mAttr;
    MObject x, y, z;

    // input attributes
    s_p0 = mAttr.create("parentMatrix", "prmtx");
    mAttr.setKeyable(true);

    s_p = mAttr.create("inputMatrix", "inmtx");
    mAttr.setKeyable(true);

    s_r = nAttr.create("radius", "rad", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_iter = nAttr.create("iterations", "iter", MFnNumericData::kLong, 3);
    nAttr.setKeyable(true);
    nAttr.setMin(0);
    nAttr.setMax(10);

    // collider attributes
    // sphereCollider
    s_sphereColMtx = mAttr.create("sphereColMatrix", "scmtx");
    mAttr.setKeyable(true);
    
    s_sphereColRad = nAttr.create("sphereColRadius", "scrad", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_sphereCollider = cmpAttr.create("sphereCollider", "sc");
    cmpAttr.setArray(true);
    cmpAttr.addChild(s_sphereColMtx);
    cmpAttr.addChild(s_sphereColRad);
    cmpAttr.setReadable(true);
    cmpAttr.setUsesArrayDataBuilder(true);

    // capsuleCollider
    s_capsuleColMtxA = mAttr.create("capsuleColMatrixA", "ccmtxa");
    mAttr.setKeyable(true);
    
    s_capsuleColMtxB = mAttr.create("capsuleColMatrixB", "ccmtxb");
    mAttr.setKeyable(true);

    s_capsuleColRadA = nAttr.create("capsuleColRadiusA", "ccrada", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_capsuleColRadB = nAttr.create("capsuleColRadiusB", "ccradb", MFnNumericData::kDouble, 0.0);
    nAttr.setKeyable(true);
    nAttr.setMin(0);

    s_capsuleCollider = cmpAttr.create("capsuleCollider", "cc");
    cmpAttr.setArray(true);
    cmpAttr.addChild(s_capsuleColMtxA);
    cmpAttr.addChild(s_capsuleColMtxB);
    cmpAttr.addChild(s_capsuleColRadA);
    cmpAttr.addChild(s_capsuleColRadB);
    cmpAttr.setReadable(true);
    cmpAttr.setUsesArrayDataBuilder(true);

    // infinitePlaneCollider
    s_infinitePlaneColMtx = mAttr.create("infinitePlaneColMatrix", "pcmtx");
    mAttr.setKeyable(true);

    s_infinitePlaneCollider = cmpAttr.create("infinitePlaneCollider", "pc");
    cmpAttr.setArray(true);
    cmpAttr.addChild(s_infinitePlaneColMtx);
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
    addAttribute(s_iter);
    addAttribute(s_sphereColMtx);
    addAttribute(s_sphereColRad);
    addAttribute(s_sphereCollider);
    addAttribute(s_capsuleColMtxA);
    addAttribute(s_capsuleColMtxB);
    addAttribute(s_capsuleColRadA);
    addAttribute(s_capsuleColRadB);
    addAttribute(s_capsuleCollider);
    addAttribute(s_infinitePlaneColMtx);
    addAttribute(s_infinitePlaneCollider);
    addAttribute(s_enableGroundCol);
    addAttribute(s_groundHeight);
    addAttribute(s_output);
    
    attributeAffects(s_p0, s_output);
    attributeAffects(s_p, s_output);
    attributeAffects(s_r, s_output);
    attributeAffects(s_iter, s_output);
    attributeAffects(s_sphereColMtx, s_output);
    attributeAffects(s_sphereColRad, s_output);
    attributeAffects(s_sphereCollider, s_output);
    attributeAffects(s_capsuleColMtxA, s_output);
    attributeAffects(s_capsuleColMtxB, s_output);
    attributeAffects(s_capsuleColRadA, s_output);
    attributeAffects(s_capsuleColRadB, s_output);
    attributeAffects(s_capsuleCollider, s_output);
    attributeAffects(s_infinitePlaneColMtx, s_output);
    attributeAffects(s_infinitePlaneCollider, s_output);
    attributeAffects(s_enableGroundCol, s_output);
    attributeAffects(s_groundHeight, s_output);

    return MS::kSuccess;
}

MStatus colDetectionMtxNode::compute(const MPlug& plug, MDataBlock& data)
{
    if (plug == s_output)
    {
        // input values
        MTransformationMatrix p0Mtx = data.inputValue(s_p0).asMatrix();
        MTransformationMatrix pMtx = data.inputValue(s_p).asMatrix();
        MVector p0 = p0Mtx.getTranslation(MSpace::kWorld);
        MVector p = pMtx.getTranslation(MSpace::kWorld);
        double s[3];
        p0Mtx.getScale(s, MSpace::kWorld);
        double r = data.inputValue(s_r).asDouble() * s[2];
        double d = (p - p0).length();
        
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
        MTransformationMatrix sphereCol_m;
        MVector sphereCol_p;
        double sphereCol_s[3];
        double sphereCol_r;
        MTransformationMatrix capsuleCol_mA;
        MTransformationMatrix capsuleCol_mB;
        MVector capsuleCol_pA;
        MVector capsuleCol_pB;
        double capsuleCol_s[3];
        double capsuleCol_rA;
        double capsuleCol_rB;
        MTransformationMatrix iPlaneCol_m;
        MVector iPlaneCol_p;
        MVector iPlaneCol_n = MVector::yAxis;

        for (int i = 0; i < iter; i++)
        {
            //sphere collision
            for (unsigned int i = 0; i < scCount; i++) {
                sphereColArrayHandle.jumpToElement(i);
                MDataHandle sphereCollider = sphereColArrayHandle.inputValue();
                sphereCol_m = sphereCollider.child(s_sphereColMtx).asMatrix();
                sphereCol_p = sphereCol_m.getTranslation(MSpace::kWorld);
                sphereCol_m.getScale(sphereCol_s, MSpace::kWorld);
                sphereCol_r = sphereCollider.child(s_sphereColRad).asDouble() * sphereCol_s[2];
                
                if ((p - sphereCol_p).length() < (sphereCol_r + r))
                {
                    p = sphereCol_p + ((p - sphereCol_p).normal() * (sphereCol_r + r));
                };
            };

            //capsule collision
            for (unsigned int i = 0; i < ccCount; i++) {
                capsuleColArrayHandle.jumpToElement(i);
                MDataHandle capsuleCollider = capsuleColArrayHandle.inputValue();
                capsuleCol_mA = capsuleCollider.child(s_capsuleColMtxA).asMatrix();
                capsuleCol_pA = capsuleCol_mA.getTranslation(MSpace::kWorld);
                capsuleCol_mB = capsuleCollider.child(s_capsuleColMtxB).asMatrix();
                capsuleCol_pB = capsuleCol_mB.getTranslation(MSpace::kWorld);
                capsuleCol_mA.getScale(capsuleCol_s, MSpace::kWorld);
                capsuleCol_rA = capsuleCollider.child(s_capsuleColRadA).asDouble() * capsuleCol_s[2];
                capsuleCol_rB = capsuleCollider.child(s_capsuleColRadB).asDouble() * capsuleCol_s[2];
                
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
                iPlaneCol_m = iPlaneCollider.child(s_infinitePlaneColMtx).asMatrix();
                iPlaneCol_p = iPlaneCol_m.getTranslation(MSpace::kWorld);
                iPlaneCol_n = MVector::yAxis.transformAsNormal(iPlaneCol_m.asMatrix());
                
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