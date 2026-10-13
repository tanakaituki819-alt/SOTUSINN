#pragma once

//警告についてのコード分析を無効化する
#pragma warnig(disable :4005);


//Effekseer関連のヘッダー、ライブラリ
#include<Effekseer/include/Effekseer.h>
#include<Effekseer/include/EffekseerRendererDX11.h>


#ifdef _DEBUG
#pragma comment(lib,"Effekseerd.lib")
#pragma comment(lib,"EffekseerRendererDX11d.lib")
#else
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"EffekseerRendererDX11.lib")
#endif
//エイリアスを用意
//コードが長くなって読みずらいため
// 
//コードの下のほうで定義しているほかのコードではEFE::で使える
//namespace {
//	using EFE = Effect::enList;
//}


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

class Effect2D
{
public:
	//エフェクト種類列挙型
	enum enList
	{
		NAMI = 0,	//
		ORA1,			//
		KANSEI,			//
		KIRAN,//
		lightning,		//雷
		Max				//最大数
	};
public:

	~Effect2D();

public:

	//コピーコンストラクタによるコピーを禁止する
	Effect2D(const Effect2D& rhs) = delete;
	//代入演算子によるコピーを禁止する
	Effect2D& operator=(Effect2D& rhs) = delete;

	HRESULT Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//データ読み込み
	HRESULT LoadData();


	//描画
	void Draw( const LIGHT&lLight, const CAMERA&camera);

	//-------------------------------------.
	//	変換系.
	//-------------------------------------.
	//vec3.
	::EsVec3 ToEfkVector3(const D3DXVECTOR3* pSrcVec3Dx);
	D3DXVECTOR3 ToDxVector3(const ::EsVec3* pSrcVec3Efk);
	//matrix(行列)
	::EsMatrix ToEfkMatrix(const D3DXMATRIX* pSrcMatDx);
	D3DXMATRIX ToDxMatrix(const ::EsMatrix* pSrcMatEfk);
	//-------------------------------------.
	//	制御系.
	//-------------------------------------.
	//再生
	 ::EsHandle Play(enList list, const D3DXVECTOR3& pos) {
	
		return m_pManager->Play(m_pEffect[list], pos.x, pos.y, pos.z);
	}
	 ::EsHandle Play(enList list, const D3DXVECTOR3& pos,int32_t startFrame ) {
	
		EsVec3 i(pos.x, pos.y, pos.z);
		EsVec3& c = i;
		return m_pManager->Play(m_pEffect[list],c, startFrame);
	}
	//停止
	 void Stop(::EsHandle handle) {
		m_pManager->StopEffect(handle);
		
	}
	//全てのエフェクトを停止
	 void StopAll() {
		m_pManager->StopAllEffects();
	}

	//一時停止
	 void Paused(::EsHandle handle, bool bPaused) {
		m_pManager->SetPaused(handle, bPaused);
	}
	//再生速度の設定
	 void SetSpeed(::EsHandle handle, float speed) {
		m_pManager->SetSpeed(handle, speed);
	}
	//位置を指定する
	 void SetLocation(::EsHandle handle, const D3DXVECTOR3& pos) {
		m_pManager->SetLocation(handle, pos.x, pos.y, pos.z);
	}
	//回転を指定する
	 void SetRotation(::EsHandle handle, const D3DXVECTOR3& rot) {
		 m_pManager->SetRotation(handle, rot.x, rot.y, rot.z);
	}
	//回転を指定する（軸回転）
	 void SetRotation(::EsHandle handle, const D3DXVECTOR3& axis, float angle) {
		m_pManager->SetRotation(handle, ::EsVec3(axis.x, axis.y, axis.z), angle);
	}
	//サイズを指定する
	 void SetScale(::EsHandle handle, const D3DXVECTOR3& scale) {
		 m_pManager->SetScale(handle, scale.x, scale.y, scale.z);
	}


private:
	//開放
	HRESULT Releasdara();
	//生成やコピーを禁止する
	Effect2D();
	//ビュー行列の設定
	void SetViewMatrix(const D3DXMATRIX& mView);
	//プロジェクション行列の設定
	void SetProjectionMatrix(const D3DXMATRIX& mProj);


private:
	//エフェクトを動作させるのにひつよう
	::EsManagerRef	m_pManager;
	::EsRenderefRef m_pRender;
	//エフェクトの数だけ必要
	::EsEffecRef	m_pEffect[enList::Max];
};
namespace {
	using EFE = Effect2D::enList;
}