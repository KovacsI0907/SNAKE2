#pragma once

inline const char* PASSTHROUGH_VERTEX = R"(
	#version 330
	precision highp float;

	uniform mat4 MVP;
	layout(location = 0) in vec3 vp;

	void main() {
		gl_Position = vec4(vp.x, vp.y, vp.z, 1) * MVP;
	}
)";

inline const char* PASSTHROUGH_FRAGMENT = R"(
	#version 330
	precision highp float;
	
	uniform vec3 color;
	out vec4 outColor;

	void main() {
		outColor = vec4(color,1);
	}
)";

inline const char* PASSTHROUGH_GEOMETRY = R"(
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

inline const char* GOURAUD_VERTEX = R"(
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

	uniform mat4 lightMVP;

	layout (location = 0) in vec3 mVertexPos;
	layout (location = 1) in vec3 mNormal;
	layout (location = 2) in vec2 uv;

	out vec4 diffuseMult;
	out vec4 ambient;
	out vec4 specular;
	out vec4 diffuse;
	out vec2 texCoord;
	out vec3 lightSpacePos;

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
		
		ambient = radAmbient * coeffAmbient;
		diffuse = radLight * coeffDiffuse * cosAngleLN;
		diffuseMult = radLight * cosAngleLN;
		specular = radLight * coeffSpecular * cosAngleLN * pow(cosAngleHN, shine) / dot(L + V, L + V);

		texCoord = uv;

		vec4 lightSpacePos4 = vec4(mVertexPos, 1) * lightMVP;
		lightSpacePos = lightSpacePos4.xyz / lightSpacePos4.w;
		lightSpacePos = lightSpacePos * 0.5 + 0.5;
	}
	
)";

inline const char* GOURAUD_FRAGMENT = R"(
	#version 330

	uniform sampler2D diffuseTexture;
	uniform sampler2D depthMap;
	uniform bool hasTexture;

	in vec4 ambient;
	in vec4 specular;
	in vec4 diffuseMult;
	in vec4 diffuse;
	in vec2 texCoord;
	in vec3 lightSpacePos;

	out vec4 fragmentColor;
	void main() {

		float depth = texture(depthMap, lightSpacePos.xy).r;
		float eps = 0.005f;
		float shadowMult = depth < (lightSpacePos.z - eps) ? 0.0f : 1.0f;

		if(hasTexture){
			fragmentColor = ambient + shadowMult * (specular + diffuseMult * texture(diffuseTexture, texCoord));
		}else{
			fragmentColor = ambient + shadowMult * (specular + diffuse);
		}
	}
)";

inline const char* PHONG_VERTEX = R"(
	#version 330

	uniform mat4 MVP;
	uniform mat4 M;
	uniform mat4 Minv;

	uniform vec4 wLightPos;
	uniform vec3 wEyePos;

	uniform mat4 lightMVP;

	layout (location = 0) in vec3 mVertexPos;
	layout (location = 1) in vec3 mNormal;
	layout (location = 2) in vec2 uv;

	out vec3 wToLight;
	out vec3 wNormal;
	out vec3 wToEye;
	out vec2 texCoord;
	out vec3 lightSpacePos;

	void main() {
		gl_Position = vec4(mVertexPos, 1) * MVP;

		vec4 wVertexPos = vec4(mVertexPos, 1) * M;
		wToLight = wLightPos.xyz*wVertexPos.w - wVertexPos.xyz*wLightPos.w;
		wNormal = (vec4(mNormal, 0) * Minv).xyz;
		wToEye = wEyePos - wVertexPos.xyz/wVertexPos.w;
		texCoord = uv;

		vec4 lightSpacePos4 = vec4(mVertexPos, 1) * lightMVP;
		lightSpacePos = lightSpacePos4.xyz / lightSpacePos4.w;
		lightSpacePos = lightSpacePos * 0.5 + 0.5;
	}
)";

inline const char* PHONG_FRAGMENT = R"(
	#version 330

	uniform vec4 radAmbient;
	uniform vec4 radLight;

	uniform vec4 coeffAmbient;
	uniform vec4 coeffDiffuse;
	uniform vec4 coeffSpecular;
	
	uniform float shine;

	uniform sampler2D diffuseTexture;
	uniform bool hasTexture;

	uniform sampler2D depthMap;

	in vec3 wToLight;
	in vec3 wNormal;
	in vec3 wToEye;
	in vec3 lightSpacePos;

	in vec2 texCoord;

	out vec4 fragmentColor;

	void main() {
		vec3 L = normalize(wToLight);
		vec3 N = normalize(wNormal);
		vec3 V = normalize(wToEye);

		float cosAngleLN = max(dot(N, L), 0);
		vec3 halfway = normalize(L+V);
		float cosAngleHN = max(dot(N, halfway), 0);

		vec4 ambient = radAmbient * coeffAmbient;

		vec4 diffuse = radLight * cosAngleLN;
		if(hasTexture){
			diffuse *= texture(diffuseTexture, texCoord);
		}else{
			diffuse *= coeffDiffuse;
		}

		vec4 specular = radLight * coeffSpecular * cosAngleLN * pow(cosAngleHN, shine) / dot(L + V, L + V);

		float depth = texture(depthMap, lightSpacePos.xy).r;
		float eps = 0.005f;
		float shadowMult = depth < (lightSpacePos.z - eps) ? 0.0f : 1.0f;

		fragmentColor = ambient + shadowMult * (diffuse + specular);
	}
)";

inline const char* DEPTH_VERTEX = R"(
	#version 330
	layout (location = 0) in vec3 vertex;
	uniform mat4 lightMVP;

	void main()
	{
		gl_Position = vec4(vertex, 1) * lightMVP;
	}
)";

inline const char* DEPTH_FRAGMENT = R"(
	#version 330

	void main()
	{             
		// gl_FragDepth = gl_FragCoord.z;
	}  
)";