#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "Input.h"
#include "WinApp.h"
#include <cassert>

#include <basetsd.h>
#include <commctrl.h>
#include <dbt.h>
#include <oleauto.h>
#include <shellapi.h>
#include <wbemidl.h>
#include <wrl/client.h>

#include <dinputd.h>
#include <memory>

Input* Input::Input_ = nullptr;

Input* Input::GetInstance() {
	if (Input_ == nullptr)
	{
		Input_ = new Input();
	}

	return Input_;
}

Input::~Input() {
	if (devKeyboard_) {
		devKeyboard_->Unacquire();
	}

	if (dInput_) {
		dInput_.Reset();
	}

}

void Input::Destroy()
{
	delete Input_;
}

void Input::Initialize()
{
	WinApp* winApp = WinApp::GetInstance();

	hwnd_ = winApp->Gethwnd();
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成
	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = dInput_->CreateDevice(GUID_SysKeyboard, &devKeyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = devKeyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = devKeyboard_->SetCooperativeLevel(
		hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//マウスデバイスの初期化
	mouse = std::make_unique<Mouse>();
	mouse->Initialize(dInput_.Get());
}

void Input::Update()
{
	// 前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	// キーボード情報の取得開始
	devKeyboard_->Acquire();

	// 全キーの入力状態を取得する
	devKeyboard_->GetDeviceState(sizeof(key), key);

	//マウス更新
	mouse->Update();
}

bool Input::PushKey(BYTE keyNumber)
{
	// 指定したキーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定キーを前フレームで押していなく、今のフレームで押していればtrueを返す
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::ReleasedKey(BYTE keyNumber)
{
	// 指定キーを前フレームで押していて、今のフレームで押していなければtrueを返す
	if (keyPre[keyNumber] && !key[keyNumber]) {
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::MouseInputTrigger(int button)
{
	return mouse->MouseInputTrigger(button);
}

bool Input::MouseInputing(int button)
{
	return mouse->MouseInput(button);
}

bool Input::MouseOffTrigger(int button)
{
	return mouse->MouseOffTrigger(button);
}

const Vector2 Input::GetMousePos()const
{
	return mouse->GetMousePos();
}

const Vector3 Input::GetMouseMove()
{
	return mouse->GetMouseMove();
}