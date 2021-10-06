#pragma once

class Timer
{
public:
	Timer();
	Timer(float maxTime, bool useUnscaledTime = false, bool startMax = false);
	~Timer();

	void update();
	void setMaxTime(float value);
	void reset();
	void setUseUnscaledTime(bool value) { m_UseUnscaledTime = value; }

	float getCurrentTime();
	float getMaxTime();

	float getRatio();
	float getRatioClamped();

	bool isTime();

private:
	float m_CurrentTime;
	float m_MaxTime;
	bool m_UseUnscaledTime;
};

