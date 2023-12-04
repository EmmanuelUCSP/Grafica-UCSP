#ifndef DRAWABLE_H
#define DRAWABLE_H

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#endif

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <utility>
#include "math/lib4f.h"
#include "transform.h"

using namespace std;

class Drawable {
protected:
    vector<Vec4f> vertices;
    vector<Vec4f> normals; // For each vertex
    map<int, vector<unsigned int>> indices;
    unsigned int vertex_array, vertex_buffer, element_buffer;
    Vec4f color, centroid;
public:
    Drawable();
    Drawable(string filename, Vec4f color);
    Drawable(const Drawable &drawable);
    Drawable(vector<Vec4f> vertices, map<int, vector<unsigned int>> indices, Vec4f color);
    ~Drawable();	

    void translation(Vec4f direction, float distance);
    void translation(Vec4f destiny);
    void rotation_centroid(Vec4f axis, float angle);
    void rotation_point(Vec4f axis, float angle, Vec4f point= Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
    void scaling(Vec4f scale);
    void scaling(Vec4f scale, Vec4f point);
    void transform_mtx_centroid(Mtx4f transform);
    void transform_mtx_point(Mtx4f transform, Vec4f point= Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
    vector<float> get_vertices();
    Vec4f get_centroid(){return centroid;}
    void calc_centroid();
    void calc_normals();
    void update_normals(Mtx4f transform);
    void update_data(int draw_mode);
    void draw(GLuint &color_location, int draw_mode);
};

Drawable::Drawable() {}

Drawable::Drawable(string filename, Vec4f color) : color(color) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo OBJ." << endl;
        return;
    }
    float max_x = -INFINITY, max_y = -INFINITY, max_z = -INFINITY;
    float min_x = INFINITY, min_y = INFINITY, min_z = INFINITY;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        iss >> token;
        if (token == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(Vec4f(x, z, -y, 1.0f));
            min_x = std::min(min_x, x);
            min_y = std::min(min_y, y);
            min_z = std::min(min_z, z);
            max_x = std::max(max_x, x);
            max_y = std::max(max_y, y);
            max_z = std::max(max_z, z);
        } else if (token == "f") {
            unsigned int v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            indices[GL_TRIANGLES].push_back(v1 - 1);
            indices[GL_TRIANGLES].push_back(v2 - 1);
            indices[GL_TRIANGLES].push_back(v3 - 1);
        }
    }
    file.close();
    calc_centroid();
    calc_normals();
    // Generate vertex and element buffers
    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &element_buffer);
}

Drawable::Drawable(const Drawable &drawable) : Drawable(drawable.vertices, drawable.indices, drawable.color) {}

Drawable::Drawable(vector<Vec4f> vertices, map<int, vector<unsigned int>> indices, Vec4f color) {
    this->vertices = vertices;
    this->indices = indices;
    this->color = color;
    calc_centroid();
    calc_normals();

    // Generate vertex and element buffers
    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &element_buffer);
}

Drawable::~Drawable() {
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &element_buffer);
}

void Drawable::transform_mtx_centroid(Mtx4f transform){
    for (auto &vertex : vertices){
        vertex = vertex - centroid; // Move to origin
        vertex = transform * vertex;
        vertex = vertex + centroid; // Move back
    }
    calc_centroid();
    update_normals(transform);
}

void Drawable::transform_mtx_point(Mtx4f transform, Vec4f point){
    point(3) = 0.0f;
    for (auto &vertex : vertices){
        vertex = vertex - point;
        vertex = transform * vertex;
        vertex = vertex + point;
    }
    calc_centroid();
    update_normals(transform);
}

void Drawable::translation(Vec4f direction, float distance){
    Mtx4f transform = translation_general(direction, distance);
    transform_mtx_point(transform);
}

void Drawable::translation(Vec4f destiny){
    Mtx4f transform = translation_general(destiny);
    transform_mtx_point(transform);
}

void Drawable::rotation_centroid(Vec4f axis, float angle){
    Mtx4f transform = rotation_general(axis, angle);
    transform_mtx_centroid(transform);
}

void Drawable::rotation_point(Vec4f axis, float angle, Vec4f point){
    Mtx4f transform = rotation_general(axis, angle);
    transform_mtx_point(transform, point);
}

void Drawable::scaling(Vec4f scale){
    Mtx4f transform = scale_general(scale);
    transform_mtx_centroid(transform);
}

void Drawable::scaling(Vec4f scale, Vec4f point){
    Mtx4f transform = scale_general(scale);
    transform_mtx_point(transform, point);
}

vector<float> Drawable::get_vertices(){
    vector<float> result;
    for (int i = 0; i < vertices.size(); i++){
        vector<float> vertex = vertices[i].to_vec();
        vector<float> normal = normals[i].to_vec();
        result.insert(result.end(), vertex.begin(), vertex.end());
        result.insert(result.end(), normal.begin(), normal.end());
    }
    return result;
}

void Drawable::calc_centroid(){
    centroid = Vec4f::zeros();
    for (auto &vertex : vertices){
        centroid = centroid + vertex;
    }
    centroid = centroid / vertices.size();
    centroid(3) = 0.0f;
}

void Drawable::calc_normals(){
    normals.resize(vertices.size());
    for (auto &normal : normals)
        normal = Vec4f::zeros();
    for (int i = 0; i < indices[GL_TRIANGLES].size(); i += 3){
        int idx_v1 = indices[GL_TRIANGLES][i];
        int idx_v2 = indices[GL_TRIANGLES][i + 1];
        int idx_v3 = indices[GL_TRIANGLES][i + 2];
        Vec4f v1 = vertices[idx_v1];
        Vec4f v2 = vertices[idx_v2];
        Vec4f v3 = vertices[idx_v3];
        Vec4f normal = (v2 - v1).cross(v3 - v1);
        normals[idx_v1] = normals[idx_v1] + normal;
        normals[idx_v2] = normals[idx_v2] + normal;
        normals[idx_v3] = normals[idx_v3] + normal;
    }
    for (auto &normal : normals){
        normal = normal.unit();
    }
}

void Drawable::update_normals(Mtx4f transform){
    Mtx4f Q = transform.inv().trp();
    for (auto &normal : normals){
        normal = Q * normal;
    }
}

void Drawable::draw(GLuint &color_location, int draw_mode){
    update_data(draw_mode);
    
    vector<float> color_ = color.to_vec();
    vector<unsigned int> &indices_ = indices[draw_mode];
    glUniform4fv(color_location, 1, color_.data());
    glBindVertexArray(vertex_array);
    glDrawElements(draw_mode, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Drawable::update_data(int draw_mode){
    vector<float> vertices_ = get_vertices();
    vector<unsigned int> &indices_ = indices[draw_mode];
    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices_.size(), indices_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#endif // DRAWABLE_H