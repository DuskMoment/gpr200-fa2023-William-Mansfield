#version 450
	out vec4 FragColor;
	uniform vec3 _Color;
	uniform float _Brightness;
	uniform vec2 _Resolution;
	in vec2 UV;
	void main(){

	//use comments to break up mixes
		FragColor = vec4(UV,0.0,1.0);
	}

//	// partialy completed shadercallcoherent
//   
//void mainImage( out vec4 fragColor, in vec2 fragCoord )
//{
//    // Normalized pixel coordinates (from 0 to 1)
//     vec2 uv = fragCoord/iResolution.xy;
//     
//     uv = uv*2.0-1.0;
//     float aspectRatio =  iResolution.x/ iResolution.y;
//     uv.x*=aspectRatio;
//     float t = sin(iTime);
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