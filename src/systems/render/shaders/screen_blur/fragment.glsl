#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screen_texture;
uniform float time;

uniform vec2 screen_resolution;

void main() {
    vec2 pos = TexCoords;
    float r = 10;
    float xs = screen_resolution.x;
    float ys = screen_resolution.y;
    float x,y,xx,yy,rr=r*r,dx,dy,w,w0;
    w0=0.3780/pow(r,1.975);
    vec2 p;
    vec4 blur=vec4(0.0,0.0,0.0,0.0);
    for (dx=1.0/xs,x=-r,p.x=pos.x+(x*dx);x<=r;x++,p.x+=dx){ xx=x*x;
     for (dy=1.0/ys,y=-r,p.y=pos.y+(y*dy);y<=r;y++,p.y+=dy){ yy=y*y;
      if (xx+yy<=rr)
        {
        w=w0*exp((-xx-yy)/(2.0*rr));
        blur+=texture(screen_texture,p)*w;
        }}}
    vec4 sample = texture(screen_texture, vec2(TexCoords.x, TexCoords.y));
    color = mix(sample, blur, abs(TexCoords.y*2 - 1)*abs(TexCoords.y*2 - 1));
    // color = mix(vec4(1), vec4(0), abs(TexCoords.y*2 - 1)*abs(TexCoords.y*2 - 1));
}