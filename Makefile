SRCS =	srcs/main/main.cpp \
		srcs/requestParser/RequestParser.cpp \
		srcs/data/status_codes.cpp \
		srcs/tools/date.cpp \
		srcs/tools/file_to_string.cpp \
		srcs/tools/split.cpp \
		srcs/tools/trim.cpp \
		srcs/response/Response.cpp \
		srcs/Response/ResponseHeader.cpp \
		srcs/configFileReader/Server.cpp \
		srcs/configFileReader/ConfigFileReader.cpp \

OBJS = ${SRCS:.cpp=.o}

NAME = webserv

INCDIR = ./srcs/include/

CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -I${INCDIR}

DELETE = rm -f

all: ${NAME}
	
${NAME}: ${OBJS}
	${CXX} -o ${NAME} ${OBJS}

clean:
	${DELETE} ${OBJS}

fclean: clean
	${DELETE} ${NAME}

re: fclean all

run: ${NAME}
	./${NAME}

.PHONY: all clean flcean re