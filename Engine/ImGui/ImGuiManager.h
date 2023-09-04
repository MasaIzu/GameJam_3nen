#pragma once
#include <imgui.h>
#include "WinApp.h"
#include <d3dx12.h>
#include "DirectXCore.h"

//ImGUI�̊Ǘ�
class ImGuiManager
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCore* dxcommon);

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui��t�J�n
	/// </summary>
	void Bigin();

	/// <summary>
	/// ImGui��t�I��
	/// </summary>
	void End();

	/// <summary>
	/// ��ʂւ̕`��
	/// </summary>
	void Draw();

private:
	DirectXCore* dxcore_ = nullptr;

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;

};