#define MAX_PARTICLES 2048
#define PARTICLE_SIZE 1
#define PARTICLE_MASS 1
#define MAX_SOURCE_SIZE (0x100000)

#include <CL\cl.h>

struct vector3{
	float x;
	float y;
	float z;
};

class CLParticles{
public:
	vector3 *P;
	vector3 *F;

	cl_mem p_mem_obj;
	cl_mem f_mem_obj;

	cl_command_queue command_queue;
	cl_kernel kernel;

	cl_platform_id platform_id;
    cl_device_id device_id;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
	cl_int ret;


	vector3 **particles_position, **particles_transform;
	int **particles_collisions_xpos,**particles_collisions_xneg,**particles_collisions_ypos,**particles_collisions_yneg,**particles_collisions_zpos,**particles_collisions_zneg;
	vector3 **colliders_position, **colliders_dimension;
	vector3 **forces;
	int n_particles,n_colliders,n_forces;
	void setParticles(vector3 **positions,int count);
	void setForces(vector3 **forces,int count);
	void setColliders(vector3 **positions, vector3 **dimensions, int count);
	CLParticles();
	void init();
	void updateForces();
	void updateCollisions();
	void step();
	void clean();
	int getCount();
};