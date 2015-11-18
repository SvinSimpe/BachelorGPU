#include "../Header/Scene.h"

HRESULT Scene::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
{
    HRESULT hr = S_OK;
 
    DWORD dwShaderFlags =   D3DCOMPILE_ENABLE_STRICTNESS |
                            D3DCOMPILE_IEEE_STRICTNESS;
 
    std::string shader_code;
    std::ifstream in( shaderFile, std::ios::in | std::ios::binary );
 
    if ( in )
    {
        in.seekg( 0, std::ios::end );
        shader_code.resize( (unsigned int)in.tellg() );
        in.seekg( 0, std::ios::beg );
        in.read( &shader_code[0], shader_code.size() );
        in.close();
    }
 
    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompile( shader_code.data(),
                             shader_code.size(),
                             NULL,
                             pDefines,
                             nullptr,
                             pEntrypoint,
                             pTarget,
                             D3DCOMPILE_DEBUG,
                             NULL,
                             pCompiledShader,
                             &pErrorBlob );
 
    if( pErrorBlob )
    {
        OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        SAFE_RELEASE( pErrorBlob );
    }
 
 
    return hr;
}

HRESULT Scene::InitializeShaders( ID3D11Device* device )
{
    HRESULT hr = S_OK;
 
    //-------------------------------
    // Compile Particle Vertex Shader
    //-------------------------------
    ID3DBlob* pvs = nullptr;
     
	if ( SUCCEEDED( hr = CompileShader( "Code/Shader/ParticleShader.hlsl", "VS_main", "vs_5_0", nullptr, &pvs ) ) )
    {
        if( SUCCEEDED( hr = device->CreateVertexShader( pvs->GetBufferPointer(),
                                                          pvs->GetBufferSize(),
                                                          nullptr,
                                                          &mVertexShader ) ) )
        {
            D3D11_INPUT_ELEMENT_DESC inputDesc[] = {                
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };
 
            hr = device->CreateInputLayout( inputDesc,
                                            ARRAYSIZE( inputDesc ),
                                            pvs->GetBufferPointer(),
                                            pvs->GetBufferSize(),
                                            &mInputLayout );
        }
 
        SAFE_RELEASE( pvs );
    }
 
 
    //---------------------------------
    // Compile Particle Geometry Shader
    //-------------------------------- 
    ID3DBlob* pgs = nullptr;
 
    if( SUCCEEDED( hr = CompileShader( "Code/Shader/ParticleShader.hlsl", "GS_main", "gs_5_0", nullptr, &pgs ) ) )
    {
        hr = device->CreateGeometryShader( pgs->GetBufferPointer(),
                                           pgs->GetBufferSize(),
                                           nullptr,
                                           &mGeometryShader );
        SAFE_RELEASE( pgs );
    }
 
 
    //------------------------------
    // Compile Particle Pixel Shader
    //------------------------------
    ID3DBlob* pps = nullptr;
 
    if( SUCCEEDED( hr = CompileShader( "Code/Shader/ParticleShader.hlsl", "PS_main", "ps_5_0", nullptr, &pps ) ) )
    {
        hr = device->CreatePixelShader( pps->GetBufferPointer(),
                                        pps->GetBufferSize(),
                                        nullptr,
                                        &mPixelShader );
        SAFE_RELEASE( pps );
    }

	//--------------------------------
    // Compile Particle Compute Shader
    //--------------------------------
    ID3DBlob* cps = nullptr;
 
    if( SUCCEEDED( hr = CompileShader( "Code/Shader/ComputeShader.hlsl", "CS_main", "cs_5_0", nullptr, &cps ) ) )
    {
        hr = device->CreateComputeShader( cps->GetBufferPointer(),
                                        cps->GetBufferSize(),
                                        nullptr,
                                        &mComputeShader );
        SAFE_RELEASE( cps );
    }
 
 
    return hr;
}

HRESULT Scene::UpdateCBPerFrame()
{
	HRESULT hr = S_OK;

	// Retrive view and projection matrices from camera to per frame buffer data
	XMStoreFloat4x4( &mPerFrameData.viewMatrix, XMMatrixTranspose( XMLoadFloat4x4( &mCamera.GetViewMatrix() ) ) );
	XMStoreFloat4x4( &mPerFrameData.projMatrix, XMMatrixTranspose( XMLoadFloat4x4( &mCamera.GetProjectionMatrix() ) ) );

	// Get camera eye position and add to per frame data.
	// Must have to perform Geometry shader billboarding
	mPerFrameData.eyePosition = mCamera.GetEyePosition();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mPerFrameCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mPerFrameData, sizeof(PerFrameData) );	
		mDeviceContext->Unmap( mPerFrameCBuffer, 0 );

		// Set constant buffer to Geometry shader stage
		mDeviceContext->GSSetConstantBuffers( 0, 1, &mPerFrameCBuffer );
	}

	return hr;
}

