#include "ScoreSystem.h"

USING_NS_CC;

ScoreSystem::ScoreSystem()
: score(0) {}

int ScoreSystem::getScore()
{
	return score;
}

void ScoreSystem::increaseScore(int dscore)
{
	score += dscore;
	// gọi hàm thông báo
	if (OnScoreChanged)
	{
		OnScoreChanged(this);
	}
}

void ScoreSystem::decreaseScore(int dscore)
{
	score -= dscore;

	// gọi hàm thông báo
	if (OnScoreChanged)
	{
		OnScoreChanged(this);
	}
}