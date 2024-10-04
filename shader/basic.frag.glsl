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
		float alpha;

		if (texcolor.a == 0.0) {
			discard;
		} 

		if ((fflags & 4) != 0) {	// using alpha
			if ((fflags & 8) != 0) {	// doing semitransparent pass
				if (texcolor.a > 0.75) {
					discard;
				}
			} else {	// doing opaque pass
				if (texcolor.a <= 0.75) {
					discard;
				}
			}
		}

		alpha = fcolor.a * (texcolor.a * 2.0);

		vec4 outcolor;
		if ((fflags & 1) != 0) {
			outcolor = vertcolor * texcolor;
		} else {
			outcolor = vertcolor * texcolor * vec4(vec3(2.0), 1.0);
		}

		fragColor = vec4(outcolor.rgb, alpha);
	} else {
		fragColor = fcolor;
	}
	
}
