// replace every vertex with one-ring neighbors.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;
int main(int argc, char **argv)
{
	MyMesh  mesh;

	if (!OpenMesh::IO::read_mesh(mesh, "head.ply"))
	{
		std::cerr << "Error: Cannot read mesh from " << "head.ply" << std::endl;
		return 1;
	}
	// this vector stores the computed centers of gravity
	std::vector<MyMesh::Point>  cogs;
	std::vector<MyMesh::Point>::iterator cog_it;
	cogs.reserve(mesh.n_vertices());

	MyMesh::VertexIter          v_it, v_end(mesh.vertices_end());
	MyMesh::VertexVertexIter    vv_it;
	MyMesh::Point               cog;
	MyMesh::Scalar              valence,x=0;

	for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
	{
		cog[0] = cog[1] = cog[2] = valence = 0.0;

		for (vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); ++vv_it)
		{
			cog += mesh.point(*vv_it);
			++valence;
		}
		cogs.push_back(cog / valence);
	}

	for (v_it = mesh.vertices_begin(), cog_it = cogs.begin();
		v_it != v_end; ++v_it, ++cog_it)
		if (!mesh.is_boundary(*v_it))
			mesh.set_point(*v_it, *cog_it);


	//write mesh to stdout
	if (!OpenMesh::IO::write_mesh(mesh, "head2.ply"))
	{
		std::cerr << "Error: cannot write mesh to " << "head2.ply" << std::endl;
		return 1;
	}
	return 0;
}
