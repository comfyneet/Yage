cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binomal  : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only and then normalize the final value.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    // Calculate the tangent vector against the world matrix only and then normalize the final value.
    output.tangent = mul(input.tangent, (float3x3) worldMatrix);
    output.tangent = normalize(output.tangent);

    // Calculate the binormal vector against the world matrix only and then normalize the final value.
    output.binormal = mul(input.binormal, (float3x3) worldMatrix);
    output.binormal = normalize(output.binormal);

    return output;
}

//cbuffer cbPerFrame
//{
//    float3 gEyePositionW;
//    float padding;
//};
//
//cbuffer cbPerObject
//{
//    float4x4 gWorld;
//    float4x4 gWorldInvTranspose;
//    float4x4 gWorldViewProj;
//    float4x4 gTexTransform;
//};
//
//struct VertexIn
//{
//    float3 PositionL : POSITION;
//    float3 NormalL   : NORMAL;
//    float2 TexCoord  : TEXCOORD;
//    float3 TangentL  : TANGENT;
//};
//
//struct VertexOut
//{
//    float4 PositionH : SV_Position;
//    float3 PositionW : POSITION;
//    float3 NormalW   : NORMAL;
//    float3 TangentW  : TANGENT;
//    float2 TexCoord  : TEXCOORD;
//};
//
//VertexOut VS(VertexIn vin)
//{
//    VertexOut vout;
//
//    // Transform to world space.
//    vout.PositionW = mul(float4(vin.PositionL, 1.0f), gWorld).xyz;
//    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose);
//    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld);
//
//    // Transform to homogeneous clip space.
//    vout.PositionH = mul(float4(vin, PositionL, 1.0f), gWorldViewProj);
//
//    // Output vertex attributes for interpolation across triangle.
//    vout.TexCoord = mul(float4(vin.TexCoord, 0.0f, 1.0f), gTexTransform);
//
//    return vout;
//}
//
//float4 PS(VertexOut pin) : SV_Target
//{
//}
