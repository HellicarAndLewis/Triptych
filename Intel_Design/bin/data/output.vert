
#define KERNEL_SIZE 25.0

uniform vec2 uImageIncrement;

void main() {
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = ftransform();
			
}