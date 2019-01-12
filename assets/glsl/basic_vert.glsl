#version 100

precision mediump float;
uniform mat4 proj;
uniform mat4 view;
attribute vec2 pos;
attribute vec2 tc;
varying vec2 v_tc;

void main() {
	v_tc = tc;
	gl_Position = proj * view * vec4(pos, 0.0, 1.0);
}
