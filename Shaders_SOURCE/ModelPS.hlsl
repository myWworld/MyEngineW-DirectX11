#include "Samplers.hlsli"
#include "Textures.hlsli"

struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{

    float4 pos : SV_Position;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

float4 main(VSOutput input) : SV_Target
{
    float4 color = albedo.Sample(postProcessingSampler, input.uv);
    return color;
}