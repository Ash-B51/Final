#ifndef __MESH_H__
#define __MESH_H__

class Mesh{
public:
    Mesh();

    void Init();
    void DrawFloor();
    void DrawLeft();
    void DrawRight();

private:
 	int numOfFace, numOfVert,holder;
	float** verticies;
	int** faces;
	float** colours;

	

};

#endif
