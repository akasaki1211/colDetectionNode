#pragma once

#include <maya/MPxNode.h>
#include <maya/MTypeId.h> 

class colDetectionMtxNode : public MPxNode
{
public:
    colDetectionMtxNode() {};
    virtual ~colDetectionMtxNode() {};
    MStatus compute(const MPlug& plug, MDataBlock& data) override;
    static void* creator();
    static MStatus initialize();
    virtual SchedulingType schedulingType() const;

    static MTypeId s_id;

    static MObject s_p0;              //parent matrix
    static MObject s_p;               //input matrix
    static MObject s_r;               //input position radius
    
    static MObject s_iter;            //iterations of detection

    static MObject s_sphereCollider;  //sphereCollider Array
    static MObject s_sphereColMtx;    //sphereCollider matrix
    static MObject s_sphereColRad;    //sphereCollider radius

    static MObject s_capsuleCollider;   //capsuleCollider Array
    static MObject s_capsuleColMtxA;    //capsuleCollider matrix A
    static MObject s_capsuleColMtxB;    //capsuleCollider matrix B
    static MObject s_capsuleColRadA;    //capsuleCollider radius A
    static MObject s_capsuleColRadB;    //capsuleCollider radius B

    static MObject s_infinitePlaneCollider;  //infinitePlaneCollider Array
    static MObject s_infinitePlaneColMtx;    //infinitePlaneCollider matrix
    
    static MObject s_enableGroundCol; //calc ground colision
    static MObject s_groundHeight;    //ground height
    
    static MObject s_output;          //output position vector
};