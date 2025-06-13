#ifndef _GL_SHADER_H_
#define _GL_SHADER_H_

// Vertex shader.
static const char kVertexShader[] =
        "#version 100\n\
    varying vec2 v_texcoord; \
    attribute vec4 position; \
    attribute vec4 texcoord; \
    uniform mat4 scale; \
    uniform mat4 rotation; \
    void main() { \
        vec4 transformed = rotation * scale * vec4(texcoord.xy - 0.5, 0.0, 1.0); \
        v_texcoord = transformed.xy + 0.5; \
        gl_Position = position; \
    }";

// Pixel shader, YUV420 to RGB conversion.
static const char kFragmentShader[] =
        "#version 100\n \
    precision highp float; \
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform float u_threshold;\
\
    void main() {\
        float y, u, v, r, g, b, brightness;\
        y = texture2D(s_textureY, v_texcoord).r;\
        u = texture2D(s_textureU, v_texcoord).r;\
        v = texture2D(s_textureV, v_texcoord).r;\
        u = u - 0.5;\
        v = v - 0.5;\
        r = y + 1.403 * v;\
        g = y - 0.344 * u - 0.714 * v;\
        b = y + 1.770 * u;\
        gl_FragColor = vec4(r, g, b, 1.0);\
    }";

// causes all pixels to be green under a threshold set by slider bar
static const char kFragmentShader0[] =
        "#version 100\n \
precision highp float; \
varying vec2 v_texcoord;\
uniform lowp sampler2D s_textureY;\
uniform lowp sampler2D s_textureU;\
uniform lowp sampler2D s_textureV;\
uniform float u_threshold;\
void main() {\
    float y, u, v, r, g, b,  brightness;\
    y = texture2D(s_textureY, v_texcoord).r;\
    u = texture2D(s_textureU, v_texcoord).r - 0.5;\
    v = texture2D(s_textureV, v_texcoord).r - 0.5;\
    r = y + 1.403 * v;\
    g = y - 0.344 * u - 0.714 * v;\
    b = y + 1.770 * u;\
    brightness = (r + g + b) / 3.0;\
    if (brightness < u_threshold) {\
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); /* green */ \
    } else {\
        gl_FragColor = vec4(r, g, b, 1.0);\
    }\
}";

// Blur Filter
static const char kFragmentShader1[] =
        "#version 100\n \
    precision highp float; \
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform float u_threshold;\
    vec4 YuvToRgb(vec2 uv) {\
        float y, u, v, r, g, b;\
        y = texture2D(s_textureY, uv).r;\
        u = texture2D(s_textureU, uv).r;\
        v = texture2D(s_textureV, uv).r;\
        u = u - 0.5;\
        v = v - 0.5;\
        r = y + 1.403 * v;\
        g = y - 0.344 * u - 0.714 * v;\
        b = y + 1.770 * u;\
        return vec4(r, g, b, 1.0);\
    }\
    void main() {\
        vec4 sample0, sample1, sample2, sample3;\
        float blurStep = u_threshold;\
        float step = blurStep / 100.0;\
        sample0 = YuvToRgb(vec2(v_texcoord.x - step, v_texcoord.y - step));\
        sample1 = YuvToRgb(vec2(v_texcoord.x + step, v_texcoord.y + step));\
        sample2 = YuvToRgb(vec2(v_texcoord.x + step, v_texcoord.y - step));\
        sample3 = YuvToRgb(vec2(v_texcoord.x - step, v_texcoord.y + step));\
        gl_FragColor = (sample0 + sample1 + sample2 + sample3) / 4.0;\
    }";

// Swirl Filter – u_threshold controls swirl angle & radius
static const char kFragmentShader2[] =
        "#version 100\n\
    precision highp float; \
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    void main() {\
        float radius = mix(100.0, 400.0, u_threshold);\
        float angle  = mix(0.2, 2.5,  u_threshold);\
        vec2 center = texSize * 0.5;\
        vec2 tc = v_texcoord * texSize - center;\
        float dist = length(tc);\
        if (dist < radius) {\
            float percent = (radius - dist) / radius;\
            float theta = percent * percent * angle * 8.0;\
            float s = sin(theta);\
            float c = cos(theta);\
            tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));\
        }\
        tc += center;\
        vec2 uv = tc / texSize;\
        float y = texture2D(s_textureY, uv).r;\
        float u = texture2D(s_textureU, uv).r - 0.5;\
        float v = texture2D(s_textureV, uv).r - 0.5;\
        vec3 rgb = vec3(y + 1.403*v, y - 0.344*u - 0.714*v, y + 1.770*u);\
        gl_FragColor = vec4(rgb,1.0);\
    }";

