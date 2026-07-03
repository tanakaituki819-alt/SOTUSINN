
#ifndef NOMINMAX
#define NOMINMAX // min/maxマクロを無効化
#endif
#define TINYOBJLOADER_IMPLEMENTATION


//  標準C/C++ライブラリのインクルード
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <stdlib.h>	 // マルチバイト文字→Unicode文字変換で必要
#include <locale.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
// ライブラリ本体のインクルード（ここで安全にマクロが展開される）
#include "tiny_obj_loader.h"

#include "CStaticObjMesh.h"
#include "CDirectX9.h"
#include "CDirectX11.h"

#include <stdlib.h>	//マルチバイト文字→Unicode文字変換で必要.
#include <locale.h>


//constexpr 関数 'fast_float::loop_parse_if_eight_digits' を定数式にすることはできません
// が出たらC++17にすると治る
//シェーダファイル名(ディレクトリも含む).
const TCHAR SHADER_NAME[] = _T( "Data\\Shader\\StaticMesh.hlsl" );

//コンストラクタ.
CStaticObjMesh::CStaticObjMesh()
	: m_pDx9(nullptr)
	, m_pDevice9(nullptr)

	, m_pDx11(nullptr)
	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)

	, m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pPixelShader(nullptr)

	, m_pCBufferPerMesh(nullptr)
	, m_pCBufferPerMaterial(nullptr)
	, m_pCBufferPerFrame(nullptr)

	, m_pVertexBuffer(nullptr)
	, m_ppIndexBuffer(nullptr)
	, m_pSampleLinear(nullptr)

	, m_Model()
	, m_ModelForRay()

	, m_pMaterials(nullptr)
	, m_NumAttr()
	, m_AttrID()
	, m_EnableTexture(false)

	, m_Position()
	, m_Rotation()
	, m_Scale(1.0f, 1.0f, 1.0f)
	, isCOLOR(false)
	, LightCOLOR()
{
}

//デストラクタ.
CStaticObjMesh::~CStaticObjMesh()
{
	//解放処理.
	Release();
}

//初期化関数.
HRESULT CStaticObjMesh::Init(
	CDirectX9& pDx9, CDirectX11& pDx11, LPCTSTR lpFileName )
{
	//.obj


	m_pDx11 = &pDx11;
	m_pDevice11 = m_pDx11->GetDevice();
	m_pContext11 = m_pDx11->GetContext();
	// LPCTSTR (TCHAR) から std::string への変換処理
	_tcscpy_s(m_Model.FileName, lpFileName);
	_tcscpy_s(m_ModelForRay.FileName, lpFileName);

	// 1. まずTCHAR(Unicode)の状態で後ろから「\」や「/」を探す
	std::wstring wFilename(lpFileName);
	std::wstring wBaseDir = L"";
	size_t lastSlash = wFilename.find_last_of(L"\\/");
	if (lastSlash != std::wstring::npos) {
		wBaseDir = wFilename.substr(0, lastSlash + 1);
	}

	// 2. 安全に std::string (マルチバイト) へ変換する
	std::string filename(wFilename.begin(), wFilename.end());
	std::string baseDir(wBaseDir.begin(), wBaseDir.end());
	//ファイル読み込み.
	// 
// LoadXMesh の代わりに新設した .obj 用パーサー関数を呼び出す
	if (FAILED(LoadObjToMyStructures(filename, baseDir)))
	{
		return E_FAIL;
	}
	//モデル作成.
	if( FAILED( CreateModel() ) )
	{
		return E_FAIL;
	}
	//シェーダ作成.
	if( FAILED(CreateShader() ))
	{
		return E_FAIL;
	}
	//コンスタントバッファ作成.
	if( FAILED( CreateConstantBuffer() ) )
	{
		return E_FAIL;
	}
	//サンプラ作成.
	if( FAILED( CreateSampler() ) )
	{
		return E_FAIL;
	}

	return S_OK;
}




