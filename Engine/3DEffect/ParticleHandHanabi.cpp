﻿#include "ParticleHandHanabi.h"
#include "DirectXCore.h"
#include "Model.h"
#include <algorithm>
#include <cassert>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include <d3d12.h>
#include "d3dx12.h"
#include <CreateResource.h>
#include <combaseapi.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* ParticleHandHanabi::device = nullptr;
ID3D12GraphicsCommandList* ParticleHandHanabi::cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleHandHanabi::rootsignature;
ComPtr<ID3D12RootSignature> ParticleHandHanabi::rootSignature;//コンピュートシェーダー用
ComPtr<ID3D12PipelineState> ParticleHandHanabi::pipelinestate;
ComPtr<ID3D12PipelineState> ParticleHandHanabi::pipelineState;//コンピュートシェーダー用

std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> ParticleHandHanabi::m_pipelines;
ComPtr<ID3D12DescriptorHeap> ParticleHandHanabi::m_cbvSrvUavHeap;

const std::string ParticleHandHanabi::PSO_DEFAULT = "PSO_DEFAULT";
const std::string ParticleHandHanabi::PSO_CS_INIT = "PSO_CS_INIT";
const std::string ParticleHandHanabi::PSO_CS_EMIT = "PSO_CS_EMIT";
const std::string ParticleHandHanabi::PSO_CS_UPDATE = "PSO_CS_UPDATE";
const std::string ParticleHandHanabi::PSO_DRAW_PARTICLE = "PSO_DRAW_PARTICLE";
const std::string ParticleHandHanabi::PSO_DRAW_PARTICLE_USE_TEX = "PSO_DRAW_PARTICLE_USE_TEX";

UINT ParticleHandHanabi::m_incrementSize;

UINT ParticleHandHanabi::m_cbvSrvUavDescriptorSize = 0;

void ParticleHandHanabi::StaticInitialize(ID3D12Device* device)
{
	// nullptrチェック
	assert(device);

	ParticleHandHanabi::device = device;

	m_cbvSrvUavDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// パイプライン初期化
	InitializeGraphicsPipeline();

}

void ParticleHandHanabi::StaticFinalize()
{

}

void ParticleHandHanabi::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ParticleHandHanabi::cmdList == nullptr);

	// コマンドリストをセット
	ParticleHandHanabi::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(m_pipelines[PSO_DEFAULT].Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleHandHanabi::PostDraw()
{
	// コマンドリストを解除
	ParticleHandHanabi::cmdList = nullptr;
}

void ParticleHandHanabi::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob; //ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	//gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true;//深度グリッピングを有効に
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	blenddesc.BlendEnable = true;//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う
	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う
	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う
	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない
	////半透明合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング




	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsUnorderedAccessView(0);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_pipelines[PSO_DEFAULT]));
	assert(SUCCEEDED(result));

	{
		// カウンタ付き UAV はルートパラメータとして設定できない.
		CD3DX12_DESCRIPTOR_RANGE uavIndexList{};
		uavIndexList.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
		// ルートシグネチャの作成
		std::array<CD3DX12_ROOT_PARAMETER, 3> rootParameters;
		rootParameters[0].InitAsConstantBufferView(0); // b0: Params
		rootParameters[1].InitAsUnorderedAccessView(0);// u0: Particles
		rootParameters[2].InitAsDescriptorTable(1, &uavIndexList); // u1: ParticleIndexList

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Init(
			UINT(rootParameters.size()), rootParameters.data(),
			1, &samplerDesc);

		ComPtr<ID3DBlob> signature, errBlob;
		D3D12SerializeRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errBlob);
		device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	}

	ComPtr<ID3DBlob> csBlobInit; //コンピュートシェーダオブジェクト
	ComPtr<ID3DBlob> csBlobEmit;
	ComPtr<ID3DBlob> csBlobUpdate;
	// コンピュートシェーダーのコンパイル
	D3DCompileFromFile(
		L"Resources/Shaders/ParticleHandHanabi.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"initParticle", "cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobInit,
		nullptr);
	D3DCompileFromFile(
		L"Resources/Shaders/ParticleHandHanabi.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"emitParticle", "cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobEmit,
		nullptr);
	D3DCompileFromFile(
		L"Resources/Shaders/ParticleHandHanabi.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&csBlobUpdate,
		nullptr);

	// PSOの作成
	ComPtr<ID3D12PipelineState> pipelineState;
	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = rootSignature.Get();

	//initialize用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobInit.Get());
	device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	m_pipelines[PSO_CS_INIT] = pipelineState;

	//emit用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobEmit.Get());
	device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	m_pipelines[PSO_CS_EMIT] = pipelineState;

	//update用
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(csBlobUpdate.Get());
	device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
	m_pipelines[PSO_CS_UPDATE] = pipelineState;

	const int MaxDescriptorCount = 2048; // SRV,CBV,UAV など.
	// SRV のディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	  MaxDescriptorCount,
	  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
	  0
	};
	device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&m_cbvSrvUavHeap));

	m_incrementSize = device->GetDescriptorHandleIncrementSize(cbvSrvUavHeapDesc.Type);
}

