#define MAX_PARTICLES 2048
#define PARTICLE_SIZE 2
#define FLOOR_VALUE 0
#define X_VALUE 8
#define Z_VALUE 8
#define MAX_SPEED 1

typedef struct {
	float x;
	float y;
	float z;
} vector3;

__kernel void vector_add(__global vector3* p) {
    int i = get_global_id(0);
	p[i].x+=1;
}
__kernel void vector_sub(__global vector3* p){
	int i = get_global_id(0);
	p[i].x -= 1;
}
__kernel void calculate_forces(__global vector3* position, __global vector3* force){
	int i = get_global_id(0);
	vector3 tempPoint;
	tempPoint.x = 0;
	tempPoint.y = 0;
	tempPoint.z = 0;

	if(force[i].x > MAX_SPEED) force[i].x = MAX_SPEED;
	else if(force[i].x < -MAX_SPEED) force[i].x = -MAX_SPEED;
	if(force[i].y > MAX_SPEED) force[i].y = MAX_SPEED;
	else if(force[i].y < -MAX_SPEED) force[i].y = -MAX_SPEED;
	if(force[i].z > MAX_SPEED) force[i].z = MAX_SPEED;
	else if(force[i].z < -MAX_SPEED) force[i].z = -MAX_SPEED;

	//aplica força reto para baixo
	//soma com a força anterior
	if(!(position[i].y < FLOOR_VALUE + PARTICLE_SIZE)){
		force[i].x += 0;
		force[i].y += -0.0001;
		force[i].z += 0;
	}	
		//confere se vai colidir
	tempPoint.x = position[i].x + force[i].x;
	tempPoint.y = position[i].y + force[i].y;
	tempPoint.z = position[i].z + force[i].z;

	if(!(position[i].y < FLOOR_VALUE + PARTICLE_SIZE)){
		for(int j = 0; j < MAX_PARTICLES; j++){
			if( j == i) continue;
			if(sqrt((float)((position[j].x - tempPoint.x)*(position[j].x - tempPoint.x)) +
			((position[j].y - tempPoint.y)*(position[j].y - tempPoint.y)) +
			((position[j].z - tempPoint.z)*(position[j].z - tempPoint.z))) < PARTICLE_SIZE/3){
			
				//anda até o ponto de contato da colisão

				float angle1 = atan((float)(position[j].y - tempPoint.y)/(position[j].x - tempPoint.x));
				float angle2 = atan((float)(position[j].x - tempPoint.x)/(position[j].z - tempPoint.z));
				float angle3 = atan((float)(position[j].z - tempPoint.z)/(position[j].y - tempPoint.y));
				float dist = sqrt((float)((position[j].x - tempPoint.x)*(position[j].x - tempPoint.x)) +
									((position[j].y - tempPoint.y)*(position[j].y - tempPoint.y)) +
									((position[j].z - tempPoint.z)*(position[j].z - tempPoint.z))) - PARTICLE_SIZE;
			
				position[i].x += dist * sin(angle2);
				position[i].y += dist * sin(angle1);
				position[i].z += dist * sin(angle3);
		
				//modifica a força conforme a colisão
				force[i].x *= sin(angle2)/2;
				force[i].y *= sin(angle1)/2;
				force[i].z *= sin(angle3)/2;

			}else{
				if(!(position[i].y < FLOOR_VALUE + PARTICLE_SIZE)){
					//muda a posição conforme a força
					position[i].x += force[i].x;
					position[i].y += force[i].y;
					position[i].z += force[i].z;
				}
			}
		}	
	}

	//se chegar no chão para de ganhar força em y
	if(position[i].y < FLOOR_VALUE + PARTICLE_SIZE){
		position[i].y = FLOOR_VALUE + PARTICLE_SIZE;
		force[i].x = 0;
		force[i].y = 0;
		force[i].z = 0;
	}
	if(position[i].x < (X_VALUE*-1) + PARTICLE_SIZE || position[i].x > X_VALUE + PARTICLE_SIZE){
		position[i].x = FLOOR_VALUE + PARTICLE_SIZE;
		force[i].x = 0;
	}
	if(position[i].z < (Z_VALUE*-1) + PARTICLE_SIZE || position[i].z > Z_VALUE + PARTICLE_SIZE){
		position[i].z = FLOOR_VALUE + PARTICLE_SIZE;
		force[i].z = 0;
	}
			
}