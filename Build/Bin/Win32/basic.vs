attribute vec4 iPos;
attribute vec4 iColor;
attribute vec2 iTexCoord1;

uniform mat4 uViewProj;
uniform mat4 uModel;

varying vec2 vTexCoord1;
varying vec4 vColor;

void main()
{
	vec4 pos = uModel * iPos;
	gl_Position = uViewProj * vec4(pos.xyz, 1.0);
    vColor = iColor;
	vTexCoord1 = iTexCoord1;
}