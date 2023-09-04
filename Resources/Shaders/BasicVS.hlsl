#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION,float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mat, pos);
    output.uv = uv;
    output.normal = normal;
    return output;
}