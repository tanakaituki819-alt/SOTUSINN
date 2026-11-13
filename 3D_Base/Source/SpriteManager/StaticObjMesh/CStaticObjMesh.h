/**************************************************
*	CStaticObjMesh.h
*	.obj ファイルを読み込み、DirectX11 で描画するスタティックメッシュクラス.
*	背面法(反転ハル法)による黒い輪郭線の描画にも対応している.
*	輪郭を付ける前の元の版は CStaticObjMeshOld に残してある.
**/
#pragma once
#include <vector>
#include <string>




//警告についてのコード分析を無効にする. 4005:再定義.
#pragma warning( disable:4005)

//_declspec()	:DLLから(関数,クラス,クラスのメンバ関数を)エクスポートする.
//align()		:(強制的に)16byte で使用する.
#define ALIGN16	_declspec( align(16) )

//前方宣言.
class CDirectX9;
class CDirectX11;



/**************************************************
*	スタティックメッシュクラス	
*	メッシュデータをファイルから取り出す為だけにDirectX9を使用する.
*	レンダリング(描画)は、DirectX11で行う.
**/
class CStaticObjMesh
{
public:
	//======================================
	//	構造体.
	//======================================
	//コンスタントバッファのアプリ側の定義.
	//※シェーダ内のコンスタントバッファと一致している必要あり.
	//メッシュ単位で渡す情報.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;			//ワールド行列.
		D3DXMATRIX	mWVP;		//ワールド,ビュー,プロジェクションの合成変換行列.	
	};
	//マテリアル単位で渡す情報.
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4	Diffuse;	//ディフューズ色(拡散反射色).
		D3DXVECTOR4 Ambient;	//アンビエント色(環境色).
		D3DXVECTOR4	Specular;	//スペキュラ色(鏡面反射色).
	};
	//フレーム単位で渡す情報.
	// カメラ,ライト情報はここに入れる.
	struct CBUFFER_PER_FRAME
	{
		D3DXVECTOR4	CameraPos;		//カメラ位置(視点位置).
		D3DXVECTOR4	vLightDir;		//ライト方向.
	};
	//輪郭線用に渡す情報.
	// シェーダ側 per_outline(b3) と一致させる. コンスタントバッファは16バイト単位なので詰め物を入れる.
	struct CBUFFER_PER_OUTLINE
	{
		float		Width;			//輪郭の太さ(モデル空間での押し出し量).
		float		TexSize;		//テクスチャ2の画像1枚分の大きさ(ビュー空間での長さ).
		float		ProjScale[2];	//射影行列の _11 と _22(画面上のズレをビュー空間の長さに戻すため).
		D3DXVECTOR4	ModelCenter;	//モデルの中心(モデル空間). 画像の中心をここに合わせる.
	};

	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
		D3DXVECTOR3	Normal;	//法線(陰影計算に必須).
		D3DXVECTOR2	UV;		//テクスチャ座標.
	};
	//マテリアル構造体.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Diffuse;	//ディフューズ.
		D3DXVECTOR4	Ambient;	//アンビエント.
		D3DXVECTOR4	Specular;	//スペキュラ.
		D3DXVECTOR4 Emissive;	//エミッシブ.
		float		Power;		//スペキュラパワー.
		TCHAR TextureName[64];	//テクスチャファイル名.
		ID3D11ShaderResourceView* pTexture;//テクスチャ.
		DWORD dwNumFace;		//マテリアルのポリゴン数.

		//コンストラクタ.
		MY_MATERIAL()
			: Diffuse		()
			, Ambient		()
			, Specular		()
			, Emissive		()
			, Power			()
			, TextureName	()
			, pTexture		( nullptr )
			, dwNumFace		( 0 )
		{
		}
		//デストラクタ.
		~MY_MATERIAL() {
			SAFE_RELEASE( pTexture );
		}
	};

	//Xファイルのロードで必要。受け取るもの.
	struct MY_MODEL
	{
		TCHAR			FileName[64];		//ファイル名.
		std::vector<VERTEX> Vertices;		// 全頂点データ
		std::vector<std::vector<DWORD>> MaterialIndices; // マテリアル毎のインデックス配列
		ID3DXMesh* m_pMeshForRay;//レイとの当たり判定用
		DWORD			NumMaterials;		//マテリアル数.

		MY_MODEL()
			: FileName			()
			, NumMaterials		()
		{}
		~MY_MODEL(){
		}
	};

