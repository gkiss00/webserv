SRCS =	srcs/main/main.cpp \
		srcs/main/MyWebServer.cpp \
		srcs/requestParser/RequestParser.cpp \
		srcs/data/status_codes.cpp \
		srcs/data/implemented_methods.cpp \
		srcs/data/file_extensions.cpp \
		srcs/tools/date.cpp \
		srcs/tools/files.cpp \
		srcs/tools/split.cpp \
		srcs/tools/numbers.cpp \
		srcs/tools/trim.cpp \
		srcs/tools/ft_htons.cpp \
		srcs/response/Response.cpp \
		srcs/response/ResponseHeader.cpp \
		srcs/response/PostHeader.cpp \
		srcs/configFileReader/Server.cpp \
		srcs/configFileReader/Location.cpp \
		srcs/configFileReader/NewConfigFileReader.cpp \
		srcs/client/Client.cpp \


OBJS = ${SRCS:.cpp=.o}

NAME = webserv

INCDIR = ./srcs/include/

CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -I${INCDIR} -pthread

DELETE = rm -f

all: ${NAME}
	
${NAME}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS}

clean:
	${DELETE} ${OBJS}

fclean: clean
	${DELETE} ${NAME}

re: fclean all

run: ${NAME}
	./${NAME}

.PHONY: all clean flcean re