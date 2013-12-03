#include "clParticles.h"
#include <iostream>
#include <math.h>
using namespace std;

CLParticles::CLParticles(){
	cout<<"inicializa CLParticles"<<endl;
}

void CLParticles::init(){

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
	updateForces();
	updateCollisions();
	for(int i=0;i<n_particles;i++){
		particles_position[i]->x+=particles_transform[i]->x;
		particles_position[i]->y+=particles_transform[i]->y;
		particles_position[i]->z+=particles_transform[i]->z;
	}
}
void CLParticles::clean(){

}