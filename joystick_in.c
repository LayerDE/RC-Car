#include <fcntl.h>
#include <stdio.h>
#include <linux/joystick.h>
#include <stdlib.h>


typedef int (*calc_axe_ptr)(__s16);
typedef void (*axe_isr_ptr)(int);
typedef void (*btn_isr_ptr)(bool);
calc_axe_ptr calc_axis[];
axe_isr_ptr axe_functions[];
btn_isr_ptr btn_functions[]; //executed with a button event
__s16 axis[];
bool buttons[];
int js_handle;
__u32 last_time;
__u32 time_last_event;
size_t get_axis_count(int fd)
{
    __u8 axis;

    if (ioctl(fd, JSIOCGaxis, &axis) == -1)
        return 0;

    return axis;
}

__s16 calc_axe_0mp(__s16 raw){
    return raw;
}
__s16 calc_axe_1m(__s16 raw){
    return (1<<(sizeof(__s16)*8-1)+raw)/2;
}
__s16 calc_axe_1p(__s16 raw){
    return (-1<<(sizeof(__s16)*8-1)+raw)/2;
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
    else{
        axis_cnt = get_axis_count(js_handle);
        btn_cnt = get_button_count(js_handle);
        axis = malloc(sizeof(int)*axis_cnt);
        calc_axis = malloc(sizeof(calc_axe_ptr)*axis_cnt;
        buttons = malloc(sizeof(bool)*btn_cnt);
        axe_functions = malloc(sizeof(axe_isr_ptr)*axis_cnt);
        btn_functions = malloc(sizeof(btn_isr_ptr)*btn_cnt);
        for(x = 0;x<axis_cnt;x++){
            axis[x] = 0;
            calc_axis = calc_axe_0mp;
            axe_functions = axe_null_exec;
        }
        for(x = 0;x<btn_cnt;x++){
            buttons = false;
            btn_functions = btn_null_exec;
        }
    }
    char name[128];
	if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0);
		read_config(NULL);
    else
        read_config(name);
	printf("Name: %s\n", name); // read config for xxx name if exists
}
void read_config(char name[]){
    FILE * fp;
    size_t len = 0;
    ssize_t read;
    if(name != NULL)
        if(fp = fopen("/etc/" name, "r") != NULL)
            goto skip_read_btn;//TODO: kill goto! goto is bad code!
//read_btn
    sprintf(name, "btn_c%d_axis%d.cfg", get_button_count(js_handle), get_axis_count(js_handle))
    if(fp = fopen(name, "r") != NULL)
        return; //error
skip_read_btn:
    int line_cnt = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        line_cnt++;
        if(line[0]=='#'||line[0]=='!')
            continue;//kommentar zeile in der config
        if(!exec_config_command(line))
            perror("%s at line %d is not a valid config command",line,line_cnt);
    }
    fclose(fp);
    if (line)
        free(line);
}
bool exec_config_command(char command[]){
    return false;
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
                axe_functions[event.number](axis[event.number] = calc_axis[event.number](event.value));
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
