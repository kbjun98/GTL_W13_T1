
Texture2D Texture : register(t0);
Texture2D CompositeResultTexture : register(t1);

SamplerState Sampler : register(s1); // Linear Clamp

cbuffer QuadTransform : register(b0)
{
    float2 QuadScale;
    float2 QuadOffset;
}

struct PS_Input
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

PS_Input mainVS(uint VertexID : SV_VertexID)
{
    PS_Input Output;

    float2 QuadPositions[4] = {
        float2(-1, -1),
        float2( 1, -1),
        float2(-1,  1),
        float2( 1,  1)
    };

    float2 UVs[4] = {
        float2(0, 1),
        float2(1, 1),
        float2(0, 0),
        float2(1, 0)
    };

    uint Indices[6] = {
        0, 2, 1,
        1, 2, 3
    };

    uint Index = Indices[VertexID];
    float2 QuadPosition = QuadPositions[Index];
    QuadPosition = QuadPosition * QuadScale + QuadOffset;
    Output.Position = float4(QuadPosition, 0, 1);
    Output.UV = UVs[Index];

    return Output;
}

float4 mainPS(PS_Input Input) : SV_TARGET
{
    float4 Base = CompositeResultTexture.Sample(Sampler, Input.UV);
    float4 Overlay = Texture.Sample(Sampler, Input.UV);
    return lerp(Base, Overlay, Overlay.a);
}
