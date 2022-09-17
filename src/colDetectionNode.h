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

    static MTypeId s_id;

    static MObject s_p0;            //parent position vector
    static MObject s_p;             //input position vector
    static MObject s_r;             //input position radius
    static MObject s_d;             //initial length
    
    static MObject s_iter;          //detection iterations

    static MObject s_sphereCol_p;   //sphere collision position
    static MObject s_sphereCol_r;   //sphere collision radius

    static MObject s_enableGroundCol;   //calc ground colision
    static MObject s_groundHeight;  //ground height
    
    static MObject s_output;        //output position vector
};