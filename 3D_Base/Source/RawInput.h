#pragma once
#include <vector>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
class RawInput
{
public:
	
	~RawInput();
	static void firstSetting(HWND hwnd) {
		GetInstance()->Setting(hwnd);
	}
	static void SetInpt(HRAWINPUT log) {
		GetInstance()->setInpt(log);
	}

	//マウスの左を取得,Pushは押したか離しているか Momentは押し続けているか押した瞬間か（trueは押した瞬間）
	static bool GetMouseLeft(bool Push, bool Moment) { return GetInstance()->getMouseLeft(Push, Moment); };


	//マウスの右を取得,Pushは押したか離しているか Momentは押し続けているか押した瞬間か（trueは押した瞬間）
	static bool GetMouseRight(bool Push, bool Moment) { return GetInstance()->getMouseRight(Push, Moment); };


	//マウスの真ん中の押込みを取得,Pushは押したか離しているか Momentは押し続けているか押した瞬間か（trueは押した瞬間）
	static bool GetMouseWheel(bool Push, bool Moment) { return GetInstance()->getMouseWheel(Push, Moment); };//マウスホイール


	static void Update() {
		GetInstance()->upd();
	}
	//マウスの動き取得
	static D3DXVECTOR2 GetMouseMove() {
		return GetInstance()->MouseMove;
	}
	//マウスホイールの上下取得
	static int GetMouseHOILL() {
		return GetInstance()->MouseHOILL;
	}
private:
	
	bool getMouseLeft(bool Push, bool Moment);
	bool getMouseRight(bool Push, bool Moment);
	bool getMouseWheel(bool Push, bool Moment);

	void Setting(HWND hwnd);

	void setInpt(HRAWINPUT log);

	void upd();

	static RawInput* GetInstance()
	{
		static RawInput s_Instance;
		return &s_Instance;
	}
	RawInput();
	//コピーコンストラクタによるコピーを禁止する
	RawInput(const RawInput& rhs) = delete;
	//代入演算子によるコピーを禁止する
	RawInput& operator=(RawInput& rhs) = delete;
	//↑この3つの演算子のセットでシングルトン化が成立する
	D3DXVECTOR2 MouseMovement;
	D3DXVECTOR2 MouseMove;
	//ホイールの回転
	int  MouseHOILLNow;
	int  MouseHOILL;

	bool MouseLeft [3];
	bool MouseRight[3];
	bool MouseWheel[3];

	std::vector<HANDLE> mouseHandles;
};
