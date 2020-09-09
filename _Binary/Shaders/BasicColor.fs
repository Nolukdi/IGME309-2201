#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform bool Comp = false;

out vec4 Fragment;

void main()
{
    vec3 tempC = Color;
    if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
        tempC= SolidColor;

    if(Comp)
    {
        tempC = vec3(1, 1, 1) - tempC;
    }

    Fragment = vec4(tempC,1)	;

    return;
}