CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

NAME = ircserv

INCLUDE = ./include

HEADER = $(wildcard $(INCLUDE)/*.hpp)

SRC = $(wildcard \
	./src/*.cpp \
	./src/controller/*.cpp \
	./src/domain/*.cpp \
	./src/domain/Channel/*.cpp \
	./src/repository/*.cpp \
	./src/service/*.cpp \
	./src/service/ChannelService/*.cpp \
	./src/service/ConnectionService/*.cpp \
	./src/util/*.cpp \
 	./src/Channel/*.cpp \
)

OBJ = $(SRC:%.cpp=%.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) $^ -o $(NAME)

%.o : %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@

clean :
	@rm -f $(OBJ)

fclean :
	@make clean
	@rm -f $(NAME)

re :
	@make fclean
	@make all

.PHONY : all clean fclean re