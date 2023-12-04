#ifndef RUBIK_H
#define RUBIK_H

#include "cube.h"
#include <vector>
#include <utility>
#include <chrono>
#include <thread>
#include <cstdlib>  // Para rand()
#include <ctime>    // Para time()

using namespace cube_side;

const float cube_size = 0.3333;
/*
namespace cube_side {
    enum Side {UP, DOWN, LEFT, RIGHT, FRONT, BACK};
}
*/

namespace rubik_side {
    enum Side {U, D, L, R, F, B, M, E, S};
}

vector<vector<pair<int, int>>> range_indices{
    // X , Y , Z
    {{0,0},{0,2},{0,2}},
    {{2,2},{0,2},{0,2}},
    {{0,2},{0,2},{0,0}},
    {{0,2},{0,2},{2,2}},
    {{0,2},{2,2},{0,2}},
    {{0,2},{0,0},{0,2}},
    {{0,2},{1,1},{0,2}},
    {{1,1},{0,2},{0,2}},
    {{0,2},{0,2},{1,1}}
};

vector<pair<Vec4f, set<cube_side::Side>>> cubes_variables({
    // UP
    {Vec4f(-cube_size, cube_size, -cube_size, 1.0f), {UP, LEFT, BACK}},
    {Vec4f(0.0f, cube_size, -cube_size, 1.0f), {UP, BACK}},
    {Vec4f(cube_size, cube_size, -cube_size, 1.0f), {UP, RIGHT, BACK}},
    {Vec4f(-cube_size, cube_size, 0.0f, 1.0f), {UP, LEFT}},
    {Vec4f(0.0f, cube_size, 0.0f, 1.0f), {UP}},
    {Vec4f(cube_size, cube_size, 0.0f, 1.0f), {UP, RIGHT}},
    {Vec4f(-cube_size, cube_size, cube_size, 1.0f), {UP, LEFT, FRONT}},
    {Vec4f(0.0f, cube_size, cube_size, 1.0f), {UP, FRONT}},
    {Vec4f(cube_size, cube_size, cube_size, 1.0f), {UP, FRONT, RIGHT}},
    // MID
    {Vec4f(-cube_size, 0.0f, -cube_size, 1.0f), {LEFT, BACK}},
    {Vec4f(0.0f, 0.0f, -cube_size, 1.0f), {BACK}},
    {Vec4f(cube_size, 0.0f, -cube_size, 1.0f), {RIGHT, BACK}},
    {Vec4f(-cube_size, 0.0f, 0.0f, 1.0f), {LEFT}},
    //Vec4f(0.0f, 0.0f, 0.0f, 1.0f), // CENTER
    {Vec4f(cube_size, 0.0f, 0.0f, 1.0f), {RIGHT}},
    {Vec4f(-cube_size, 0.0f, cube_size, 1.0f), {FRONT, LEFT}},
    {Vec4f(0.0f, 0.0f, cube_size, 1.0f), {FRONT}},
    {Vec4f(cube_size, 0.0f, cube_size, 1.0f), {FRONT, RIGHT}},
    // DOWN
    {Vec4f(-cube_size, -cube_size, -cube_size, 1.0f), {DOWN, LEFT, BACK}},
    {Vec4f(0.0f, -cube_size, -cube_size, 1.0f), {DOWN, BACK}},
    {Vec4f(cube_size, -cube_size, -cube_size, 1.0f), {DOWN, RIGHT, BACK}},
    {Vec4f(-cube_size, -cube_size, 0.0f, 1.0f), {DOWN, LEFT}},
    {Vec4f(0.0f, -cube_size, 0.0f, 1.0f), {DOWN}},
    {Vec4f(cube_size, -cube_size, 0.0f, 1.0f), {DOWN, RIGHT}},
    {Vec4f(-cube_size, -cube_size, cube_size, 1.0f), {DOWN, LEFT, FRONT}},
    {Vec4f(0.0f, -cube_size, cube_size, 1.0f), {DOWN, FRONT}},
    {Vec4f(cube_size, -cube_size, cube_size, 1.0f), {DOWN, FRONT, RIGHT}},
});

