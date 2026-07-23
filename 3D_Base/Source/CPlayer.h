#pragma once

#include "CStaticMeshObjObject.h"
#include <list>
#include "CIngredients.h"
#include "CPlayerUI.h"
class CPlayer
	:public  CStaticMeshObjObject	//スタティックメッシュオブジェのオブジェクトを継承.
{
private:	
	static constexpr float Speed = 0.1;		//スピード
	static const int Chopsticks_Max = 2;	//お箸描画最大数.
public:
	CPlayer();
	~CPlayer();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;
	//コントローラー情報.
	void SetXInput( CXInput* Input) { MyController = Input; }	
	//UIの描画.
	void DrawUI();
	//プレイヤーNoセット.
	void SetPlayerNo(int No) { PlayerNo = No; };
	void GetIngredients(CIngredients*  YASAI);
	std::list<Ingredients>& GetAllIngredients() {return my_list;}
private:
	D3DXVECTOR3				m_Pos;							//ポジション.
	CXInput*				MyController;					//コントローラー.
	CSprite3D*				m_Cousor;						//カーソル.
	CStaticMeshObjObject*	m_pChopsticks[Chopsticks_Max];	//お箸(お箸を二つ描画するために配列化).
	std::list<Ingredients>	my_list;						//リスト.
	int						Score;							//スコア.
	CPlayerUI*				P_UI;							//プレイヤーUI.
	int						PlayerNo;						//プレイヤーNo.
	bool					m_IsCollecting	 = false;		//今回収中かどうか.
	float					m_Collectingtime = 0;			//回収中のカウント.
};
