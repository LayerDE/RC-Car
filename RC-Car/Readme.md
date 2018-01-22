Functions:
    Universal:
        Constants:
            SCHEDULE_MAX
            default_schedule

            SPI_EMPTY
            SPI_BUFFER_SIZE

        Global Vars:
            unsigned char schedule_first
            unsigned char schedule_last
            task scheduler[SCHEDULE_MAX]

            unsigned char spi_buffer_index
            unsigned char spi_buffer[SPI_BUFFER_SIZE]
            unsigned char spi_pack_start
            unsigned char spi_in_start
            unsigned char spi_pack_end


        Tasks:
            void sleep()
                sets µC to sleepmode
                wakeup per interrups
            
        ISR:

        Control Funcs:
            void add_task(task schedule)
                Vars:
                    scheduler[]
                    schedule_last
                adds Task to exec queue...

            void spi_buffer_add(uint8_t value)
                Vars:
                    spi_buffer[]
                    spi_pack_end
                adds 8 bit to spi buffer
                
    Car:
        Constants:

        Global Vars:
            
        Tasks:
            
        ISR:

        Control Funcs: 
            void exec_command(unsigned char command0,unsigned char command1)
                Vars:
                    get set!
                
    Transmitter: