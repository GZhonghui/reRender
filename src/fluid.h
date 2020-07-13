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
    Particle(double mass,double lambda,double density):
        mass(mass),lambda(lambda),density(density){}
};

class Solver
{
protected:
    int frame;
    double time_step;
    Box box_range;
    Box particle_range;
    Point box_start;
    Point particle_start;
    std::vector<Point> particles;
public:
    Solver()=default;
    ~Solver()=default;
    Solver
    (
        double time_step,
        const Box &box_range,
        const Box &particle_range,
        const Point &box_start,
        const Point &particle_start
    ):
        frame(0),
        time_step(time_step),
        box_range(box_range),
        particle_range(particle_range),
        box_start(box_start),
        particle_start(particle_start)
    {}
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
    void build_wall();
    void init_particles();
    void fill_surface(std::vector<Object *> &objects);
    void generate_next();
};

#endif