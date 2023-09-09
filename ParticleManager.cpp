#include "ParticleManager.h"
#include <ParticleCS.h>
#include <ParticleHandHanabi.h>

void ParticleManager::Initialize(ID3D12Device* device)
{
	ParticleCS::StaticInitialize(device);
	ParticleHandHanabi::StaticInitialize(device);
}

void ParticleManager::Finalize()
{
	ParticleHandHanabi::StaticFinalize();
	ParticleCS::StaticFinalize();
}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	ParticleCS::PreDraw(cmdList);
}

void ParticleManager::PostDraw()
{
	ParticleCS::PostDraw();
}
