#* ************************************************************************** *#
#* *                            GENERAL INFO                                * *#
#* ************************************************************************** *#

TARGET := led

#* ************************************************************************** *#
#* *                             COMPILATION                                * *#
#* ************************************************************************** *#

CC := avr-gcc
CFLAGS += -Wall -Werror -Wextra -Os -mmcu=atmega328p
F_CPU := 16000000
BAUD_RATE := 115200
PORT := /dev/tty.usbserial-10

#* ************************************************************************** *#
#* *                              INCLUDES                                  * *#
#* ************************************************************************** *#

INCLUDES := include
INCLUDES_FLAGS += $(addprefix -I, $(INCLUDES))

#* ************************************************************************** *#
#* *                               SOURCES                                  * *#
#* ************************************************************************** *#

SRCDIR := ./src/
-include make/sources.mk

#* ************************************************************************** *#
#* *                              OBJECTS                                   * *#
#* ************************************************************************** *#

OBJDIR := obj
OBJECTS := ${SOURCES:%.c=${OBJDIR}/%.o}

#* ************************************************************************** *#
#* *                            DEPENDENCIES                                * *#
#* ************************************************************************** *#

DEPENDENCIES := $(OBJECTS:.o=.d)

#* ************************************************************************** *#
#* *                          TEXT CONSTANTS                                * *#
#* ************************************************************************** *#

BLACK = \033[30m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
BOLD = \033[1m
THIN = \033[2m
ITALIC = \033[3m
UNDERLINE = \033[4m
RESET_COLOR = \033[0m
NEW			:= \r\033[K

#* ************************************************************************** *#
#* *                           PRESENTATION                                 * *#
#* ************************************************************************** *#

define success_message
	@printf "✨ $(NEW)$(GREEN)$(BOLD)COMPILATION SUCCESSFUL$(RESET_COLOR) ✨\n"
endef

define linking_message
	@printf "$(YELLOW)Linking $(BOLD)$(CYAN)$@ $(RESET_COLOR)$(YELLOW)...$(RESET_COLOR)\n"
endef

LAST_DIR = ""
FIRST_COMPILE_MESSAGE = true
define compile_message
	@if [ "$(FIRST_COMPILE_MESSAGE)" = "true" ]; then \
		printf "\n"; \
		FIRST_COMPILE_MESSAGE=false; \
	fi
	@if [ "$(dir $<)" != "$(LAST_DIR)" ]; then \
		printf "$(BOLD)$(YELLOW)Compiling files in directory $(RESET_COLOR)$(BOLD)$(CYAN)$(dir $<)$(RESET_COLOR)\n"; \
		LAST_DIR="$(dir $<)"; \
	fi
	printf "$(CYAN)    • $(notdir $<)";
	@$(eval LAST_DIR := $(dir $<))
endef


#* ************************************************************************** *#
#* *                          MAKEFILE RULES                                * *#
#* ************************************************************************** *#

all: flash

hex: $(TARGET)
	@printf "\n$(YELLOW)Creating $(CYAN)$(TARGET) $(RED)hexfile $(YELLOW)...$(RESET_COLOR)\n"
	@avr-objcopy -j .text -j .data -O ihex $(TARGET).bin $(TARGET).hex

flash: hex
	@printf "$(YELLOW)Flashing $(BOLD)$(CYAN)the card $(RESET_COLOR)$(YELLOW)...$(RESET_COLOR)\n\n"
	@avrdude -c arduino -p m328p -P $(PORT) -b $(BAUD_RATE) -U flash:w:$(TARGET).hex

# -------------------- #
# Create object files. #
# -------------------- #

$(OBJECTS): $(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(call compile_message)
	@$(CC) $(CFLAGS) -MMD -MF $(@:.o=.d) $(INCLUDES_FLAGS) -D F_CPU=$(F_CPU) -c $< -o $@

# ---------------------------------------- #
# Link the files after compiling them all. #
# ---------------------------------------- #

-include $(DEPENDENCIES)
$(TARGET): $(OBJECTS)
	@echo ""
	@$(call linking_message)
	@$(CC) $(CFLAGS) $(INCLUDES_FLAGS) -o $@.bin $(OBJECTS)
	@echo ""
	@$(call success_message)

# --------------------- #
# Delete compiled data. #
# --------------------- #

clean:
	@rm -rf $(TARGET).bin
	@rm -rf $(TARGET).hex
	@printf "$(YELLOW)Deleting $(CYAN)$(TARGET) executable $(YELLOW)...$(RESET_COLOR)\n"

fclean: clean
	@rm -rf $(OBJDIR)
	@printf "$(YELLOW)Deleting $(CYAN)$(OBJDIR) $(YELLOW)...$(RESET_COLOR)\n"

# ------------------------------- #
# Create the list of all sources. #
# ------------------------------- #

gmk:
	if [ -d make ];then echo ok;else mkdir make;fi
	@find $(SRCDIR) -name '*.c' -printf "%d%p\n" | sort -n | sed 's/^[[:digit:]]/SOURCES += /' > make/sources.mk

# --------------------- #
#      Recompile.       #
# --------------------- #

re: fclean all

.PHONY: clean fclean gmk re hex flash