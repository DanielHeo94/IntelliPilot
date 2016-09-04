#user specific settings:
#where to find the IDE
ADIR:=$(shell pwd)/.arduino15/packages/arduino
#which serial port to use (add a file with SUBSYSTEMS=="usb", ATTRS{product}=="Arduino Due Prog. Port", ATTRS{idProduct}=="003d", ATTRS{idVendor}=="2341", SYMLINK+="arduino_due" in /etc/udev/rules.d/ to get this working)
PORTNAME:=$(shell ls /dev | grep ttyACM)
PORT:=/dev/$(PORTNAME)
#if we want to verify the bossac upload, define this to -v
VERIFY:= -v
ERASE:= -e

#then some general settings. They should not be necessary to modify.
CXX:=$(ADIR)/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-g++
CC:=$(ADIR)/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-gcc
C:=$(CC)
SAM:=hardware/sam/1.6.9
CMSIS:=hardware/sam/1.6.9/system/CMSIS
LIBSAM:=hardware/sam/1.6.9/system/libsam
TMPDIR:=$(shell pwd)/build
AR:=$(ADIR)/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-ar

#all these values are hard coded and should maybe be configured somehow else,
#like olikraus does in his makefile.
DEFINES:=-Dprintf=iprintf -DF_CPU=84000000L -DARDUINO=152 -D__SAM3X8E__ -DUSB_PID=0x003e -DUSB_VID=0x2341 -DUSBCON
ARCHITECTURE:=-DARDUINO_ARCH_SAM

INCLUDES:=-I$(ADIR)/$(LIBSAM) -I$(ADIR)/$(CMSIS)/CMSIS/Include/ -I$(ADIR)/$(CMSIS)/Device/ATMEL/ -I$(ADIR)/$(SAM)/cores/arduino -I$(ADIR)/$(SAM)/variants/arduino_due_x -I$(ADIR)/$(SAM)/cores/arduino
INCLUDES += -I$(ADIR)/$(SAM)/libraries/HID/src -I$(ADIR)/$(SAM)/libraries/SPI/src -I$(ADIR)/$(SAM)/libraries/Wire/src -I$(ADIR)/$(SAM)/libraries/Servo/src -I$(ADIR)/$(SAM)/libraries/Servo/src/sam

#also include the current dir for convenience
INCLUDES += -I.
INCLUDES += -I$(shell pwd)/IntelliCopter -I$(shell pwd)/libraries -I$(shell pwd)/FreeRTOS_ARM/src -I$(shell pwd)/FreeRTOS_ARM/src/utility

#compilation flags common to both c and c++
COMMON_FLAGS:=-g -Os -w -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -mcpu=cortex-m3  -mthumb $(ARCHITECTURE)

CFLAGS:=$(COMMON_FLAGS)
CXXFLAGS:=$(COMMON_FLAGS) -fno-rtti -fexceptions

#let the results be named after the project
PROJNAME:=$(shell basename $(shell pwd)/IntelliCopter/*.ip .ip)

#we will make a new mainfile from the ino file.
NEWMAINFILE:=$(TMPDIR)/$(PROJNAME).ip.cpp

#our own sourcefiles is the (converted) ino file and any local cpp files
MYSRCFILES:=$(NEWMAINFILE) $(shell ls *.cpp 2>/dev/null)
MYSRCFILES+=$(shell ls $(shell pwd)/FreeRTOS_ARM/src/*.c $(shell pwd)/FreeRTOS_ARM/src/utility/*.c)
MYSRCXXFILES:=$(shell ls $(shell pwd)/IntelliCopter/*.cpp $(shell pwd)/libraries/IC_GPS/*.cpp $(shell pwd)/libraries/IC_InertialSensor/*.cpp $(shell pwd)/libraries/IC_AttitudeControl/*.cpp $(shell pwd)/libraries/IC_LED/*.cpp $(shell pwd)/libraries/IC_Math/*.cpp $(shell pwd)/libraries/IC_Motors/*.cpp $(shell pwd)/libraries/IC_PID/*.cpp $(shell pwd)/libraries/IC_Radio/*.cpp $(shell pwd)/FreeRTOS_ARM/src/*.cpp $(shell pwd)/libraries/IC_Storage/*.cpp)
MYOBJFILES:=$(addsuffix .o,$(addprefix $(TMPDIR)/,$(notdir $(MYSRCFILES))))
MYOBJFILESXX:=$(addsuffix .o,$(addprefix $(TMPDIR)/,$(notdir $(MYSRCXXFILES))))

#These source files are the ones forming core.a
CORESRCXX:=$(shell ls ${ADIR}/${SAM}/cores/arduino/*.cpp ${ADIR}/${SAM}/cores/arduino/USB/*.cpp  ${ADIR}/${SAM}/variants/arduino_due_x/variant.cpp ${ADIR}/${SAM}/libraries/HID/src/*.cpp ${ADIR}/${SAM}/libraries/SPI/src/*.cpp ${ADIR}/${SAM}/libraries/Wire/src/*.cpp ${ADIR}/${SAM}/libraries/Servo/src/sam/*.cpp)
CORESRC:=$(shell ls ${ADIR}/${SAM}/cores/arduino/*.c)


#convert the core source files to object files. assume no clashes.
COREOBJSXX:=$(addprefix $(TMPDIR)/core/,$(notdir $(CORESRCXX)) )
COREOBJSXX:=$(addsuffix .o,$(COREOBJSXX))
COREOBJS:=$(addprefix $(TMPDIR)/core/,$(notdir $(CORESRC)) )
COREOBJS:=$(addsuffix .o,$(COREOBJS))

default:
	@echo default rule, does nothing. Try make build or make flash or make screen.

#This rule is good to just make sure stuff compiles, without having to wait
#for bossac.
build: $(TMPDIR)/$(PROJNAME).bin

#This is a make rule template to create object files from the source files.
# arg 1=src file
# arg 2=object file
# arg 3= XX if c++, empty if c
define OBJ_template
$(2): $(1)
	$(eval STEPCNT=$(shell echo $$(($(STEPCNT)+1))))
	@echo -n "\r[File $(STEPCNT)] Create object files from the source files =>"
	@$(C$(3)) -MD -c $(C$(3)FLAGS) $(DEFINES) $(INCLUDES) $(1) -o $(2)
endef
#now invoke the template both for c++ sources:
$(foreach src,$(CORESRCXX), $(eval $(call OBJ_template,$(src),$(addsuffix .o,$(addprefix $(TMPDIR)/core/,$(notdir $(src)))),XX) ) )
#...and for c sources:
$(foreach src,$(CORESRC), $(eval $(call OBJ_template,$(src),$(addsuffix .o,$(addprefix $(TMPDIR)/core/,$(notdir $(src)))),) ) )

#and our own c and c++ sources:
$(foreach src,$(MYSRCXXFILES), $(eval $(call OBJ_template,$(src),$(addsuffix .o,$(addprefix $(TMPDIR)/,$(notdir $(src)))),XX) ) )
$(foreach src,$(MYSRCFILES), $(eval $(call OBJ_template,$(src),$(addsuffix .o,$(addprefix $(TMPDIR)/,$(notdir $(src)))),) ) )


clean:
	@test ! -d $(TMPDIR) || rm -rf $(TMPDIR)
	@echo build files all removed.

.PHONY: upload default

$(TMPDIR):
	mkdir -p $(TMPDIR)

$(TMPDIR)/core:
	@mkdir -p $(TMPDIR)/core

#creates the cpp file from the .ip file
$(NEWMAINFILE): $(shell pwd)/IntelliCopter/$(PROJNAME).ip
	@echo -n " Create the cpp file from the .ip file..."
	@cat $(ADIR)/hardware/sam/1.6.9/cores/arduino/main.cpp > $(NEWMAINFILE)
	@echo '#include <System.h> \n void setup() { copter.config(); copter.start(); } \n void loop() { /* Not used */ };' >> $(NEWMAINFILE)
	@echo 'extern "C" void __cxa_pure_virtual() {while (true);}' >> $(NEWMAINFILE)
	@echo "\t\t\t✔"

