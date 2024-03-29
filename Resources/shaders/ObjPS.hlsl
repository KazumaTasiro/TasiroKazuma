#include "Obj.hlsli"

Texture2D<float4>tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

//float4 main(VSOutput input) :SV_TARGET
//{
//	float3 light = normalize(float3(1,-1,1));	//右下奥　向きのライト
//	float light_diffuse = saturate(dot(-light, input.normal));
//	float3 shade_color;
//	shade_color = m_ambient;//アンビエント項
//	shade_color += m_diffuse * light_diffuse;//ディフューズ項
//	float4 texcolor = tex.Sample(smp, input.uv);
//	return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
//}
float4 main(VSOutput input) : sv_TARGET
{
	//テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
	//光沢度
    const float shininess = 4.0f;
	//頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//環境反射光
    float3 ambient = m_ambient;

	////シェーディングによる色
 //   float4 shadecolor = float4(ambientColor * ambient, m_alpha);
    
    // Lambert反射
    float3 light = normalize(float3(1, -1, 1)); // 右下奥　向きのライト
    float diffuse = saturate(dot(-light, input.normal));
    float brightness = diffuse + 0.3f;
    float4 shadecolor = float4(brightness * ambientColor.x, brightness * ambientColor.y, brightness * ambientColor.z, m_alpha);
	
	
   // //全て加算する

   // for (int i = 0; i < DIR_LIGHT_NUM; i++)
   // {
   //     if (dirLights[i].active)
   //     {
			////ライトに向かうベクトルと法線の内積
   //         float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			////反射光ベクトル
   //         float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			////拡散反射光
   //         float3 diffuse = dotlightnormal * m_diffuse;
			////鏡面反射光 
   //         float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			////全て加算する
   //         shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
   //     }
   // }
	//点光源
    for (int i = 0; i < POINTLIGHT_NUM; i++)
    {
        if (pointLights[i].active)
        {

			//ライトへのベクトル
            float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
            float d = length(lightv);
			//正規化し、単位ベクトルとする
            lightv = normalize(lightv);

			//距離減衰係数
            float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);

			//ライトに向かうベクトルと法線のない席
            float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
            float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			//すべて加算する
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;

        }
    }
	//スポットライト
    for (i = 0; i < SPOTLIGHT_NUM; i++)
    {
        if (spotLights[i].active)
        {
			//ライトへの方向ベクトル
            float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
            float d = length(lightv);
            lightv = normalize(lightv);
			//距離減衰係数
            float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
			//角度減衰
            float cos = dot(lightv, spotLights[i].lightv);
			//減衰開始角度から、減衰終了角度にかけて減衰
			//減衰開始角度の内側は1倍減衰終了角度の外側は0倍の光度
            float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			//角度減衰を乗算
            atten *= angleatten;

			//ライトに向かうベクトルと法線のない席
            float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
            float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//すべて加算する
            shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
        }
    }
	//丸影
    for (int i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        if (circleShadows[i].active)
        {
			//オブジェクト表面からキャスターへのベクトル
            float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			//東映方向での減衰
            float d = dot(casterv, circleShadows[i].dir);
			//距離減衰係数
            float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			//距離がminusなら0にする
            atten *= step(0, d);
			//仮想ライトの座標
            float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//オブジェクト表面からライトへのベクトル
            float3 lightv = normalize(lightpos - input.worldpos.xyz);
			//角度減衰
            float cos = dot(lightv, circleShadows[i].dir);
			//減衰開始角度から、減衰終了角度にかけて減衰
			//減衰開始角度の内側は1倍減衰終了角度の外側は0倍の光度
            float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			//角度減衰を乗算
            atten *= angleatten;

			//すべてを減算する
            shadecolor.rgb -= atten;
        }
    }

	//シェーディングによる色で描画
    return float4(shadecolor.rgb * texcolor.rgb, shadecolor.a * texcolor.a);
}