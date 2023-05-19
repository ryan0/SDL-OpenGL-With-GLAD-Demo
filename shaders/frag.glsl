#version 330 core
out vec4 FragColor;

in vec2 UV;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixVal;

void main() {
    FragColor = mix(texture(texture1, UV), texture(texture2, vec2(-UV.x, UV.y)), mixVal);
}
