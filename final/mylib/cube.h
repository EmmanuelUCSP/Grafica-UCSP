#ifndef CUBE_H
#define CUBE_H

#include<set>
#include "drawable.h"
using std::set;

//Constants
const float SIZE = 0.5f;
namespace cube_side {
    enum Side {UP, DOWN, LEFT, RIGHT, FRONT, BACK};
}

Vec4f default_color= Vec4f(0.0f, 0.0f, 0.0f, 1.0f); // Black
vector<Vec4f> quad_vertices({
    Vec4f(-SIZE, -SIZE, 0.0f, 1.0f),
    Vec4f(SIZE, -SIZE, 0.0f, 1.0f),
    Vec4f(SIZE, SIZE, 0.0f, 1.0f),
    Vec4f(-SIZE, SIZE, 0.0f, 1.0f)
});
map<int, vector<unsigned int>> quad_indices({
    {GL_POINTS, {0, 1, 2, 3}},
    {GL_LINES, {0, 1, 1, 2, 2, 3, 3, 0}},
    {GL_TRIANGLES, {0, 1, 2, 0, 2, 3}},
    {GL_LINE_LOOP, {0, 1, 2, 3}},
    {GL_LINE_STRIP, {0, 1, 2, 3}}
});

class Quad : public Drawable {
public:
    Quad() : Drawable(quad_vertices, quad_indices, default_color) {}
    Quad(const Quad &quad) : Drawable(quad) {}
    Quad(vector<Vec4f> vertices, Vec4f color) : Drawable(vertices, quad_indices, color) {}
    void set_color(Vec4f color){ this->color = color; }
    Vec4f get_centroid(){ return this->centroid; }
    Vec4f get_color(){ return this->color; }
};

vector<Vec4f> quads_vertices({
    Vec4f(-SIZE, -SIZE, -SIZE, 1.0f),
    Vec4f(SIZE, -SIZE, -SIZE, 1.0f),
    Vec4f(SIZE, SIZE, -SIZE, 1.0f),
    Vec4f(-SIZE, SIZE, -SIZE, 1.0f),
    Vec4f(-SIZE, -SIZE, SIZE, 1.0f),
    Vec4f(SIZE, -SIZE, SIZE, 1.0f),
    Vec4f(SIZE, SIZE, SIZE, 1.0f),
    Vec4f(-SIZE, SIZE, SIZE, 1.0f)
});
vector<vector<unsigned int>> quads_indices({
    {2, 3, 7, 6}, //+y = up
    {0, 1, 5, 4}, //-y = down
    {0, 4, 7, 3}, //-x = left
    {1, 2, 6, 5}, //+x = right
    {4, 5, 6, 7}, //+z = front
    {0, 3, 2, 1}  //-z = back
});
vector<Vec4f> quads_colors({
    Vec4f(1.0f, 1.0f, 0.0f, 1.0f), // yellow
    Vec4f(0.0f, 1.0f, 0.0f, 1.0f), // green
    Vec4f(0.0f, 0.0f, 1.0f, 1.0f), // blue
    Vec4f(1.0f, 1.0f, 1.0f, 1.0f), // white
    Vec4f(1.0f, 0.5f, 0.0f, 1.0f), // orange
    Vec4f(1.0f, 0.0f, 0.0f, 1.0f)  // red
});

class Cube {
protected:
    vector<Quad> quads;
    Vec4f centroid;
    vector<int> draw_modes;
    set<cube_side::Side> sides;
	float shakeAngleX;
    float shakeAngleY;
    float shakeAngleZ;
public:
    Cube();
    Cube(const Cube &cube);
    Cube(Vec4f initial_pos, vector<int> draw_modes, 
        set<cube_side::Side> sides = {cube_side::UP, cube_side::DOWN, cube_side::LEFT, cube_side::RIGHT, cube_side::FRONT, cube_side::BACK}, 
        float size= 1.0f);
    Cube(vector<Vec4f> colors, Vec4f initial_pos, vector<int> draw_modes, set<cube_side::Side> sides, float size);

