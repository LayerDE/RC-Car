#include <fcntl.h>
#include <stdio.h>
#include <linux/joystick.h>
#include <stdlib.h>

typedef int (*calc_axe_ptr)(__s16);
typedef void (*axe_isr_ptr)(int);
typedef void (*btn_isr_ptr)(bool);
calc_axe_ptr calc_axes[];
axe_isr_ptr axe_functions[];
btn_isr_ptr btn_functions[]; //executed with a button event
int axes[];
bool buttons[];
int js_handle;
__u32 last_time;
__u32 time_last_event;
size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}

int calc_axe_0mp(__s16 raw){
    return raw;
}
int calc_axe_1m(__s16 raw){
    return 1<<(sizeof(__s16)*8-1)+raw;
}
int calc_axe_1p(__s16 raw){
    return -1<<(sizeof(__s16)*8-1)+raw;
}
void btn_null_exec(bool state){

}
void axe_null_exec(int value){

}

size_t get_button_count(int fd)
{
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}

int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

void init(char js_device[]){
    if ((js_handle = open(js_device, O_RDONLY)) < 0)
        perror("Could not open joystick");
    axes = malloc(sizeof(int)*get_axis_count(js_handle));
    calc_axes = malloc(sizeof(calc_axe_ptr)*get_axis_count(js_handle));
    buttons = malloc(sizeof(bool)*get_button_count(js_handle));
    axe_functions = malloc(sizeof(axe_isr_ptr)*get_axis_count(js_handle));
    btn_functions = malloc(sizeof(btn_isr_ptr)*get_button_count(js_handle));
    for(x = 0;x<get_axis_count(js_handle);x++){
        axes[x] = 0;
        calc_axes = calc_axe_0mp;
        axe_functions = axe_null_exec;
    }
    for(x = 0;x<get_button_count(js_handle);x++){
        buttons = false;
        btn_functions = btn_null_exec;
    }
    char name[128];
	if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0);
		strncpy(name, "Unknown", sizeof(name));
	printf("Name: %s\n", name); // read config for xxx name if exists
}
void input_thread(){
    while (read_event(js, &event) == 0)
    {
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
                btn_functions[event.number](buttons[event.number] = event.value ? true : false);
                break;
            case JS_EVENT_AXIS:
                axe_functions[event.number](axes[event.number] = calc_axes[event.number](event.value));
                break;
            case JS_EVENT_INIT:
                break;
            default:
                /* Ignore init events. */
                break;
        }
    }
    close(js_handle);
}

int main(int argc,char* argv[]){
	struct js_event event;
	printf("Launched with:\n");
	for(int cnt=0;cnt<argc;cnt++){
		printf(argv[cnt]);
		printf("\n");
	}
	
	init("/dev/input/js0");

    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0)
    {
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
                printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
                break;
            case JS_EVENT_AXIS:
                printf("Axis %u at %6d\n", event.number, event.value);  
                break;
            case JS_EVENT_INIT:
                break;
            default:
                /* Ignore init events. */
                break;
        }
    }
    close(js_handle);
    printf("joystick unplugged\n");
    return 0;
}
