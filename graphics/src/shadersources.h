const char* VERTEX_SOURCE = R"(
	#version 330
	precision highp float;

	uniform mat4 MVP;
	layout(location = 0) in vec3 vp;

	void main() {
		gl_Position = vec4(vp.x, vp.y, vp.z, 1) * MVP;
	}
)";

const char* FRAGMENT_SOURCE = R"(
	#version 330
	precision highp float;
	
	uniform vec3 color;
	out vec4 outColor;

	void main() {
		outColor = vec4(color,1);
	}
)";

const char* GEOMETRY_SOURCE = R"(
	#version 330 

	layout(triangles) in;
	layout(triangle_strip, max_vertices = 3) out;

	void main()
	{
		for (int i = 0; i < 3; i++) {
			gl_Position = gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
)";