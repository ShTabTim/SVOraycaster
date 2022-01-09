#version 450 core

//max amount of iterations of ray steps in voxel array
//#define MaxRenderL 1024
#define SvoMaxDepth 23
#define MaxDepth 1024
#define MaxLayer 7
struct vox {
    uint id, child;
};

//buffer of voxel array
layout(std430, binding=0) readonly buffer voxels_block {
    vox voxels[];
};

layout(location=1) uniform uint voxCount;
layout(location=2) uniform uint layers;

layout(location=3) uniform vec3 pos;
layout(location=4) uniform vec2 angles;

out vec4 FragColor;
in vec2 FragCord;

vec3 getColor(uint id) {
    switch (id) {
    case 1: return vec3(1, 1, 1);
    case 2: return vec3(1, 1, 0);
    case 3: return vec3(1, 0, 1);
    case 4: return vec3(1, 0, 0);
    case 5: return vec3(0, 1, 1);
    case 6: return vec3(0, 1, 0);
    case 7: return vec3(0, 0, 1);
    default:return vec3(0, 0, 0);
    }
}

const vec3 PPP = vec3(1, 1, 1);
const vec3 PNP = vec3(1, -1, 1);
const vec3 PNN = vec3(1, -1, -1);
const vec3 NPN = vec3(-1, 1, -1);
const vec3 NNN = vec3(-1, -1, -1);
const vec3 NNP = vec3(-1, -1, 1);
const vec3 NPP = vec3(-1, 1, 1);
const vec3 PPN = vec3(1, 1, -1);
const vec3 POS[8] = vec3[8](PNN, PNP, PPN, PPP, NNN, NNP, NPN, NPP);
struct Hit {
	float l;
	vec3 n;
    bool inter;
	vec3 point;
	uint id;
	uint iteration;
};
bool cube_intersect(const vec3 ro, const vec3 rdInv, const float rad, inout vec3 t2, inout float tmin, inout float tmax) {
    vec3 t0, t1, t3;
    t2 = rad*rdInv;
    t3 = -ro*rdInv;
    t0 = t3 + t2;
    t1 = t3 - t2;
    t2 = min(t0, t1);
    t3 = max(t0, t1);
    tmin = max(max(max(t2.x, t2.y), t2.z), 0);
    tmax = min(min(t3.x, t3.y), t3.z);//t = min(tmin, tmax);
    return tmax > tmin;
}
//struct Stack {
//	uint  index;
//	vec3  center;
//	float scaleChild;
//};
//bool octree_intersect(vec3 ro, vec3 invRd, out uint id, inout Hit hit) {    
//	vec3 center = vec3(0), centerChild;
//    float scaleChild = 1;
//    float tmin, tmax; vec3 t2;
//    if (!cube_intersect(ro-center, invRd, scaleChild, t2, tmin, tmax)) return false;
//    float maxLayer = 2*scaleChild/(1<<MaxLayer);
//    hit.inter = false;
//    uint index = 0;
//    Stack stack[20];
//    scaleChild *= 0.5f;
//    stack[0] = Stack(index, center, scaleChild);
//    hit.l = MaxRenderL;
//    
//    for(int stackPos = 0; stackPos >= 0; stackPos--) {
//        index  = stack[stackPos].index;
//        center = stack[stackPos].center;
//		scaleChild = stack[stackPos].scaleChild;
//        uint accumulated_offset = 0;
//        for (uint i = 0; i < 8; i++) {
//            vox child = voxels[voxels[index].child+i];
//            if (child.id == 0) continue;//is empty
//            bool is_leaf = child.child == 0 || scaleChild < maxLayer;
//            
//            centerChild = center + scaleChild * POS[i];
//
//            if (!cube_intersect(ro-centerChild, invRd, scaleChild, t2, tmin, tmax)) {
//                if(is_leaf) accumulated_offset++;
//                continue;
//            }
//
//            if (is_leaf){ //not empty, but a leaf
//                float l = min(tmin, tmax);
//                if (hit.l > l) {
//                    id = child.id;
//                    hit.l = l;
//                    hit.inter = true;
//                    hit.n = t2;
//                }
//            } else { //not empty and not a leaf
//            	stack[stackPos++] = Stack(voxels[index].child + i , centerChild, scaleChild*0.5);
//                accumulated_offset++;
//            }
//        }
//    }
//    if(hit.inter)hit.n = -sign(invRd) * step(hit.n.yzx, hit.n) * step(hit.n.zxy, hit.n);
//    return hit.inter;
//}
//
//uint intersect(const vec3 ro, const vec3 rd, inout Hit hit) {
//    vec3 invRd = 1/rd;
//	Hit hot;
//    uint id0, id;
//
//    if (octree_intersect(ro, invRd, id0, hot) && hit.l > hot.l) {
//        id = id0;
//        hit = hot;
//    }
//    return id;
//}
struct OctreeContext
{
	int index;
	ivec3 sub_coords;

	vec3 start_coords;

