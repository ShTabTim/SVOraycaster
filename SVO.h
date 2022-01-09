#pragma once
#include <glad/glad.h>
#include <vector>

typedef struct vox {
	GLuint mask;
	GLuint child;
	vox() { this->mask = 0; }
	vox(GLuint mask) { this->mask = mask; }
}vox;

class SVO {
private:
	GLuint voxels_buffer;
	std::vector<vox> vs;
	GLuint minScale = 2, maxScale = 8;
public:
	SVO() {
		vs.emplace_back();
		vs[0].child = 1;
		for (int i(8); i--;)
			vs.emplace_back();

		for (int i = 0; i < 1<<maxScale; i++)
			for (int j = 0; j < 1<<maxScale; j++)
				for (int k = 0; k < 1<<maxScale; k++)
					if(k <  - 64 * sin(3.1415 * i / 32.0f) * sin(3.1415 * j / 32.0f) * sin(3.1415 * k / 32.0f))
						setVox(i, k, j, rand()%9);
	}



	void setVox(int x, int y, int z, GLuint id) {
		int current_x = x;
		int current_y = y;
		int current_z = z;

		int current_size = 1 << maxScale;
		int current_index = 0;

		int last_index = -1;
		int last_sub;

		while (current_size >= (1 << minScale)) {
			vox& current_element = vs[current_index];
			current_size >>= 1;

			int sub_x = current_x / current_size;
			int sub_y = current_y / current_size;
			int sub_z = current_z / current_size;

			int sub_index = 4 * sub_x + 2 * sub_y + sub_z;

			current_x -= current_size * sub_x;
			current_y -= current_size * sub_y;
			current_z -= current_size * sub_z;

			GLuint current_id = (current_element.mask << sub_index);

			if (!(current_id & 0b1000000000000000)) {
				// Index for new element's subs
				int new_index = vs.size();
				vs[current_element.child + sub_index].child = new_index;

				// Update current sub status
				current_element.mask |= (0b1000000000000000 >> sub_index);

				// Update parent's leaf status
				if (last_index != -1)
					vs[last_index].mask |= (0b10000000 >> last_sub);

				// Allocate new subs if not leaf
				if (current_size >= (1 << minScale))
					for (int i = 0; i < 8; i++)
						vs.emplace_back(id << 16);
			}

			last_index = current_index;
			last_sub = sub_index;

			int next_index = vs[current_index].child + sub_index;
			current_index = next_index;
		}
	}
	void createBuffer() {
		glGenBuffers(1, &voxels_buffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, voxels_buffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vox) * vs.size(), vs.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, voxels_buffer);
	}
	void bindBuffer() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, voxels_buffer);
	}
};