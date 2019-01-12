#version 100

precision mediump float;
uniform mat4 proj;
uniform mat4 view;
attribute vec2 pos;
attribute vec2 origin;
attribute float radius;
attribute vec4 color;
varying vec2 v_pos;
varying vec2 v_origin;
varying float v_radius;
varying vec4 v_color;

void main() {
	v_pos = pos;
	v_origin = origin;
	v_radius = radius;
	v_color = color;
	gl_Position = proj * view * vec4(pos, 0.0, 1.0);
}
