uniform sampler2D texture;  // Текстура экрана
uniform float time;         // Время для эффекта волны
uniform float bias;         // Сила закручивания (от 0.0 до, скажем, 80.0 для сильного эффекта)
uniform vec2 resolution;    // Разрешение экрана (для нормализации)
//uniform float angle;        // Угол поворота украта. Насколько сильно завален горизонт.

uniform float pixelizeCoeff;// Коофициент пикселезиции картинки чем больше тем картинка менее пикселизуеца.
// uniform bool isDrunk;       
// uniform float coeffOfDrunk; // Насколько сильно ты напился

vec2 drunkEffect() {
    vec2 uv = (gl_FragCoord.xy + vec2(40, 0)) / resolution;
    float wave = sin(time * 0.001 + uv.y * 5.0) * (bias / 5. / resolution.x);
     
    vec2 newUV = uv + vec2(wave, 0);;
    return newUV;
}

vec4 pixelEffect(vec4 color) {
    if (mod(gl_FragCoord.y, 2.0 * pixelizeCoeff) < 1.0) {  
        color.rgb *= 0.3;
    }

    if (mod(gl_FragCoord.x, 2.0 * pixelizeCoeff) < 1.0) {  
        color.rgb *= 0.3;
    }

    return color;
}

void main() {
    vec2 uv = (gl_FragCoord.xy ) / resolution;
    vec2 newUV = uv;

    bool isDrunk = false;
    if(isDrunk) {
        newUV = drunkEffect();
    }
    
    vec4 color = texture2D(texture, newUV);
    if(newUV.x > 1. || newUV.y > 1.) {
        color.a = 0.;
    }
    color = pixelEffect(color);
    
    gl_FragColor = color;
}