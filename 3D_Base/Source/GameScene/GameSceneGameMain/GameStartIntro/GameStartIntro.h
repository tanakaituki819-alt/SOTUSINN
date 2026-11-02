#pragma once

class GameStartIntro
{
private:
	static constexpr float Waitingtime = 60;
	static constexpr float Closingtime = 60;
	static constexpr float offSetX = 20;
	static constexpr float offSetY = 40;
	static constexpr float miserutime = 60;
public:
	enum class steat
	{
		close,
		miseru,
		Up,
		END,
	};
	GameStartIntro();
	~GameStartIntro();
	void Update() ;
	void Draw() ;
private:
	float SCX = WND_W / 2 - offSetX;
	CSprite2D* m_pKONDATE;
	D3DXVECTOR2 L_Position = { offSetX,offSetY };
	D3DXVECTOR3	L_Rotation = {};
	D3DXVECTOR2 R_Position = {WND_W/2,offSetY };
	D3DXVECTOR3	R_Rotation = {};

	int Timu;
	int C;
	bool End=false;
	float R(float i);
	steat m_steat = steat::close;
	float UpS = 1;
	int PatternNoY = 0;
};
