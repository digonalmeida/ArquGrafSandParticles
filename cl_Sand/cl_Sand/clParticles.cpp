#include "clParticles.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

CLParticles::CLParticles(){
	cout<<"inicializa CLParticles"<<endl;
}

void CLParticles::init(){
	int i;
	srand(time(NULL));
	P = (vector3*) malloc(sizeof(vector3)*MAX_PARTICLES);
	F = (vector3*) malloc(sizeof(vector3)*MAX_PARTICLES);

    for(i = 0; i < MAX_PARTICLES; i++) {
		//P[i].x = (i%50) -25;
		//P[i].z = (i%50) -25;
		P[i].y = (float)((rand()%150)/(float)100.0)+10;
		P[i].z = (float)((rand()%40)/(float)100.0);
		P[i].x = (float)((rand()%40)/(float)100.0);
    }
	for(i = 0; i < MAX_PARTICLES; i++) {
		F[i].x=0;
		F[i].y=0;
		F[i].z=0;
    }

    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("vector_add_kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );

    // Get platform and device information
    platform_id = NULL;
    device_id = NULL;   
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_ALL, 1, 
            &device_id, &ret_num_devices);


	// Create an OpenCL context
	cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);

	// Create a command queue
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	// Create memory buffers on the device for each vector 
	p_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
			MAX_PARTICLES * sizeof(vector3), NULL, &ret);
	f_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
			MAX_PARTICLES * sizeof(vector3), NULL, &ret);

	// Copy the lists A and B to their respective memory buffers
	ret = clEnqueueWriteBuffer(command_queue, p_mem_obj, CL_TRUE, 0, 
			MAX_PARTICLES * sizeof(vector3), P, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, f_mem_obj, CL_TRUE, 0, 
			MAX_PARTICLES * sizeof(vector3), F, 0, NULL, NULL);

	// Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1, 
			(const char **)&source_str, (const size_t *)&source_size, &ret);

	// Build the program
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// Create the OpenCL kernel
	kernel = clCreateKernel(program, "calculate_forces", &ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&p_mem_obj);
	ret = clSetKernelArg(kernel,1,sizeof(cl_mem),(void*)&f_mem_obj);
   
	// Clean up
    //ret = clFlush(command_queue);
    //ret = clFinish(command_queue);
    //ret = clReleaseKernel(kernel);
    //ret = clReleaseProgram(program);
    //ret = clReleaseMemObject(a_mem_obj);
    //ret = clReleaseMemObject(b_mem_obj);
    //ret = clReleaseMemObject(c_mem_obj);
	//ret = clReleaseMemObject(p_mem_obj);
    //ret = clReleaseCommandQueue(command_queue);
    //ret = clReleaseContext(context);
   // free(A);
   // free(B);
   // free(C);
}
void CLParticles::setParticles(vector3 **positions,int count){
	particles_position=positions;
	particles_transform= (vector3**) malloc(count*sizeof(vector3*)); 

	particles_collisions_xpos= (int**) malloc(count*sizeof(int*));
	particles_collisions_ypos= (int**) malloc(count*sizeof(int*));
	particles_collisions_zpos= (int**) malloc(count*sizeof(int*));
	particles_collisions_xneg= (int**) malloc(count*sizeof(int*));
	particles_collisions_yneg= (int**) malloc(count*sizeof(int*));
	particles_collisions_zneg= (int**) malloc(count*sizeof(int*));
	for(int i=0;i<count;i++){
		particles_transform[i]= new vector3();
		particles_collisions_xpos[i]=new int();
		particles_collisions_ypos[i]=new int();
		particles_collisions_zpos[i]=new int();
		particles_collisions_xneg[i]=new int();
		particles_collisions_yneg[i]=new int();
		particles_collisions_zneg[i]=new int();
		(*(particles_collisions_xpos[i]))=0;
		
		(*(particles_collisions_ypos[i]))=0;
		(*(particles_collisions_zpos[i]))=0;
		(*(particles_collisions_xneg[i]))=0;
		(*(particles_collisions_yneg[i]))=0;
		(*(particles_collisions_zneg[i]))=0;

	}
	n_particles=count;
}
void CLParticles::setForces(vector3 **newForces,int count){
	forces=newForces;
	n_forces=count;

}
void CLParticles::setColliders(vector3 **positions, vector3 **dimensions, int count){
	colliders_position=positions;
	colliders_dimension=dimensions;
	n_colliders=count;
}
int CLParticles::getCount(){
	return n_particles;
}
void CLParticles::updateForces(){
	for(int i=0;i<n_particles;i++){
		particles_transform[i]->x=0;
		particles_transform[i]->y=0;
		particles_transform[i]->z=0;
		
	}
	for(int f=0;f<n_forces;f++){
		for(int i=0;i<n_particles;i++){
			particles_transform[i]->x+=forces[f]->x;
			particles_transform[i]->y+=forces[f]->y;
			particles_transform[i]->z+=forces[f]->z;
		}
	}
}
float distance(vector3 a, vector3 b){
	float d=sqrt( (a.x+b.x)+(a.y+b.y)+(a.z+b.z));
	return d;
}
void CLParticles::updateCollisions(){

	//collision math stuff =]
	//calcula só do chão por enquanto
	for(int i=0;i<n_particles;i++){
		
		(*(particles_collisions_xpos[i]))=0;
		(*(particles_collisions_ypos[i]))=0;
		(*(particles_collisions_zpos[i]))=0;
		(*(particles_collisions_xneg[i]))=0;
		(*(particles_collisions_yneg[i]))=0;
		(*(particles_collisions_zneg[i]))=0;
		if(particles_position[i]->y<=0){
			particles_transform[i]->y=0;
		}
	}
	//calcula n_collisions
	for(int i=0;i<n_particles;i++){
		//cout<<(*(particles_collisions_xpos[i]));
		for(int j=0;j<n_particles;j++){
			if(j!=i){
				if(distance(particles_position[i],particles_position[j])<0.1){
					if(particles_position[j]->x>particles_position[i]->x){
						(*(particles_collisions_xpos[i]))+=1;
						
					}else{
					
						(*(particles_collisions_xneg[i]))+=1;
					}
					if(particles_position[j]->y>particles_position[i]->y){
						(*(particles_collisions_ypos[i]))+=1;
					}else{
						(*(particles_collisions_yneg[i]))+=1;
					}
					if(particles_position[j]->z>particles_position[i]->z){
						(*(particles_collisions_zpos[i]))+=1;
					}else{
						(*(particles_collisions_zneg[i]))+=1;
					}
				}
			}
		}
	}
	cout<<particles_position[0]->x<<endl;
	for(int i=0;i<n_particles;i++){
		
		if(particles_transform[i]->x>0)
			particles_transform[i]->x=particles_transform[i]->x/((*(particles_collisions_xneg[i]))+1);
		else
			particles_transform[i]->x=particles_transform[i]->x/((*(particles_collisions_xpos[i]))+1);

		if(particles_transform[i]->y>0)
			particles_transform[i]->y=particles_transform[i]->y/((*(particles_collisions_yneg[i]))+1);
		else
			particles_transform[i]->y=particles_transform[i]->y/((*(particles_collisions_ypos[i]))+1);

		if(particles_transform[i]->z>0)
			particles_transform[i]->z=particles_transform[i]->z/((*(particles_collisions_zneg[i]))+1);
		else
			particles_transform[i]->z=particles_transform[i]->z/((*(particles_collisions_zpos[i]))+1);
	}
}
void CLParticles::step(){
	int i = 0;
	// Execute the OpenCL kernel on the list
	size_t global_item_size = MAX_PARTICLES; // Process the entire lists
	size_t local_item_size = 64; // Process in groups of 64
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
			&global_item_size, &local_item_size, 0, NULL, NULL);
	
	// Read the memory buffer C on the device to the local variable C
  	vector3 *nP = (vector3*) malloc(sizeof(vector3)*MAX_PARTICLES);
	vector3 *nF = (vector3*) malloc(sizeof(vector3)*MAX_PARTICLES);
		
	ret = clEnqueueReadBuffer(command_queue, p_mem_obj, CL_TRUE, 0, 
			MAX_PARTICLES * sizeof(vector3), nP, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, f_mem_obj, CL_TRUE, 0, 
			MAX_PARTICLES * sizeof(vector3), nF, 0, NULL, NULL);
		
	// Display the result to the screen
	//for(i = 0; i < MAX_PARTICLES; i++)
	//	printf("%f, %f, %f || %f, %f, %f \n",P[i].x,P[i].y,P[i].z, nP[i].x,nP[i].y,nP[i].z);

	P = nP;
	F = nF;
}
void CLParticles::clean(){

}