
SRCS =	srcs/main/main.cpp \

OBJS = ${SRCS:.cpp=.o}

NAME = webserv

INCDIR = ./srcs/include/

CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -I ${INCDIR} 

DELETE = rm -rf

all: ${NAME}
	
${NAME}: ${OBJS}
	${CXX} -o ${NAME} ${CPPFLAGS} ${OBJS}

clean:
	${DELETE} ${OBJS}

fclean: clean
	${DELETE} ${NAME};

re: fclean all

.PHONY: all clean flcean re