bool CStaticObjMesh::LoadObjToMyStructures(const std::string& filename, const std::string& baseDir)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	// 1. .obj と .mtl の読み込み
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), baseDir.c_str());
	if (!ret) return false;

	// マテリアル数をカウント
	m_Model.NumMaterials = static_cast<DWORD>(materials.size());
	if (m_Model.NumMaterials == 0)
	{
		m_Model.NumMaterials = 1;
	}; // マテリアルがない場合の安全対策

	// 属性数（マテリアルグループ数）を確定させる
	m_NumAttr = m_Model.NumMaterials;

	// 描画順の最適化用テーブル m_AttrID を初期化する（300個上限の安全策）
	for (DWORD i = 0; i < m_NumAttr && i < 300; i++) {
		m_AttrID[i] = i;
	}

	// 動的確保されていたマテリアル配列の初期化
	SAFE_DELETE_ARRAY(m_pMaterials);
	m_pMaterials = new MY_MATERIAL[m_Model.NumMaterials](); // ()を付けて確実にゼロクリア

	// マテリアル情報の詰め替え
	if (!materials.empty()) {
		for (size_t i = 0; i < materials.size(); i++) {
			m_pMaterials[i].Diffuse = D3DXVECTOR4(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2], 1.0f);
			m_pMaterials[i].Ambient = D3DXVECTOR4(materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2], 1.0f);
			m_pMaterials[i].Specular = D3DXVECTOR4(materials[i].specular[0], materials[i].specular[1], materials[i].specular[2], 1.0f);
			m_pMaterials[i].Emissive = D3DXVECTOR4(materials[i].emission[0], materials[i].emission[1], materials[i].emission[2], 1.0f);
			m_pMaterials[i].Power = materials[i].shininess;

			// テクスチャ名のコピー
#ifdef _UNICODE
			size_t converted = 0;
			mbstowcs_s(&converted, m_pMaterials[i].TextureName, materials[i].diffuse_texname.c_str(), _TRUNCATE);
#else
			strcpy_s(m_pMaterials[i].TextureName, materials[i].diffuse_texname.c_str());
#endif
			m_pMaterials[i].dwNumFace = 0;
			m_pMaterials[i].pTexture = nullptr;

			// 🔴 【最重要追加】マテリアルにテクスチャ名が書かれていたら、画像をロードする処理
			if (!materials[i].diffuse_texname.empty()) {
				m_EnableTexture = true; // クラス全体のテクスチャ有効フラグを立てる

				// フォルダパスと画像ファイル名を結合 (例: "Data\\Model\\" + "saibashi_texture.png")
				std::string fullTexPathAnsii = baseDir + materials[i].diffuse_texname;

				TCHAR fullTexPath[256] = _T("");
#ifdef _UNICODE
				size_t texConverted = 0;
				mbstowcs_s(&texConverted, fullTexPath, fullTexPathAnsii.c_str(), _TRUNCATE);
#else
				strcpy_s(fullTexPath, fullTexPathAnsii.c_str());
#endif

				// 実際の画像ファイルを読み込んで DirectX11 用のテクスチャリソース(SRV)を作成
				HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
					m_pDevice11, fullTexPath, nullptr, nullptr, &m_pMaterials[i].pTexture, nullptr);

				if (FAILED(hr)) {
					// 画像が見つからない・読み込めない場合は警告ログを出し、白マテリアルとして続行
					_RPTF1(_CRT_WARN, "テクスチャのロード失敗: %s\n", fullTexPathAnsii.c_str());
					m_pMaterials[i].pTexture = nullptr;
				}
			}
		}
	}
	else {
		// マテリアルがない場合はデフォルトの白を設定
		m_pMaterials[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pMaterials[0].Ambient = D3DXVECTOR4(0.4f, 0.4f, 0.4f, 1.0f); // 陰の部分が真っ黒にならないようにする
		m_pMaterials[0].Specular = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		m_pMaterials[0].Emissive = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		m_pMaterials[0].Power = 0.0f;
		m_pMaterials[0].dwNumFace = 0;
		m_pMaterials[0].pTexture = nullptr;
	}

	// 各マテリアルごとのインデックス配列を準備
	m_Model.MaterialIndices.resize(m_Model.NumMaterials);

	// 2. 頂点データとマテリアル毎のインデックスの構築
	m_Model.Vertices.clear();
	DWORD vertexCounter = 0;

	for (const auto& shape : shapes) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shape.mesh.num_face_vertices[f]);

			int matID = shape.mesh.material_ids[f];
			if (matID < 0 || matID >= (int)m_Model.NumMaterials) matID = 0;

			m_pMaterials[matID].dwNumFace++; // 各マテリアルが持つポリゴン数をカウント

			// 三角形の3頂点を処理
			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
				VERTEX vertex;

				// 座標
				vertex.Pos.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				vertex.Pos.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				vertex.Pos.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				// 法線
				if (idx.normal_index >= 0) {
					vertex.Normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
					vertex.Normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
					vertex.Normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}
				else {
					vertex.Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				}

				// UV
				if (idx.texcoord_index >= 0) {
					vertex.UV.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					vertex.UV.y = 1.0f - attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]; // DirectX用にV反転
				}
				else {
					vertex.UV = D3DXVECTOR2(0.0f, 0.0f);
				}

				m_Model.Vertices.push_back(vertex);
				m_Model.MaterialIndices[matID].push_back(vertexCounter);
				vertexCounter++;
			}
			index_offset += fv;
		}
	}

	// 3. レイ判定用のモデルにもデータを丸ごとコピー
	m_ModelForRay.Vertices = m_Model.Vertices;
	m_ModelForRay.NumMaterials = m_Model.NumMaterials;

	return true;
}

