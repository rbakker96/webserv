# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: roybakker <roybakker@student.codam.nl>       +#+                      #
#                                                    +#+                       #
#    Created: 2021/02/25 14:55:06 by roybakker     #+#    #+#                  #
#    Updated: 2021/03/25 11:28:38 by gbouwen       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME			=	webserv

SOURCES			=	main.cpp \
                    CPP-FILES/gnl/get_next_line.cpp \
                    CPP-FILES/gnl/get_next_line_utils.cpp \
                    CPP-FILES/server/location_context.cpp \
                    CPP-FILES/server/server.cpp \
                    CPP-FILES/server/webserver.cpp

OBJECTS 		=	${SOURCES:%.c=%.o}

FLAGS 			=	-Wall -Wextra -Werror

CLASS_HEADERS   =	-IHEADER_FILES/CLASS
UTILS_HEADERS   =   -IHEADER_FILES/UTILS

COMPILE			=	clang++ -std=c++98

GREEN 			= 	\033[38;5;46m
WHITE 			= 	\033[38;5;15m
GREY 			= 	\033[38;5;8m
ORANGE 			= 	\033[38;5;202m
RED 			= 	\033[38;5;160m

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "$(GREEN)----------------------------------------------------"
	@$(COMPILE) $(CLASS_HEADERS) $(UTILS_HEADERS) $(FLAGS) -o $(NAME) $(OBJECTS)
	@echo "Executable				./webserv"
	@echo "$(GREEN)----------------------------------------------------"

%.o: %.c
	@echo "$(GREY)Compiling...				$(WHITE)$<"
	@$(COMPILE) $(CLASS_HEADERS) $(UTILS_HEADERS) $(FLAGS) -c -o $@ $<
	@$(COMPILE) $(FLAGS) -c -o $@ $<

clean:
	@echo "$(RED)----------------------------------------------------"
	@echo "$(WHITE)DELETE ./webserv"
	@echo "$(RED)----------------------------------------------------"
	@/bin/rm -f $(NAME)

re: clean all

.PHONY: all clean re