HRESULT Scene::UpdateDeltaCBPerFrame( float deltaTime )
{
	HRESULT hr = S_OK;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mPerFrameDeltaCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{
		// Set 'each frame'-delta time and copy to per frame delta buffer
		mPerFrameDeltaData.deltaTime.x = deltaTime;

		memcpy( mappedResource.pData, &mPerFrameDeltaData, sizeof(PerFrameDelta) );	
		mDeviceContext->Unmap( mPerFrameDeltaCBuffer, 0 );

		// Set per frame delta buffer to Compute shader stage
		mDeviceContext->CSSetConstantBuffers( 0, 1, &mPerFrameCBuffer );
	}

	return hr;
}

void Scene::Update( float deltaTime )
{
	UpdateCBPerFrame();

	UpdateDeltaCBPerFrame( deltaTime );

	mParticleSystem.Update( deltaTime );

	mCamera.UpdateViewMatrix();
}

void Scene::Render()
{
	//============================  Compute  ============================

	// Fill input buffer with modified particle data. Input SRV
	// act as a handle to the input buffer
	mParticleSystem.FillConstantDataBuffer( mDeviceContext, mInputBuffer );


	// Enable Compute Shader
	mDeviceContext->CSSetShader( mComputeShader, nullptr, 0 );


	// Set input SRV to Compute shader
	mDeviceContext->CSSetShaderResources( 0, 1, &mInputView );


	// Set output UAV to Compute shader
	mDeviceContext->CSSetUnorderedAccessViews( 0, 1, &mOutputUAV, 0 );


	// Dispatch
	mDeviceContext->Dispatch( 32, 1, 1 );


	// Unbind the input textures from the CS for good housekeeping
	ID3D11ShaderResourceView* nullSRV[] = { NULL };
	mDeviceContext->CSSetShaderResources( 0, 1, nullSRV );


	// Unbind output from compute shader ( we are going to use this output as an input in the next pass, 
	// and a resource cannot be both an output and input at the same time
	ID3D11UnorderedAccessView* nullUAV[] = { NULL };
	mDeviceContext->CSSetUnorderedAccessViews( 0, 1, nullUAV, 0 );


	// Disable Compute Shader
	mDeviceContext->CSSetShader( nullptr, nullptr, 0 );


	// Copy result from Compute shader output resource to
	// system memory buffer
	mDeviceContext->CopyResource( mOutputResultBuffer, mOutputBuffer );


	// Update particle system data with output from Compute Shader
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = mDeviceContext->Map( mOutputResultBuffer, 0, D3D11_MAP_READ, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{	
		ParticleData* dataView = reinterpret_cast<ParticleData*>(mappedResource.pData);

		// Update particle positions and velocities
		mParticleSystem.UpdatePositionAndVelocity( dataView );
				
		mDeviceContext->Unmap( mOutputResultBuffer, 0 );
	}
	//===================================================================




	//=============================  Render  ============================

	//Set shader stages
	mDeviceContext->VSSetShader( mVertexShader, nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( mGeometryShader, nullptr, 0 );
	mDeviceContext->PSSetShader( mPixelShader, nullptr, 0 );


	// Set position vertex description
	mDeviceContext->IASetInputLayout( mInputLayout );


	// Set topology
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

	// Render particles
	mParticleSystem.Render( mDeviceContext );
	//===================================================================
}

HRESULT Scene::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	HRESULT hr = S_OK;

	mDeviceContext = deviceContext;

	// Initialize Particle system with device to create                           
	// internal resources as well as emitter position
	if( FAILED( hr = mParticleSystem.Initialize( device, XMFLOAT3( 0.0f, 2.0f, 0.0f ) ) ) )
		return hr;

	if( FAILED( hr = InitializeShaders( device ) ) )
		return hr;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth			= sizeof( mPerFrameData );
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags			= 0;
	cbDesc.StructureByteStride	= 0;

	hr = device->CreateBuffer( &cbDesc, nullptr, &mPerFrameCBuffer );


	//==============================================//
	//			Compute Shader Components			//
	///=============================================//

			
	// Create a buffer to be bound as Compute Shader input (D3D11_BIND_SHADER_RESOURCE).
	D3D11_BUFFER_DESC constantDataDesc;
	constantDataDesc.Usage					= D3D11_USAGE_DYNAMIC;
    constantDataDesc.ByteWidth				= sizeof(ParticleConstantData) * NUM_PARTICLES;
    constantDataDesc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
    constantDataDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	constantDataDesc.StructureByteStride	= sizeof(ParticleConstantData);
    constantDataDesc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	//ID3D11Buffer* inputConstantDataBuffer = nullptr;
	hr =  device->CreateBuffer ( &constantDataDesc, 0, &mInputBuffer );


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement	= 0;
	srvDesc.BufferEx.Flags			= 0;
	srvDesc.BufferEx.NumElements	= NUM_PARTICLES;

    hr = device->CreateShaderResourceView( mInputBuffer, &srvDesc, &mInputView );

	
	
	// Create a read-write buffer the compute shader can write to (D3D11_BIND_UNORDERED_ACCESS).
	D3D11_BUFFER_DESC outputDesc;
    outputDesc.Usage				= D3D11_USAGE_DEFAULT;
    outputDesc.ByteWidth			= sizeof(ParticleData) * NUM_PARTICLES;
    outputDesc.BindFlags			= D3D11_BIND_UNORDERED_ACCESS;
    outputDesc.CPUAccessFlags		= 0;
	outputDesc.StructureByteStride	= sizeof(ParticleData);
    outputDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

    hr = ( device->CreateBuffer( &outputDesc, 0, &mOutputBuffer ) );


	// Create a system memory version of the buffer to read the results back from.
	outputDesc.Usage			= D3D11_USAGE_STAGING;
	outputDesc.BindFlags		= 0;
	outputDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_READ;
	hr = ( device->CreateBuffer( &outputDesc, 0, &mOutputResultBuffer ) );


	// Create UAV for compute shader to write results
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Buffer.FirstElement		= 0;
	uavDesc.Buffer.Flags			= 0;
	uavDesc.Buffer.NumElements		= NUM_PARTICLES;
	uavDesc.Format					= DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension			= D3D11_UAV_DIMENSION_BUFFER;
	
	hr = device->CreateUnorderedAccessView( mOutputBuffer, &uavDesc, &mOutputUAV );


	// Create per frame delta buffer to pass delta time
	// to Compute shader stage for motion calculations
	cbDesc.ByteWidth			= sizeof( PerFrameDelta );

	hr = device->CreateBuffer( &cbDesc, nullptr, &mPerFrameDeltaCBuffer );

	///=============================================//


	return hr;
}

void Scene::Release()
{
	mParticleSystem.Release();

	SAFE_RELEASE( mVertexShader );
	SAFE_RELEASE( mGeometryShader );
	SAFE_RELEASE( mPixelShader );
	SAFE_RELEASE( mInputLayout );
	SAFE_RELEASE( mPerFrameCBuffer );

	//===== Compute Shader Components =====
	SAFE_RELEASE( mComputeShader );

	// Read Only
	SAFE_RELEASE( mInputBuffer );
	SAFE_RELEASE( mInputView );

	// Read/Write
	SAFE_RELEASE( mOutputBuffer );
	SAFE_RELEASE( mOutputResultBuffer );
	SAFE_RELEASE( mOutputUAV );

	// Per frame delta time
	SAFE_RELEASE( mPerFrameDeltaCBuffer );
}

Scene::Scene()
{
	mDeviceContext		= nullptr;

	mVertexShader		= nullptr;
	mGeometryShader		= nullptr;
	mPixelShader		= nullptr;
	mInputLayout		= nullptr;

	mPerFrameCBuffer	= nullptr;

	//===== Compute Shader Components =====
	mComputeShader		= nullptr;

	// Read Only
	mInputBuffer		= nullptr;
	mInputView			= nullptr;

	// Read/Write
	mOutputBuffer		= nullptr;
	mOutputResultBuffer	= nullptr;
	mOutputUAV			= nullptr;

	// Per frame delta time
	mPerFrameDeltaCBuffer	= nullptr;
}

Scene::~Scene()
{}