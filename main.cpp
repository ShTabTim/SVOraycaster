#include "FundLibs/BarakWinHelper/Barak.h"
#include "FundLibs/BarakWinHelper/Win/Keys.h"
#include "FundLibs/ObjectsGL/Shader.h"
#include <chrono>
#include "SVO.h"

//glad 4.6

float ff(int g) { return (float)g; }

void prepr(hWindow* g_Win){
	g_Win->rename(L"Graphical panel");
}


int main() {
	srand(time(0));

	program prog;
	prog.setShaderFVG("Shaders/main.vert.glsl", 0);
	prog.setShaderFVG("Shaders/main.frag.glsl", 1);
	prog.create();
	prog.use();

	//creates voxel array
	SVO svo;
	//svo.create();

	//creates and load voxel array to GPU
	svo.createBuffer();

	float speed = 256;
	float angSpeed = 2;
	char vel[3];
	float pos[3] = { 0, 9, 0 };
	char angVel[2] = { 0, 0 };
	float ang[2] = { 0, 0 };
	float dt;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	while (threadIsLive()) {
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> dTime = tp2 - tp1;
		tp1 = tp2;
		float dt = dTime.count();
		
		KeyUpdate();

		if(GetKey('B').bHeld) {
			svo.setVox(pos[2], pos[1], pos[0], 1, 8);
			svo.setDateOnGpu();
		}

		if (GetKey(VK_ESCAPE).bHeld)
			return 1;
		angVel[0] = 0; angVel[1] = 0;
		if (GetKey(VK_LEFT).bHeld)
			angVel[0] -= 1;
		if (GetKey(VK_RIGHT).bHeld)
			angVel[0] += 1;
		ang[0] += ((float)angVel[0]) * angSpeed * dt;
		if (GetKey(VK_UP).bHeld && ang[1] < 1.57079632f)
			angVel[1] += 1;
		if (GetKey(VK_DOWN).bHeld && ang[1] > -1.57079632f)
			angVel[1] -= 1;
		ang[1] += ((float)angVel[1]) * angSpeed * dt;

		if (ang[0] > 3.14159265f)
			ang[0] -= 6.2832853f;
		if (ang[0] < -3.14159265f)
			ang[0] += 6.2832853f;
		vel[0] = 0;
		vel[1] = 0;
		vel[2] = 0;
		if (GetKey('W').bHeld)
			vel[2] += 1;
		if (GetKey('A').bHeld)
			vel[0] -= 1;
		if (GetKey('S').bHeld)
			vel[2] -= 1;
		if (GetKey('D').bHeld)
			vel[0] += 1;
		if (GetKey(VK_SHIFT).bHeld)
			vel[1] -= 1;
		if (GetKey(VK_SPACE).bHeld)
			vel[1] += 1;
		pos[0] += (((float)vel[0])*cos(ang[0]) + ((float)vel[2])*sin(ang[0]))*speed*dt;
		pos[1] += ((float)vel[1])*speed*dt;
		pos[2] += (((float)vel[2])*cos(ang[0]) - ((float)vel[0])*sin(ang[0]))*speed*dt;
		//sets position and angles of camera on GPU
		glUniform3f(3, pos[0], pos[1], pos[2]);
		glUniform2f(4, ang[0], ang[1]);

		//clears color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		//sets aspect area of screen
		glUniform1f(0, ff(getWind().getW())/ff(getWind().getH()));
		//draws canvas
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SwapBuffers();
	}
	return 99;
}