//モデル作成.
HRESULT CStaticObjMesh::CreateModel()
{
	////マテリアル作成.
	//if( FAILED( CreateMaterials() ) )
	//{
	//	return E_FAIL;
	//}
	//インデックスバッファ作成.
	if( FAILED( CreateIndexBuffer() ) )
	{
		return E_FAIL;
	}
	//頂点バッファ作成.
	if( FAILED( CreateVertexBuffer() ) )
	{
		return E_FAIL;
	}
	return S_OK;
}

//マテリアル作成.



//インデックスバッファ作成.
HRESULT CStaticObjMesh::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC	bd;
	D3D11_SUBRESOURCE_DATA	InitData;

	m_ppIndexBuffer = new ID3D11Buffer * [m_Model.NumMaterials]();
	for (DWORD No = 0; No < m_Model.NumMaterials; No++) {
		m_ppIndexBuffer[No] = nullptr;
	}

	
	for (DWORD No = 0; No < m_Model.NumMaterials; No++)
	{
		//そのマテリアルIDがどのポリゴンにも割り当てられていなければスキップ
		if (m_Model.MaterialIndices[No].empty()) continue;

		bd.Usage = D3D11_USAGE_DEFAULT;
		// vectorの要素数から正確なバイト幅を算出
		bd.ByteWidth = sizeof(DWORD) * static_cast<UINT>(m_Model.MaterialIndices[No].size());
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//vector の内部バッファの先頭ポインタ(.data())をそのままハードウェアに転送
		InitData.pSysMem = m_Model.MaterialIndices[No].data();

		if (FAILED(m_pDevice11->CreateBuffer(&bd, &InitData, &m_ppIndexBuffer[No])))
		{
			_ASSERT_EXPR(false, _T("インデックスバッファ作成失敗"));
			return E_FAIL;
		}
	}

	return S_OK;
}


//頂点バッファ作成.
HRESULT CStaticObjMesh::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC	bd;
	D3D11_SUBRESOURCE_DATA	InitData;

	//std::vector からダイレクトに作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	//VERTEXのサイズ × 配列の要素数
	bd.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(m_Model.Vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	//vectorの内部ポインタからデータをダイレクトにグラフィックスカードへ送信
	InitData.pSysMem = m_Model.Vertices.data();

	if (FAILED(m_pDevice11->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		_ASSERT_EXPR(false, _T("頂点バッファ作成失敗"));
		return E_FAIL;
	}

	return S_OK;
}

//サンプラ作成.
HRESULT CStaticObjMesh::CreateSampler()
{
	//テクスチャ用のサンプラ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//リニアフィルタ(線形補間).
						//POINT:高速だが粗い.
	samDesc.AddressU
		= D3D11_TEXTURE_ADDRESS_WRAP;//ラッピングモード(WRAP:繰り返し).
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//MIRROR: 反転繰り返し.
	//CLAMP : 端の模様を引き伸ばす.
	//BORDER: 別途境界色を決める.

	//サンプラ作成.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ) ) )//(out)サンプラ.
	{
		_ASSERT_EXPR( false, _T( "サンプラ作成失敗" ) );
		return E_FAIL;
	}

	return S_OK;
}

//解放関数.
void CStaticObjMesh::Release()
{
	//インデックスバッファ解放.	
	if (m_ppIndexBuffer != nullptr) {
		for (int No = (int)m_Model.NumMaterials - 1; No >= 0; No--) {
			if (m_ppIndexBuffer[No] != nullptr) { SAFE_RELEASE(m_ppIndexBuffer[No]); }
		}
		delete[] m_ppIndexBuffer; m_ppIndexBuffer = nullptr;
	}
	//マテリアル解放.
	if (m_pMaterials != nullptr) { delete[] m_pMaterials; m_pMaterials = nullptr; }

	
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexBuffer);

	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pCBufferPerFrame );
	SAFE_RELEASE( m_pCBufferPerMaterial );
	SAFE_RELEASE( m_pCBufferPerMesh );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_pDx11 = nullptr;

	m_pDevice9 = nullptr;
	m_pDx9 = nullptr;
}

