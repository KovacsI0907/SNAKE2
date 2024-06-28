const char* PASSTHROUGH_VERTEX = R"(
	#version 330
	precision highp float;

	uniform mat4 MVP;
	layout(location = 0) in vec3 vp;

	void main() {
		gl_Position = vec4(vp.x, vp.y, vp.z, 1) * MVP;
	}
)";

const char* PASSTHROUGH_FRAGMENT = R"(
	#version 330
	precision highp float;
	
	uniform vec3 color;
	out vec4 outColor;

	void main() {
		outColor = vec4(color,1);
	}
)";

const char* PASSTHROUGH_GEOMETRY = R"(
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

const char* GOURAUD_VERTEX = R"(
	#version 330

	uniform vec4 radAmbient;
	uniform vec4 radLight;

	uniform vec4 coeffAmbient;
	uniform vec4 coeffDiffuse;
	uniform vec4 coeffSpecular;
	
	uniform float shine;

	uniform vec4 wLightPos;
	uniform vec3 wEyePos;
	
	uniform mat4 MVP;
	uniform mat4 M;
	uniform mat4 Minv;

	layout (location = 0) in vec3 mVertexPos;
	layout (location = 1) in vec3 mNormal;

	out vec4 color;

	void main() {
		gl_Position = vec4(mVertexPos, 1) * MVP;
		
		vec4 wVertexPos = vec4(mVertexPos, 1) * M;
		vec3 L = normalize(wLightPos.xyz * wVertexPos.w - wVertexPos.xyz * wLightPos.w);
		vec3 V = normalize(wEyePos - wVertexPos.xyz/wVertexPos.w);
		vec4 wNormal = vec4(mNormal, 0) * Minv;
		vec3 N = normalize(wNormal.xyz);
		float cosAngleLN = max(dot(N, L), 0);
		vec3 halfway = normalize(L+V);
		float cosAngleHN = max(dot(N, halfway), 0);
		
		vec4 ambient = radAmbient * coeffAmbient;
		vec4 diffuse = radLight * coeffDiffuse * cosAngleLN;
		vec4 specular = radLight * coeffSpecular * cosAngleLN * pow(cosAngleHN, shine) / dot(L + V, L + V);

		color = ambient + diffuse + specular;
	}
	
)";

const char* GOURAUD_FRAGMENT = R"(
	#version 330

	in vec4 color;
	out vec4 fragmentColor;
	void main() {
		fragmentColor = color;
	}
)";

const char* PHONG_VERTEX = R"(
	#version 330

	uniform mat4 MVP;
	uniform mat4 M;
	uniform mat4 Minv;

	uniform vec4 wLightPos;
	uniform vec3 wEyePos;

	layout (location = 0) in vec3 mVertexPos;
	layout (location = 1) in vec3 mNormal;

	out vec3 wToLight;
	out vec3 wNormal;
	out vec3 wToEye;

	void main() {
		gl_Position = vec4(mVertexPos, 1) * MVP;

		vec4 wVertexPos = vec4(mVertexPos, 1) * M;
		wToLight = wLightPos.xyz*wVertexPos.w - wVertexPos.xyz*wLightPos.w;
		wNormal = (vec4(mNormal, 0) * Minv).xyz;
		wToEye = wEyePos - wVertexPos.xyz/wVertexPos.w;
	}
)";

const char* PHONG_FRAGMENT = R"(
	#version 330

	uniform vec4 radAmbient;
	uniform vec4 radLight;

	uniform vec4 coeffAmbient;
	uniform vec4 coeffDiffuse;
	uniform vec4 coeffSpecular;
	
	uniform float shine;

	in vec3 wToLight;
	in vec3 wNormal;
	in vec3 wToEye;

	out vec4 fragmentColor;

	void main() {
		vec3 L = normalize(wToLight);
		vec3 N = normalize(wNormal);
		vec3 V = normalize(wToEye);

		float cosAngleLN = max(dot(N, L), 0);
		vec3 halfway = normalize(L+V);
		float cosAngleHN = max(dot(N, halfway), 0);

		vec4 ambient = radAmbient * coeffAmbient;
		vec4 diffuse = radLight * coeffDiffuse * cosAngleLN;
		vec4 specular = radLight * coeffSpecular * cosAngleLN * pow(cosAngleHN, shine) / dot(L + V, L + V);

		fragmentColor = ambient + diffuse + specular;
	}
)";