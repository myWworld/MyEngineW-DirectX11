#include "ConstantBuffers.hlsli"

struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    int4 boneIndices : BONEINDICES;
    float4 boneWeight : BONEWEIGHTS;
    float2 uv : TEXCOORD;
};

struct VSOutput
{

    float4 pos : SV_Position;
    float4 color : COLOR;
    float3 normal : NORMAL;
    int4 boneIndices : BONEINDICES;
    float4 boneWeight : BONEWEIGHTS;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0.0f;
    
    float4 skinPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; i++)
    {
        int boneIndex = input.boneIndices[i];
        float boneWeight = input.boneWeight[i];
        
        
        if (boneWeight > 0.0f)
        {
            skinPos += boneWeight * mul(float4(input.pos, 1.0f), BoneMatrices[boneIndex]);
        }
    }
    
    float4 pos = mul(skinPos, WorldMatrix);
    float4 viewPos = mul(pos, ViewMatrix);
    float4 projPos = mul(viewPos, ProjectionMatrix);
    
    output.pos = projPos;
    output.color = input.color;
    output.normal = input.normal;
    output.uv = input.uv;
    return output;
}