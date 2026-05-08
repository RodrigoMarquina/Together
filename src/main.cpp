#include <iostream>
#include <jack/jack.h>
#include <cmath>
#include <unistd.h>

struct callbackArguments {
	jack_port_t* port;
	jack_nframes_t sampleRate;
	float phase;
	float frequency;
};


int callback(jack_nframes_t nframes, void* arg){
	callbackArguments* arguments = static_cast<callbackArguments*>(arg);
	float* buffer = static_cast<float*>(jack_port_get_buffer(arguments->port, nframes));
	for(int i = 0; i < nframes; i++){
		arguments->phase += (2 * M_PI * arguments->frequency) / arguments->sampleRate;
		arguments->phase = fmod(arguments->phase, 2 * M_PI);
		buffer[i] = sin(arguments->phase);	
	}
	return 0;
}

int main() {     
	JackOptions jackOptions = JackNullOption;
	jack_status_t status;
	jack_client_t* client = jack_client_open("Together", jackOptions, &status); 
	if(client == nullptr){
		std::cout << status << '\n';
		return 1;
	}    

	jack_port_t* port = jack_port_register(client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	callbackArguments arguments;
	arguments.port = port;
	arguments.sampleRate = jack_get_sample_rate(client);
	arguments.phase = 0.0f;
	arguments.frequency = 440.0f;

	int setCallbackResult = jack_set_process_callback(client, callback, &arguments);

	jack_activate(client);

	pause();
}
