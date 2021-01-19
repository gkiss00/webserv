
SRCS=	srcs/main/main.cpp \

OBJS= ${SRCS:.cpp=.o}

NAME= webserv

FLAGS = -Wall -Wextra -Werror

DELETE = rm -rf

all: ${NAME}
	
${NAME}: ${OBJS}
	clang++ -o ${NAME} ${FLAGS} ${OBJS}

.cpp.o:
	clang++ ${FLAGS} -c $< -o $@

clean:
	${DELETE} ${OBJS}

fclean: clean
	${DELETE} ${NAME};

re: fclean all

.PHONY: all clean flcean re