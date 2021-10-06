struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4 target0 : SV_TARGET0;
};

cbuffer meshBuffer : register(b2)
{
	float4 baseColor;
	float metalness;
	float specular;
	float roughness;
	float4 lightDir[3];
	float4 lightColor[3];
	float4 cameraPos;
}

static const float PI = 3.141592654f;

float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

//ディズニーのフレネル計算
float3 DisneyFresnel(float LdotH)
{
	//輝度
	float luminance = 0.3f * baseColor.r + 0.6f * baseColor.g + 0.1f * baseColor.b;
	//色合い
	float3 tintColor = baseColor.xyz / luminance;
	//非金属の鏡面反射率を計算
	float3 nonMetalColor = specular * 0.08f * tintColor;
	//metalnessによる色補間 金属の場合はベースカラー
	float3 specularColor = lerp(nonMetalColor, baseColor.xyz, metalness);

	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

float GeometricSmith(float cosine)
{
	float k = (roughness + 1.0f);
	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}

float3 CookTorrenceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	//D項
	float Ds = DistributionGGX(roughness * roughness, NdotH);

	//F項
	float3 Fs = DisneyFresnel(LdotH);

	//G項
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	//m項
	float m = 4.0f * NdotL * NdotV;

	//合成して鏡面反射の色を得る
	return Fs * Ds * Gs / m;
}

float3 BRDF(float3 L, float3 V, float3 N)
{
	//法線とライト方向の内積
	float NdotL = dot(N, L);
	//法線とカメラ方向の内積
	float NdotV = dot(N, V);

	//どちらかが90度以上であれば真っ黒を返す
	if (NdotL < 0 || NdotV < 0) { return float3(0, 0, 0); }

	//ライト方向とカメラ方向の中間(ハーフベクトル)
	float3 H = normalize(L + V);
	//法線とハーフベクトルの内積
	float NdotH = dot(N, H);
	//ライトとハーフベクトルの内積
	float LdotH = dot(L, H);

	//拡散反射率
	float diffuseReflectance = 1.0f / PI;

	float energyBias = 0.5f * roughness;

	//入射角が90度の場合の拡散反射率
	float Fd90 = energyBias + 2 * LdotH * LdotH * roughness;
	//入っていく時の拡散反射率
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	//出ていくときの拡散反射率
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
	//入って出ていく時の拡散反射率
	float Fd = FL * FV * energyFactor;

	//拡散反射項
	float3 diffuseColor = diffuseReflectance * Fd * baseColor.xyz * (1 - metalness);

	float3 specularColor = CookTorrenceSpecular(NdotL, NdotV, NdotH, LdotH);

	return diffuseColor + specularColor;
}

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

PS_OUT main(PS_IN input) : SV_TARGET
{
	PS_OUT result;
	
	float3 eyeDir = normalize(cameraPos.xyz - input.worldPos.xyz);

	float4 color = float4(0, 0, 0, 1);
	for (int i = 0; i < 3; i++)
	{
		color.xyz += BRDF(lightDir[i].xyz, eyeDir, input.normal) * lightColor[i];
	}

	result.target0 = color;

	return result;
}