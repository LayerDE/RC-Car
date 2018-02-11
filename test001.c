#include <fcntl.h>
#include <stdio.h>
#include <linux/joystick.h>

size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
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


int main(int argc,char* argv[]){
	struct js_event event;
	printf("Launched with:\n");
	for(int cnt=0;cnt<argc;cnt++){
		printf(argv[cnt]);
		printf("\n");
	}
	
	int js = open("/dev/input/js0", O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

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
            default:
                /* Ignore init events. */
                break;
        }
    }
    close(js);
    printf("joystick unplugged\n");
    return 0;
}
