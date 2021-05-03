#pragma once

namespace GraphicEngine
{
    enum ShaderBinding
    {
        // Global uniforms
        Global_CameraMatrices = 0,
        Global_Eye = 1,
        Global_Time = 2,
        Global_DirectionalLight = 3,
        Global_PointLight = 4,
        Global_SpotLight = 5,
        Global_WindParameters = 6,
        Global_RenderingOptions = 7,
        // Local uniforms
        Wireframe_WireframeModelDescriptor,
        Solid_SolidColorModelDescriptor,
        Solid_Material,
        ShadowMap_LightSpaceModelMatrices,
        ShadowMap_ModelMatrix,
        ShadowMap_LightSpaceMatrixArray,
        ShadowMap_LightPositionFarPlaneArray,
        Normal_ModelMartices = ShadowMap_LightSpaceModelMatrices + 12,
        Grass_ModelMartices,
        Grass_Material,
        Grass_GrassParameters
    };
}