#include "CWinnerResultFavoriteFood.h"
#include "ResultDataManager/CResultDataManager.h"
#include <cfloat>

namespace
{
	using OL = CSpriteManager::enMeshObjList;

	//食材ごとの表示設定.
	struct SFoodDisplaySetting
	{
		Ingredients	No;			//食材.
		OL			Mesh;		//使うモデル.
		D3DXVECTOR3	SizeRatio;	//縦横比.
		D3DXVECTOR3	RotDeg;		//向き.
	};
	const SFoodDisplaySetting FOOD_DISPLAY_TABLE[] =
	{
		{ Ingredients::NIKU,		OL::S_NIKU,		{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//肉.
		{ Ingredients::NINZIN,		OL::S_NINZIN,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//にんじん.
		{ Ingredients::DAIKON,		OL::S_DAIKON,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//大根.
		{ Ingredients::ENOKI,		OL::S_ENOKI,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//エノキ.
		{ Ingredients::HAKUSAI1,	OL::S_HAKUSAI1,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//白菜.
		{ Ingredients::HAKUSEI2,	OL::S_HAKUSAI2,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//白菜.
		{ Ingredients::KANI,		OL::S_KANI,		{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//カニ.
		{ Ingredients::KUMANOTE,	OL::S_KUMANOTE,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//クマの手.
		{ Ingredients::NEGI,		OL::S_NEGI,		{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//ねぎ.
		{ Ingredients::ROBUSTER,	OL::S_ROBUSTER,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//ロブスター.
		{ Ingredients::SAKANA,		OL::S_SAKANA,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//リュウグウノツカイ.
		{ Ingredients::SITAKE,		OL::S_SITAKE,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//シイタケ.
		{ Ingredients::TAI,			OL::S_TAI,		{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//タイ.
		{ Ingredients::TARA,		OL::S_TARA,		{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//タラ.
		{ Ingredients::TOUHU,		OL::S_TOUHU,	{ 1.f, 0.5f, 1.f }, { 25.f, 0.f, 0.f } },	//豆腐.
		{ Ingredients::UINNER,		OL::S_UINNER,	{ 1.f, 1.f,  1.f }, { 20.f, 0.f, 0.f } },	//ウインナー.
	};

	//個人リザルトでの表示位置.
	const D3DXVECTOR2 FOOD_SCREEN_POS[CWinnerResultFavoriteFood::PLAYER_MAX] =
	{
		{  270.f, 515.f },	//プレイヤー壱.
		{  575.f, 515.f },	//プレイヤー弐.
		{  880.f, 515.f },	//プレイヤー参.
		{ 1185.f, 515.f },	//プレイヤー肆.
	};

	constexpr float FOOD_RADIUS		= 38.f;		//表示サイズ.
	constexpr float CAMERA_DISTANCE	= 1000.f;	//カメラの距離.
}

CWinnerResultFavoriteFood::CWinnerResultFavoriteFood()
	: m_Food()
	, m_mView()
	, m_mProj()
	, m_Light()
{
	//正面から見るカメラ.
	D3DXVECTOR3 Eye(0.f, 0.f, -CAMERA_DISTANCE);
	D3DXVECTOR3 Look(0.f, 0.f, 0.f);
	D3DXVECTOR3 Up(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&m_mView, &Eye, &Look, &Up);

	//画面のピクセル座標で配置できる正射影.
	//※y軸は上向きなので、画面座標のyはマイナスにして使う.
	D3DXMatrixOrthoOffCenterLH(&m_mProj,
		0.f, static_cast<float>(WND_W),
		-static_cast<float>(WND_H), 0.f,
		1.f, CAMERA_DISTANCE * 2.f);

	//右上手前から照らすライト.
	m_Light.vDirection = D3DXVECTOR3(1.f, 1.5f, -1.5f);
	m_Light.fIntensity = 2.f;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		m_Food[i].ScreenPos	= FOOD_SCREEN_POS[i];
		m_Food[i].Radius	= FOOD_RADIUS;
		m_Food[i].IsVisible	= true;
		SetupFood(i);
	}
}

void CWinnerResultFavoriteFood::SetLayout(int PlayerIndex, const D3DXVECTOR2& ScreenPos, float Radius)
{
	if (PlayerIndex < 0 || PlayerIndex >= PLAYER_MAX) return;
	m_Food[PlayerIndex].ScreenPos	= ScreenPos;
	m_Food[PlayerIndex].Radius		= Radius;
}

void CWinnerResultFavoriteFood::SetVisible(int PlayerIndex, bool IsVisible)
{
	if (PlayerIndex < 0 || PlayerIndex >= PLAYER_MAX) return;
	m_Food[PlayerIndex].IsVisible = IsVisible;
}

CWinnerResultFavoriteFood::~CWinnerResultFavoriteFood()
{
	//メッシュはCSpriteManagerが管理しているので参照を外すだけ.
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		m_Food[i].pMesh = nullptr;
	}
}

void CWinnerResultFavoriteFood::SetupFood(int PlayerIndex)
{
	SFoodView& Food = m_Food[PlayerIndex];
	Food.pMesh = nullptr;

	const SPlayerResultData& Data = CResultDataManager::GetInstance()->GetPlayerData(PlayerIndex);
	//参加していない、または何も取っていないプレイヤーは表示しない.
	if (!Data.m_bActive || Data.m_FavoriteIngredient == Ingredients::none) return;

	const SFoodDisplaySetting* pSetting = nullptr;
	for (const auto& Setting : FOOD_DISPLAY_TABLE)
	{
		if (Setting.No == Data.m_FavoriteIngredient)
		{
			pSetting = &Setting;
			break;
		}
	}
	if (pSetting == nullptr) return;

	CStaticObjMesh* pMesh = CSpriteManager::GetObjMesh(pSetting->Mesh);
	if (pMesh == nullptr || pMesh->GetMesh().empty()) return;

	//モデルの範囲を求める.
	D3DXVECTOR3 Min( FLT_MAX,  FLT_MAX,  FLT_MAX);
	D3DXVECTOR3 Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (const auto& Vertex : pMesh->GetMesh())
	{
		D3DXVECTOR3 Pos(
			Vertex.Pos.x * pSetting->SizeRatio.x,
			Vertex.Pos.y * pSetting->SizeRatio.y,
			Vertex.Pos.z * pSetting->SizeRatio.z);
		D3DXVec3Minimize(&Min, &Min, &Pos);
		D3DXVec3Maximize(&Max, &Max, &Pos);
	}
	D3DXVECTOR3 Center = (Min + Max) * 0.5f;

	//中心から一番遠い頂点までの距離.
	float Radius = 0.f;
	for (const auto& Vertex : pMesh->GetMesh())
	{
		D3DXVECTOR3 Pos(
			Vertex.Pos.x * pSetting->SizeRatio.x,
			Vertex.Pos.y * pSetting->SizeRatio.y,
			Vertex.Pos.z * pSetting->SizeRatio.z);
		D3DXVECTOR3 Diff = Pos - Center;
		float Length = D3DXVec3Length(&Diff);
		if (Length > Radius) Radius = Length;
	}
	if (Radius <= 0.f) return;

	//どの食材も同じくらいの大きさに揃える.
	float Fit = 1.f / Radius;
	Food.pMesh	= pMesh;
	Food.Scale	= pSetting->SizeRatio * Fit;
	Food.Center	= Center * Fit;
	Food.Rotation = D3DXVECTOR3(
		D3DXToRadian(pSetting->RotDeg.x),
		D3DXToRadian(pSetting->RotDeg.y),
		D3DXToRadian(pSetting->RotDeg.z));
	Food.IsShell = (Data.m_FavoriteIngredient == Ingredients::KANI || Data.m_FavoriteIngredient == Ingredients::ROBUSTER);
}

void CWinnerResultFavoriteFood::Draw()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		const SFoodView& Food = m_Food[i];
		if (Food.pMesh == nullptr || !Food.IsVisible) continue;

		//CStaticObjMeshと同じ順番で回転行列を作る.
		D3DXMATRIX mYaw, mPitch, mRoll, mRot;
		D3DXMatrixRotationY(&mYaw, Food.Rotation.y);
		D3DXMatrixRotationX(&mPitch, Food.Rotation.x);
		D3DXMatrixRotationZ(&mRoll, Food.Rotation.z);
		mRot = mYaw * mPitch * mRoll;

		//モデルの中心が表示位置に来るようにずらす.
		D3DXVECTOR3 Center = Food.Center * Food.Radius;
		D3DXVECTOR3 RotatedCenter;
		D3DXVec3TransformCoord(&RotatedCenter, &Center, &mRot);

		D3DXVECTOR3 Target(Food.ScreenPos.x, -Food.ScreenPos.y, 0.f);
		D3DXVECTOR3 CamPos(Target.x, Target.y, -CAMERA_DISTANCE);

		Food.pMesh->SetPosition(Target - RotatedCenter);
		Food.pMesh->SetRotation(Food.Rotation);
		Food.pMesh->SetScale(Food.Scale * Food.Radius);

		//ゲーム中の煮えたときと同じ色.
		if (Food.IsShell) {
			Food.pMesh->SetLightCOLOR256({ 200,100,75 });
		}
		else {
			Food.pMesh->SetLightCOLOR256({ 110,50,2 });
		}
		Food.pMesh->SetisCOLOR(true);

		Food.pMesh->Render(m_mView, m_mProj, m_Light, CamPos);
	}
}
