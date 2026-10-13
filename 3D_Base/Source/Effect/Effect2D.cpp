#include "Effect2D.h"

//描画スプライト最大数；
constexpr int RENDER_SPRITE_MAX = 8000;
//エフェクト管理用スプライトのインスタンス最大数
constexpr int EFFECT_INSTANS_MAX = 1000;

Effect2D::Effect2D()
	:m_pManager(nullptr)
	, m_pRender(nullptr)
	, m_pEffect()
{
}
Effect2D::~Effect2D()
{
	//エフェクトデータの開放
	Releasdara();
	//エフェクトマネージャーの破棄
	m_pManager.Reset();
	//エフェクトレンダラ-を破棄
	m_pRender.Reset();
}

HRESULT Effect2D::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	//エフェクトのマネージャーの作成
	m_pManager = ::EsManager::Create(RENDER_SPRITE_MAX);
	//エフェクトのレンダラーの作成
	m_pRender = ::EsRenderef::Create(pDevice, pContext, 1000);

	//描画モジュールの作成
	m_pManager->SetSpriteRenderer(m_pRender->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRender->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRender->CreateRingRenderer());
	m_pManager->SetTrackRenderer(m_pRender->CreateTrackRenderer());
	m_pManager->SetModelRenderer(m_pRender->CreateModelRenderer());

	//テクスチャー、モデルのマテリアルローダーの設定をする
	//ユーザーが独自に拡張できる、現在はファイルから読み込んでいる
	m_pManager->SetTextureLoader(m_pRender->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRender->CreateModelLoader());
	m_pManager->SetMaterialLoader(m_pRender->CreateMaterialLoader());
	m_pManager->SetCurveLoader(::Es::MakeRefPtr<::Es::CurveLoader>());

	return S_OK;
}

HRESULT Effect2D::LoadData()
{
	struct EffectList
	{
		enList listNo;				//enList列挙型を設定.
		const char16_t path[256];	//ファイルの名前(パス付き).
	};
	EffectList EList[] =
	{
		{ enList::NAMI,	u"Data\\Effekseer\\波.efk"	},
		{ enList::ORA1,	u"Data\\Effekseer\\O-RA1.efk"	},
		{ enList::KANSEI,	u"Data\\Effekseer\\完成.efk"	},
	{ enList::KIRAN,	u"Data\\Effekseer\\KIRAN.efk"	},
	{ enList::lightning,	u"Data\\Effekseer\\lightning.efk"	},
		
	
	};
	//配列の最大要素数を算出(配列全体のサイズ／配列１つ分のサイズ).
	int list_max = sizeof(EList) / sizeof(EList[0]);
	for (int i = 0; i < list_max; i++) {

		int listNo = EList[i].listNo;

		//エフェクトの読み込み.
		m_pEffect[listNo] = ::EsEffect::Create(m_pManager, EList[i].path);
		//u""は、UTF-16エンコーディングの文字列リテラルで、const char16_t* に代入可能.

		if (m_pEffect[listNo] == nullptr) {
			_ASSERT_EXPR(false, L"エフェクト読み込み失敗");
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT Effect2D::Releasdara()
{
	return S_OK;
}

void Effect2D::SetViewMatrix(const D3DXMATRIX& mView)
{

	::EsMatrix EsCamMat;//カメラ行列
	EsCamMat = ToEfkMatrix(&mView);

	//カメラ行列を設定する
	m_pRender->SetCameraMatrix(EsCamMat);

}

void Effect2D::SetProjectionMatrix(const D3DXMATRIX& mProj)
{
	::EsMatrix EsProjMat;//プロジェクション行列
	EsProjMat = ToEfkMatrix(&mProj);
	//プロジェクション行列を設定する
	m_pRender->SetProjectionMatrix(EsProjMat);
}

void Effect2D::Draw( const LIGHT& lLight, const CAMERA& camera)
{

	// ビュー行列: 単位行列(カメラは原点で+Z方向を見る)
	D3DXMATRIX mView;
	D3DXMatrixIdentity(&mView);

	// 正射影行列: 画面ピクセル座標に合わせる
	// 左上原点・Y下向きにしたい場合は top と bottom を逆にする
	D3DXMATRIX mProj;
	D3DXMatrixOrthoOffCenterLH(&mProj,
		0.0f, WND_W,      // left, right
		WND_H, 0.0f,      // bottom, top (Y下向き)
		-1000.0f, 1000.0f); // near, far (エフェクトのZ範囲が収まるように広めに)

	SetViewMatrix(mView);
	SetProjectionMatrix(mProj);

	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = ::EsVec3(0.0f, 0.0f, -1.0f);
	m_pManager->SetLayerParameter(0, layerParameter);

	m_pManager->Update();

	m_pRender->BeginRendering();
	m_pManager->Draw();
	m_pRender->EndRendering();
}

::EsVec3 Effect2D::ToEfkVector3(const D3DXVECTOR3* pSrcVec3Dx)
{
	return ::EsVec3(pSrcVec3Dx->x, pSrcVec3Dx->y, pSrcVec3Dx->z);
}

D3DXVECTOR3 Effect2D::ToDxVector3(const EsVec3* pSrcVec3efk)
{
	return D3DXVECTOR3(pSrcVec3efk->X, pSrcVec3efk->Y, pSrcVec3efk->Z);
}

::EsMatrix Effect2D::ToEfkMatrix(const D3DXMATRIX* pSrcMatDx)
{
	::EsMatrix OutMat;

	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 4;j++) {
			OutMat.Values[i][j] = pSrcMatDx->m[i][j];
		}
	}
#if 0 

	OutMat.Values[0][0] = pSrcMatDx->m[0][0];
OutMat.Values[0][1] = pSrcMatDx->m[0][1];
OutMat.Values[0][2] = pSrcMatDx->m[0][2];
OutMat.Values[0][3] = pSrcMatDx->m[0][3];

OutMat.Values[1][0] = pSrcMatDx->m[1][0];
OutMat.Values[1][1] = pSrcMatDx->m[1][1];
OutMat.Values[1][2] = pSrcMatDx->m[1][2];
OutMat.Values[1][3] = pSrcMatDx->m[1][3];

OutMat.Values[2][0] = pSrcMatDx->m[2][0];
OutMat.Values[2][1] = pSrcMatDx->m[2][1];
OutMat.Values[2][2] = pSrcMatDx->m[2][2];
OutMat.Values[2][3] = pSrcMatDx->m[2][3];

OutMat.Values[3][0] = pSrcMatDx->m[3][0];
OutMat.Values[3][1] = pSrcMatDx->m[3][1];
OutMat.Values[3][2] = pSrcMatDx->m[3][2];
OutMat.Values[3][3] = pSrcMatDx->m[3][3];

#endif  



	return OutMat;
}

D3DXMATRIX Effect2D::ToDxMatrix(const EsMatrix* pSrcMatEfk)
{
	D3DXMATRIX OutMat;


	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 4;j++) {
			OutMat.m[i][j] = pSrcMatEfk->Values[i][j];
		}
	}



	return OutMat;
}


