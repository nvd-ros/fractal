# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#              #
#    Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += -s

SRC_DIR := src/
OBJ_DIR := obj/
INC_DIR := inc/
# LIB_DIR := lib/

PROJ := fractol
PROJ_SRC := $(shell find $(SRC_DIR) -name '*.c' -exec basename {} \;)
PROJ_INC := $(shell find $(INC_DIR) -name '*.h' -exec basename {} \;)

PROJ_OBJ_DIR := $(OBJ_DIR)
PROJ_OBJ := $(PROJ_SRC:%.c=$(OBJ_DIR)%.o)

# LIBFT_DIR := $(LIB_DIR)libft/
# LIBFT := $(LIBFT_DIR)libft.a
# LIBFT_NAME := ft

SDL2_DIR := /usr/local/Cellar/sdl2/2.0.22/
SDL2_INC_DIR := $(SDL2_DIR)include/
SDL2_LIB_DIR := $(SDL2_DIR)lib/
SDL2_LIB := $(SDL2_LIB_DIR)libSDL2.a
SDL2_LIB_NAME := SDL2

INC_FLAGS := -I $(INC_DIR)
CC_FLAGS := -Wall -Werror -Wextra -std=c11 -O3
LIB_FLAGS := -l$(SDL2_LIB_NAME) -L $(SDL2_LIB_DIR) -lOpenCL

INC_LIB_FLAGS := -I $(SDL2_INC_DIR)

# Colors
TB := "\\033[36m"
TG := "\\033[32m"
TR := "\\033[31m"
EOC := "\\033[0m"
K := "\\033[K"

# Main rules

all: $(PROJ)

help:
	printf "usage: make [ all clean fclean ]\n"

# $(LIBFT):
# 	make -C $(LIBFT_DIR)

#
# compiling project
# 

$(PROJ): $(LIBFT) $(PROJ_OBJ)
	$(CC) $(PROJ_OBJ) -o $@ $(LIB_FLAGS) $(OP_OBJ)
	printf "$(TG)$@ created$(EOC)$(K)\n"
	tput cnorm

$(OBJ_DIR):
	mkdir $@

$(PROJ_OBJ): | $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) -c $< -o $@ $(CC_FLAGS) $(INC_FLAGS) $(INC_LIB_FLAGS)
	printf "$(TB)$(CW): $@$(EOC)$(K)\r"

# 
# cleaning
# 

clean:
# 	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)
	printf "$(TB)$(PROJ) $(TR)$@$(EOC)\n"

fclean:
# 	make fclean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(PROJ)
	printf "$(TB)$(PROJ) $(TR)$@$(EOC)\n"

re: fclean all

.PHONY: help all clean fclean re
