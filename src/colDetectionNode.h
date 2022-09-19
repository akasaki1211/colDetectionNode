#pragma once

#include <maya/MPxNode.h>
#include <maya/MTypeId.h> 

class colDetectionNode : public MPxNode
{
public:
    colDetectionNode() {};
    virtual ~colDetectionNode() {};
    MStatus compute(const MPlug& plug, MDataBlock& data) override;
    static void* creator();
    static MStatus initialize();
    virtual SchedulingType schedulingType() const;

    static MTypeId s_id;

    static MObject s_p0;              //parent position vector
    static MObject s_p;               //input position vector
    static MObject s_r;               //input position radius
    static MObject s_d;               //initial length
    
    static MObject s_iter;            //iterations of detection

    static MObject s_sphereCollider;  //sphereCollider Array
    static MObject s_sphereColPos;    //sphereCollider position vector
    static MObject s_sphereColRad;    //sphereCollider radius

    static MObject s_capsuleCollider;   //capsuleCollider Array
    static MObject s_capsuleColPosA;    //capsuleCollider position A vector
    static MObject s_capsuleColPosB;    //capsuleCollider position B vector
    static MObject s_capsuleColRadA;    //capsuleCollider radius A
    static MObject s_capsuleColRadB;    //capsuleCollider radius B

    static MObject s_infinitePlaneCollider;  //infinitePlaneCollider Array
    static MObject s_infinitePlaneColPos;    //infinitePlaneCollider position vector
    static MObject s_infinitePlaneColNor;    //infinitePlaneCollider world normal
    
    static MObject s_enableGroundCol; //calc ground colision
    static MObject s_groundHeight;    //ground height
    
    static MObject s_output;          //output position vector
};