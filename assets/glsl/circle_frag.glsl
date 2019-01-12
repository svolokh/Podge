#version 100

precision mediump float;
varying vec2 v_pos;
varying vec2 v_origin;
varying float v_radius;
varying vec4 v_color;

void main() {
 	float dist = (v_pos.x - v_origin.x)*(v_pos.x - v_origin.x) + (v_pos.y - v_origin.y)*(v_pos.y - v_origin.y);
 	if(dist <= v_radius*v_radius) {
 		gl_FragColor = v_color;
 	} else {
 		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
 	}
}
