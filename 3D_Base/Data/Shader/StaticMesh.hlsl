/*********************************************************************
*	スタティックメッシュクラス用シェーダファイル.
**/
//グローバル変数.
//テクスチャは レジスタ t(n).
Texture2D g_Texture : register(t0);
Texture2D g_Texture2 : register(t1);
//サンプラは レジスタ s(n).
SamplerState g_SamLinear : register(s0);

//コンスタントバッファ.
//メッシュ単位.
cbuffer per_mesh : register(b0)
{
    matrix g_mW; //ワールド行列.
    matrix g_mWVP; //ワールド,ビュー,プロジェクションの合成行列.
};
//マテリアル単位.
cbuffer per_material : register(b1)
{
    float4 g_Diffuse; //ディフューズ色(拡散反射色).
    float4 g_Ambient; //アンビエント色(環境色).
    float4 g_Specular; //スペキュラ色(鏡面反射色).
};
//フレーム単位.
cbuffer per_frame : register(b2)
{
    float4 g_CameraPos; //カメラ位置(視点位置).
    float4 g_vLightDir; //ライトの方向ベクトル.
};

//頂点シェーダの出力パラメータ.
struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float3 Normal : TEXCOORD0;
    float2 UV : TEXCOORD1;
    float4 Light : TEXCOORD2;
    float3 EyeVector : TEXCOORD3;
    float4 PosWorld : TEXCOORD4;
    float4 Color : COLOR;
};

//-------------------------------------------------
//	頂点(バーテックス)シェーダ.
//-------------------------------------------------
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float4 Norm : NORMAL,
	float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//プロジェクション変換(ワールド,ビュー,プロジェクション).
    output.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる.
	// (モデルが回転すれば法線も回転させる必要があるため).
    output.Normal = mul(Norm, (float3x3) g_mW);
    output.Normal = normalize(output.Normal);

	//ライト方向:
	// ディレクショナルライトは、どこでも同じ方向.位置は無関係.
    output.Light.xyz = normalize(g_vLightDir.xyz);
    output.Light.w = g_vLightDir.w; // w（強さ）をそのままピクセルシェーダへ送る
    output.PosWorld = mul(Pos, g_mW);

	//視線ベクトル:
	// ワールド空間上での頂点から頂点へ向かうベクトル.
    output.EyeVector = normalize(g_CameraPos - output.PosWorld);

	//テクスチャ座標.
    output.UV = UV;

    return output;
}

//-------------------------------------------------
//	ピクセルシェーダ.
//-------------------------------------------------
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	//テクスチャカラー.
    float4 texColor = g_Texture.Sample(g_SamLinear, input.UV);

	//環境光　※１.
    float4 ambient = texColor * g_Ambient;
    
	//拡散反射光 ※２.
    
     // メインの光
    float NL1 = saturate(dot(input.Normal, input.Light.xyz));
    // 裏側からの光（ライトの方向を反転させる）
    float NL2 = saturate(dot(input.Normal, -input.Light.xyz));
    
    // メインの光に、裏からの光をうっすら（例えば0.3倍して）足す
    float NL = NL1 + NL2 * 0.5;
    float4 diffuse = (g_Diffuse / 2 + texColor / 2) * NL;

	//鏡面反射光 ※３.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light.xyz);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//最終色　※１，２，３の合計.
    float4 Color = ambient + (diffuse + specular) * input.Light.w;
   // Color.a = 0.5;
    return Color;
}

//========= テクスチャ無し用 ========================================
//-------------------------------------------------
//	頂点(バーテックス)シェーダ.
//-------------------------------------------------
VS_OUTPUT VS_NoTex(
	float4 Pos : POSITION,
	float4 Norm : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//プロジェクション変換(ワールド,ビュー,プロジェクション).
    output.Pos = mul(Pos, g_mWVP);

	//法線をモデルの姿勢に合わせる.
	// (モデルが回転すれば法線も回転させる必要があるため).
    output.Normal = mul(Norm, (float3x3) g_mW);
    output.Normal = normalize(output.Normal);

	//ライト方向:
	// ディレクショナルライトは、どこでも同じ方向.位置は無関係.
    output.Light = normalize(g_vLightDir);

    output.PosWorld = mul(Pos, g_mW);

	//視線ベクトル:
	// ワールド空間上での頂点から頂点へ向かうベクトル.
    output.EyeVector = normalize(g_CameraPos - output.PosWorld);


    return output;
}

//-------------------------------------------------
//	ピクセルシェーダ.
//-------------------------------------------------
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//テクスチャカラー.
	//環境光　※１.
    float4 ambient = g_Ambient;

	//拡散反射光 ※２.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse = g_Diffuse * NL;

	//鏡面反射光 ※３.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_Specular;

	//最終色　※１，２，３の合計.
    float4 Color = ambient + diffuse + specular;
    return Color;
}

