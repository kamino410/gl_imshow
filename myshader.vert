#version 450 core

in vec3 VertexPosition;
in vec2 TextureCoordinate;

out vec2 texcoord;

void main() {
    gl_Position = vec4(VertexPosition, 1.0);
    texcoord = TextureCoordinate;
}
