#pragma once

//警告についてのコード分析を無効化する
#pragma warnig(disable :4005);


//Effekseer関連のヘッダー、ライブラリ
#include<Effekseer.h>
#include<EffekseerRendererDX11.h>


#ifdef _DEBUG
#pragma comment(lib,"Effekseerd.lib")
#pragma comment(lib,"EffekseerRendererDX11d.lib")
#else
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"EffekseerRendererDX11.lib")
#endif
//エイリアスを用意
//コードが長くなって読みずらいため
namespace {
	namespace Es = ::Effekseer;
	using EsManagerRef = ::Es::ManagerRef;
	using EsManager = ::Es::Manager;
	using EsEffecRef = ::Es::EffectRef;
	using EsEffect = ::Es::Effect;
	using EsVec3 = ::Es::Vector3D;
	using EsMatrix = ::Es::Matrix44;
	using EsHandle = ::Es::Handle;
	using EsRenderefRef = ::EffekseerRendererDX11::RendererRef;
	using  EsRenderef = ::EffekseerRendererDX11::Renderer;
}




/*
*フリーソフト　Effekseerのデータを使うためのクラス
*sigleton（シングルトン::デザインパターンの１つで）作成
*/

class Effect
{
public:

	~Effect();
	//インスタンス取得（唯一のアクセス原）
	static Effect* GetInstance()
	{
		//唯一のインスタンス作成
		static Effect s_Instance;
		return &s_Instance;
	}
public:
	//生成やコピーを禁止する
	Effect();
	//コピーコンストラクタによるコピーを禁止する
	Effect(const Effect& rhs) = delete;
	//代入演算子によるコピーを禁止する
	Effect& operator=(Effect& rhs) = delete;

	HRESULT Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//データ読み込み
	HRESULT LoadData();


	//描画
	void Draw(const D3DXMATRIX&mView, const D3DXMATRIX&mProj, const LIGHT&lLight, const CAMERA&camera);

	//変換系
	::EsVec3 ToEfkVector3(const D3DXVECTOR3* pSrcVec3Dx);
	D3DXVECTOR3 ToDxVector3(const EsVec3* pSrcVec3efk);
	//matrix(行列)
	::EsMatrix ToEfkMatorix(const D3DXMATRIX* pSrcMatDx);
	D3DXMATRIX ToDxMatorix(const EsMatrix* pSrcMatEfk);
/// <summary>
/// 制御系
/// </summary>
	//再生
	static ::EsHandle Play(const D3DXVECTOR3& pos) {
		Effect* pE = Effect::GetInstance();
		return pE->m_pManager->Play(pE->m_pEffect, pos.x, pos.y, pos.z);
	}
	//停止
	static void Stop(::EsHandle handle) {
		Effect::GetInstance()->m_pManager->StopEffect(handle);
		
	}
	//全てのエフェクトを停止
	static void StopAll() {
		Effect::GetInstance()->m_pManager->StopAllEffects();
	}

	//一時停止
	static void Paused(::EsHandle handle, bool bPaused) {
		Effect::GetInstance()->m_pManager->SetPaused(handle, bPaused);
	}
	//再生速度の設定
	static void SetSpeed(::EsHandle handle, float speed) {
		Effect::GetInstance()->m_pManager->SetSpeed(handle, speed);
	}
	//位置を指定する
	static void SetLocation(::EsHandle handle, const D3DXVECTOR3& pos) {
		Effect::GetInstance()->m_pManager->SetLocation(handle, pos.x, pos.y, pos.z);
	}
	//回転を指定する
	static void SetRotation(::EsHandle handle, const D3DXVECTOR3& rot) {
		 Effect::GetInstance()->m_pManager->SetRotation(handle, rot.x, rot.y, rot.z);
	}
	//回転を指定する（軸回転）
	static void SetRotation(::EsHandle handle, const D3DXVECTOR3& axis, float angle) {
		Effect::GetInstance()->m_pManager->SetRotation(handle, ::EsVec3(axis.x, axis.y, axis.z), angle);
	}
	//サイズを指定する
	void SetScale(::EsHandle handle, const D3DXVECTOR3& scale) {
		Effect::GetInstance()->m_pManager->SetScale(handle, scale.x, scale.y, scale.z);
	}


private:
	//開放
	HRESULT Releasdara();

	//ビュー行列の設定
	void SetViewMatrix(const D3DXMATRIX& mView);
	//プロジェクション行列の設定
	void SetProjectionMatrix(const D3DXMATRIX& mProj);


private:
	//エフェクトを動作させるのにひつよう
	::EsManagerRef	m_pManager;
	::EsRenderefRef m_pRender;
	//エフェクトの数だけ必要
	::EsEffecRef	m_pEffect;
};

