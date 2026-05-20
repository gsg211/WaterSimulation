#version 400

layout(location = 0) in vec3 vPos;


uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelMatrix;   
uniform float time;

#define WIND_DIR 0.785 
#define NUM_WAVES 8

out vec3 normal;
out vec3 pos;


vec3 calculateWave(vec2 p, float angle, float wavelength, float speed, float amp) {
    float totalAngle = angle + WIND_DIR;

    //wavelength density
    //k = 2π / wavelength 
    float k = 2.0 * 3.14159 / wavelength;

    vec2 dir = vec2(cos(totalAngle), sin(totalAngle));
    
    float phase = k * dot(dir, p) - (time * speed);
    float wave = exp(sin(phase) - 1.0); 
    
    float h = wave * amp;

    //derivative of the wave
    float df = k * h * cos(phase); 
   
    return vec3(h, dir.x * df, dir.y * df);
}

void main() {
    vec3 v = vPos;
    vec2 p = v.xz;

    float totalHeight = 0.0;
    float totalDX = 0.0;
    float totalDZ = 0.0;

  
    //staring params
    float curWavelength = 20.0; 
    float curAmp = 0.8;        
    float curSpeed = 1.2;       
    float angleStep = 1.0;      

    for(int i = 0; i < NUM_WAVES; i++) {
        float waveAngle = float(i) * angleStep;
        
        vec3 wave = calculateWave(p, waveAngle, curWavelength, curSpeed, curAmp);
        
        totalHeight += wave.x;
        totalDX += wave.y;
        totalDZ += wave.z;

        // fractional brownian motion scaling
        curWavelength *= 0.65; 
        curAmp *= 0.55;       
        curSpeed *= 1.3;     

    }

    v.y = totalHeight;


    vec3 rawNormal = normalize(vec3(-totalDX, 1.0, -totalDZ)); 
    
    pos = v;
    normal = rawNormal;
    gl_Position = mvpMatrix * vec4(v, 1.0);
}