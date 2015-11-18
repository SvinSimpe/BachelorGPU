#include "../Header/ParticleSystem.h"
#include <time.h>

//void ParticleSystem::TestingHeaven()
//{
//	size_t counter	= 0;
//	for (int baseIndex = 0; baseIndex < 125; baseIndex++)
//	{
//		//Mapping from SOA-pattern to AOS-pattern courtesy of Intel
//		//https://software.intel.com/en-us/articles/3d-vector-normalization-using-256-bit-intel-advanced-vector-extensions-intel-avx
//
//		//Load
//		__m256 xReg = _mm256_load_ps( &mXPosition[baseIndex * 8] );
//		__m256 yReg = _mm256_load_ps( &mYPosition[baseIndex * 8] );
//		__m256 zReg = _mm256_load_ps( &mZPosition[baseIndex * 8] );
//
//		//Set test values
//		xReg = _mm256_set_ps( 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f );
//		yReg = _mm256_set_ps( 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f );
//		zReg = _mm256_set_ps( 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f );
//
//		//Shuffle
//		__m256 xyReg = _mm256_shuffle_ps( xReg, yReg, _MM_SHUFFLE( 2,0,2,0 ) );
//		__m256 yzReg = _mm256_shuffle_ps( yReg, zReg, _MM_SHUFFLE( 3,1,3,1 ) );
//		__m256 zxReg = _mm256_shuffle_ps( zReg, xReg, _MM_SHUFFLE( 3,1,2,0 ) );
//
//		__m256 reg03 = _mm256_shuffle_ps( xyReg, zxReg, _MM_SHUFFLE( 2, 0, 2, 0 ) );
//		__m256 reg14 = _mm256_shuffle_ps( yzReg, xyReg, _MM_SHUFFLE( 3, 1, 2, 0 ) );
//		__m256 reg25 = _mm256_shuffle_ps( zxReg, yzReg, _MM_SHUFFLE( 3, 1, 3, 1 ) );
//		
//		
//		//Map, xyz
//		__m128* vertexRegAOS = (__m128*)mTempPtr;
//
//		vertexRegAOS[0] = _mm256_castps256_ps128( reg03 );	// x8,y8,z8,x7
//		vertexRegAOS[1] = _mm256_castps256_ps128( reg14 );	// y7,z7,x6,y6
//		vertexRegAOS[2] = _mm256_castps256_ps128( reg25 );	// z6,x5,y5,z5
//
//		vertexRegAOS[3] = _mm256_extractf128_ps( reg03, 1 );	// x4,y4,z4,x3
//		vertexRegAOS[4] = _mm256_extractf128_ps( reg14, 1 );	// y3,z3,x2,y2
//		vertexRegAOS[5] = _mm256_extractf128_ps( reg25, 1 );	// z2,x1,y1,z1
//
//		for ( int index = 0, subIndex = 0 ; index < 6; index++ )
//		{
//			mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//			mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//			mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//			mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//		}
//	}
//}