class Rubik{
    vector<Cube*> cubes;
    Vec4f centroid, center;
    vector<Vec4f> rotation_axes{
        Vec4f(0.0f, 1.0f, 0.0f, 1.0f), //U
        Vec4f(0.0f, -1.0f, 0.0f, 1.0f), //D
        Vec4f(-1.0f, 0.0f, 0.0f, 1.0f), //L
        Vec4f(1.0f, 0.0f, 0.0f, 1.0f), //R
        Vec4f(0.0f, 0.0f, 1.0f, 1.0f), //F
        Vec4f(0.0f, 0.0f, -1.0f, 1.0f), //B
        Vec4f(0.0f, 0.0f, 1.0f, 1.0f), //M=F
        Vec4f(0.0f, 1.0f, 0.0f, 1.0f), //E=U
        Vec4f(-1.0f, 0.0f, 0.0f, 1.0f) //S=L
    };
    int cube_indices[3][3][3]{{
            {0, 1, 2},
            {3, 4, 5},
            {6, 7, 8}
        },{
            {9, 10, 11},
            {12, -1, 13},
            {14, 15, 16}
        },{
            {17, 18, 19},
            {20, 21, 22},
            {23, 24, 25}
        }};
	
	
public:
    Rubik() : Rubik(Vec4f(0.0f, 0.0f, 0.0f, 1.0f), 1.0f){}
    Rubik(Vec4f initial_pos, float size= 1.0f);
    void draw(GLuint &color_location);
    void rotation_centroid(Vec4f axis, float angle);
    void rotation_point(Vec4f axis, float angle, Vec4f point= Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
    void scaling(Vec4f scale);
    void scaling(Vec4f scale, Vec4f point);
    void translation(Vec4f direction, float distance);
    void translation(Vec4f destiny);
    void update_axes(Mtx4f transform);
    void update_centroid();
	void expandCubes(); // Animacion de expocion
	void resetCubes(); // Animacion de regreso
	void temblar(); // Animacion de temblar (1)
	void shake(); // Animacion de temblar (2)
	void shake2(); // Animacion de temblar (3)
	
	void rotateWholeCubeY(float angle) {
		// Rotar todo el cubo alrededor del eje Y
		rotation_centroid(Vec4f(0.0f, 1.0f, 0.0f, 1.0f), angle);
	}
	
	
    MtxX<int> get_matrix(rubik_side::Side side);
    MtxX<int> rotate_matrix(MtxX<int> mat, bool clockwise);
    void update_cube_indices(rubik_side::Side side, bool clockwise);
    bool rotate_side(rubik_side::Side side, bool clockwise= true, float angle= 90.0f);
};

Rubik::Rubik(Vec4f initial_pos, float size) {
    for (auto vec : cubes_variables) {
        cubes.push_back(new Cube(vec.first, {GL_TRIANGLES, GL_LINES}, vec.second, cube_size));
    }
    for (auto cube : cubes) {
        cube->scaling(Vec4f(0.98, 0.98, 0.98, 1.0f));
    }
    translation(initial_pos);  // Utiliza la posición inicial proporcionada
    update_centroid();
    scaling(Vec4f(size, size, size, 1.0f));
}

void Rubik::translation(Vec4f destiny){
    for (auto &cube : cubes){
        cube->translation(destiny);
    }
    update_centroid();
}

void Rubik::translation(Vec4f direction, float distance){
    for (auto &cube : cubes){
        cube->translation(direction, distance);
    }
    update_centroid();
}

void Rubik::rotation_centroid(Vec4f axis, float angle){
    for (auto &cube : cubes){
        cube->rotation_point(axis, angle, centroid);
    }
    update_axes(rotation_general(axis, angle));
}

void Rubik::rotation_point(Vec4f axis, float angle, Vec4f point){
    for (auto &cube : cubes){
        cube->rotation_point(axis, angle, point);
    }
    update_centroid();
    update_axes(rotation_general(axis, angle));
}

void Rubik::scaling(Vec4f scale){
    for (auto &cube : cubes){
        cube->scaling(scale, centroid);
    }
    update_centroid();
}

void Rubik::scaling(Vec4f scale, Vec4f point){
    for (auto &cube : cubes){
        cube->scaling(scale, point);
    }
    update_centroid();
}


void Rubik::draw(GLuint &color_location){
    for (int i = 0; i < cubes.size(); ++i){
        cubes[i]->draw(color_location);
    }
}

MtxX<int> Rubik::get_matrix(rubik_side::Side side){
    vector<int> indices_;
    for (int i= range_indices[side][0].first; i <= range_indices[side][0].second; i++){
        for (int j= range_indices[side][1].first; j <= range_indices[side][1].second; j++){
            for (int k= range_indices[side][2].first; k <= range_indices[side][2].second; k++){
                indices_.push_back(cube_indices[i][j][k]);
            }
        }
    }
    return MtxX<int>(3, 3, indices_);
}

MtxX<int> Rubik::rotate_matrix(MtxX<int> mat, bool clockwise){
    MtxX<int> rotated(MtxX<int>::zeros(3, 3));
    if (clockwise){
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                rotated(j, 2-i) = mat(i, j);
        }
    }
    else{
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                rotated(2-j, i) = mat(i, j);
        }
    }
    return rotated;
}

