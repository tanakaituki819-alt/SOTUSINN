/*********************************************************************
*	スプライト3Dクラス用シェーダファイル.
**/
//グローバル変数.
//テクスチャは レジスタ t(n).
Texture2D		g_Texture	: register( t0 );
Texture2D g_Texture2 : register(t1);
//サンプラは レジスタ s(n).
SamplerState	g_samLinear	: register( s0 );

//コンスタントバッファ.
//アプリ側と同じバッファサイズになっている必要がある.
cbuffer per_mesh : register( b0 )	//レジスタ番号.
{
    matrix g_mWVP : packoffset(c0); //ワールド,ビュー,プロジェクションの変換合成行列.
    float4 g_Color : packoffset(c4); //色（RGBA:xyzw）.
    float4 g_UV : packoffset(c5); //UV座標（xyしか使わない）.
    float4 Texture2UV : packoffset(c6);
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
	output.Pos = mul( Pos, g_mWVP );
	output.UV = UV;
	
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
//ピクセルシェーダ.
//画面上に評されるピクセル（ドット1つ分）の色を決定する.
float4 PS_Main2(VS_OUTPUT input) : SV_Target
{
    float2 UV = { input.UV.x + Texture2UV.x, input.UV.y + Texture2UV.y };
    float4 color = g_Texture.Sample(g_samLinear, input.UV); //色を返す.
    float4 color2 = g_Texture2.Sample(g_samLinear, UV); //色を返す.
    color.r *= g_Color.r;
    color.g *= g_Color.g;
    color.b *= g_Color.b;
    if (color.a > 0.1f)
    {
        color.rgb *= color2.rgb;

    }


	//プログラム制御のα値をテクスチャが持っているα値にかけ合わせる.
    color.a *= g_Color.a;

    return color;
}