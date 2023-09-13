#include "Particle.hlsli"
#include "Easing.hlsli"

struct GpuParticleElement
{
    float4 position;
    float scale;
    float4 color;
    uint isActive; // �����t���O.
    float lifeTime;
    float elapsed;
    uint colorIndex;
    float4 velocity;
};


RWStructuredBuffer<GpuParticleElement> gParticles : register(u0);
AppendStructuredBuffer<uint> gDeadIndexList : register(u1);

[numthreads(128, 1, 1)]
void initParticle(uint3 id : SV_DispatchThreadID)
{
    if (id.x < MaxParticleCount)
    {
        uint index = id.x;
        gParticles[index].isActive = 0;

        gDeadIndexList.Append(index);
    }
}

[numthreads(128, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint index = id.x;
    if (index >= MaxParticleCount)
    {
        return;
    }
    if (gParticles[index].isActive == 0)
    {
        return;
    }
    const float dt = 1;

    gParticles[index].lifeTime = gParticles[index].lifeTime - dt;
    if (gParticles[index].lifeTime <= 0)
    {
        gParticles[index].isActive = 0;
        gDeadIndexList.Append(index);
        return;
    }

  // �����c���Ă���p�[�e�B�N���𓮂���.
    float3 velocity = gParticles[index].velocity.xyz;
    float3 position = gParticles[index].position.xyz;

    float3 gravity = float3(0, -98.0, 0);
    position += velocity;
    float4 color = gParticles[index].color;
    color -= float4(0.02, 0.002, 0.002, 0.02);
    
    float scale = gParticles[index].scale;
    scale += 0.005;
    
    
    //velocity += gravity * dt;

    //if (position.y < 0)
    //{
    //    velocity.y *= -1;
    //    velocity *= 0.6;
    //    position.y = 0;

    //    if (length(velocity) < 0.001)
    //    {
    //        velocity = 0;
    //    }
    //}
#if 0
  // �t�H�[�X�̏���.
  float3 center = forceCenter1.xyz;
  float  radius = forceCenter1.w;

  float3 toCenter = center - position.xyz;
  float L = length(toCenter);
  if (L < radius) {
    float velocityLen = length(velocity);
    velocity = reflect(normalize(velocity), -normalize(toCenter));
    velocity *= velocityLen;

    position = radius * -normalize(toCenter) + center;
  }
#endif

    gParticles[index].position.xyz = position;
    //gParticles[index].velocity.xyz = velocity;
    gParticles[index].color = color;
    gParticles[index].scale = scale;
}


ConsumeStructuredBuffer<uint> gFreeIndexList : register(u1);


[numthreads(128, 1, 1)]
void emitParticle(uint3 id : SV_DispatchThreadID)
{
    if (gParticles[id.x].isActive > 0)
    {
        return;
    }

    uint index = gFreeIndexList.Consume();
    float a = index;
    
    uint seed = id.x * 1235;

    float3 velocity;
    
    float r = nextRand(seed) * 50;
    float theta = nextRand(seed) * 3.14192 * 2.0;
    
    float3 Position = float3(0, 0, 0);
    
    if (Down == 1)
    {
        velocity.x = (nextRand(seed) / 16) + (Look.x / 8);
        velocity.z = (nextRand(seed) / 16) + (Look.z / 8);
        velocity.y = (nextRand1(seed) / 16) - 0.1;
    }
    else
    {
        velocity.x = nextRand(seed) / 16 + (Look.x / 16);
        velocity.z = nextRand(seed) / 16 + (Look.z / 16);
        velocity.y = (nextRand1(seed) / 16) + 0.001;
    }

    gParticles[id.x].isActive = 1;
    gParticles[id.x].position.xyz = float3(StartPos.xyz);
    gParticles[id.x].scale = 0.1f;
    gParticles[id.x].velocity.xyz = velocity;
    gParticles[id.x].lifeTime = 50;
    gParticles[id.x].color = float4(0.7, 0.05, 0.05, 1);
    //gParticles[index].colorIndex = floor(nextRand(seed) * 8) % 8;;
}