void ParticleHandHanabi::InitializeVerticeBuff()
{

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(GpuParticleElement)) * particleCount;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProps.CreationNodeMask = 1;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProps.VisibleNodeMask = 1;


	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;


	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(GpuParticleElement);

	auto cbDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(ShaderParameters));

	m_sceneParameterCB = MyFunction::CreateResource(cbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, D3D12_HEAP_TYPE_UPLOAD);

	UINT64 bufferSize;
	bufferSize = sizeof(GpuParticleElement) * particleCount;
	auto resDescParticleElement = CD3DX12_RESOURCE_DESC::Buffer(
		bufferSize,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS
	);
	m_gpuParticleElement = MyFunction::CreateResource(resDescParticleElement, D3D12_RESOURCE_STATE_COMMON, nullptr, D3D12_HEAP_TYPE_DEFAULT);
	m_gpuParticleElement->SetName(L"ParticleElement");

	bufferSize = sizeof(UINT) * particleCount;
	UINT uavCounterAlign = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT - 1;
	bufferSize = UINT64(bufferSize + uavCounterAlign) & ~static_cast<UINT64>(uavCounterAlign);
	bufferSize += sizeof(Vector4);   // カウンタをこの場所先頭に配置.

	auto resDescParticleIndexList = CD3DX12_RESOURCE_DESC::Buffer(
		bufferSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	m_gpuParticleIndexList = MyFunction::CreateResource(resDescParticleIndexList, D3D12_RESOURCE_STATE_COMMON, nullptr, D3D12_HEAP_TYPE_DEFAULT);
	m_gpuParticleIndexList->SetName(L"ParticleIndexList");

	UINT64 offsetToCounter = bufferSize - sizeof(Vector4);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.NumElements = particleCount;
	// インデックス用バッファの後方でカウンタを設置する.
	uavDesc.Buffer.CounterOffsetInBytes = offsetToCounter;
	uavDesc.Buffer.StructureByteStride = sizeof(UINT);


	m_handleCpu = m_cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart();
	m_handleGpu = m_cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();

	processedCommandsHandle_ = m_handleCpu;
	device->CreateUnorderedAccessView(
		m_gpuParticleIndexList.Get(),
		m_gpuParticleIndexList.Get(),
		&uavDesc, processedCommandsHandle_
	);

}

void ParticleHandHanabi::SetTextureHandle(uint32_t textureHandle) {
	textureHandle_ = textureHandle;
}

void ParticleHandHanabi::Initialize(uint32_t ParticleCount)
{
	HRESULT result;

	particleCount = ParticleCount;

	InitializeVerticeBuff();

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap));

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = particleCount;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.StructureByteStride = sizeof(GpuParticleElement);

	device->CreateUnorderedAccessView(
		vertBuff.Get(),
		nullptr,
		&uavDesc,
		descriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);


	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES UploadHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC UploadResourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(particleCount * sizeof(GpuParticleElement));

	// アップロードバッファの作成
	device->CreateCommittedResource(
		&UploadHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&UploadResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadBuffer)
	);

	time = 50;

	UINT invokeCount = particleCount / 128 + 1;
	MaxDispatchCount = invokeCount;
	DispatchCounter = MaxDispatchCount / time;

}

void ParticleHandHanabi::Update()
{

	// 死んでいるパーティクルを削除
	Particles.erase(std::remove_if(Particles.begin(), Particles.end(),
		[](const VertexPos& p) { return !p.alive; }), Particles.end());

	//// スケールが0以下になったものを削除
	//Particles.erase(std::remove_if(Particles.begin(), Particles.end(),
	//	[](const VertexPos& p) { return !p.scale; }), Particles.end());

}

void ParticleHandHanabi::Draw(const ViewProjection& view)
{
	HRESULT result;
	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	//constMap->mat = matWorld * matView * matProjection;	// 行列の合成
	Matrix4 constMatToSend = view.matView;
	constMatToSend *= view.matProjection;
	constMap->mat = constMatToSend;	// 行列の合成
	constMap->matBillboard = view.matBillboard;
	constMap->maxParticleCount = static_cast<UINT>(particleCount);
	constMap->particleCount = 1;
	constBuff->Unmap(0, nullptr);

	shaderParameters.mat = constMatToSend;
	shaderParameters.matBillboard = view.matBillboard;
	MyFunction::WriteToUploadHeapMemory(m_sceneParameterCB.Get(), sizeof(ShaderParameters), &shaderParameters);

	// nullptrチェック
	assert(device);
	assert(ParticleHandHanabi::cmdList);


	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//// インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, m_sceneParameterCB->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootUnorderedAccessView(1, m_gpuParticleElement->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(cmdList, 2, textureHandle_);
	// 描画コマンド
	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	cmdList->DrawInstanced(static_cast<UINT>(particleCount), 1, 0, 0);

}

