#include <sys/signal.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/prex.h>
#include <termios.h>
#include <stdio.h>

/* Boolean */
typedef enum {false, true} bool;

/* Global */
static struct termios old_opt, new_opt;

struct Timer {
	int seconds, minutes, hours, decaseconds;
	bool showing;
	bool ticking;
	bool paused;
} s_timer;

struct Clock {
	int seconds, minutes, hours, decaseconds;
	bool showing;
} s_clock;

/* Prototypes */
u_long get_unix_time(void);
void set_terminal(void);
void init(void);
void change_mode(void);
void clock_print(void);
void clock_tick(void);
void timer_print(void);
void timer_tick(void);

int main(int argc, char* argv[]){

	/* Variables */
	char input;
	int hz;
	u_long t0, t1;
	u_int delay_usec, delay_msec;
	device_t tty_dev;	
	struct timerinfo timer_info;	

	/* Set initial values for the variables */
	delay_usec = delay_msec = 0;

	/* Tick Rate (source: prex-0.9.0/usr/sample/alarm) */
	sys_info(INFO_TIMER, &timer_info);
	if(timer_info.hz == 0)	
		panic("Cannot get tick rate!");
	hz = timer_info.hz;

	/* Terminal Settings*/
	set_terminal();

	/* Set Initial Values to Timer and Clock*/
	init();

	/* Core */
	while(1){

		sys_time(&t0);
		
		device_open("tty", 0, &tty_dev);
		device_ioctl(tty_dev, TIOCINQ, &input);
		device_close(tty_dev);

		if(input > 0){
			
			input = getchar();

			if(input == 'q')
				break;
			
			else if(input == 't')
				change_mode();
			
			else if(input == 'h' && s_clock.showing){
				s_clock.hours++;
				if(s_clock.hours == 24)
					s_clock.hours = 0;
			}	

			else if(input == 'm' && s_clock.showing){
				s_clock.minutes++;
				if(s_clock.minutes == 60){
					s_clock.minutes = 0;
					s_clock.hours++;
					if(s_clock.hours == 24)
						s_clock.hours = 0;						
				}			
			}	

			else if(input == 'z' && s_clock.showing){
				s_clock.decaseconds = 0;
				s_clock.seconds = 0;
			}

			else if(input == 'r' && s_timer.showing){
				s_timer.decaseconds = 0;
				s_timer.seconds = 0;
				s_timer.minutes = 0;
				s_timer.hours = 0;
				s_timer.ticking = false;
				if(s_timer.paused)
					s_timer.paused = false;
			}

			else if(input == 's' && s_timer.showing){
				if(!s_timer.ticking)
					s_timer.ticking = true;
				else
					s_timer.ticking = false;
			}

			else if(input == 'p' && s_timer.showing){
				if(!s_timer.paused)			
					s_timer.paused = true;
				else
					s_timer.paused = false;		
			}
			else {}
		}

		if(s_timer.ticking)
			timer_tick();
		
		clock_tick();

		sys_time(&t1);
		delay_usec += (u_int)((t1 - t0) * 10000000 / hz);
		while(delay_usec > 1000){
			delay_msec++;
			delay_usec-=1000;	
		}
				
		timer_sleep(10 - delay_msec, 0);		
		delay_msec = 0;

		if(s_clock.showing)
			 clock_print(); 
		else{
			if(!s_timer.paused)
				timer_print(); 
		}
	}
	
	/* Restore terminal original settings */
	tcsetattr(0, TCSANOW, &old_opt);

	printf("\nEnd.");

	return 0;
}

void init(){
		
	u_long unix_time;

	unix_time = get_unix_time();
	s_clock.seconds = (int) unix_time % 60;
	s_clock.minutes = (int)(unix_time/60) % 60;
	s_clock.hours = (int)(unix_time/3600) % 24;
	s_clock.decaseconds = 0;
	s_clock.showing = true;
	s_timer.seconds = 0;
	s_timer.minutes = 0;
	s_timer.hours = 0;
	s_timer.decaseconds = 0;
	s_timer.showing = false;
	s_timer.ticking = false;
	s_timer.paused = false;

}

void change_mode(){
	
	if(s_clock.showing){
		s_clock.showing = false;
		s_timer.showing = true;
		timer_print();	/* due to possibility of pause */
	}

	else{
		s_timer.showing = false;
		s_clock.showing = true;
	}

}

void clock_tick(){

	s_clock.decaseconds++;
	if(s_clock.decaseconds==100){
		s_clock.decaseconds=0;
		s_clock.seconds++;
		if(s_clock.seconds == 60){
			s_clock.seconds = 0;
			s_clock.minutes++;
			if(s_clock.minutes == 60){
				s_clock.minutes = 0;
				s_clock.hours++;
				if(s_clock.hours == 24)
					s_clock.hours = 0;		
			}
		}		
	}
}

void timer_tick(){
	
	s_timer.decaseconds++;
	if(s_timer.decaseconds==100){
		s_timer.decaseconds=0;
		s_timer.seconds++;
		if(s_timer.seconds == 60){
			s_timer.seconds = 0;
			s_timer.minutes++;
			if(s_timer.minutes == 60){
				s_timer.minutes = 0;
				s_timer.hours++;
				if(s_timer.hours == 24)
					s_timer.hours = 0;		
			}
		}
	}
}

void clock_print(){
	printf("Clock: %02d:%02d:%02d     \r", s_clock.hours, s_clock.minutes, s_clock.seconds);
}

void timer_print(){
	printf("Timer: %02d:%02d:%02d.%02d\r", s_timer.hours, s_timer.minutes, s_timer.seconds, s_timer.decaseconds);
}

u_long get_unix_time(){

	device_t rtc_dev;
	u_long sec;

	device_open("rtc", 0, &rtc_dev);
	device_ioctl(rtc_dev, RTCIOC_GET_TIME, &sec);
	device_close(rtc_dev);

	return sec;
}

void set_terminal(){

	tcgetattr(0, &old_opt);
	new_opt = old_opt;
	new_opt.c_lflag &= ~ICANON;
	new_opt.c_lflag &= ~ECHO;
	new_opt.c_lflag &= ~ISIG;
	new_opt.c_cc[VMIN] =  0;
	new_opt.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_opt);
}

int tcgetattr(int fd, struct termios *t){

	device_t tty_dev;

	device_open("tty", 0, &tty_dev);
	device_ioctl(tty_dev, TIOCGETA, t);
	device_close(tty_dev);
	
	return 0;
}

int tcsetattr(int fd, int opt, const struct termios *t){

	struct termios localterm;
	device_t tty_dev;		

	if(opt & TCSASOFT) {
		localterm = *t;
		localterm.c_cflag |= CIGNORE;
		t = &localterm;
	}

	
	switch (opt & ~TCSASOFT){
	case TCSANOW:
		device_open("tty", 2, &tty_dev);
		device_ioctl(tty_dev, TIOCSETA, t);
		device_close(tty_dev);
	case TCSADRAIN:
		device_open("tty", 2, &tty_dev);
		device_ioctl(tty_dev, TIOCSETAW, t);
		device_close(tty_dev);
	case TCSAFLUSH:
		device_open("tty", 2, &tty_dev);
		device_ioctl(tty_dev, TIOCSETAF, t);
		device_close(tty_dev);
	default:		
		return(-1);
	}

	return 0;
}
