#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float uPixel = 1.5f / 1280.0f;
	float vPixel = 1.5f / 720.0f;

	float4 texcolor = { 0,0,0,0 };

	for (int i = 1; i < 21; i++) {
		for (int j = 1; j < 11; j++) {
			float2 uvv = { -uPixel * i,-vPixel * j };
			texcolor += tex.Sample(smp, input.uv + uvv);
			uvv = float2(0.0f, -vPixel * j);
			texcolor += tex.Sample(smp, input.uv + uvv);
			uvv = float2 (uPixel * i, -vPixel * j);
			texcolor += tex.Sample(smp, input.uv + uvv);

			uvv = float2(-uPixel * i, 0.0f);
			texcolor += tex.Sample(smp, input.uv + uvv);
			uvv = float2(0.0f, 0.0f);
			texcolor += tex.Sample(smp, input.uv + uvv);
			uvv = float2(uPixel * i, 0.0f);
			texcolor += tex.Sample(smp, input.uv + uvv);

			uvv = float2(-uPixel * i, vPixel * j);
			texcolor += tex.Sample(smp, input.uv + uvv);
			uvv = float2(0.0f, vPixel * j);
			texcolor += tex.Sample(smp, input.uv + uvv);
			uvv = float2(uPixel * i, vPixel * j);
			texcolor += tex.Sample(smp, input.uv + uvv);
		}
	}
	/*float4 texcolor = tex.Sample(smp,input.uv);*/

	return float4(texcolor.rgb / (200 * 9), texcolor.a);
	/*return float4(texcolor.rgb,1);*/
	//float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);

	//float2 movePixel = 1.0f / texSize;

	//// ぼかし処理をかける
	//color = tex.Sample(smp, input.uv);  // 自分自身のピクセル
	//// 正方向(右、上)の隣のピクセル///ここでエラーが出る
	//color += tex.Sample(smp, input.uv + movePixel * 1.0f);
	//color += tex.Sample(smp, input.uv + movePixel * 2.0f);
	//color += tex.Sample(smp, input.uv + movePixel * 3.0f);
	//color += tex.Sample(smp, input.uv + movePixel * 4.0f);
	//// 負方向(左、下)の隣のピクセル
	//color += tex.Sample(smp, input.uv - movePixel * 1.0f);
	//color += tex.Sample(smp, input.uv - movePixel * 2.0f);
	//color += tex.Sample(smp, input.uv - movePixel * 3.0f);
	//color += tex.Sample(smp, input.uv - movePixel * 4.0f);
	//// 足し合わせたピクセルの平均を求める
	//color / 9.0f;

	//return color;
}