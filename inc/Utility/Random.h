#pragma once

//ランダム用クラス
class Random
{
public:
	Random();
	~Random();

	int getRandom(int min, int max);
	float getRandom(float min, float max);
	bool getTrueOrFalse();
	bool randomPercentage(float percentage);
};