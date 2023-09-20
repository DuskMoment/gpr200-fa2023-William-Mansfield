#version 450
	out vec4 FragColor;
	uniform vec3 _Color;
	uniform float _Brightness;
	uniform vec2 _Resolution;
	uniform float _Time;

    uniform vec3 _BgTopColor;
    uniform vec3 _BgBottomColor;

    uniform vec3 _SunTopColor;
    uniform vec3 _SunBottomColor;

    uniform vec3 _FgColor;

	in vec2 UV;
	void main(){

	//use comments to break up mixes

		vec2 uv = UV;
		vec2 iResolution = _Resolution;
		float iTime = _Time;
        vec3 bgTopColor = _BgTopColor;
        vec3 bgBottomColor = _BgBottomColor;
        vec3 sunTopColor = _SunTopColor;
        vec3 sunBottomColor = _SunBottomColor;
        vec3 fgColor = _FgColor;

        float t = sin(iTime);

		uv = uv*2.0-1.0;
		float aspectRatio =  iResolution.x/ iResolution.y;
        uv.x*=aspectRatio;

          
    //backround
       float edge = t*0.8-0.2;
       float z = smoothstep(t-1.0,t+1.0,uv.y);
       
       vec3 color = mix(bgBottomColor,bgTopColor,z);

       //sun
       float sun = distance(uv,vec2(0.0,t - 0.6));
       sun = smoothstep(0.4,0.7,sun);
       vec3 sunColor = mix(sunBottomColor,sunTopColor,t);
        
       color = mix(sunColor,color, sun);

        //forground
        float hills = step(sin(uv.x*6.0) * 0.1 - 0.5,uv.y);
        
        color = mix(fgColor,color,hills);

		FragColor = vec4(color,1.0);
	}

//	// partialy completed shadercallcoherent
   
//void mainImage( out vec4 fragColor, in vec2 fragCoord )
//{
//    // Normalized pixel coordinates (from 0 to 1)
//     vec2 uv = fragCoord/iResolution.xy;
//     float t = sin(iTime);
//     uv = uv*2.0-1.0;
//     float aspectRatio =  iResolution.x/ iResolution.y;
//     uv.x*=aspectRatio;
//     
//  
//     
//    //backround
//      float edge = t*0.8-0.2;
//      float z = smoothstep(t-1.0,t+1.0,uv.y);
//       
//      vec3 color = mix(vec3(1.0,0.5,0.3),vec3(0.1,0.1,0.2),z);
//       
//      //sun
//       float sun = distance(uv,vec2(0.0,t - 0.6));
//        sun = smoothstep(0.4,0.7,sun);
//        vec3 sunColor = mix(vec3(0.6,0.0,0.0),vec3(0.8,0.6,0.0),t);
//        
//        color = mix(sunColor,color, sun);
//        
//        //forground
//        float hills = step(sin(uv.x*6.0) * 0.1 - 0.5,uv.y);
//        
//        color = mix(vec3(0.2),color,hills);
//
//
//  
//    // Output to screen
//   fragColor = vec4(color,1.0);
//}