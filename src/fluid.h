#ifndef FLUID_H
#define FLUID_H

#include<vector>

#include"math.h"
#include"object.h"

const double g=9.8;

class Particle
{
protected:
    double mass;
    double lambda;
    double density;
public:
    Particle()=default;
    ~Particle()=default;
};

class Solver
{
protected:
    double time_step;
    Box start_pos;
    Box particle_range;
    std::vector<Point> particles;
public:
    Solver()=default;
    ~Solver()=default;
    Solver(const Box &start_pos,const Box &particle_range):
        start_pos(start_pos),particle_range(particle_range){}
protected:
    double poly6_wkernel(const Vectorf &r);
    double spiky_wkernel_grad(const Vectorf &r);
    void compute_density(unsigned int particle_index);
    void compute_lagrange_multiplier(unsigned int particle_index);
    void solve_density_constraint(unsigned int particle_index);
    void update_velocity(unsigned int particle_index);
    void apply_gravity(unsigned int particle_index);
    void constraint_projection();
    void compute_xsph_viscosity();
public:
    void load_assets();
    void init_particles();
    void fill_surface(std::vector<Object *> &objects);
    void generate_next();
};

#endif