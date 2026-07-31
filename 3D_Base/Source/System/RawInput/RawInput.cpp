#include "RawInput.h"
#include <iostream>


RawInput::RawInput()
:MouseMove()
, MouseMovement()
, MouseHOILLNow(0)
, MouseHOILL(0)
{
    for (int i = 0; i < 3;i++) {
        MouseLeft[i]=false;
        MouseRight[i]=false;
        MouseWheel[i]=false;
    }
}

RawInput::~RawInput()
{
}


bool RawInput::getMouseLeft(bool Push, bool Moment)
{
    //Pushがtrueなら押している
    if (Push) {
        //Momentがtrueなら押された瞬間、falseなら押し続けている間
        if (Moment == true) {
            if (MouseLeft[1] == true && MouseLeft[0] == false) {
                return true;
            }
        }
        else {
            if (MouseLeft[1] == true) {
                return true;
            }
        }
        return false;
    }
    else
    {
        //Momentがtrueなら離された瞬間、falseなら離し続けている間
        if (Moment == true) {
            if (MouseLeft[1] == false && MouseLeft[0] == true) {
                return true;
            }
        }
        else {
            if (MouseLeft[1] == false) {
                return true;
            }
        }
        return false;
    }


}

bool RawInput::getMouseRight(bool Push, bool Moment)
{
    //Pushがtrueなら押している
    if (Push) {
        //Momentがtrueなら押された瞬間、falseなら押し続けている間
        if (Moment == true) {
            if (MouseRight[1] == true && MouseRight[0] == false) {
                return true;
            }
        }
        else {
            if (MouseRight[1] == true) {
                return true;
            }
        }
        return false;
    }
    else
    {
        //Momentがtrueなら離された瞬間、falseなら離し続けている間
        if (Moment == true) {
            if (MouseRight[1] == false && MouseRight[0] == true) {
                return true;
            }
        }
        else {
            if (MouseRight[1] == false) {
                return true;
            }
        }
        return false;
    }

}

bool RawInput::getMouseWheel(bool Push, bool Moment)
{
    //Pushがtrueなら押している
    if (Push) {
        //Momentがtrueなら押された瞬間、falseなら押し続けている間
        if (Moment == true) {
            if (MouseWheel[1] == true && MouseWheel[0] == false) {
                return true;
            }
        }
        else {
            if (MouseWheel[1] == true) {
                return true;
            }
        }
        return false;
    }
    else
    {
        //Momentがtrueなら離された瞬間、falseなら離し続けている間
        if (Moment == true) {
            if (MouseWheel[1] == false && MouseWheel[0] == true) {
                return true;
            }
        }
        else {
            if (MouseWheel[1] == false) {
                return true;
            }
        }
        return false;
    }
}

void RawInput::Setting(HWND hwnd)
{
 

	RAWINPUTDEVICE rid[1];
    rid[0].hwndTarget = 0;
	rid[0].usUsagePage = 0x01;          // 汎用デスクトップコントロール
	rid[0].usUsage = 0x02;              // マウス (キーボードなら 0x06)
	rid[0].dwFlags = 0;   // RIDEV_INPUTSINK バックグラウンドでも取得するか等の設定// ← ここを 0 にすると、フォーカス時のみ受け取る
	rid[0].hwndTarget = hwnd;           // 対象のウィンドウハンドル

	RegisterRawInputDevices(rid, 1, sizeof(rid[0]));



    UINT nDevices;
    // 1. まず接続されているデバイスの「個数」を取得
    GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));
    // 2. 個数分のメモリを確保
    PRAWINPUTDEVICELIST pRawInputDeviceList = new RAWINPUTDEVICELIST[nDevices];
    // 3. 実際にリストを取得
    GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST));
   
    for (UINT i = 0; i < nDevices; i++) {
        if (pRawInputDeviceList[i].dwType == RIM_TYPEMOUSE) {
            // マウスのハンドルをリストに追加
            mouseHandles.push_back(pRawInputDeviceList[i].hDevice);
        }
    }
    // --- ここでリストをループして解析する ---
    delete[] pRawInputDeviceList;
}

void RawInput::setInpt(HRAWINPUT log)
{


    UINT dwSize=0;
    GetRawInputData(log, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

    LPBYTE lpb = new BYTE[dwSize];
    if (GetRawInputData(log, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)))
    {
         RAWINPUT* raw = (RAWINPUT*)lpb;
         // raw->header.hDevice にマウスごとの固有IDが入っている！
         HANDLE deviceHandle = raw->header.hDevice;
         

        if (raw->header.dwType == RIM_TYPEMOUSE)//RIM_TYPEMOUSE==マウスのデータ
        {
            // マウスの相対的な移動量を取得
            long lastX = raw->data.mouse.lLastX;
            long lastY = raw->data.mouse.lLastY;
            //マウスの1つめなら＋
            if (mouseHandles[0]== deviceHandle) {

                MouseMovement.x += lastX;
                MouseMovement.y += lastY;
            }
            else {

                MouseMovement.x -= lastX;
                MouseMovement.y -= lastY;
            }

            // これを使って視点移動などを計算する

            if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                // 左ボタンが押された
                MouseLeft[2] = true;
            }
            if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                MouseLeft[2] = false;
            }
            if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                // 左ボタンが押された
                MouseRight[2] = true;
            } 
            if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                MouseRight[2] = false;
            }
            if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                MouseWheel[2] = true;
            }
            if (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                MouseWheel[2] = false;
            }
            if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
                // ホイールが回転した
                // 回転量は usButtonData を short にキャストして取得
                short wheelDelta = (short)raw->data.mouse.usButtonData;
                MouseHOILLNow += wheelDelta;
           

            }

        
        }
    }
    SAFE_DELETE(lpb);

}

void RawInput::upd()
{

    MouseMove.x = MouseMovement.x ;
    MouseMove.y = MouseMovement.y;
    MouseHOILL = MouseHOILLNow;

    MouseLeft [0] = MouseLeft[ 1];
    MouseRight[0] = MouseRight[1];
    MouseWheel[0] = MouseWheel[1];


    MouseLeft [1] = MouseLeft[2];
    MouseRight[1] = MouseRight[2];
    MouseWheel[1] = MouseWheel[2];

 

    MouseHOILLNow = 0;
    MouseMovement = { 0,0 };

    AllocConsole();
    FILE* pFile;
    //stdout（標準出力）を新しく作成したコンソールにリダイレクト
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    //使うならしたのだけでいい
    ////std::cout << MouseMove.x << std::endl;
    ////std::cout << MouseMove.y << std::endl;
    //std::system("cls");//コンソールのリセット
}