public:
	CStaticObjMesh();		//コンストラクタ.
	~CStaticObjMesh();		//デストラクタ.

	HRESULT Init( CDirectX9& pDx9, CDirectX11& pDx11, LPCTSTR lpFileName );

	//解放関数.
	void Release();

	//レンダリング用.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		LIGHT& Light, D3DXVECTOR3& CamPos);

	//座標情報を設定.
	void SetPosition( const D3DXVECTOR3& Pos ) { m_Position = Pos; }
	//座標xを設定.
	void SetPositionX( float x ){ m_Position.x = x; }
	//座標yを設定.
	void SetPositionY( float y ){ m_Position.y = y; }
	//座標zを設定.
	void SetPositionZ( float z ){ m_Position.z = z; }

	//回転情報を設定.
	void SetRotation( const D3DXVECTOR3& Rot ) { m_Rotation = Rot;	}
	//回転軸Yを設定(Yaw).
	void SetRotationY( float y ){ m_Rotation.y = y; }
	//回転軸Xを設定(Pitch).
	void SetRotationX( float x ){ m_Rotation.x = x; }
	//回転軸Zを設定(Roll).
	void SetRotationZ( float z ){ m_Rotation.z = z; }

	//拡縮情報を設定.
	void SetScale( const D3DXVECTOR3& Scale ) {	m_Scale = Scale;	}
	void SetScale( float scale ){
		m_Scale.x = scale;
		m_Scale.y = scale;
		m_Scale.z = scale;
	}

	//メッシュを取得.
	// 【変更後】頂点配列を取得.
	const std::vector<VERTEX>& GetMesh() const { return m_Model.Vertices; }
	// 【変更後】レイとの当たり判定用の頂点配列を取得.
	//const std::vector<VERTEX>& GetMeshForRay() const { return m_ModelForRay.Vertices; }
	 ID3DXMesh* GetMeshForRay() const { return m_ModelForRay.m_pMeshForRay; }

	void SetisCOLOR(bool is) { isCOLOR = is; };

	void SetLightCOLOR(D3DXVECTOR3 COLORE) { 
		LightCOLOR = COLORE; };
	void SetLightCOLOR256(D3DXVECTOR3 COLORE);

	//輪郭線を描くかどうかを設定する.
	// 引数 enable : true で輪郭あり、false で輪郭なし(元の描画と同じ見た目).
	void SetEnableOutline( bool enable ) { m_EnableOutline = enable; }
	//輪郭線の太さを設定する.
	// 引数 width : モデル空間での押し出し量. SetScale の拡大率が掛かるので、モデルの大きさに合わせて調整する.
	void SetOutlineWidth( float width ) { m_OutlineWidth = width; }
	//輪郭に貼るテクスチャ2の画像の大きさの倍率を設定する.
	// 引数 scale : 1.0 で画像1枚がモデルの直径と同じ大きさ. 大きくすると画像が拡大される.
	void SetOutlineTexScale( float scale ) { m_OutlineTexScale = scale; }
	//使用前に、ポジションスケールローテーションを設定すること//ちなみにｚには奥行きが入っている
	D3DXVECTOR3 GetPos2D( D3DXMATRIX& mView, D3DXMATRIX& mProj);




	bool CreateMeshForRay();
private:
	//メッシュ読み込み.

	bool LoadObjToMyStructures(const std::string& filename, const std::string& baseDir);
	//シェーダ作成.
	HRESULT CreateShader();
	//モデル作成.
	HRESULT CreateModel();
	//マテリアル作成.
	/*HRESULT CreateMaterials();*/
	//インデックスバッファ作成.
	HRESULT CreateIndexBuffer();
	//頂点バッファ作成.
	HRESULT CreateVertexBuffer();
	//コンスタントバッファ作成.
	HRESULT CreateConstantBuffer();
	//サンプラ作成.
	HRESULT CreateSampler();

	//レンダリング関数(クラス内でのみ使用する).
	void RenderMesh( D3DXMATRIX& mWorld, D3DXMATRIX& mView, D3DXMATRIX& mProj );

	//輪郭線の描画に必要なもの(シェーダ,頂点バッファ等)を作成する.
	// 戻り値 : 成功で S_OK、失敗で E_FAIL.
	HRESULT CreateOutline();
	//輪郭線を描画する. RenderMesh の中から、メッシュ用コンスタントバッファ(b0)を設定した後に呼ぶ.
	// 引数 mProj : 射影行列. 画像の大きさを画面上で正しく合わせるのに使う.
	void RenderOutline( const D3DXMATRIX& mProj );