//HRESULT ParticleSystem::UpdateAndSetBuffer( ID3D11DeviceContext* deviceContext )
//{
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	HRESULT hr = deviceContext->Map( mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
//
//	if( SUCCEEDED( hr ) )
//	{
//		//TEST
//		std::vector<ParticleVertex12> particleVertices;
//		ParticleVertex12 tempParticle;
//
//		size_t counter	= 0;
//		for (int baseIndex = 0; baseIndex < 125; baseIndex++)
//		{
//			//Mapping from SOA-pattern to AOS-pattern courtesy of Intel
//			//https://software.intel.com/en-us/articles/3d-vector-normalization-using-256-bit-intel-advanced-vector-extensions-intel-avx
//
//			//Load
//			__m256 xReg = _mm256_load_ps( &mXPosition[baseIndex * 8] );
//			__m256 yReg = _mm256_load_ps( &mYPosition[baseIndex * 8] );
//			__m256 zReg = _mm256_load_ps( &mZPosition[baseIndex * 8] );
//
//			//Set test values
//			xReg = _mm256_set_ps( 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f );
//			yReg = _mm256_set_ps( 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f );
//			zReg = _mm256_set_ps( 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f );
//
//			//Shuffle
//			__m256 xyReg = _mm256_shuffle_ps( xReg, yReg, _MM_SHUFFLE( 2,0,2,0 ) );
//			__m256 yzReg = _mm256_shuffle_ps( yReg, zReg, _MM_SHUFFLE( 3,1,3,1 ) );
//			__m256 zxReg = _mm256_shuffle_ps( zReg, xReg, _MM_SHUFFLE( 3,1,2,0 ) );
//
//			__m256 reg03 = _mm256_shuffle_ps( xyReg, zxReg, _MM_SHUFFLE( 2, 0, 2, 0 ) );
//			__m256 reg14 = _mm256_shuffle_ps( yzReg, xyReg, _MM_SHUFFLE( 3, 1, 2, 0 ) );
//			__m256 reg25 = _mm256_shuffle_ps( zxReg, yzReg, _MM_SHUFFLE( 3, 1, 3, 1 ) );
//			
//			
//			//Map, xyz
//			__m128* vertexRegAOS = (__m128*)mTempPtr;
//
//			vertexRegAOS[0] = _mm256_castps256_ps128( reg03 );	// x8,y8,z8,x7
//			vertexRegAOS[1] = _mm256_castps256_ps128( reg14 );	// y7,z7,x6,y6
//			vertexRegAOS[2] = _mm256_castps256_ps128( reg25 );	// z6,x5,y5,z5
//
//			vertexRegAOS[3] = _mm256_extractf128_ps( reg03, 1 );	// x4,y4,z4,x3
//			vertexRegAOS[4] = _mm256_extractf128_ps( reg14, 1 );	// y3,z3,x2,y2
//			vertexRegAOS[5] = _mm256_extractf128_ps( reg25, 1 );	// z2,x1,y1,z1
//
//			for ( int index = 0, subIndex = 0 ; index < 6; index++ )
//			{
//				mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//				mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//				mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//				mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//			}
//
//			//Implement a test loop here that inserts particles into a std::vector and use
//			//as resource for mapping buffer
//			for ( int index = 0, subIndex = 0 ; index < 6; index++ )
//			{
//				mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//				mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//				mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//				mVertices[counter++] = vertexRegAOS[index].m128_f32[(subIndex++) % 4];
//			}
//		}
//		
//		memcpy( mappedResource.pData, &mVertices, NUM_PARTICLES * sizeof( ParticleVertex12 ) );
//		deviceContext->Unmap( mVertexBuffer, 0 );
//	}
//
//	return hr;
//}

HRESULT ParticleSystem::UpdateAndSetPerObjectData( ID3D11DeviceContext* deviceContext, float deltaTime )
{
	HRESULT hr = S_OK;

	XMStoreFloat4x4( &mPerObjectData.worldMatrix, XMMatrixTranspose( XMMatrixIdentity() ) );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = deviceContext->Map( mPerObjectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mPerObjectData, sizeof( PerObjectData ) );	
		deviceContext->Unmap( mPerObjectCBuffer, 0 );

		// Set constant buffer to shader stages
		deviceContext->GSSetConstantBuffers( 1, 1, &mPerObjectCBuffer );	
	}

	return hr;
}

HRESULT ParticleSystem::UpdateVertexBuffer( ID3D11DeviceContext* deviceContext )
{
	HRESULT hr = S_OK;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = deviceContext->Map( mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) ) 
	{
		ParticleVertex12 vertices[NUM_PARTICLES];

		for (size_t i = 0; i < NUM_PARTICLES; i++)
		{
			vertices[i].x = mXPosition[i];
			vertices[i].y = mYPosition[i];
			vertices[i].z = mZPosition[i];
		}

		memcpy( mappedResource.pData, vertices, sizeof(ParticleVertex12) * NUM_PARTICLES );
		deviceContext->Unmap( mVertexBuffer, 0 );
	}
	
	return hr;
}

void ParticleSystem::SetRandomVelocity( int index )
{
	XMFLOAT3 randomVelocity;

	//Generate random velocity
	float randomSpreadAngle = (float)( ( rand() % 3000 * 2 ) - 3000 ) * 0.01f; // 3000 == 100 * 30 degree spread angle
	XMVECTOR randomAimingDirection	= XMVector3TransformCoord( XMLoadFloat3( &XMFLOAT3( 0.0f, 1.0f, 0.0f ) ), XMMatrixRotationX( XMConvertToRadians( randomSpreadAngle ) ) );
	
	// Test a magnitude
	int magnitude = ( ( rand() % 15 ) + 38 );// - 5;
	randomAimingDirection *= magnitude;
	
	XMStoreFloat3( &randomVelocity, XMVector3TransformCoord( randomAimingDirection, XMMatrixRotationZ( XMConvertToRadians( randomSpreadAngle ) ) ) );

	mXVelocity[index] = mInitialXVelocity[index] = randomVelocity.x;
	mYVelocity[index] = mInitialYVelocity[index] = randomVelocity.y;
	mZVelocity[index] = mInitialZVelocity[index] = randomVelocity.z;
}

void ParticleSystem::Update( float deltaTime )
{
}

void ParticleSystem::Render( ID3D11DeviceContext* deviceContext )
{
	UpdateAndSetPerObjectData( deviceContext, 0.0f );
	UpdateVertexBuffer( deviceContext );


	UINT32 offset				 	= 0;
	UINT32 vertexSize				= sizeof( ParticleVertex12 );
	ID3D11Buffer* buffersToSet[]	= { mVertexBuffer };
	deviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	deviceContext->Draw( NUM_PARTICLES, 0 );
}

