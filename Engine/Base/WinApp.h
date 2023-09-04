#pragma once
#include<Windows.h>
#include <cstdint>
#include <string>
#include "Vector2.h"

class WinApp
{
public:

	// �E�B���h�E�T�C�Y
	static const uint32_t window_width; // ����
	static const uint32_t window_height; // �c��

public: // �ÓI�����o�֐�

	// �V���O���g���C���X�^���X�̎擾
	static WinApp* GetInstance();

	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://�����o�֐�
	void MakeWindow(std::wstring title);
	bool ProcessMessage();
	void DeleteGameWindow();

	void Destroy();

	HINSTANCE GetHInstance() const { return w.hInstance; }
	Vector2 GetWindowSize() const;

public://GetterSetter

	HWND Gethwnd() { return hwnd; }

private://�����o�ϐ�
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

	WNDCLASSEX w{};
	HWND hwnd;

private:
	static WinApp* WinApp_;

};
