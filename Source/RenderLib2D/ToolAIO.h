#pragma once

// Uniform
#pragma warning(disable:26451)

// Construction
#pragma warning(disable:26495)

#include<algorithm>
#include<cstdint>
#include<cstdio>
#include<string>
#include<vector>
#include<memory>
#include<cmath>
#include<ctime>

#include<unordered_set>
#include<unordered_map>

#include<STB/stb_image.h>
#include<STB/stb_image_write.h>

inline double Uniform()
{
#define RANDOM_MAX_LIMIT (1024)

	return 1.0 * (rand() % RANDOM_MAX_LIMIT) / (RANDOM_MAX_LIMIT - 1);
}