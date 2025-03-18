#include <gzpch.h>

#include "Mesh.h"

namespace GZ {
	Mesh::Mesh() {

	}

	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<u32>& indices)
	{
		m_vertex_buffer.insert(m_vertex_buffer.end(), vertices.begin(), vertices.end());
		m_index_buffer.insert(m_index_buffer.end(), indices.begin(), indices.end());
	}

	Mesh::~Mesh() {

	}

	
	std::shared_ptr<Mesh> Mesh::get_icosphere_mesh(f32 radius /*= 0.5f*/, i32 recursion_level)
	{
		static std::shared_ptr<Mesh> s_sphere_inst = nullptr;
		if (s_sphere_inst != nullptr) return s_sphere_inst;

		std::shared_ptr<Mesh> sphere_mesh = std::make_shared<Mesh>();
		sphere_mesh->m_vertex_buffer = {};
		std::unordered_map<i64, i32> midpoint_index_cache;
		i32 index = 0;

		auto add_vertex = [&](const vec3& p) -> i32 {
			f32 mag = glm::length(p);
			Vertex vert;
			vert.pos = p / mag * radius * 2.0f;
			sphere_mesh->m_vertex_buffer.push_back(vert);
			return index++;
		};

		auto get_middle_point = [&](i32 p1_index, i32 p2_index) -> i32 {
			b8 first_is_smaller = p1_index < p2_index;
			i64 smaller_index = first_is_smaller ? p1_index : p2_index;
			i64 greater_index = first_is_smaller ? p2_index : p1_index;
			i64 key = (smaller_index << 32) | greater_index;

			i32 ret;
			const auto &it = midpoint_index_cache.find(key);
			if (it != midpoint_index_cache.end()) {
				ret = it->second;
				return ret;
			}

			vec3 p1 = sphere_mesh->m_vertex_buffer[p1_index].pos;
			vec3 p2 = sphere_mesh->m_vertex_buffer[p2_index].pos;
			i32 p_mid_index = add_vertex((p1 + p2) / 2.0f);

			// store it, return index
			midpoint_index_cache[key] = p_mid_index;
			return p_mid_index;
		};

		
		// create 12 vertices of a icosahedron
		f32 t = (1.0f + glm::sqrt(5.0f)) / 2.0f;

		add_vertex(vec3(-1, t, 0));
		add_vertex(vec3(1, t, 0));
		add_vertex(vec3(-1, -t, 0));
		add_vertex(vec3(1, -t, 0));

		add_vertex(vec3(0, -1, t));
		add_vertex(vec3(0, 1, t));
		add_vertex(vec3(0, -1, -t));
		add_vertex(vec3(0, 1, -t));

		add_vertex(vec3(t, 0, -1));
		add_vertex(vec3(t, 0, 1));
		add_vertex(vec3(-t, 0, -1));
		add_vertex(vec3(-t, 0, 1));

		// create 20 triangles of the icosahedron
		struct triangle_indices {
			i32 v1, v2, v3;
			triangle_indices(i32 x, i32 y, i32 z) {
				v1 = x;
				v2 = y;
				v3 = z;
			}
		};
		std::vector<triangle_indices> faces;
		faces.reserve(20);
		
		// 5 faces around point 0
		faces.push_back(triangle_indices(0, 11, 5));
		faces.push_back(triangle_indices(0, 5, 1));
		faces.push_back(triangle_indices(0, 1, 7));
		faces.push_back(triangle_indices(0, 7, 10));
		faces.push_back(triangle_indices(0, 10, 11));

		// 5 adjacent faces 
		faces.push_back(triangle_indices(1, 5, 9));
		faces.push_back(triangle_indices(5, 11, 4));
		faces.push_back(triangle_indices(11, 10, 2));
		faces.push_back(triangle_indices(10, 7, 6));
		faces.push_back(triangle_indices(7, 1, 8));

		// 5 faces around point 3
		faces.push_back(triangle_indices(3, 9, 4));
		faces.push_back(triangle_indices(3, 4, 2));
		faces.push_back(triangle_indices(3, 2, 6));
		faces.push_back(triangle_indices(3, 6, 8));
		faces.push_back(triangle_indices(3, 8, 9));

		// 5 adjacent faces 
		faces.push_back(triangle_indices(4, 9, 5));
		faces.push_back(triangle_indices(2, 4, 11));
		faces.push_back(triangle_indices(6, 2, 10));
		faces.push_back(triangle_indices(8, 6, 7));
		faces.push_back(triangle_indices(9, 8, 1));

		// refine triangles
		for (i32 i = 0; i < recursion_level; i++)
		{
			std::vector<triangle_indices> faces2;
			faces2.reserve(faces.size() * 4);
			for (const auto &tri: faces)
			{
				// replace triangle by 4 triangles
				int a = get_middle_point(tri.v1, tri.v2);
				int b = get_middle_point(tri.v2, tri.v3);
				int c = get_middle_point(tri.v3, tri.v1);

				faces2.push_back(triangle_indices(tri.v1, a, c));
				faces2.push_back(triangle_indices(tri.v2, b, a));
				faces2.push_back(triangle_indices(tri.v3, c, b));
				faces2.push_back(triangle_indices(a, b, c));
			}
			std::swap(faces, faces2);
		}

		// done, now add triangles to mesh
		sphere_mesh->m_index_buffer = {};
		sphere_mesh->m_index_buffer.reserve(faces.size() * 3);
		for(auto &tri: faces)
		{
			sphere_mesh->m_index_buffer.push_back(tri.v1);
			sphere_mesh->m_index_buffer.push_back(tri.v2);
			sphere_mesh->m_index_buffer.push_back(tri.v3);
		}

		// uvs
		for (int i = 0; i < sphere_mesh->m_vertex_buffer.size(); i++)
		{
			Vertex &vert = sphere_mesh->m_vertex_buffer[i];
			vec3 pos = vert.pos;

			vert.uv.x = (f32)(1.0f - (0.5 - glm::atan(pos.x, pos.z) * (1.0f/glm::pi<f32>() * 2.0f)));
			vert.uv.y = (f32)(1.0f - (0.5 - glm::asin(pos.y) * (1.0f/glm::pi<f32>() * 2.0f)));
		}

		s_sphere_inst = sphere_mesh;
		return sphere_mesh;
	}
	
	std::shared_ptr<Mesh> Mesh::get_box_mesh(vec3 extent /*= {0.5f, 0.5f, 0.5f}*/)
	{
		static std::shared_ptr<Mesh> s_box_inst = nullptr;

		if (s_box_inst != nullptr) return s_box_inst;
        std::shared_ptr<Mesh> box_mesh = std::make_shared<Mesh>();
        std::vector<Vertex> &vertex_buffer = box_mesh->m_vertex_buffer;
        
        box_mesh->m_vertex_buffer.reserve(8);
        
        // All 8 vertices
        vertex_buffer.push_back(Vertex{vec3(-extent.x,  extent.y, extent.z), vec3(0.0f, 1.0f, 0.0f), vec2(0.0, 0.0)});  // Front top left
        vertex_buffer.push_back(Vertex{vec3( extent.x,  extent.y, extent.z), vec3(0.0f, 1.0f, 0.0f), vec2(0.0, 1.0)});  // Front top right
        vertex_buffer.push_back(Vertex{vec3(-extent.x, -extent.y, extent.z), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f/3.0f, 0.0)}); // Front bottom left
        vertex_buffer.push_back(Vertex{vec3( extent.x, -extent.y, extent.z), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f/3.0f, 1.0)}); // Front bottom right
        
        vertex_buffer.push_back(Vertex{vec3(-extent.x,  extent.y, -extent.z), vec3(1.0f, 0.0f, 0.0f), vec2(2.0f/3.0f,0.0)});  // Back top left
        vertex_buffer.push_back(Vertex{vec3( extent.x,  extent.y, -extent.z), vec3(1.0f, 0.0f, 0.0f), vec2(2.0f/3.0f,1.0)});  // Back top right
        vertex_buffer.push_back(Vertex{vec3(-extent.x, -extent.y, -extent.z), vec3(1.0f, 0.0f, 0.0f), vec2(1.0,0.0)}); // Back bottom left
        vertex_buffer.push_back(Vertex{vec3( extent.x, -extent.y, -extent.z), vec3(1.0f, 0.0f, 0.0f), vec2(1.0,1.0)}); // Back bottom right
        
        // Front
        std::vector<u32> &index_buffer = box_mesh->m_index_buffer;
        index_buffer = {
            // Front
            0, 2, 1,
            1, 2, 3,
            
            // Back
            4, 5, 6,
            5, 7, 6,
            
            // Right
            1, 3, 5,
            5, 3, 7,
            
            // Left
            4, 6, 0,
            0, 6, 2,
            
            // Top
            4, 0, 5,
            5, 0, 1,
            
            // Bottom
            2, 6, 3,
            3, 6, 7,
        };
        s_box_inst = box_mesh;
        return box_mesh;
	}
	

	
}

