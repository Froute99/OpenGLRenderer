#version 430 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(r32f, binding = 0) uniform image2D imgOutput;

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

    float inValue = imageLoad(imgOutput, pos).r;
    imageStore(imgOutput, pos, vec4(inValue + 1.0, 0.0, 0.0, 0.0));
}
