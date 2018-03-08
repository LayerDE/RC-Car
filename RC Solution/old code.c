//library.c
//V1
void add_task(task schedule) {// add task to scheduler
	scheduler[SCHEDULE_INC(schedule_last)]=schedule;
}

//main.c
//V1
int main(void)
{
	reset_scheduler();
	add_task(init); // run init with scheduler
	main_loop: // while(1){
		// contexswitch means stack mark and goto not
		scheduler[schedule_first]();
		scheduler[schedule_first]=default_schedule;
		
		if(schedule_first!=schedule_last)
			SCHEDULE_INC(schedule_first);//looks better but doesnt work with SCHEDULE_MAX tasks because the task sticks
	goto main_loop;//}
}