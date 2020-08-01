#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#include "math/matrix.h"

typedef struct Index {
	uint32_t v; // Vertex
	uint32_t t; // Texture
	uint32_t n; // Normal
} Index;

typedef struct Mesh {
	float* vertices;
    uint32_t vertex_count;
    float* texcoords;
    uint32_t texcoord_count;
    float* normals;
    uint32_t normal_count;
    Index* indices;
    uint32_t index_count;
} Mesh;

typedef struct {
    Mesh* meshes;
    uint32_t mesh_count;
    uint8_t visible;
    Matrix4f matrix;
} Model;

Model* model_load_obj(const char* file_path);

#endif
