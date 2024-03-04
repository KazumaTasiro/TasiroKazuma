#include "Particle.hlsli"
//VSOutput main(float4 pos : POSITION)
VSOutput main(float4 pos : POSITION, float scale : TEXCOORD, float4 colors : COLOR)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = pos;
	output.scale = scale;
    output.color = colors;
	return output;
}