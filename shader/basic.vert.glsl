#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 color;
layout (location = 3) in int texture;
layout (location = 4) in int flags;

layout (location = 0) out vec2 fuv;
layout (location = 1) out vec4 fcolor;
layout (location = 2) out int ftexture;
layout (location = 3) out int fflags;

void main() {
	fuv = uv;
	fcolor = color;
	ftexture = texture;
	fflags = flags;

	gl_Position = vec4(position);
}
