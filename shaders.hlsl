technique10 RenderTerrain
{
	
	pass P0//Draw triangles with MTRS
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSTERRAIN() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSTriangles() ) );
        SetPixelShader( CompileShader( ps_4_0, MAPPER() ) );
		SetRasterizerState(rs1); 
    }

	pass P1//Shadow triangles
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSTERRAINSHADOW() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSSHADOWTriangles() ) );
        SetPixelShader( CompileShader( ps_4_0, SHADOWMAPPER() ) );
		SetRasterizerState(rs1); 
    }

	pass P2//Draw lines with MTRS
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSTERRAIN() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSLines() ) );
        SetPixelShader( CompileShader( ps_4_0, MAPPER() ) );
		SetRasterizerState(rs1); 
    }

	pass P3//Shadow lines
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSTERRAINSHADOW() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSSHADOWLines() ) );
        SetPixelShader( CompileShader( ps_4_0, SHADOWMAPPER() ) );
		SetRasterizerState(rs1); 
    }
}

technique10 RenderGeometry
{

	pass P0//Draw triangles with MTRS
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSTriangles() ) );
        SetPixelShader( CompileShader( ps_4_0, MAPPER() ) );
		SetRasterizerState(rs1); 
    }

	pass P1//Shadow triangles
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSSHADOW() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSSHADOWTriangles() ) );
        SetPixelShader( nullptr );
		SetRasterizerState(rs1); 
    }

	pass P2//Draw lines with MTRS
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSLines() ) );
        SetPixelShader( CompileShader( ps_4_0, MAPPER() ) );
		SetRasterizerState(rs1); 
    }

	pass P3//Shadow lines
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSSHADOW() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSSHADOWLines() ) );
        SetPixelShader( nullptr );
		SetRasterizerState(rs1); 
    }
}

technique10 Light
{

	pass P0//PointLight
    {
		SetBlendState(AdditiveBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSLIGHT() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSLIGHT()) );
        SetPixelShader( CompileShader( ps_4_0, POINTLIGHTER() ) );
		SetRasterizerState(rs2); 
    }
}

technique10 ScreenEffects
{
	pass P0//MotionBlur
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSSCREEN() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSSCREEN()) );
        SetPixelShader( CompileShader( ps_4_0, MOTIONBLUR() ) );
		SetRasterizerState(rs1); 
    }
}

technique10 Particle
{
	
	pass P0//LightParticle
	{

		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSPARTICLE() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSPARTICLETRIANGLE()) );
        SetPixelShader( CompileShader( ps_4_0, LIGHTPARTICLE() ) );
		SetRasterizerState(rs2); 

	}
	
	pass P1//Particle wireframe
	{

		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSPARTICLE() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSPARTICLELINES()) );
        SetPixelShader( CompileShader( ps_4_0, SHADOWMAPPER()) );
		SetRasterizerState(rs2); 

	}
}

technique10 Debug
{
	pass P0//Draw normalspikes for geometry
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSNORMALSPIKE()) );
        SetPixelShader( CompileShader( ps_4_0, SHADOWMAPPER() ) );
		SetRasterizerState(rs1); 
    }

	pass P1//Draw normalspikes for terrain
    {
		SetBlendState(nullptr, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VSTERRAIN() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSNORMALSPIKE()) );
        SetPixelShader( CompileShader( ps_4_0, SHADOWMAPPER() ) );
		SetRasterizerState(rs1); 
    }
}