#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "../Header/3DLibs.h"
#include <vector>

class ParticleSystem
{
	private:
		float*		mXPosition;
		float*		mYPosition;
		float*		mZPosition;

		float*		mXVelocity;
		float*		mYVelocity;
		float*		mZVelocity;

		float*		mInitialXVelocity;
		float*		mInitialYVelocity;
		float*		mInitialZVelocity;

		ID3D11Buffer*					mVertexBuffer;
		std::vector<ParticleVertex12>	mParticles;

		ID3D11Buffer*	mPerObjectCBuffer;
		PerObjectData	mPerObjectData;

	private:
		HRESULT		UpdateAndSetBuffer( ID3D11DeviceContext* deviceContext );
		HRESULT		UpdateAndSetPerObjectData( ID3D11DeviceContext* deviceContext, float deltaTime );
		HRESULT		UpdateVertexBuffer( ID3D11DeviceContext* deviceContext );
		//void		UpdateParticleLogic( float deltaTime );		Move to ----> GPU

		void		SetRandomVelocity( int index );
		//void		CheckDeadParticles();						Move to ----> GPU
		//void		ResetParticle( int index );					Move to ----> GPU

	public:
		void Update( float deltaTime );
		void Render( ID3D11DeviceContext* deviceContext );

		HRESULT FillConstantDataBuffer(  ID3D11DeviceContext* deviceContext, ID3D11Buffer* inputBuffer );
		void	FillRWStructure( ID3D11DeviceContext* deviceContext, ID3D11Buffer* outputBuffer );
		void	UpdatePositionAndVelocity( ParticleData* dataView );

		HRESULT Initialize( ID3D11Device* device, XMFLOAT3 emitterPosition );
		void Release();

		ParticleSystem();
		~ParticleSystem();
};
#endif