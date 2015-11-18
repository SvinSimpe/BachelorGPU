#ifndef _3DLIBS_H_
#define _3DLIBS_H_


#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" )


#define SAFE_RELEASE( x ) if( x ) { ( x )->Release(); ( x ) = NULL; }
#define SAFE_DELETE( x ) if( x ) { delete( x ); ( x ) = NULL;}

static const int NUM_PARTICLES = 50000;

struct ParticleVertex12
{
	float x;
	float y;
	float z;
};

struct PerObjectData
{
	XMFLOAT4X4 worldMatrix;
};

struct PerFrameData
{
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projMatrix;
	XMFLOAT3   eyePosition;
	float	   padding;
};

struct ParticleConstantData // Bound as input data to Compute Shader stage
{
	XMFLOAT3 position;
	XMFLOAT3 velocity;
	XMFLOAT3 initialVelocity;	
};

struct ParticleData // Used as RWStructure type
{
	XMFLOAT3 position;
	XMFLOAT3 velocity;
};

struct PerFrameDelta
{
	XMFLOAT4 deltaTime;
};

#endif