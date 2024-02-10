#define NUM_FREQS ( 64 ) // Number of Goertzel instances running in parallel (musical AND tempi, 128 total)
#define MAX_WEBSOCKET_CLIENTS ( 4 ) // Max simultaneous remote controls allowed at one time

const char* wtf_error_message_header = "^&*@!#^$*WTF?!(%$*&@@^@^$^#&$^ \nWTF ERROR: How the hell did you get here:";
const char* wtf_error_message_tail   = "--------------------------------------------------------------------------";

inline void wtf_error(){
	printf("%s\n", wtf_error_message_header);
	printf("FILE: %s *NEAR* THIS LINE: %li\n", __FILE__, __LINE__ ); // If I inline this function, will this line number roughly match where this function was called from?
	printf("%s\n", wtf_error_message_tail);
}