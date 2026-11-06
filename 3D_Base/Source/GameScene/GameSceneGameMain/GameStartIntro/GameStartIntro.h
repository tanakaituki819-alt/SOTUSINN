#pragma once

class GameStartIntro
{
private:
	static constexpr float Waitingtime =240;
	static constexpr float Closingtime = 120;
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
		AllEND
	};
	GameStartIntro();
	~GameStartIntro();
	void Update() ;
	void Draw() ;
	bool GetEnd() { return End; }
private:
	float SCX = WND_W / 2 - offSetX;
	CSprite2D* m_pKONDATE;
	D3DXVECTOR2 L_Position = { offSetX,offSetY };
	D3DXVECTOR3	L_Rotation = {};
	D3DXVECTOR3 L_RotPot = { 0.5,0.5,0 };
	D3DXVECTOR2 R_Position = {WND_W/2,offSetY };
	D3DXVECTOR3	R_Rotation = {};
	D3DXVECTOR2 LPatternNo = { 0,0 };
	D3DXVECTOR2 RPatternNo = { 1,0 };
	int Timu;
	float C;
	bool End=false;
	float R(float i);
	steat m_steat = steat::close;

	float UpS = 1;

	float REINVO = 0;

	CSprite2D* OSAKINIITADAKIMASU;
	float OSAKINIITADAKIMASU_T = 0;
	float Quadraticfunction(float i);
};
