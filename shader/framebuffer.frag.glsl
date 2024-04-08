#version 450

layout (binding = 0) uniform sampler2D diffuse;

layout (location = 0) in vec2 fuv;

layout (location = 0) out vec4 fragColor;

// uv is input texture coord, srcRes is original texture resolution, returns filtered texture coords
vec2 filterCoord(vec2 uv, vec2 srcRes) {
	vec2 invSrc = 1.0 / srcRes;

	// calculate destination resolution
	vec2 duv1 = vec2(dFdx(uv.x), dFdy(uv.y));
	vec2 duv2 = vec2(dFdy(uv.x), dFdx(uv.y));
    
	vec2 duv = max(abs(duv1), abs(duv2));	// pick the larger derivatives (accounting for sideways sampling)

	vec2 dstRes = 1.0 / duv;

	vec2 scale = dstRes * invSrc;	// (dstRes / invSrc)
    
	vec2 scaleFactor = floor(scale) + 1.0;	// add one to integer scale factor so we scale down, not up

	vec2 texelCoord = uv * srcRes;	// coordinate in texel space

	vec2 roundCoord = floor(texelCoord) + 0.5;	// center of nearest texel of uv
	vec2 fractCoord = fract(texelCoord) - 0.5;	// offset from nearest texel
    
	vec2 offset = abs(fractCoord * scaleFactor); // absolute offset multiplied by scale factor
	offset -= 0.5 * (scaleFactor - 1.0);	// subtract border of scaled texel
	offset = max(offset, 0.0) * sign(fractCoord);	// get rid of wrong direction offsets, restore sign

	return (roundCoord + offset) * invSrc;
}

void main() {
	fragColor = vec4(texture(diffuse, fuv).rgb, 1.0);
}
