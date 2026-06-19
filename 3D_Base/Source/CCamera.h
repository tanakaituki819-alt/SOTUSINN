#pragma once

class CCamera
{
public:
	//カメラ情報構造体.

	CCamera();
	~CCamera();
	void Cmera();


	 void Update() ;
	 void SetPosition(D3DXVECTOR3 Camera) { m_Camera.Position= Camera; }
	 void SetPositionX(float Camera) { m_Camera.Position.x= Camera; }
	 void SetPositionY(float Camera) { m_Camera.Position.y = Camera; }
	 void SetPositionZ(float Camera) { m_Camera.Position.z= Camera; }

	 void SetLookPosition(D3DXVECTOR3 Lok) { m_Camera.Look= Lok; }
	 void SetLookPositionX(float Lok) { m_Camera.Look.x = Lok; }
	 void SetLookPositionY(float Lok) { m_Camera.Look.y = Lok; }
	 void SetLookPositionZ(float Lok) { m_Camera.Look.z = Lok; }
	// void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) ;
	 D3DXVECTOR3 GetPosition() {return m_Camera.Position;}
	 CAMERA&		 GetCamera() { return m_Camera;}
	 D3DXVECTOR3 GetLookPosition() { return m_Camera.Look; }
	 D3DXMATRIX&  GetView() { return m_mView; };

private:
	//カメラ情報.
	CAMERA			m_Camera;
	D3DXVECTOR3 Vec;		//ビュー
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
};

