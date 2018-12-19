#version 330 core

struct materialInfo {
	vec4 oA;
	vec4 oD;
	vec4 oS;
	float ns;
};

struct lightInfo {
	vec4 color;
	vec3 position;
	vec3 direction;
	float angle;
	float falloff;
};


uniform lightInfo luzes[8];
uniform materialInfo material;
uniform vec3 cameraPosition;
uniform mat3 normalMatrix;
uniform vec4 iA; //Ambient Light
uniform vec3 rL; //Reflection direction (lL - (2 *(N dot lL) * N) )
uniform int l_count;
uniform int light_type; //0 for directional and 1 for point
uniform float max_angle;

in vec4 P; //Illuminated point position
in vec3 N; //Illuminated point normal vector

out vec4 fragmentColor;

void main(){
	float distance_aux;
	vec3 normal = normalize(N);
	vec3 V = normalize(vec3(P) - cameraPosition);

	vec4 ambient = material.oA * iA;
	vec4 diffuse = {0};
	vec4 specular = {0};

	vec3 L;
	for (int i = 0; i < l_count; i++){
		if (light_type == 1){
			L = normalize(luzes[i].position - vec3(P));
		} else {
			L = normalize(luzes[i].direction);
		}

		float l_angle = 0;
		if (light_type == 2){
			l_angle = acos(dot(V=luzes[i].direction, L));
		}
		
		if (light_type != 2 || light_type == 2 && l_angle <= acos(max_angle)){
			vec3 rl = 2 * dot(normal, L) * normal - L;
			if (luzes[i].falloff == 0) {
				distance_aux = 1;
			} else {
				distance_aux = distance(vec3(P), luzes[i].position);

				if (luzes[i].falloff == 2) {
					distance_aux = distance_aux * distance_aux;
				}
			}

			vec4 iL = luzes[i].color / distance_aux;

			diffuse = diffuse + material.oD * iL * dot(normal, L);
			specular = specular + material.oS * iL * pow(max(dot(-rl, V), 0), material.ns);
		}
	}
	

	fragmentColor = ambient + diffuse + specular;
}