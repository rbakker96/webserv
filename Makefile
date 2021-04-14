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

SOURCES			=	webserver/main.cpp \
                    webserver/gnl/get_next_line.cpp \
                    webserver/gnl/get_next_line_utils.cpp \
                    webserver/server/location_context.cpp \
                    webserver/server/server.cpp \
                    webserver/server/webserver.cpp \
                    webserver/server/handlers/handler.cpp \
                    webserver/server/handlers/request_handler.cpp \
                    webserver/server/handlers/response_handler.cpp \
					webserver/helper/ft_itoa.cpp \
					webserver/helper/ft_atoi.cpp

OBJECTS 		=	${SOURCES:%.c=%.o}

FLAGS 			=	-Wall -Wextra -Werror

COMPILE			=	clang++ -std=c++98

GREEN 			= 	\033[38;5;46m
WHITE 			= 	\033[38;5;15m
GREY 			= 	\033[38;5;8m
ORANGE 			= 	\033[38;5;202m
RED 			= 	\033[38;5;160m

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "$(GREEN)----------------------------------------------------"
	@$(COMPILE) $(FLAGS) -o $(NAME) $(OBJECTS)
	@echo "Executable				./webserv"
	@echo "$(GREEN)----------------------------------------------------"

%.o: %.cpp
	@echo "$(GREY)Compiling...				$(WHITE)$<"
	@$(COMPILE) $(FLAGS) -c -o $@ $<

clean:
	@echo "$(RED)----------------------------------------------------"
	@echo "$(WHITE)DELETE ./webserv"
	@echo "$(RED)----------------------------------------------------"
	@/bin/rm -f $(NAME)

re: clean all

.PHONY: all clean re
