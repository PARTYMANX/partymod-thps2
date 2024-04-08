#version 450
//#extension GL_EXT_nonuniform_qualifier : enable

layout (binding = 0) uniform sampler2D diffuse[2048];	// kind of annoying to keep this size around but i don't want to write the code to get the feature

layout (location = 0) in vec2 fuv;
layout (location = 1) in vec4 fcolor;
layout (location = 2) flat in int ftexture;
layout (location = 3) flat in int fflags;

layout (location = 0) out vec4 fragColor;

void main() {
	if (ftexture >= 0) {
		vec4 vertcolor = fcolor;
		vec4 texcolor = texture(diffuse[ftexture], fuv);
		if (texcolor.a == 0.0) {
			if (texcolor.rgb == vec3(0.0) || fcolor.a == 1.0) {
				discard;
			}
		}

		if ((fflags & 1) != 0) {
			fragColor = vertcolor * texcolor;
		} else {
			fragColor = vertcolor * texcolor * vec4(vec3(2.0), 1.0);
		}
	} else {
		fragColor = fcolor;
	}
	
}
