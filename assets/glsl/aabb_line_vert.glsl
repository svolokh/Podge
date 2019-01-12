#version 100

uniform mat4 proj;
uniform mat4 view;
uniform vec2 first_pos;
uniform vec2 second_pos;
attribute float t;

void main() {
	gl_Position = proj * view * vec4((1.0 - t) * first_pos  + t * second_pos, 0.0, 1.0);
}
