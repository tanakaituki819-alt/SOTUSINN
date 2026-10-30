/*********************************************************************
*	スタティックメッシュクラス用シェーダファイル.
**/
//グローバル変数.
//テクスチャは レジスタ t(n).
Texture2D g_Texture : register(t0);
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
    float LightPower; //ライトの強さ.
    float3 arr; //アライメント.
};
// ポイントライト1個分の構造体
struct PointLight
{
    float3 position; // ワールド位置
    float range; // 影響範囲
    float3 color; // ライトの色 (RGB)
    float intensity; // 強度
    float3 attenuation; // 減衰係数 (x: 定数項, y: 線形項, z: 2次項)
    float pad; // アライメント用
};
// C++側から送られる動的配列バッファ (t1レジスタにバインド)
StructuredBuffer<PointLight> g_PointLights : register(t1);

// ライトの個数を管理する定数バッファ (b3レジスタ等に追加)
cbuffer per_light_count : register(b3)
{
    uint g_PointLightCount; // 実際に描画するポイントライトの数
    float3 g_LightPad;
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
    float3 N = normalize(input.Normal);
    float3 viewDir = normalize(g_CameraPos.xyz - input.PosWorld.xyz);
    
    float3 totalPointDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 totalPointSpecular = float3(0.0f, 0.0f, 0.0f);

    // --- 1. 複数ポイントライトのループ処理 ---
    for (uint i = 0; i < g_PointLightCount; ++i)
    {
        PointLight light = g_PointLights[i];

        float3 lightVector = light.position - input.PosWorld.xyz;
        float distance = length(lightVector);

        // ライトの影響範囲外ならこのライトの計算をスキップ
        if (distance > light.range)
        {
            continue;
        }

        // 方向ベクトルの正規化
        float3 lightDir = lightVector / distance;

        // 距離による減衰の計算
        float att = 1.0f / (light.attenuation.x +
                            light.attenuation.y * distance +
                            light.attenuation.z * (distance * distance));

        // ランバート拡散反射 (Diffuse)
        float diffuseFactor = max(dot(N, lightDir), 0.0f);
        totalPointDiffuse += diffuseFactor * light.color * light.intensity * att;

        // 鏡面反射 (Specular) - フォン反射モデル
        float3 reflectDir = reflect(-lightDir, N);
        float specFactor = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
        totalPointSpecular += specFactor * light.color * light.intensity * att;
    }
    
// --- 2. 平行光源（ディレクショナルライト）と環境光の計算 ---
    float4 ambient = g_Ambient;

    // input.Light.xyz の方向ベクトルを再正規化
    float3 dirLightDir = normalize(input.Light.xyz);
    float NL = saturate(dot(N, dirLightDir));
    float4 diffuse = g_Diffuse * NL;

    float3 reflectVec = normalize(2.0f * NL * N - dirLightDir);
    float4 specular = pow(saturate(dot(reflectVec, input.EyeVector)), 4.0f) * g_Specular;


    // --- 3. 最終カラーの合成 ---
    // テクスチャなしのため、マテリアル色（g_Diffuse）ベースで合成
    float3 finalDiffuse = (totalPointDiffuse + diffuse.rgb * LightPower + ambient.rgb * LightPower) * texColor.rgb;
    float3 finalSpecular = totalPointSpecular + specular.rgb * LightPower;

    float3 finalColor = finalDiffuse + finalSpecular;

    return float4(finalColor, 1.0f);
    
    
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
    float4 texColor = { 1, 1, 1, 1 };
    
    
    float3 N = normalize(input.Normal);
    float3 viewDir = normalize(g_CameraPos.xyz - input.PosWorld.xyz);
    
    float3 totalPointDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 totalPointSpecular = float3(0.0f, 0.0f, 0.0f);

    // --- 1. 複数ポイントライトのループ処理 ---
    for (uint i = 0; i < g_PointLightCount; ++i)
    {
        PointLight light = g_PointLights[i];

        float3 lightVector = light.position - input.PosWorld.xyz;
        float distance = length(lightVector);

        // ライトの影響範囲外ならこのライトの計算をスキップ
        if (distance > light.range)
        {
            continue;
        }

        // 方向ベクトルの正規化
        float3 lightDir = lightVector / distance;

        // 距離による減衰の計算
        float att = 1.0f / (light.attenuation.x +
                            light.attenuation.y * distance +
                            light.attenuation.z * (distance * distance));

        // ランバート拡散反射 (Diffuse)
        float diffuseFactor = max(dot(N, lightDir), 0.0f);
        totalPointDiffuse += diffuseFactor * light.color * light.intensity * att;

        // 鏡面反射 (Specular) - フォン反射モデル
        float3 reflectDir = reflect(-lightDir, N);
        float specFactor = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
        totalPointSpecular += specFactor * light.color * light.intensity * att;
    }
    
// --- 2. 平行光源（ディレクショナルライト）と環境光の計算 ---
    float4 ambient = g_Ambient;

    // input.Light.xyz の方向ベクトルを再正規化
    float3 dirLightDir = normalize(input.Light.xyz);
    float NL = saturate(dot(N, dirLightDir));
    float4 diffuse = g_Diffuse * NL;

    float3 reflectVec = normalize(2.0f * NL * N - dirLightDir);
    float4 specular = pow(saturate(dot(reflectVec, input.EyeVector)), 4.0f) * g_Specular;


    // --- 3. 最終カラーの合成 ---
    // テクスチャなしのため、マテリアル色（g_Diffuse）ベースで合成
    float3 finalDiffuse = (totalPointDiffuse + diffuse.rgb + ambient.rgb) * texColor.rgb;
    float3 finalSpecular = totalPointSpecular+ specular.rgb;

    float3 finalColor = finalDiffuse + finalSpecular;

    return float4(finalColor, 1.0f);
    

}