#include the dependencies for our own files
-include $(MYOBJFILES:.o=.d)
-include $(MYOBJFILESXX:.o=.d)

#create the core library from the core objects. Do this EXACTLY as the
#arduino IDE does it, seems *really* picky about this.
#Sorry for the hard coding.
$(TMPDIR)/core.a: $(TMPDIR)/core $(COREOBJS) $(COREOBJSXX)
	@echo -n " Create the core library from the core objects..."
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/wiring_shift.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/wiring_analog.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/itoa.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/cortex_handlers.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/hooks.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/wiring.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/WInterrupts.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/syscalls_sam3.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/iar_calls_sam3.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/wiring_digital.c.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/Print.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/PluggableUSB.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/USARTClass.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/WString.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/USBCore.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/CDC.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/HID.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/SPI.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/Wire.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/Servo.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/wiring_pulse.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/watchdog.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/UARTClass.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/main.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/Stream.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/RingBuffer.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/IPAddress.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/Reset.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/WMath.cpp.o
	@$(AR) rcs $(TMPDIR)/core.a $(TMPDIR)/core/variant.cpp.o
	@echo "\t\t✔"

#link our own object files with core to form the elf file
$(TMPDIR)/$(PROJNAME).elf: $(TMPDIR)/core.a $(TMPDIR)/core/syscalls_sam3.c.o $(MYOBJFILESXX) $(MYOBJFILES)
	@echo -n " Link object files with core to form the elf"
	@$(CXX) -Os -Wl,--gc-sections -mcpu=cortex-m3 -T$(ADIR)/$(SAM)/variants/arduino_due_x/linker_scripts/gcc/flash.ld -Wl,-Map,$(NEWMAINFILE).map -o $@ -L$(TMPDIR) -lm -lgcc -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols -Wl,--start-group $(TMPDIR)/core/syscalls_sam3.c.o $(MYOBJFILESXX) $(MYOBJFILES) $(ADIR)/$(SAM)/variants/arduino_due_x/libsam_sam3x8e_gcc_rel.a $(TMPDIR)/core.a -Wl,--end-group
	@echo "\t✔"

#copy from the hex to our bin file
$(TMPDIR)/$(PROJNAME).bin: $(TMPDIR)/$(PROJNAME).elf
	@echo "Copy from the hex to binary..."
	@$(ADIR)/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/arm-none-eabi-objcopy -O binary $< $@
	@echo "Done. ( ͡° ͜ʖ ͡°)"

#upload to the arduino by first resetting it (stty) and the running bossac
flash: $(TMPDIR)/$(PROJNAME).bin
	@stty -F $(PORT) 1200
	@$(ADIR)/tools/bossac/1.6.1-arduino/bossac --port=$(PORTNAME) -U false -i
	@$(ADIR)/tools/bossac/1.6.1-arduino/bossac --port=$(PORTNAME) -U false $(ERASE) -w $(VERIFY) -b $(TMPDIR)/$(PROJNAME).bin -R

#to view the serial port with screen.
screen:
	@screen $(PORT) 115200
