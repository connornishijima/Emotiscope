CRGBF tunnel_image[NUM_LEDS];
CRGBF tunnel_image_prev[NUM_LEDS];
float angle = 0.0;

void draw_beat_tunnel(){
	//draw_spectrum();
	//scale_CRGBF_array_by_constant(leds, (1.0 - sqrt(sqrt(tempo_confidence)))*0.85 + 0.15, NUM_LEDS);
	//memcpy(leds_temp, leds, sizeof(CRGBF)*NUM_LEDS);

	memset(tunnel_image, 0, sizeof(CRGBF)*NUM_LEDS);

	angle += 0.001;

	float position = (0.125 + 0.875*configuration.speed.value.f32)*(sin(angle)) * 0.5;
	draw_sprite(tunnel_image, tunnel_image_prev, NUM_LEDS, NUM_LEDS, position, 0.965);

	for(uint16_t i = 0; i < NUM_TEMPI; i++){
		float phase = 1.0 - ((tempi[i].phase + PI) / (2.0*PI));

		float mag = 0.0;
		if( fabs(phase - 0.65) < 0.02 ){
			mag = clip_float(tempi_smooth[i]);
		}

		CRGBF tempi_color = hsv(
			get_color_range_hue(num_tempi_float_lookup[i]),
			configuration.saturation.value.f32,
			(mag)
		);

		tunnel_image[i].r += tempi_color.r;
		tunnel_image[i].g += tempi_color.g;
		tunnel_image[i].b += tempi_color.b;
	}

	if(configuration.mirror_mode.value.u32 == true){
		for(uint16_t i = 0; i < NUM_TEMPI-2; i++){
			leds[ (NUM_LEDS>>1)    + ((i+2)>>1)] = tunnel_image[i];
			leds[((NUM_LEDS>>1)-1) - ((i+2)>>1)] = tunnel_image[i];
		}
	}
	else{
		memcpy(leds, tunnel_image, sizeof(CRGBF)*NUM_LEDS);
	}

	memcpy(tunnel_image_prev, tunnel_image, sizeof(CRGBF)*NUM_LEDS);

	//add_CRGBF_arrays(leds, leds_temp, NUM_LEDS);
}
