#include "Object3D.hlsli"

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float32_t4 texColor = gTexture.Sample(gSampler, input.texcoord);
    
    // 周囲を0に、中心になるほど明るくなるように計算で調整
    float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    // correctだけで計算すると中心の最大値が0.0625で暗すぎるのでScaleで調整。この例では16倍して1にしている
    float vignette = correct.x * correct.y * 16.0f;
    // とりあえず0.8乗でそれっぽく
    vignette = saturate(pow(vignette, 0.15f));

    // 色
    float32_t3 purpleColor = float32_t3(0.5f, 0.0f, 0.8f);

    // vinette値に応じて補間
    output.color.rgb = lerp(purpleColor, texColor.rgb, vignette);
    output.color.a = texColor.a;
    
    return output;
}