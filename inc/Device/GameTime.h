#pragma once

class GameTime
{
public:
	GameTime();

	void update();

	float getDeltaTime() const;
	float getUnscaledDeltaTime() const;

	float getTotalTime() const;
	float getUnscaledTotalTime() const;

private:
	void updateDeltaTime();
	void updateTotalTime();

public:
	float m_TimeScale;

private:
	float m_DeltaTime;
	float m_UnscaledDeltaTime;

	float m_TotalTime;
	float m_UnscaledTotalTime;

	long m_CurTime;
	long m_PreTime;
};