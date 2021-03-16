# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: roybakker <roybakker@student.codam.nl>       +#+                      #
#                                                    +#+                       #
#    Created: 2021/02/25 14:55:06 by roybakker     #+#    #+#                  #
#    Updated: 2021/02/25 14:55:06 by roybakker     ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME			=	webserv

SOURCES			=

OBJECTS 		=	${SOURCES:%.c=%.o}

FLAGS 			=	-Wall -Wextra -Werror
#INCLUDES 		=	-IHEADER_FILES/
#UTILS			=	-IHEADER_FILES/UTILS/
#UNIT_TEST		=	-IUNIT_TEST/
COMPILE			=	clang++ -std=c++98

GREEN 			= 	\033[38;5;46m
WHITE 			= 	\033[38;5;15m
GREY 			= 	\033[38;5;8m
ORANGE 			= 	\033[38;5;202m
RED 			= 	\033[38;5;160m

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "$(GREEN)----------------------------------------------------"
#	@$(COMPILE) $(INCLUDES) $(UTILS) $(UNIT_TEST) $(FLAGS) -o $(NAME) $(OBJECTS)
	@$(COMPILE) $(FLAGS) -o $(NAME) $(OBJECTS)
	@echo "Executable				./webserv"
	@echo "$(GREEN)----------------------------------------------------"

%.o: %.c
	@echo "$(GREY)Compiling...				$(WHITE)$<"
#	@$(COMPILE) $(INCLUDES) $(UTILS) $(UNIT_TEST) $(FLAGS) -c -o $@ $<
	@$(COMPILE) $(FLAGS) -c -o $@ $<

clean:
	@echo "$(RED)----------------------------------------------------"
	@echo "$(WHITE)DELETE ./webserv"
	@echo "$(RED)----------------------------------------------------"
	@/bin/rm -f $(NAME)

re: clean all

.PHONY: all clean re