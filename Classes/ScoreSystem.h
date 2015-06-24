#pragma once
#include "cocos2d.h"
#include <functional>

class ScoreSystem
{
private:
	// biến để lưu trữ điểm số hiện tại
	int score;

public:
	// hàm tạo
	ScoreSystem();

	// getter để lấy ra điểm số hiện tại
	int getScore();

	// hàm tăng điểm
	void increaseScore(int dscore);
	// hàm giảm điểm
	void decreaseScore(int dscore);

	// đây là biến lưu trữ một cấu trúc hàm
	// trong C++, ta dùng nó để lưu trữ hàm
	// cần gọi thông báo khi điểm số thay đổi
	// đại loại, các bạn cứ tưởng tượng nó lưu
	// con trỏ đến một hàm để sau này gọi lại
	std::function<void(ScoreSystem *)> OnScoreChanged;
};