// Magnifying Glass – u_threshold controls zoom and circle radius
static const char kFragmentShader3[] =
        "#version 100\n \
    precision highp float; \
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    vec4 YuvToRgb(vec2 uv){\
        float y = texture2D(s_textureY, uv).r;\
        float u = texture2D(s_textureU, uv).r - 0.5;\
        float v = texture2D(s_textureV, uv).r - 0.5;\
        return vec4(y + 1.403*v, y - 0.344*u - 0.714*v, y + 1.770*u, 1.0);\
    }\
    void main(){\
        float circleRadius = mix(0.25, 0.75, u_threshold);\
        float zoom       = mix(0.4,  1.4,  u_threshold);\
        vec2 center = texSize * 0.5;\
        vec2 uv = v_texcoord;\
        uv.x *= texSize.x / texSize.y;\
        vec2 realCenter = vec2(center.x / texSize.y, center.y / texSize.y);\
        vec2 diff = uv - realCenter;\
        float dist = length(diff);\
        if (dist < circleRadius) {\
            uv = realCenter + diff * zoom;\
            uv.x *= texSize.y / texSize.x;\
        }\
        gl_FragColor = YuvToRgb(uv);\
    }";

// Fisheye – u_threshold adjusts aperture
static const char kFragmentShader4[] =
        "#version 100\n\
    precision highp float;\
    const float PI = 3.1415926535;\
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform float u_threshold;\
    void main(){\
        float aperture = mix(60.0,  180.0, u_threshold);\
        float apertureHalf = 0.5 * aperture * (PI / 180.0);\
        float maxFactor = sin(apertureHalf);\
        vec2 xy = 2.0 * v_texcoord - 1.0;\
        float d = length(xy);\
        vec2 uv = v_texcoord;\
        if(d < (2.0 - maxFactor)){\
            d *= maxFactor;\
            float z = sqrt(1.0 - d*d);\
            float r = atan(d, z) / PI;\
            float phi = atan(xy.y, xy.x);\
            uv = vec2(r*cos(phi)+0.5, r*sin(phi)+0.5);\
        }\
        float y = texture2D(s_textureY, uv).r;\
        float u = texture2D(s_textureU, uv).r - 0.5;\
        float v = texture2D(s_textureV, uv).r - 0.5;\
        vec3 rgb = vec3(y+1.403*v, y-0.344*u-0.714*v, y+1.770*u);\
        gl_FragColor = vec4(rgb,1.0);\
    }";

// Lichtenstein Dots – u_threshold controls dot size
static const char kFragmentShader5[] =
        "#version 100\n\
    precision highp float;\
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    void main(){\
        float size   = mix(texSize.x/150.0, texSize.x/30.0, u_threshold);\
        float radius = size*0.5;\
        vec2 fragCoord = v_texcoord * texSize;\
        vec2 quadPos = floor(fragCoord / size) * size;\
        vec2 quad = quadPos / texSize;\
        vec2 quadCenter = quadPos + size*0.5;\
        float dist = length(quadCenter - fragCoord);\
        float y = texture2D(s_textureY, quad).r;\
        float u = texture2D(s_textureU, quad).r - 0.5;\
        float v = texture2D(s_textureV, quad).r - 0.5;\
        vec3 rgb = vec3(y+1.403*v, y-0.344*u-0.714*v, y+1.770*u);\
        gl_FragColor = (dist > radius) ? vec4(0.25) : vec4(rgb,1.0);\
    }";

// Triangle Mosaic – u_threshold controls tile count
static const char kFragmentShader6[] =
        "#version 100\n\
    precision highp float;\
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    vec4 YuvToRgb(vec2 uv){\
        float y = texture2D(s_textureY, uv).r;\
        float u = texture2D(s_textureU, uv).r - 0.5;\
        float v = texture2D(s_textureV, uv).r - 0.5;\
        return vec4(y+1.403*v, y-0.344*u-0.714*v, y+1.770*u,1.0);\
    }\
    void main(){\
        vec2 tileNum = vec2(10.0) + u_threshold * vec2(60.0,30.0);\
        vec2 uv = v_texcoord;\
        vec2 uv2 = floor(uv * tileNum) / tileNum;\
        uv -= uv2; uv *= tileNum;\
        vec3 color = YuvToRgb(uv2 + vec2(step(1.0-uv.y, uv.x)/(2.0*tileNum.x), step(uv.x, uv.y)/(2.0*tileNum.y))).rgb;\
        gl_FragColor = vec4(color,1.0);\
    }";

