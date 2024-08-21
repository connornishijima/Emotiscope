/*
-------------------------------------------------------------------------
                                                                   _
                                                                  | |
  __ _   _ __    _   _             ___    ___    _ __    ___      | |__
 / _` | | '_ \  | | | |           / __|  / _ \  | '__|  / _ \     | '_ \ 
| (_| | | |_) | | |_| |          | (__  | (_) | | |    |  __/  _  | | | |
 \__, | | .__/   \__,_|           \___|  \___/  |_|     \___| (_) |_| |_|
  __/ | | |              ______
 |___/  |_|             |______|

Main loop of the GPU core
*/

void run_gpu() {
	start_profile(__COUNTER__, __func__);
	static int64_t t_last_us = 0;
	t_now_us = esp_timer_get_time();
	t_now_ms = t_now_us / 1000;

	// Calculate the "delta" value, to scale movements based on FPS, like a game engine
	const uint32_t ideal_us_interval = (1000000 / REFERENCE_FPS);
	uint32_t t_elapsed_us = t_now_us - t_last_us;
	float delta = (float)t_elapsed_us / ideal_us_interval;

	// Save the current timestamp for next loop
	t_last_us = t_now_us;

	// Update the FPS_GPU variable
	watch_gpu_fps();  // (system.h)

	// Update the novelty curve
	update_novelty();  // (tempo.h)

	// Update the tempi phases
	update_tempi_phase(delta);	// (tempo.h)

	// Update perlin noise offsets
	update_perlin_noise(delta); // (perlin.h)

	// Update auto color cycling
	update_auto_color();  // (leds.h)

	// Clamp configuration items to their min/max values
	clamp_configuration();  // (configuration.h)


	// DRAWING ---------------------------------------------------

	clear_display(0.0);
	light_modes[configuration.current_mode.value.u32].draw();
	//draw_debug();  // (debug.h)

	apply_background(configuration.background.value.f32);
	apply_blur( configuration.blur.value.f32 * 12.0 );
	
	draw_ui_overlay();  // (ui.h)

	if( EMOTISCOPE_ACTIVE == true && configuration.screensaver.value.u32 == true){
		run_screensaver();
	}

	apply_brightness();

	if( EMOTISCOPE_ACTIVE == false ){
		run_standby();
	}

	render_touches();  // (touch.h)
	
	apply_softness();

	apply_tonemapping();

	// Apply an incandescent LUT to reduce harsh blue tones
	apply_warmth( configuration.warmth.value.f32 );  // (leds.h)

	// Apply white balance
	multiply_CRGBF_array_by_LUT( leds, WHITE_BALANCE, NUM_LEDS );

	apply_master_brightness();

	apply_gamma_correction();

	// Quantize the image buffer with dithering, 
	// output to the 8-bit LED strand
	transmit_leds();

	end_profile();
}

void loop_gpu(void *pvParameters) {
	while (1) {
		run_gpu();
		run_gpu();
		run_gpu();
		run_gpu();
	}
}