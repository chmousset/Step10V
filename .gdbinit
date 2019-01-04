echo "# GDB script automatically connect to STM32L432kc nucleo board"
echo "# st-util or openocd must be started prior to starting GDB."
echo "  openocd -f interface/stlink.cfg -f target/stm32l4x.cfg"
echp "  st-util -p 3333"

file build/Step10V.elf
target remote :3333

def lrun
	load
	monitor reset init
	continue
end