void Rubik::update_cube_indices(rubik_side::Side side, bool clockwise){
    clockwise = (side == rubik_side::B || side == rubik_side::D || side == rubik_side::R) ? clockwise : !clockwise;
    
    MtxX<int> side_mtx = get_matrix(side);
    MtxX<int> rotated = rotate_matrix(side_mtx, clockwise);
    int idx= 0;
    vector<int> flattened_mtx = rotated.to_vec();
    for (int i= range_indices[side][0].first; i <= range_indices[side][0].second; i++){
        for (int j= range_indices[side][1].first; j <= range_indices[side][1].second; j++){
            for (int k= range_indices[side][2].first; k <= range_indices[side][2].second; k++){
                cube_indices[i][j][k] = flattened_mtx[idx++];
            }
        }
    }
}

bool Rubik::rotate_side(rubik_side::Side side, bool clockwise, float angle){
    static float moved_= 0.0f;
    angle = clockwise ? angle : -angle;
    MtxX<int> side_mtx = get_matrix(side);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (side_mtx(i, j) != -1) cubes[side_mtx(i, j)]->rotation_point(rotation_axes[side], angle, centroid);
    moved_ += angle;
    if (moved_ == 90.0f || moved_ == -90.0f){
        moved_ = 0.0f;
        update_cube_indices(side, clockwise);
        return true;
    }
    return false;
}

void Rubik::update_axes(Mtx4f transform){
    for (auto &axis : rotation_axes){
        axis = transform * axis;
    }
}

void Rubik::update_centroid(){
    centroid = Vec4f::zeros();
    for (auto &cube : cubes){
        centroid = centroid + cube->get_centroid();
    }
    centroid = centroid / cubes.size();
    centroid(3) = 0.0f;
}




void Rubik::expandCubes() {
    // Cada cubo se mueve en una dirección única basada en su posición relativa al centro
    for (auto &cube : cubes) {
        Vec4f cubePosition = cube->get_centroid();
        Vec4f direction = cubePosition - centroid;
        float scaleFactor = 0.4f; 
        Vec4f translationVector = direction * scaleFactor;
        cube->translation(translationVector);
    }
    update_centroid();
}

