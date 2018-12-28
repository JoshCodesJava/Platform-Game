#pragma once
#include <Windows.h>

const int WIDTH = GetSystemMetrics(SM_CXSCREEN);
const int HEIGHT = GetSystemMetrics(SM_CYSCREEN);
const int BLOCKSIZE = 40;
const int X_BLOCKS = 1700;
const int Y_BLOCKS = 19;
const float SCALE = (float) (HEIGHT/Y_BLOCKS)/BLOCKSIZE;