    void draw(GLuint &color_location);
    void translation(Vec4f direction, float distance);
    void translation(Vec4f destiny);
    void rotation_centroid(Vec4f axis, float angle);
    void rotation_point(Vec4f axis, float angle, Vec4f point= Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
    void scaling(Vec4f scale);
    void scaling(Vec4f scale, Vec4f point);
    void transform_mtx_centroid(Mtx4f transform);
    void transform_mtx_point(Mtx4f transform, Vec4f point= Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
    void calc_centroid();
    Vec4f get_centroid(){ return this->centroid; }
	

};

Cube::Cube() : Cube(quads_colors, Vec4f(0.0f, 0.0f, 0.0f, 1.0f), {GL_TRIANGLES, GL_LINES}, 
    {cube_side::UP, cube_side::DOWN, cube_side::LEFT, cube_side::RIGHT, cube_side::FRONT, cube_side::BACK}, 1.0f){}

Cube::Cube(const Cube &cube) : quads(cube.quads), centroid(cube.centroid), draw_modes(cube.draw_modes), sides(cube.sides) {}

Cube::Cube(Vec4f initial_pos, vector<int> draw_modes, set<cube_side::Side> sides, float size) : Cube(quads_colors, initial_pos, draw_modes, sides, size){}

Cube::Cube(vector<Vec4f> colors, Vec4f initial_pos, vector<int> draw_modes, set<cube_side::Side> sides, float size) : 
      draw_modes(draw_modes), centroid(initial_pos), sides(sides){
    Mtx4f scale_mtx = scale_general({size, size, size, 1.0f});
    vector<Vec4f> quads_vertices_(quads_vertices);
    for (auto &vertex : quads_vertices_){
        vertex = scale_mtx * vertex;
    }
    int i = 0;
    for (auto &index : quads_indices){
        Vec4f color = (sides.count((cube_side::Side)i)) ? colors[i] : default_color;
        quads.push_back(Quad({quads_vertices_[index[0]], quads_vertices_[index[1]],
                              quads_vertices_[index[2]], quads_vertices_[index[3]]}, color));
        i++;
    }
    for (auto &quad : quads){
        quad.translation(initial_pos);
    }
}

void Cube::draw(GLuint &color_location){
    for (int i = 0; i < 6; i++){
        Quad &quad = quads[i];
        for (auto &mode : draw_modes){
            if (mode == GL_LINES || mode == GL_LINE_LOOP || mode == GL_POINTS){
                Vec4f tmp= quad.get_color();
                quad.set_color(default_color);
                quad.draw(color_location, mode);
                quad.set_color(tmp);
            }
            else
                quad.draw(color_location, mode);
        }
    }
}

void Cube::translation(Vec4f direction, float distance){
    Mtx4f transform = translation_general(direction, distance);
    transform_mtx_point(transform);
}

void Cube::translation(Vec4f destiny){
    Mtx4f transform = translation_general(destiny);
    transform_mtx_point(transform);
}

void Cube::rotation_centroid(Vec4f axis, float angle){
    Mtx4f transform = rotation_general(axis, angle);
    transform_mtx_centroid(transform);
}

void Cube::rotation_point(Vec4f axis, float angle, Vec4f point){
    Mtx4f transform = rotation_general(axis, angle);
    transform_mtx_point(transform, point);
}

void Cube::scaling(Vec4f scale){
    Mtx4f transform = scale_general(scale);
    transform_mtx_centroid(transform);
}

void Cube::scaling(Vec4f scale, Vec4f point){
    Mtx4f transform = scale_general(scale);
    transform_mtx_point(transform, point);
}

void Cube::transform_mtx_centroid(Mtx4f transform){
    for (auto &quad : quads){
        quad.transform_mtx_point(transform, centroid);
    }
    calc_centroid();
}

void Cube::transform_mtx_point(Mtx4f transform, Vec4f point){
    for (auto &quad : quads){
        quad.transform_mtx_point(transform, point);
    }
    calc_centroid();
}

void Cube::calc_centroid(){
    centroid = Vec4f::zeros();
    for (auto &quad : quads){
        centroid = centroid + quad.get_centroid();
    }
    centroid = centroid / quads.size();
    centroid(3) = 0.0f;
}

#endif