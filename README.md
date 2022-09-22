# colDetectionNode

Remaked with plugin-node of collision detection part (expression node) of [maya_expressionCollision](https://github.com/akasaki1211/maya_expressionCollision).  
A slight improvement in processing speed can be expected.  

> **Warning**  
> Internal Use ID (0x7c001, 0x7c002) is used.

## Nodes  
There are two types of nodes of: input as vectors (`colDetectionNode`) or input as matrices (`colDetectionMtxNode`). These outputs are the same.
![nodes](.images/nodes.jpg)  

On its own, `colDetectionNode` is faster, but `colDetectionMtxNode` may be faster as a result because it does not require decomposeMatrix, etc. 　
|use `colDetectionNode`|use `colDetectionMtxNode`|
|---|---|
|![colDetectionNode](./.images/colDetectionNode.jpg)|![colDetectionMtxNode](./.images/colDetectionMtxNode.jpg)|


## Performance
The following is the processing time for a single node when one each of sphere, capsule, and infinitePlane Collider is used, Iterations : 3, GroundCol : On.

|`colDetectionNode`|`colDetectionMtxNode`|expression node ([maya_expressionCollision](https://github.com/akasaki1211/maya_expressionCollision))|
|---|---|---|
|14.48us|26.12us|78.33us|

> * Windows 11
> * Intel(R) Core(TM) i7-10700 CPU @ 2.90GHz
> * 16GB RAM
> * NVIDIA GeForce RTX 3060

## Development Environment
* Windows 11
* Maya 2022.3

## Note  
* Processing order of collider type cannot be changed.  
* Too many colliders reduce accuracy.  
* Scale is not supported.  