void Rubik::resetCubes() {
    // Ajusta las posiciones de los cubos para devolverlos a sus posiciones originales
    for (auto &cube : cubes) {
        Vec4f cubePosition = cube->get_centroid();
        Vec4f direction = centroid - cubePosition; // Invertimos la dirección
        float scaleFactor = 0.04f;
        Vec4f translationVector = direction * scaleFactor;
        cube->translation(translationVector);
    }
    update_centroid();
}

void Rubik::temblar(){
// Semilla para generar números aleatorios
    std::srand(static_cast<unsigned>(std::time(0)));

    // Ángulo de temblor máximo en grados
    float maxShakeAngle = 5.0f;

    // Itera sobre cada cubo y aplica una rotación aleatoria
    for (auto &cube : cubes) {
		// Genera ángulos de temblor aleatorios para cada eje
        float shakeAngleX = (std::rand() % static_cast<int>(2 * maxShakeAngle * 100)) / 100.0f - maxShakeAngle;
        float shakeAngleY = (std::rand() % static_cast<int>(2 * maxShakeAngle * 100)) / 100.0f - maxShakeAngle;
        float shakeAngleZ = (std::rand() % static_cast<int>(2 * maxShakeAngle * 100)) / 100.0f - maxShakeAngle;

        // Aplica la rotación al cubo
        cube->rotation_point(Vec4f(1.0f, 0.0f, 0.0f, 1.0f), shakeAngleX);
        cube->rotation_point(Vec4f(0.0f, 1.0f, 0.0f, 1.0f), shakeAngleY);
        cube->rotation_point(Vec4f(0.0f, 0.0f, 1.0f, 1.0f), shakeAngleZ);
    }
}


void Rubik::shake() {
        // Semilla para generar números aleatorios
        std::srand(static_cast<unsigned>(std::time(0)));

        // Ángulo de temblor máximo en grados
        float maxShakeAngle = 2.0f;

        // Itera sobre cada cubo y aplica una rotación aleatoria
        for (auto &cube : cubes) {
            // Genera ángulos de temblor aleatorios para cada eje
            float shakeAngleX = (std::rand() % static_cast<int>(2 * maxShakeAngle * 100)) / 100.0f - maxShakeAngle;
            float shakeAngleY = (std::rand() % static_cast<int>(2 * maxShakeAngle * 100)) / 100.0f - maxShakeAngle;
            float shakeAngleZ = (std::rand() % static_cast<int>(2 * maxShakeAngle * 100)) / 100.0f - maxShakeAngle;

            // Aplica la rotación al cubo
            cube->rotation_point(Vec4f(1.0f, 0.0f, 0.0f, 1.0f), shakeAngleX, cube->get_centroid());
            cube->rotation_point(Vec4f(0.0f, 1.0f, 0.0f, 1.0f), shakeAngleY, cube->get_centroid());
            cube->rotation_point(Vec4f(0.0f, 0.0f, 1.0f, 1.0f), shakeAngleZ, cube->get_centroid());
        }
    }


void Rubik::shake2() {
    const float maxShakeAngle = 15.0f;  // Definir el rango máximo de temblor

    // Itera sobre cada cubo
    for (auto &cube : cubes) {
        // Aplica rotaciones de 15 grados en los ejes x+, y+, z+, x-, y-, z- secuencialmente
        cube->rotation_point(Vec4f(1.0f, 0.0f, 0.0f, 0.0f), maxShakeAngle, cube->get_centroid());
        cube->rotation_point(Vec4f(0.0f, 1.0f, 0.0f, 0.0f), maxShakeAngle, cube->get_centroid());
        cube->rotation_point(Vec4f(0.0f, 0.0f, 1.0f, 0.0f), maxShakeAngle, cube->get_centroid());
        cube->rotation_point(Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), maxShakeAngle, cube->get_centroid());
        cube->rotation_point(Vec4f(0.0f, -1.0f, 0.0f, 0.0f), maxShakeAngle, cube->get_centroid());
        cube->rotation_point(Vec4f(0.0f, 0.0f, -1.0f, 0.0f), maxShakeAngle, cube->get_centroid());
    }
}


#endif