//========= 輪郭線(アウトライン)用 ==================================
// 背面法(反転ハル法)で輪郭を描く.
//  モデルを法線方向に少し膨らませ、裏面だけ(表面はカリングで捨てる)を黒で塗る.
//  膨らんだ分だけ元のモデルの外側にはみ出し、その部分が輪郭線に見える.
//  カリング設定(CULL_FRONT)は C++ 側 CStaticObjMesh で行う.

//  輪郭の色は テクスチャ2(t1) の画像を使う.
//  画像は画面に向けて貼り、モデルの中心が画像の中心(UV 0.5,0.5)に来るようにする.
//  画像の大きさはモデルの大きさに合わせるので、カメラが近づけば画像も大きく映る.

//輪郭線用のコンスタントバッファ.
// C++ 側 CStaticObjMesh::CBUFFER_PER_OUTLINE と並びを一致させる.
cbuffer per_outline : register(b3)
{
    float  g_OutlineWidth; //輪郭の太さ(モデル空間での押し出し量).
    float  g_TexSize;      //画像1枚分の大きさ(ビュー空間での長さ). モデルの直径×スケールが入る.
    float2 g_ProjScale;    //射影行列の _11 と _22. 画面上のズレをビュー空間の長さに戻すのに使う.
    float4 g_ModelCenter;  //モデルの中心(モデル空間). xyz のみ使用.
};

//輪郭用 頂点シェーダの出力.
struct VS_OUTLINE_OUTPUT
{
    float4 Pos        : SV_Position;
    float4 ClipPos    : TEXCOORD0; //この頂点の射影後の座標(ピクセルシェーダで画面上の位置を求めるため).
    float4 CenterClip : TEXCOORD1; //モデル中心の射影後の座標(全頂点で同じ値).
};

//-------------------------------------------------
//	輪郭用 頂点シェーダ.
//	引数 Pos  : 頂点座標(モデル空間).
//	引数 Norm : 輪郭用の法線(同じ位置の頂点の法線を平均したもの).
//	            面ごとの法線で押し出すと、角で面がバラバラに離れて輪郭が途切れるため、平均した法線を使う.
//	戻り値    : 射影後の座標と、画像を貼るための情報.
//-------------------------------------------------
VS_OUTLINE_OUTPUT VS_Outline(
	float4 Pos : POSITION,
	float3 Norm : NORMAL)
{
    VS_OUTLINE_OUTPUT output = (VS_OUTLINE_OUTPUT) 0;

	//法線方向に頂点を押し出して、元のモデルより一回り大きくする.
    Pos.xyz += normalize(Norm) * g_OutlineWidth;

	//プロジェクション変換(ワールド,ビュー,プロジェクション).
    output.Pos = mul(Pos, g_mWVP);
    output.ClipPos = output.Pos;

	//モデル中心も同じ行列で射影しておく. 画像の中心をここに合わせる.
    output.CenterClip = mul(float4(g_ModelCenter.xyz, 1.0f), g_mWVP);

    return output;
}

//-------------------------------------------------
//	輪郭用 ピクセルシェーダ.
//	ライティングはせず、テクスチャ2の色をそのまま出す.
//	画像が読み込めていない場合は t1 が空になり、黒(0,0,0,0)が返る.
//	戻り値 : 輪郭の色.
//-------------------------------------------------
// 色相(0〜1) → 虹色のRGB. h が 1 を超えても frac で自動的にループする.
float3 HueToRGB(float h)
{
    return saturate(abs(frac(h + float3(1.0f, 2.0f / 3.0f, 1.0f / 3.0f)) * 6.0f - 3.0f) - 1.0f);
}

float4 PS_Outline(VS_OUTLINE_OUTPUT input) : SV_Target
{
	//画面上の位置(-1～1 の正規化座標)を求める.
	// 頂点シェーダで割り算すると補間がゆがむため、ピクセルごとに w で割る.
    float2 ndc    = input.ClipPos.xy / input.ClipPos.w;
    float2 center = input.CenterClip.xy / input.CenterClip.w;


    
	//モデル中心からのズレを、モデル中心の奥行きでのビュー空間の長さに戻す.
	// 正規化座標は「ビュー座標 × 射影スケール ÷ 奥行き(w)」なので、逆に w を掛けて射影スケールで割る.
	// こうすると画像の大きさがモデルと同じ割合で拡大縮小し、画面の縦横比でゆがむこともない.
    float2 offset = (ndc - center) * input.CenterClip.w / g_ProjScale;

	//ズレを UV に変換する. 中心が 0.5、画像1枚分(g_TexSize)で 0～1 になる.
	// 画面の上方向は +y、テクスチャの V は下方向が + なので y を反転する.
    float2 uv = float2(offset.x, -offset.y) / g_TexSize + 0.5f;

    
    
    return g_Texture2.Sample(g_SamLinear, uv);
}
