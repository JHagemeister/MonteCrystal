#ifndef SPINMODEL_H
#define SPINMODEL_H

#include "Mesh.h"

#include "typedefs.h"

#include "Shader.h"

class SpinMesh: public Mesh
{
public:
	SpinMesh(QOpenGLFunctions_3_3_Core *glf, SpinMeshParams params);
	~SpinMesh();
	void update(SpinMeshParams params);

private:
	void adjust_parameters(SpinMeshParams params);
	void generate_mesh_data(void);
	int _n;
	float _r1;
	float _r2;
	float _l1;
	float _l2;
	
	float _zg;
};

#endif // SPINMODEL_H