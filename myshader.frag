#version 450 core

in vec2 texcoord;

out vec4 FragColor;
  
uniform sampler2D tex;

void main() {
    FragColor = texture(tex, texcoord);
    // FragColor = vec4(texcoord, 0.0, 0.5);
    // FragColor = vec4(1.0, 0.0, 0.0, 0.5);
}