	vec3 t_max;
	float t_max_min;
};
void initialize_context(inout OctreeContext context, in ivec3 dir, in vec3 inv_rd, int current_size) {

	context.sub_coords = min(ivec3(context.start_coords / current_size), 1);
	
	context.t_max_min = 0;

	context.t_max = ((dir + context.sub_coords)*current_size - context.start_coords) * inv_rd;
}
void advance(inout OctreeContext context, in ivec3 step_vec, vec3 t_delta, inout Hit hit)
{
	if (context.t_max.x < context.t_max.y)
	{
		if (context.t_max.x < context.t_max.z)
		{
			context.sub_coords.x += step_vec.x;
			context.t_max_min = context.t_max.x;
			context.t_max.x  += t_delta.x;
			hit.n = vec3(-step_vec.x, 0.0, 0.0);
		}
		else
		{
			context.sub_coords.z += step_vec.z;
			context.t_max_min = context.t_max.z;
			context.t_max.z  += t_delta.z;
			hit.n = vec3(0.0, 0.0, -step_vec.z);
		}
	}
	else
	{
		if (context.t_max.y < context.t_max.z)
		{
			context.sub_coords.y += step_vec.y;
			context.t_max_min = context.t_max.y;
			context.t_max.y  += t_delta.y;
			hit.n = vec3(0.0, -step_vec.y, 0.0);
		}
		else
		{
			context.sub_coords.z += step_vec.z;
			context.t_max_min = context.t_max.z;
			context.t_max.z  += t_delta.z;
			hit.n = vec3(0.0, 0.0, -step_vec.z);
		}
	}
}
void castRay(const vec3 ro, const vec3 rd, const vec3 inv_rd, in int max_iter, out Hit hit) {
	hit.iteration = 0;
	const int max_scale = 8;
	
	hit.inter = false;

	int scale      = max_scale-1;
	int max_size   = 1<<(scale);
	ivec3 step_vec = ivec3(sign(inv_rd));
	ivec3 dir      = max(step_vec, 0);
	vec3 t_delta   = abs(vec3(max_size) * inv_rd);
	bool skip      = false;
	int current_stack_index = 0;

	const int max_stack_size = max_scale+1;
	OctreeContext stack[max_stack_size];

	stack[0].index = 0;
	stack[0].start_coords = ro;

	initialize_context(stack[0], dir, inv_rd, max_size);

	while (hit.iteration <= 1 << max_iter) {
		hit.iteration++;
		// Current context (location, index, sub_index, ...)
		OctreeContext context = stack[current_stack_index];
		vox current_elem = voxels[context.index];

		int current_size    = 1<<scale;
		int sub_index       = context.sub_coords.x + (context.sub_coords.y<<1) + (context.sub_coords.z<<2);

		int current_id = (int(voxels[context.index].id) << sub_index);

		bool is_empty = !bool(current_id & 0x8000);
		bool is_leaf = !bool(current_id & 0x80);//voxels[context.index].child == 0;
	
		// If current sub empty -> move to next one
		if (is_empty || skip) {
			if (skip) skip = false;

			advance(stack[current_stack_index], step_vec, t_delta, hit);

			if (!(clamp(stack[current_stack_index].sub_coords, 0, 1) == stack[current_stack_index].sub_coords)) {
				scale++;
				current_stack_index--;
				t_delta *= 2;
				
				if (current_stack_index<0) return;
				else skip = true;
			}
			continue;
		}

		if (is_leaf) {
			hit.inter = true;
			hit.id = voxels[context.index].id >> 16;
			// Compute absolute hit point position
			float total_t_max = 0;
			for (int i=0; i<current_stack_index; i++) total_t_max += stack[i].t_max_min;
				
			hit.point = stack[0].start_coords + rd*(total_t_max-0.0001);
			return;
		} else {
			// Adding sub context to the stack
			scale--;
			t_delta *= 0.5;
			current_stack_index++;

			if (current_stack_index > max_stack_size-1) return;

			// Translating relative coords into sub context
			stack[current_stack_index].index = int(current_elem.child) + sub_index;
			stack[current_stack_index].start_coords = context.start_coords + context.t_max_min*rd - vec3(context.sub_coords)*current_size;

			// Initializing sub raycast 
			initialize_context(stack[current_stack_index], dir, inv_rd, current_size>>1);
		}
		
	}
}

void main() {
    vec2 s = sin(angles);
    vec2 c = cos(angles);
    vec3 rd = normalize(vec3(FragCord*0.1, 1) + vec3(FragCord, 0)) 
            * 
            mat3( 1,   0,   0,
                  0,   c.y, s.y,
                  0,  -s.y, c.y )
            *
            mat3( c.x, 0,   s.x,
                  0,   1,   0,
                 -s.x, 0,   c.x );
    vec3 ro = pos, col = vec3(0.1);
    Hit hit;
	
	int max_iter = 8;
	castRay(ro, rd, 1.0f/rd, max_iter, hit);
    //uint id = intersect(ro, rd, hit);

	//col = vec3(hit.iteration / float(1 << max_iter));// */ vec3(dot(hit.n, normalize(vec3(0.2, 1, 0.1))));
    if (hit.inter) {
        vec3 lightDir = normalize(vec3(-1.0, 3.0, -1.0));
        col = getColor(hit.id);
	
        Hit hot;
        //hot.l = MaxRenderL;
		castRay(hit.point, lightDir, 1.0f/lightDir, max_iter, hot);
        //intersect(ro + rd*(hit.l-0.00001), lightDir, hot);
        
        if (hot.inter) col /= 4;
        float diffuseAttn = max(dot(hit.n, lightDir), 0.0);
        vec3 light = vec3(1.0,0.9,0.7);
        
        vec3 ambient = vec3(0.2, 0.2, 0.3);
        
        vec3 reflected = reflect(rd, hit.n);
        float specularAttn = max(dot(reflected, lightDir), 0.0);
        
        col *= diffuseAttn*light*1.0 + specularAttn*light*0.6 + ambient;
        
        
    }
    FragColor = vec4(col, 1);

}