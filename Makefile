SRCS =	srcs/main/main.cpp \
		srcs/requestParser/RequestParser.cpp \
		srcs/data/status_codes.cpp \
		srcs/data/implemented_methods.cpp \
		srcs/data/file_extensions.cpp \
		srcs/tools/date.cpp \
		srcs/tools/files.cpp \
		srcs/tools/split.cpp \
		srcs/tools/numbers.cpp \
		srcs/tools/trim.cpp \
		srcs/response/Response.cpp \
		srcs/response/ResponseHeader.cpp \
		srcs/configFileReader/Server.cpp \
		srcs/configFileReader/Location.cpp \
		srcs/configFileReader/NewConfigFileReader.cpp \

OBJS = ${SRCS:.cpp=.o}

NAME = webserv

INCDIR = ./srcs/include/

CXX = clang++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -I${INCDIR}

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