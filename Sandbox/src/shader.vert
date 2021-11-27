#version 450

layout(binding = 0) uniform UniformBufferObject {
	vec3 colorA;
	vec3 colorB;
	vec3 colorC;
	vec3 colorD;
} ubo;

layout(location = 0) in vec2 aPosition;
//layout(location = 1) in vec3 aColor;

layout(location = 0) out vec3 vColor;

void main()
{
	switch (gl_VertexIndex)
	{
	case 0:
		vColor = ubo.colorA;
		break;
	case 1:
		vColor = ubo.colorB;
		break;
	case 2:
		vColor = ubo.colorC;
		break;
	case 3:
		vColor = ubo.colorD;
		break;
	}

	//vColor = ubo.colorD;
	
	//vColor = aColor + vec3(0.25, 0.6, 0.8);
	float color = (gl_VertexIndex % 5) / 5.0;
	//vColor = vec3(color, color, color);

	//gl_Position = ubo.model * ubo.view * ubo.proj * vec4(aPosition, 0.0, 1.0);	
	gl_Position = vec4(aPosition, 0.0, 1.0);
}