HRESULT ParticleSystem::FillConstantDataBuffer( ID3D11DeviceContext* deviceContext, ID3D11Buffer* inputBuffer )
{
	HRESULT hr = S_OK;
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = deviceContext->Map( inputBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{
		// Fill buffer with particle data
		std::vector<ParticleConstantData> particleData( NUM_PARTICLES );
		for ( size_t i = 0; i < NUM_PARTICLES; i++ )
		{
			particleData[i].position.x = mXPosition[i];
			particleData[i].position.y = mYPosition[i];
			particleData[i].position.z = mZPosition[i];

			particleData[i].velocity.x = mXVelocity[i];
			particleData[i].velocity.y = mYVelocity[i];
			particleData[i].velocity.z = mZVelocity[i];

			particleData[i].initialVelocity.x = mInitialXVelocity[i];
			particleData[i].initialVelocity.y = mInitialYVelocity[i];
			particleData[i].initialVelocity.z = mInitialZVelocity[i];
		}


		memcpy( mappedResource.pData, &particleData[0], sizeof(ParticleConstantData) * NUM_PARTICLES );
		deviceContext->Unmap( inputBuffer, 0 );
	}


	return hr;
}

void  ParticleSystem::UpdatePositionAndVelocity( ParticleData* dataView )
{
	int k = sizeof(*dataView);

	for ( size_t i = 0; i < NUM_PARTICLES; i++ )
	{
		// Update position
		mXPosition[i] = dataView[i].position.x;
		mYPosition[i] = dataView[i].position.y;
		mZPosition[i] = dataView[i].position.z;

		// Update velocity
		mXVelocity[i] = dataView[i].velocity.x;
		mYVelocity[i] = dataView[i].velocity.y;
		mZVelocity[i] = dataView[i].velocity.z;
	}
}

HRESULT ParticleSystem::Initialize( ID3D11Device* device, XMFLOAT3 emitterPosition )
{
	//Allocating aligned memory for each of the attribute component. Allocated aligned
	//memory should be deallocated using '_aligned_free( void *memblock )'
	mXPosition = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );
	mYPosition = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );
	mZPosition = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );

	mXVelocity = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );
	mYVelocity = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );
	mZVelocity = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );

	mInitialXVelocity = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );
	mInitialYVelocity = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );
	mInitialZVelocity = (float*) _aligned_malloc( NUM_PARTICLES * sizeof(float), 32 );


	srand(time(NULL));

	//Zero members
	for ( size_t i = 0; i < NUM_PARTICLES; i++ )
	{
		mXPosition[i] = emitterPosition.x;//-250.0f + 1.0f * (float)i;
		mYPosition[i] = emitterPosition.y;//150.0f;
		mZPosition[i] = emitterPosition.z;//0.0f;

		mXVelocity[i] = 0.0f;
		mYVelocity[i] = 0.0f;
		mZVelocity[i] = 0.0f;

		mInitialXVelocity[i] = 0.0f;
		mInitialYVelocity[i] = 5.0f;
		mInitialZVelocity[i] = 0.0f;

		SetRandomVelocity( i );
	}

	//Create Vertex Buffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );

    desc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
    desc.Usage					= D3D11_USAGE_DYNAMIC;
    desc.ByteWidth				= sizeof(ParticleVertex12) * NUM_PARTICLES;
	desc.StructureByteStride	= sizeof(ParticleVertex12);
	desc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subData;

	ParticleVertex12 vertices[NUM_PARTICLES];

	for (size_t i = 0; i < NUM_PARTICLES; i++)
	{
		vertices[i].x = mXPosition[i];
		vertices[i].y = mYPosition[i];
		vertices[i].z = mZPosition[i];
	}

	subData.pSysMem		= vertices;
	
	if( FAILED( device->CreateBuffer( &desc, &subData, &mVertexBuffer ) ) )
		return E_FAIL;


	//Create per object cbuffer
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth			= sizeof( mPerObjectData );
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags			= 0;
	cbDesc.StructureByteStride	= 0;


	if( FAILED( device->CreateBuffer( &cbDesc, nullptr, &mPerObjectCBuffer ) ) )
		return E_FAIL;

	return S_OK;
}

void ParticleSystem::Release()
{
	_aligned_free( mXPosition );
	_aligned_free( mYPosition );
	_aligned_free( mZPosition );

	_aligned_free( mXVelocity );
	_aligned_free( mYVelocity );
	_aligned_free( mZVelocity );

	_aligned_free( mInitialXVelocity );
	_aligned_free( mInitialYVelocity );
	_aligned_free( mInitialZVelocity );

	SAFE_RELEASE( mVertexBuffer );
	SAFE_RELEASE( mPerObjectCBuffer );
}

ParticleSystem::ParticleSystem()
{
	mXPosition		= nullptr;
	mYPosition		= nullptr;
	mZPosition		= nullptr;
			  
	mXVelocity		= nullptr;
	mYVelocity		= nullptr;
	mZVelocity		= nullptr;


	mVertexBuffer	= nullptr;
	mPerObjectCBuffer	= nullptr;
}

ParticleSystem::~ParticleSystem()
{}