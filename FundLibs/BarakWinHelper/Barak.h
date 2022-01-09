#pragma once

#include <glad/glad.h>
#include <thread>
#include "Win/win.h"

void prepr(hWindow* g_Win);
int main();
bool threadIsLive();
void SwapBuffers();
hWindow getWind();