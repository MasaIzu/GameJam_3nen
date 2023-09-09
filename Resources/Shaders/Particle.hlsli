struct ParticleCount
{
    uint UseCount : ParticleUseCount;
    uint StartIndex : ParticleStartIndex;
    uint2 pad : Pad;
};

cbuffer ShaderParameters : register(b0)
{
    float4 StartPos : packoffset(c0);
    matrix mat : packoffset(c1); // 3D変換行列
    matrix matBillboard : packoffset(c5); //ビルボード行列
    uint MaxParticleCount : packoffset(c9);
    uint EmitterCount : packoffset(c9.y);
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 position : Position;
    float scale : Scale;
    float4 color : Color;
    float4 DownColor : DownColor;
    float4 velocity : Velocity;
    float3 FinalVelocity : FinalVelocity;
    int Frame : Frame; // このパーティクルが生まれたフレーム
    int MaxFrame : MaxFrame; //このパーティクルの寿命
    bool alive : Alive; // このパーティクルが生きているかどうか
    float MinusAlpha : MinusAlpha;
};

//ジオメトリシェーダからピクセルシェーダーへの出力
struct GSOutput
{
	float4 svpos:SV_POSITION;//システム用頂点座標
	//float3 normal:NORMAL;//法線ベクトル
	float2 uv:TEXCOORD;//uv値
	float4 color:COLOR;
};