private:
	//Dx9.
	CDirectX9*				m_pDx9;
	LPDIRECT3DDEVICE9		m_pDevice9;	//Dx9デバイスオブジェクト.

	//Dx11.
	CDirectX11*				m_pDx11;
	ID3D11Device*			m_pDevice11;	//デバイスｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pContext11;	//デバイスコンテキスト.

	ID3D11VertexShader*		m_pVertexShader;	//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;		//ピクセルシェーダ.

	ID3D11Buffer*			m_pCBufferPerMesh;		//コンスタントバッファ(メッシュ毎).
	ID3D11Buffer*			m_pCBufferPerMaterial;	//コンスタントバッファ(マテリアル毎).
	ID3D11Buffer*			m_pCBufferPerFrame;		//コンスタントバッファ(フレーム毎).

	ID3D11Buffer*			m_pVertexBuffer;	//頂点バッファ.
	ID3D11Buffer**			m_ppIndexBuffer;	//インデックスバッファ.
	ID3D11SamplerState*		m_pSampleLinear;	//サンプラ:テクスチャに各種フィルタをかける.

	MY_MODEL		m_Model;
	MY_MODEL		m_ModelForRay;	//レイとの当たり判定で使用する.

	MY_MATERIAL*	m_pMaterials;	//マテリアル構造体.
	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_EnableTexture;//テクスチャあり.

	D3DXVECTOR3		m_Position;	//位置(x,y,z).
	D3DXVECTOR3		m_Rotation;	//回転値(x,y,z).
								//※x=Pitch, y=Yaw, z=Roll.
	D3DXVECTOR3		m_Scale;	//拡大縮小値(x,y,z等倍).

	bool isCOLOR;
	D3DXVECTOR3 LightCOLOR;

	//----- 輪郭線用 -----.
	ID3D11VertexShader*		m_pOutlineVS;			//輪郭用頂点シェーダ.
	ID3D11PixelShader*		m_pOutlinePS;			//輪郭用ピクセルシェーダ.
	ID3D11InputLayout*		m_pOutlineLayout;		//輪郭用頂点レイアウト(座標はスロット0, 平均法線はスロット1).
	ID3D11Buffer*			m_pCBufferOutline;		//輪郭用コンスタントバッファ.
	ID3D11Buffer*			m_pOutlineNormalBuffer;	//輪郭用の平均法線の頂点バッファ.
	ID3D11RasterizerState*	m_pRSCullFront;			//表面を捨てて裏面だけ描くラスタライザステート.
	std::vector<D3DXVECTOR3> m_OutlineNormals;		//輪郭用の平均法線(m_Model.Vertices と同じ並び).
	bool					m_EnableOutline;		//輪郭を描くか.
	float					m_OutlineWidth;			//輪郭の太さ.
	float					m_OutlineTexScale;		//輪郭に貼る画像の大きさの倍率.
	D3DXVECTOR3				m_ModelCenter;			//モデルのバウンディングボックスの中心(モデル空間).
	float					m_ModelRadius;			//モデルの中心から一番遠い角までの距離(モデル空間).
	ID3D11SamplerState*		m_pSampleClamp;			//輪郭用サンプラ. 画像の外側は端の色を伸ばす(繰り返さない).
	void Multiply(D3DXVECTOR4* base, D3DXVECTOR4 multiplier);

	ID3D11ShaderResourceView* m_pTexture2;			//テクスチャ2. 輪郭に貼る画像.
	//テクスチャ2(輪郭に貼る画像)を読み込む. 失敗しても止めずに続行し、輪郭は黒になる.
	void CreateTexture();
 };
