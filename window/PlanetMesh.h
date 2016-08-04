#ifndef PLANET_MESH_H
#define PLANET_MESH_H

#include "../celmath/intersect.h"

class cSphereRectPatch
{
public:

	cSphereRectPatch(void);
	~cSphereRectPatch(void);

	void CreatePatch(int dir, int xdim, int ydim);
	Array2D< Vector3<float> >* GetPoints();


	int numPoints;
	int dir;
	int xDim;
	int yDim;

	Array2D< Vector3<float> > *m_points;
};

#endif