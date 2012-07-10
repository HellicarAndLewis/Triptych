/**
 *  Bloom.cpp
 *
 *  Created by Marek Bereza on 05/10/2011.
 */

#include "Bloom.h"
void tricks::gl::effects::Bloom::setup(bool drawToFbo) {
	this->drawToFbo = drawToFbo;
	blurX = ofVec2f( 0.001953125, 0.0 );
	blurY = ofVec2f( 0.0, 0.001953125 );
	resize(ofGetWidth(), ofGetHeight());
	loadShader();
	amount = 0.5;
	brightness = 0.8;
}

void tricks::gl::effects::Bloom::resize(int w, int h) {
	output.allocate(w, h, GL_RGBA);
	out1.allocate(w, h, GL_RGBA);
	out2.allocate(w, h, GL_RGBA);
}

void tricks::gl::effects::Bloom::begin() {
	output.begin();
	ofClear(0, 0, 0, 255);
}

void tricks::gl::effects::Bloom::end() {
	output.end();
	
	
	out1.begin();
	ofClear(0, 0, 0, 0);
	// convolution 1
	shader.begin();
	shader.setUniformTexture("tDiffuse", output.getTextureReference(0), 0);
	shader.setUniform2f("uImageIncrement", blurX.x*ofGetWidth(), blurX.y*ofGetHeight());
	//	shader.setUniform1fv("cKernel", values);
	ofSetHexColor(0xFFFFFF);
	output.draw(0, 0);
	shader.end();
	out1.end();
	
	
	out2.begin();
	ofClear(0, 0, 0, 0);
	// convolution 2
	shader.begin();
	shader.setUniformTexture("tDiffuse", output.getTextureReference(0), 0);
	shader.setUniform2f("uImageIncrement", blurY.x*ofGetWidth(), blurY.y*ofGetHeight());
	//	shader.setUniform1fv("cKernel", values);
	ofSetHexColor(0xFFFFFF);
	out1.draw(0, 0);
	shader.end();
	out2.end();
	
	if(drawToFbo) {
		out1.begin();
		ofClear(0, 0, 0, 0);
	}
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetHexColor(0xFFFFFF);	
	//out1.draw(0, 0);
	//out1.draw(0, 0);
	glColor4f(1, 1, 1, amount);
	out2.draw(0, 0);
	//// draw over
	glColor4f(1,1,1, brightness);
	
	output.draw(0, 0);
	if(drawToFbo) {
		out1.end();
	}
	
}

ofFbo *tricks::gl::effects::Bloom::getOutput() {
	return &out1;
}
void tricks::gl::effects::Bloom::loadShader() {
	string vert = "#define KERNEL_SIZE 25.0\n"
		"uniform vec2 uImageIncrement;\n"
		"void main() {\n"
			"gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"

			"gl_Position = ftransform();\n"
			
		"}\n";
	
	string frag = 
	
"#define KERNEL_SIZE 25\n"
	
"	uniform sampler2DRect tDiffuse;\n"
"	uniform vec2 uImageIncrement;\n"
	
	
	
"	void main() {\n"
		
"		vec2 imageCoord = gl_TexCoord[0].st-uImageIncrement*12.0;\n"
		
		
"		vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );\n"
"		float t = 1.5;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.001110*t;\n"
"		imageCoord += uImageIncrement;		\n"

		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.002277*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.004390*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.007949*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.013521*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.021607*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.032436*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.045741*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.060597*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.075415*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.088169*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.096835*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.099908*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.096835*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.088169*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.075415*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.060597*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.045741*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.032436*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.021607*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.013521*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.007949*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.004390*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.002277*t;\n"
"		imageCoord += uImageIncrement;\n"
		
"		sum += texture2DRect( tDiffuse, imageCoord ) * 0.001110*t;\n"
		
"		gl_FragColor = sum;\n"
//"		if(sum.r>0.0) gl_FragColor.a = 1.0; \n"
	"}\n";
	
	//printf("%s\n", frag.c_str());
	// type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER_EXT etc.
	shader.setupShaderFromSource(GL_VERTEX_SHADER, vert);
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
	shader.linkProgram();

}