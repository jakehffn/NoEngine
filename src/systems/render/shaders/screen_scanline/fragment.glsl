#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screen_texture;
uniform vec2 screen_resolution;
uniform float time;

// uniform float density;
// uniform float opacity_scanline;
// uniform float opacity_noise;
// uniform float flickering;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

vec4 sampleTexture(float x, float y) {
    vec2 sample_pixel_center = vec2(ivec2(x, y)) + vec2(0.5, 0.5);
    return texture(screen_texture, sample_pixel_center/screen_resolution);
}

float density = 1.3;
float opacity_scanline = .3;
float opacity_noise = .3;
float flickering = 0.1;

// float blend(const in float x, const in float y) {
// 	return (x < 0.5) ? (2.0 * x * y) : (1.0 - 2.0 * (1.0 - x) * (1.0 - y));
// }

// vec3 blend(const in vec3 x, const in vec3 y, const in float opacity) {
// 	vec3 z = vec3(blend(x.r, y.r), blend(x.g, y.g), blend(x.b, y.b));
// 	return z * opacity + x * (1.0 - opacity);
// }

void main() {   

    // vec2 pixel = vec2(TexCoords.x, 1-TexCoords.y) * screen_resolution;

    // color = sampleTexture(pixel.x, pixel.y) * clamp(mod(pixel.y + int(time / 1000), 3) - mod(time / 1000, 3), 0.7, 1);

    vec2 uv = gl_FragCoord.xy/screen_resolution.xy;
    vec3 col = texture(screen_texture, uv).rgb;
    
    float count = screen_resolution.y * density;
    vec2 sl = vec2(sin(uv.y * count), cos(uv.y * count));
	vec3 scanlines = vec3(sl.x, sl.y, sl.x);

    col += col * scanlines * opacity_scanline;
    col += col * vec3(random(uv*time)) * opacity_noise;
    col += col * sin(110.0*time) * flickering;

    color = vec4(col,1.0);
} 