//===========================================================
//	HLSLファイルを読み込みシェーダを作成する.
//	HLSL: High Level Shading Language の略.
//===========================================================
HRESULT CStaticObjMesh::CreateShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからバーテックスシェーダのブロブを作成.
	if( m_EnableTexture == true ){
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, nullptr, nullptr, "VS_Main", "vs_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr )))
		{
			_ASSERT_EXPR( false, _T( "hlsl読み込み失敗" ) );
			return E_FAIL;
		}
	}
	else {
		if( FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, nullptr, nullptr, "VS_NoTex", "vs_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr )))
		{
			_ASSERT_EXPR( false, _T( "hlsl読み込み失敗" ) );
			return E_FAIL;
		}
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader )))	//(out)バーテックスシェーダ.
	{
		_ASSERT_EXPR( false, _T( "バーテックスシェーダ作成失敗" ) );
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = 0;
	if( m_EnableTexture == true )
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		numElements = sizeof( tmp ) / sizeof( tmp[0] );	//要素数算出.
		memcpy_s( layout, sizeof( layout ),
			tmp, sizeof( D3D11_INPUT_ELEMENT_DESC ) * numElements );
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		numElements = sizeof( tmp ) / sizeof( tmp[0] );	//要素数算出.
		memcpy_s( layout, sizeof( layout ),
			tmp, sizeof( D3D11_INPUT_ELEMENT_DESC ) * numElements );
	}

	//頂点インプットレイアウトを作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)頂点インプットレイアウト.
	{
		_ASSERT_EXPR(false, _T( "頂点インプットレイアウト作成失敗" ));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからピクセルシェーダのブロブを作成.
	if( m_EnableTexture == true ){
		if( FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, nullptr, nullptr, "PS_Main", "ps_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr )))
		{
			_ASSERT_EXPR(false, _T( "hlsl読み込み失敗" ));
			return E_FAIL;
		}
	}
	else {
		if( FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, nullptr, nullptr, "PS_NoTex", "ps_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr )))
		{
			_ASSERT_EXPR(false, _T( "hlsl読み込み失敗" ));
			return E_FAIL;
		}
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if( FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader )))	//(out)ピクセルシェーダ.
	{
		_ASSERT_EXPR( false, _T( "ピクセルシェーダ作成失敗" ) );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	return S_OK;
}

//コンスタントバッファ作成.
HRESULT CStaticObjMesh::CreateConstantBuffer()
{
	//コンスタントバッファ(メッシュ用).
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//コンスタントバッファを指定.
	cb.ByteWidth = sizeof( CBUFFER_PER_MESH );	//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//書き込みでアクセス.
	cb.MiscFlags = 0;				//その他のフラグ(未使用).
	cb.StructureByteStride = 0;		//構造体のサイズ(未使用).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

	//コンスタントバッファの作成.
	if( FAILED(
		m_pDevice11->CreateBuffer( &cb, nullptr, &m_pCBufferPerMesh ) ) )
	{
		_ASSERT_EXPR( false, _T( "コンスタントバッファ(メッシュ)作成失敗" ) );
		return E_FAIL;
	}

	//コンスタントバッファ(マテリアル用).
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//コンスタントバッファを指定.
	cb.ByteWidth = sizeof( CBUFFER_PER_MATERIAL );	//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//書き込みでアクセス.
	cb.MiscFlags = 0;				//その他のフラグ(未使用).
	cb.StructureByteStride = 0;		//構造体のサイズ(未使用).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

	//コンスタントバッファの作成.
	if( FAILED(
		m_pDevice11->CreateBuffer( &cb, nullptr, &m_pCBufferPerMaterial ) ) )
	{
		_ASSERT_EXPR( false, _T( "コンスタントバッファ(マテリアル用)作成失敗" ) );
		return E_FAIL;
	}

	//コンスタントバッファ(ﾌﾚｰﾑ用).
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//コンスタントバッファを指定.
	cb.ByteWidth = sizeof( CBUFFER_PER_FRAME );	//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//書き込みでアクセス.
	cb.MiscFlags = 0;				//その他のフラグ(未使用).
	cb.StructureByteStride = 0;		//構造体のサイズ(未使用).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

	//コンスタントバッファの作成.
	if( FAILED(
		m_pDevice11->CreateBuffer( &cb, nullptr, &m_pCBufferPerFrame ) ) )
	{
		_ASSERT_EXPR( false, _T( "コンスタントバッファ(フレーム用)作成失敗" ) );
		return E_FAIL;
	}

	return S_OK;
}

//レンダリング用.
//※DirectX内のレンダリング関数.
//  最終的に画面に出力するのは別クラスのレンダリング関数がやる.
void CStaticObjMesh::Render(
	D3DXMATRIX& mView, D3DXMATRIX& mProj,
	LIGHT& Light, D3DXVECTOR3& CamPos)
{
	//ワールド行列、スケール行列、回転行列、平行移動行列.
	D3DXMATRIX mWorld, mScale, mRot, mTran;
	D3DXMATRIX mYaw, mPitch, mRoll;

	//拡大縮小行列作成.
	D3DXMatrixScaling(
		&mScale,	//(out)計算結果.
		m_Scale.x, m_Scale.y, m_Scale.z);	//x,y,zそれぞれの拡縮値.
	//Y軸回転行列作成.
	D3DXMatrixRotationY( &mYaw, m_Rotation.y);
	//X軸回転行列作成.
	D3DXMatrixRotationX( &mPitch, m_Rotation.x);
	//Z軸回転行列作成.
	D3DXMatrixRotationZ( &mRoll, m_Rotation.z);
	//平行移動行列作成.
	D3DXMatrixTranslation(
		&mTran,	//(out)計算結果.
		m_Position.x, m_Position.y, m_Position.z);	//x,y,z座標.

	//回転行列を作成.
	mRot = mYaw * mPitch * mRoll;

	//ワールド行列作成.
	//拡縮×回転×移動 ※順番がとても大切！！.
	mWorld = mScale * mRot * mTran;


	//使用するシェーダのセット.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	//頂点シェーダ.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	//ピクセルシェーダ.

	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	//バッファ内のデータの書き換え開始時にMap.
	if (SUCCEEDED(m_pContext11->Map(
		m_pCBufferPerFrame, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		//コンスタントバッファ(フレーム用).
		CBUFFER_PER_FRAME cb;

		//カメラ位置.
		cb.CameraPos = D3DXVECTOR4( CamPos.x, CamPos.y, CamPos.z, 0.0f );

		//----- ライト情報 -----.
		//ライト方向.
		cb.vLightDir = D3DXVECTOR4(
			Light.vDirection.x, Light.vDirection.y, Light.vDirection.z, 0.0f );
		//ライト方向の正規化(ノーマライズ）.
		// ※モデルからライトへ向かう方向. ディレクショナルライトで重要な要素.
		D3DXVec4Normalize( &cb.vLightDir, &cb.vLightDir );
		cb.vLightDir.w = Light.fIntensity;

		
		memcpy_s(
			pData.pData,	//コピー先のバッファ.
			pData.RowPitch,	//コピー先のバッファサイズ.
			(void*)( &cb ),	//コピー元のバッファ.
			sizeof( cb ) );	//コピー元のバッファサイズ.

		//バッファ内のデータの書き換え終了時にUnmap.
		m_pContext11->Unmap( m_pCBufferPerFrame, 0 );
	}

	//このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers(	2, 1, &m_pCBufferPerFrame );	//頂点シェーダ.
	m_pContext11->PSSetConstantBuffers(	2, 1, &m_pCBufferPerFrame );	//ピクセルシェーダ.


	//メッシュのレンダリング.
	RenderMesh( mWorld, mView, mProj );
}

//レンダリング関数(クラス内でのみ使用する).
void CStaticObjMesh::RenderMesh(
	D3DXMATRIX& mWorld, D3DXMATRIX& mView, D3DXMATRIX& mProj)
{
	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	//バッファ内のデータの書き換え開始時にMap.
	if( SUCCEEDED( m_pContext11->Map(
		m_pCBufferPerMesh, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData )))
	{
		//コンスタントバッファ(メッシュ用).
		CBUFFER_PER_MESH cb;

		//ワールド行列を渡す.
		cb.mW = mWorld;
		D3DXMatrixTranspose( &cb.mW, &cb.mW );

		//ワールド,ビュー,プロジェクション行列を渡す.
		D3DXMATRIX mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose( &mWVP, &mWVP );	//行列を転置する.
		//※行列の計算方法がDirectXとGPUで異なるため転置が必要.
		cb.mWVP = mWVP;

		memcpy_s(
			pData.pData,	//コピー先のバッファ.
			pData.RowPitch,	//コピー先のバッファサイズ.
			(void*)(&cb),	//コピー元のバッファ.
			sizeof(cb));	//コピー元のバッファサイズ.

		//バッファ内のデータの書き換え終了時にUnmap.
		m_pContext11->Unmap( m_pCBufferPerMesh, 0 );
	}

	//このコンスタントバッファをどのシェーダで使用するか？.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pCBufferPerMesh );	//頂点シェーダ.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pCBufferPerMesh );	//ピクセルシェーダ.

	//頂点インプットレイアウトをセット.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	//プリミティブ・トポロジーをセット.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// 頂点バッファをセット.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	for (DWORD No = 0; No < m_NumAttr; No++)
	{
		// 1. 描画対象のマテリアルIDを確定させる
		DWORD matID = m_AttrID[No];

		// 2. そのマテリアルIDが割り当てられたポリゴンがなければ安全にスキップ
		if (m_Model.MaterialIndices[matID].empty()) { continue; }

		// 3. インデックスバッファが正しく生成されているか安全チェック（念のため）
		if (m_ppIndexBuffer[matID] == nullptr) { continue; }

		// インデックスバッファをセット（matID を使用するように修正）
		m_pContext11->IASetIndexBuffer(m_ppIndexBuffer[matID], DXGI_FORMAT_R32_UINT, 0);

		// マテリアルの各要素をシェーダに渡す.
		D3D11_MAPPED_SUBRESOURCE pDataMat;
		if (SUCCEEDED(
			m_pContext11->Map(m_pCBufferPerMaterial,
				0, D3D11_MAP_WRITE_DISCARD, 0, &pDataMat)))
		{
			CBUFFER_PER_MATERIAL cb;
			// データを matID で一貫して参照
			cb.Diffuse = m_pMaterials[matID].Diffuse;
			if (isCOLOR) {
				Multiply(&cb.Diffuse, &LightCOLOR);
			}

			cb.Ambient = m_pMaterials[matID].Ambient;
			cb.Specular = m_pMaterials[matID].Specular;

			memcpy_s(pDataMat.pData, pDataMat.RowPitch, (void*)&cb, sizeof(cb));
			m_pContext11->Unmap(m_pCBufferPerMaterial, 0);
		}

		m_pContext11->VSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);
		m_pContext11->PSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);

		// テクスチャをシェーダに渡す.
		if (m_pMaterials[matID].pTexture != nullptr) {
			m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pContext11->PSSetShaderResources(0, 1, &m_pMaterials[matID].pTexture);
		}
		else {
			ID3D11ShaderResourceView* pNothing[1] = { 0 };
			m_pContext11->PSSetShaderResources(0, 1, pNothing);
		}

		// 描画するインデックス数を正確に取得（matID を使用）
		UINT indexCount = static_cast<UINT>(m_Model.MaterialIndices[matID].size());

		// 描画実行
		m_pContext11->DrawIndexed(indexCount, 0, 0);
	}
}

void CStaticObjMesh::Multiply(D3DXVECTOR4* base, D3DXVECTOR3* multiplier)
{
	base->x *= multiplier->x;
	base->y *= multiplier->y;
	base->z *= multiplier->z;
	//base->w *= multiplier->w;
}

