#ifndef IO_CONFIG
#define	IO_CONFIG


// 8e1, see termios.h for more information
// https://www.gnu.org/software/libc/manual/html_node/Control-Modes.html
#define RS_FLAGS CS8 | CREAD
//| CLOCAL
// speed
//#define RS_SPEED B9600 // default Arduino
#define RS_SPEED B38400

// TCP server port
#define IO_TCP_SERVER_PORT (2002)

#endif