// Pixelation – u_threshold controls pixel size
static const char kFragmentShader7[] =
        "#version 100\n\
    precision highp float;\
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    void main(){\
        float maxPx = texSize.x / 10.0;\
        float minPx = texSize.x / 250.0;\
        float px = mix(minPx, maxPx, u_threshold);\
        vec2 pixelSize = vec2(px, px * texSize.y/texSize.x);\
        vec2 coord = vec2(pixelSize.x * floor(v_texcoord.x * texSize.x / pixelSize.x) / texSize.x,\
                          pixelSize.y * floor(v_texcoord.y * texSize.y / pixelSize.y) / texSize.y);\
        float y = texture2D(s_textureY, coord).r;\
        float u = texture2D(s_textureU, coord).r - 0.5;\
        float v = texture2D(s_textureV, coord).r - 0.5;\
        vec3 rgb = vec3(y+1.403*v, y-0.344*u-0.714*v, y+1.770*u);\
        gl_FragColor = vec4(rgb,1.0);\
    }";

// Cross‑stitching – u_threshold controls stitch size
static const char kFragmentShader8[] =
        "#version 100\n\
    precision highp float;\
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    vec4 YuvToRgb(vec2 uv){\
        float y = texture2D(s_textureY, uv).r;\
        float u = texture2D(s_textureU, uv).r - 0.5;\
        float v = texture2D(s_textureV, uv).r - 0.5;\
        return vec4(y+1.403*v, y-0.344*u-0.714*v, y+1.770*u,1.0);\
    }\
    vec4 CrossStitching(vec2 uv){\
        float stitchSize = mix(texSize.x/80.0, texSize.x/20.0, u_threshold);\
        float size = stitchSize;\
        vec2 cPos = uv * texSize;\
        vec2 tlPos = floor(cPos / size) * size;\
        vec2 blPos = tlPos + vec2(0.0, size-1.0);\
        int remX = int(mod(cPos.x, size));\
        int remY = int(mod(cPos.y, size));\
        bool diag = (remX == remY) || ((int(cPos.x)-int(blPos.x)) == (int(blPos.y)-int(cPos.y)));\
        return diag ? YuvToRgb(tlPos / texSize) * 1.4 : vec4(0.15,0.1,0.05,1.0);\
    }\
    void main(){ gl_FragColor = CrossStitching(v_texcoord);}";

// Toonify – u_threshold controls edge & quantisation strength
static const char kFragmentShader9[] =
        "#version 100\n\
    precision highp float;\
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    const int kHueLevCount=6; const int kSatLevCount=7; const int kValLevCount=4;\
    float hueLevels[kHueLevCount]; float satLevels[kSatLevCount]; float valLevels[kValLevCount];\
    float edge_thres; float edge_gain;\
    vec4 YuvToRgb(vec2 uv){\
        float y=texture2D(s_textureY,uv).r; float u=texture2D(s_textureU,uv).r-0.5; float v=texture2D(s_textureV,uv).r-0.5;\
        return vec4(y+1.403*v, y-0.344*u-0.714*v, y+1.770*u,1.0);}\
    float avgIntensity(vec3 c){return (c.r+c.g+c.b)/3.0;}\
    void main(){\
        edge_thres = mix(0.02,0.4,u_threshold); edge_gain = mix(3.0,10.0,u_threshold);\
        // initialise levels\
        hueLevels[0]=0.;hueLevels[1]=140.;hueLevels[2]=160.;hueLevels[3]=240.;hueLevels[4]=300.;hueLevels[5]=360.;\
        satLevels[0]=0.;satLevels[1]=.15;satLevels[2]=.3;satLevels[3]=.45;satLevels[4]=.6;satLevels[5]=.8;satLevels[6]=1.;\
        valLevels[0]=0.;valLevels[1]=.3;valLevels[2]=.6;valLevels[3]=1.;\
        vec3 c = YuvToRgb(v_texcoord).rgb; float lum = avgIntensity(c);\
        // edge detect 3x3 sobel lite\
        vec2 px = 1.0/texSize; float horiz = 0.0; float vert = 0.0;\
        for(int i=-1;i<=1;i++){for(int j=-1;j<=1;j++){ float w = float((i+j)==0?2:1); vec3 s = YuvToRgb(v_texcoord+vec2(i,j)*px).rgb; float l = avgIntensity(s); horiz += w*l*float(i); vert += w*l*float(j);} }\
        float edge = edge_gain*length(vec2(horiz,vert)); edge = clamp(edge,0.,1.);\
        // simple posterisation\
        float levels = mix(4.,9.,u_threshold); c = floor(c*levels)/levels;\
        vec3 finalCol = mix(c, vec3(0.), edge>edge_thres ? 1.0 : 0.0);\
        gl_FragColor = vec4(finalCol,1.0);\
    }";

