#version 100

precision mediump float;
uniform mat4 proj;
uniform mat4 view;
attribute vec2 pos;
attribute vec4 color;
varying vec4 v_color;

void main() {
	v_color = color;
	gl_Position = proj * view * vec4(pos, 0.0, 1.0);
}
