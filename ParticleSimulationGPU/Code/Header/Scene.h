#ifndef _SCENE_H_
#define _SCENE_H_

#include "../Header/ParticleSystem.h"
#include "../Header/Camera.h"
#include <fstream>

#define THREAD_GRID_SIZE_X 32
#define THREAD_GRID_SIZE_Y 32
#define THREAD_GRID_SIZE_Z 1



class Scene
{
	private:
		ID3D11DeviceContext*		mDeviceContext;
		ParticleSystem				mParticleSystem;
		Camera						mCamera;

		ID3D11VertexShader*			mVertexShader;
		ID3D11GeometryShader*		mGeometryShader;
		ID3D11PixelShader*			mPixelShader;
		ID3D11InputLayout*			mInputLayout;	

		PerFrameData				mPerFrameData;
		ID3D11Buffer*				mPerFrameCBuffer;

		//===== Compute Shader Components =====
		ID3D11ComputeShader*		mComputeShader;

		// Read Only
		ID3D11Buffer*				mInputBuffer;
		ID3D11ShaderResourceView*	mInputView;

		// Read/Write
		ID3D11Buffer*				mOutputBuffer;
		ID3D11Buffer*				mOutputResultBuffer;
		ID3D11UnorderedAccessView*	mOutputUAV;

		// Per frame delta time
		PerFrameDelta				mPerFrameDeltaData;
		ID3D11Buffer*				mPerFrameDeltaCBuffer;


	private:
		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
		HRESULT InitializeShaders( ID3D11Device* device );
		HRESULT UpdateCBPerFrame();
		HRESULT UpdateDeltaCBPerFrame( float deltaTime );

	public:		
		void Update( float deltaTime );
		void Render();

		HRESULT	Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
		void	Release();

		Scene();
		~Scene();
};
#endif