// Predator Thermal – u_threshold controls palette stretch
static const char kFragmentShader10[] =
        "#version 100\n\
    precision highp float;\
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform float u_threshold;\
    void main(){\
        float y = texture2D(s_textureY, v_texcoord).r;\
        float u = texture2D(s_textureU, v_texcoord).r - 0.5;\
        float v = texture2D(s_textureV, v_texcoord).r - 0.5;\
        float lum = clamp((y + 1.403*v + y - 0.344*u - 0.714*v + y + 1.770*u)/3.,0.,1.);\
        lum = pow(lum, mix(0.5,2.0,u_threshold)); // adjust contrast via threshold\
        vec3 colors[3]; colors[0]=vec3(0.,0.,1.); colors[1]=vec3(1.,1.,0.); colors[2]=vec3(1.,0.,0.);\
        float t = lum*2.0; int idx = int(floor(t)); idx = clamp(idx,0,1);\
        vec3 rgb = mix(colors[idx],colors[idx+1],t-float(idx));\
        gl_FragColor = vec4(rgb,1.0);\
    }";

// Emboss – u_threshold controls emboss strength
static const char kFragmentShader11[] =
        "#version 100\n \
    precision highp float; \
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    vec4 YuvToRgb(vec2 uv){\
        float y=texture2D(s_textureY,uv).r; float u=texture2D(s_textureU,uv).r-0.5; float v=texture2D(s_textureV,uv).r-0.5;\
        return vec4(y+1.403*v, y-0.344*u-0.714*v, y+1.770*u,1.0);}\
    void main(){\
        float strength = mix(2.0,10.0,u_threshold);\
        vec2 px = vec2(1.0)/texSize;\
        vec4 col = vec4(0.5);\
        col -= YuvToRgb(v_texcoord - px) * strength;\
        col += YuvToRgb(v_texcoord + px) * strength;\
        col.rgb = vec3((col.r+col.g+col.b)/3.0);\
        gl_FragColor = vec4(col.rgb,1.0);\
    }";

// Edge Detection – u_threshold controls edge intensity
static const char kFragmentShader12[] =
        "#version 100\n \
    precision highp float; \
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    uniform vec2 texSize;\
    uniform float u_threshold;\
    vec4 YuvToRgb(vec2 uv){\
        float y=texture2D(s_textureY,uv).r; float u=texture2D(s_textureU,uv).r-0.5; float v=texture2D(s_textureV,uv).r-0.5;\
        return vec4(y+1.403*v, y-0.344*u-0.714*v, y+1.770*u,1.0);}\
    void main(){\
        vec2 onePixel = vec2(1.0)/texSize;\
        float kernel[9]; kernel[0]=-1.;kernel[1]=-1.;kernel[2]=-1.;kernel[3]=-1.;kernel[4]=8.;kernel[5]=-1.;kernel[6]=-1.;kernel[7]=-1.;kernel[8]=-1.;\
        vec3 sum=vec3(0.0); int idx=0;\
        for(int i=-1;i<=1;i++){for(int j=-1;j<=1;j++){ vec3 c = YuvToRgb(v_texcoord+vec2(i,j)*onePixel).rgb; sum += c*kernel[idx++]; }}\
        sum = clamp(sum * mix(0.2,2.0,u_threshold) + 0.5, 0.0,1.0);\
        gl_FragColor = vec4(sum,1.0);\
    }";

#endif //_GL_SHADER_H_
