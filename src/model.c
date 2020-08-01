#include "model.h"

#define FAST_OBJ_IMPLEMENTATION
#include "third_party/fast_obj.h"

Model* model_load_obj(const char* file_path) {
	Model* m = (Model *) malloc(sizeof(Model));
	Mesh mesh;
	fastObjMesh* fmesh = fast_obj_read(file_path);
	
	mesh.vertex_count = fmesh->position_count;
	mesh.vertices = (float *) malloc(sizeof(float) * mesh.vertex_count * 3);
    for (uint32_t i = 0; i < mesh.vertex_count * 3; i++) {
		mesh.vertices[i] = fmesh->positions[i];
	}
	
	mesh.texcoord_count = fmesh->texcoord_count;
	mesh.texcoords = (float *) malloc(sizeof(float) * mesh.texcoord_count * 2);
    for (uint32_t i = 0; i < mesh.texcoord_count * 2; i++) {
		mesh.texcoords[i] = fmesh->texcoords[i];
	}
	
	mesh.normal_count = fmesh->normal_count;
	mesh.normals = (float *) malloc(sizeof(float) * mesh.normal_count * 3);
    for (uint32_t i = 0; i < mesh.normal_count * 3; i++) {
		mesh.normals[i] = fmesh->normals[i];
	}
	
	mesh.index_count = fmesh->face_count * 3;
    mesh.indices = (Index *) malloc(sizeof(Index) * mesh.index_count);
    for (uint32_t i = 0; i < mesh.index_count; i++) {
		mesh.indices[i].v = fmesh->indices[i].p;
		mesh.indices[i].t = fmesh->indices[i].t;
		mesh.indices[i].n = fmesh->indices[i].n;
	}
	
    m->mesh_count = 1;
    m->meshes = (Mesh *) malloc(sizeof(Mesh) * m->mesh_count);
    m->meshes[0] = mesh;
    m->visible = 1;
    m->matrix = mat4f_identity();
    
    fast_obj_destroy(fmesh);
	
	return m;
}