void ParticleHandHanabi::CSUpdate(Vector4 StartPos, UINT Down, Vector4* Look)
{

	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvUavHeap.Get() };
	DirectXCore::GetInstance()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (time > 0) {
		time--;
		DispatchCount += 1;
	}
	else {
		DispatchCount = 1;
		time = 50;
	}

	//初期化
	if (m_frameCount == 0) {
		shaderParameters.maxParticleCount = particleCount;
		shaderParameters.particleCount = 0;
		shaderParameters.StartPos = StartPos;
		shaderParameters.Down = Down;

		MyFunction::WriteToUploadHeapMemory(m_sceneParameterCB.Get(), sizeof(ShaderParameters), &shaderParameters);

		CD3DX12_RESOURCE_BARRIER transitionBarrier[2];
		transitionBarrier[0] = CD3DX12_RESOURCE_BARRIER::Transition(m_gpuParticleElement.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		transitionBarrier[1] = CD3DX12_RESOURCE_BARRIER::Transition(m_gpuParticleIndexList.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		DirectXCore::GetInstance()->GetCommandList()->ResourceBarrier(2, transitionBarrier);

		UINT frameDescriptorOffset = 3;
		D3D12_GPU_DESCRIPTOR_HANDLE cbvSrvUavHandle = m_cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();
		// Particle の初期化コード.
		DirectXCore::GetInstance()->GetCommandList()->SetComputeRootSignature(rootSignature.Get());


		DirectXCore::GetInstance()->GetCommandList()->SetComputeRootConstantBufferView(0, m_sceneParameterCB->GetGPUVirtualAddress());
		DirectXCore::GetInstance()->GetCommandList()->SetComputeRootUnorderedAccessView(1, m_gpuParticleElement->GetGPUVirtualAddress());
		DirectXCore::GetInstance()->GetCommandList()->SetComputeRootDescriptorTable(2, m_handleGpu);
		DirectXCore::GetInstance()->GetCommandList()->SetPipelineState(m_pipelines[PSO_CS_INIT].Get());

		UINT invokeCount = particleCount / 128 + 1;
		DirectXCore::GetInstance()->GetCommandList()->Dispatch(invokeCount, 1, 1);
	}

	{
		shaderParameters.StartPos = StartPos;
		if (Look) {
			shaderParameters.Look = *Look;
		}

		MyFunction::WriteToUploadHeapMemory(m_sceneParameterCB.Get(), sizeof(ShaderParameters), &shaderParameters);
		// Particle の発生.
		UINT frameDescriptorOffset = 3;
		D3D12_GPU_DESCRIPTOR_HANDLE cbvSrvUavHandle = m_cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();

		DirectXCore::GetInstance()->GetCommandList()->SetComputeRootSignature(rootSignature.Get());
		DirectXCore::GetInstance()->GetCommandList()->SetComputeRootConstantBufferView(0, m_sceneParameterCB->GetGPUVirtualAddress());
		DirectXCore::GetInstance()->GetCommandList()->SetComputeRootUnorderedAccessView(1, m_gpuParticleElement->GetGPUVirtualAddress());
		DirectXCore::GetInstance()->GetCommandList()->SetComputeRootDescriptorTable(2, m_handleGpu);
		DirectXCore::GetInstance()->GetCommandList()->SetPipelineState(m_pipelines[PSO_CS_EMIT].Get());

		UINT invokeCount = particleCount / 128 + 1;
		if (DispatchCount > invokeCount) {
			DispatchCount = invokeCount;
		}
		if (DispatchCount <= 0) {
			DispatchCount = 1;
		}
		{
			DirectXCore::GetInstance()->GetCommandList()->Dispatch(DispatchCount, 1, 1);
		}

		CD3DX12_RESOURCE_BARRIER barriers[] = {
		  CD3DX12_RESOURCE_BARRIER::UAV(m_gpuParticleElement.Get()),
		  CD3DX12_RESOURCE_BARRIER::UAV(m_gpuParticleIndexList.Get()),
		};
		DirectXCore::GetInstance()->GetCommandList()->ResourceBarrier(_countof(barriers), barriers);

		// Particle の更新処理.
		DirectXCore::GetInstance()->GetCommandList()->SetPipelineState(m_pipelines[PSO_CS_UPDATE].Get());
		DirectXCore::GetInstance()->GetCommandList()->Dispatch(invokeCount, 1, 1);
		DirectXCore::GetInstance()->GetCommandList()->ResourceBarrier(_countof(barriers), barriers);
	}

	++m_frameCount;

}



void ParticleHandHanabi::CopyData()
{

	VertexPos* outPutDeta = nullptr;
	vertBuff->Map(0, nullptr, (void**)&outPutDeta);
	memcpy(Particles.data(), outPutDeta, Particles.size() * sizeof(VertexPos));
	vertBuff->Unmap(0, nullptr);

}

