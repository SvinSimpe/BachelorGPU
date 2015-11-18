/*=====================================================
					COMPUTE SHADER
			Particle motion calculations
/=====================================================*/
cbuffer PER_FRAME_DELTA : register( b0 )
{
	float4 deltaAndPadding;
}

struct ConstantParticleData
{
	float3 position;
	float3 velocity;
	float3 initialVelocity;
};

struct ParticleData
{
	float3 position;
	float3 velocity;
};

StructuredBuffer<ConstantParticleData>	inputConstantParticleData	: register( t0 ); 
RWStructuredBuffer<ParticleData>		outputParticleData			: register( u0 );


[numthreads(32, 1, 1)]
void CS_main( int3 dispatchThreadID : SV_DispatchThreadID )
{
	///==========================================================
	float  timeStepFactor	= 0.03f; // LAPTOP
	//float  timeStepFactor	= 0.003f;
	float  timeStep			= deltaAndPadding.x * timeStepFactor;
	float  acceleration		= -9.82f;
	float  verticalLimit	= -140.0f;
	float3 worldOrigo		= float3( 0.0f, 0.0f, 0.0f );
	///==========================================================

	// Calculate new velocity
	outputParticleData[dispatchThreadID.x].velocity.y = inputConstantParticleData[dispatchThreadID.x].velocity.y + acceleration * timeStep;

	// Calculate new position
	outputParticleData[dispatchThreadID.x].position = inputConstantParticleData[dispatchThreadID.x].position + outputParticleData[dispatchThreadID.x].velocity * timeStep;

	// If particle is below vertical limit
	if( outputParticleData[dispatchThreadID.x].position.y <= verticalLimit ) 
	{
		// Reset position
		outputParticleData[dispatchThreadID.x].position = worldOrigo;

		// Set velocity to initial velocity
		outputParticleData[dispatchThreadID.x].velocity = inputConstantParticleData[dispatchThreadID.x].initialVelocity;
	}
}