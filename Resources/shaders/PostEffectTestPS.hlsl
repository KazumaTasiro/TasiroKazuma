#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー



float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	float uPixel = 1.0f / 1280.0f;
	float vPixel = 1.0f / 720.0f;

	/*float4 texcolor = { 0,0,0,0 };*/

	float3 test = { 1.0f,1.0f,1.0f };
	colortex0 = float4(test - colortex0.rgb, 1);
	float4 color = colortex0;
	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		for (int i = 1; i < 7; i++) {
			for (int j = 1; j < 4; j++) {
				float2 uvv = { -uPixel * i,-vPixel * j };
				colortex1 += tex1.Sample(smp, input.uv + uvv);
				uvv = float2(0.0f, -vPixel * j);
				colortex1 += tex1.Sample(smp, input.uv + uvv);
				uvv = float2 (uPixel * i, -vPixel * j);
				colortex1 += tex1.Sample(smp, input.uv + uvv);

				uvv = float2(-uPixel * i, 0.0f);
				colortex1 += tex1.Sample(smp, input.uv + uvv);
				uvv = float2(0.0f, 0.0f);
				colortex1 += tex1.Sample(smp, input.uv + uvv);
				uvv = float2(uPixel * i, 0.0f);
				colortex1 += tex1.Sample(smp, input.uv + uvv);

				uvv = float2(-uPixel * i, vPixel * j);
				colortex1 += tex1.Sample(smp, input.uv + uvv);
				uvv = float2(0.0f, vPixel * j);
				colortex1 += tex1.Sample(smp, input.uv + uvv);
				uvv = float2(uPixel * i, vPixel * j);
				colortex1 += tex1.Sample(smp, input.uv + uvv);
			}
		}
		colortex1 = float4(colortex1.rgb / (28 * 9), colortex1.a);
		color = colortex1;
	}

	

	/*return float4(texcolor.rgb / (200 * 9), texcolor.a);*/



	
	return float4(color.rgb, 1);
}