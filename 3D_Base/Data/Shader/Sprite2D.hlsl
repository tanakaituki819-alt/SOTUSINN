/*********************************************************************
*	スプライト2Dクラス用シェーダファイル.
**/
//グローバル変数.
//テクスチャは レジスタ t(n).
Texture2D		g_Texture	: register( t0 );
//サンプラは レジスタ s(n).
SamplerState	g_samLinear	: register( s0 );

//コンスタントバッファ.
//アプリ側と同じバッファサイズになっている必要がある.
cbuffer per_mesh : register( b0 )	//レジスタ番号.
{
    matrix	g_mWorld	: packoffset(c0);	//ワールド行列.
	float4	g_Color		: packoffset(c4);	//色（RGBA:xyzw）.
	float4	g_UV		: packoffset(c5);	//UV座標（xyしか使わない）.
	float	g_ViewPortW	: packoffset(c6);	//ビューポート幅.
    float	g_ViewPortH	: packoffset(c7);	//ビューポート高さ.
    float   g_i : packoffset(c8); //
};

//頂点シェーダの出力パラメータ.
//頂点シェーダで制御した複数の値をピクセルシェーダ側に渡すために用意している.
struct VS_OUTPUT
{
	float4	Pos		: SV_Position;	//座標（SV_:System-Value Semantics）.
	float2	UV		: TEXCOORD0;	//UV座標.
};

//頂点シェーダ.
//主にモデルの頂点の表示位置を決定する.
VS_OUTPUT VS_Main(
	float4 Pos	: POSITION,
	float2 UV	: TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul( Pos, g_mWorld );
	
	//スクリーン座標に合わせる計算.
    output.Pos.x = (output.Pos.x / g_ViewPortW) * 2.f - 1.f;
    output.Pos.y = 1.f - (output.Pos.y / g_ViewPortH) * 2.f;	
	
    float2 margin = 0.00001; // 狭める割合（0.01 = 1%）
    output.UV = UV * (1.0 - margin * 2.0) + margin;
	
	//UVスクロール(UV座標を操作する).
    output.UV.x += g_UV.x;
    output.UV.y += g_UV.y;

	return output;
}

//ピクセルシェーダ.
//画面上に評されるピクセル（ドット1つ分）の色を決定する.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	
	float4 color = g_Texture.Sample( g_samLinear, input.UV );//色を返す.
    color.r *= g_Color.r;
    color.g *= g_Color.g;
    color.b *= g_Color.b;
	//プログラム制御のα値をテクスチャが持っているα値にかけ合わせる.
	color.a *= g_Color.a;

	return color;
}


float4 PS_Main2(VS_OUTPUT input) : SV_Target
{
	
	
    float4 color = g_Texture.Sample(g_samLinear, input.UV); //色を返す.
	
    float2 Senter = input.UV - (0.5, 0.5);
	// 円の外側を切り抜く場合（半径0.5の円）
    if (length(Senter) > 0.5)
    {
        discard; // 円の外側は描画しない
    }
    // 12時の方向（真上）から時計回りに進めたい場合の計算:
    float angle = atan2(Senter.x, -Senter.y);
	
	//角度を 0.0 ～ 1.0 の範囲に正規化 (0.0が真上、0.5が真下、1.0が一周)
    float normalizedAngle = (angle + 3.141592) / (2.0 * 3.141592);
	
    if (normalizedAngle >1- g_i)
    {
        discard; // 判定範囲外ならピクセルを棄却
    }
	
    color.r *= g_Color.r;
    color.g *= g_Color.g;
    color.b *= g_Color.b;
	//プログラム制御のα値をテクスチャが持っているα値にかけ合わせる.
    color.a *= g_Color.a;

    return color;
}