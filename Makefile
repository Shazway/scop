NAME		=	scop
DEBUG_NAME	=	scopDebug

LDFLAGS =	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

CFLAGS	=	-std=c++17 -O2 -g3
DEBUG_CFLAGS	=	-DNDEBUG -std=c++17 -O2 -g3

OBJ_PATH		=	obj/
DEBUG_OBJ_PATH		=	debug_obj/

CC			=	g++
SRC_PATH	=	srcs/
INCLUDES	=	-Iincludes
SRC_NAME	=	main.cpp					\
				app.cpp						\
				window.cpp					\
				pipeline.cpp				\
				device.cpp					\
				swap_chain.cpp				\
				model.cpp					\
				renderer.cpp				\
				simple_render_system.cpp	\
				camera.cpp					\
				keyboard_inputs.cpp			\
				obj_loader.cpp


OBJ_NAME	=	$(SRC_NAME:.cpp=.o)
OBJ		=	$(addprefix $(OBJ_PATH), $(OBJ_NAME))
DEBUG_OBJ	=	$(addprefix $(DEBUG_OBJ_PATH), $(OBJ_NAME))

#----------colors---------#
BLACK		=	\033[1;30m
RED			=	\033[1;31m
GREEN		=	\033[1;32m
BLUE		=	\033[1;34m
PURPLE		=	\033[1;35m
CYAN		=	\033[1;36m
WHITE		=	\033[1;37m
EOC			=	\033[0;0m



all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(PURPLE)=====>Compiling Shaders<===== $(WHITE)"
	./compile_shaders.sh
	@echo "$(GREEN)Done ! ✅ $(EOC)"
	@echo "$(RED)=====>Compiling Scop Test<===== $(WHITE)"
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "$(GREEN)Done ! ✅ $(EOC)"


$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

-include $(OBJ:%.o=%.d)

debug: $(DEBUG_NAME)

$(DEBUG_NAME): $(DEBUG_OBJ)
	@echo "$(PURPLE)=====>Compiling Shaders<===== $(WHITE)"
	./compile_shaders.sh
	@echo "$(RED)=====>Compiling Scop DEBUG<===== $(WHITE)"
	$(CC) $(DEBUG_CFLAGS) $(INCLUDES) $(DEBUG_OBJ) -o $(DEBUG_NAME) $(LDFLAGS)
	@echo "$(GREEN)Done ! ✅ $(EOC)"

$(DEBUG_OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	mkdir -p $(@D)
	$(CC) $(DEBUG_CFLAGS) $(INCLUDES) -MMD -c $< -o $@

-include $(DEBUG_OBJ:%.o=%.d)

clean:
	@echo "$(CYAN)♻  Cleaning obj files ♻ $(WHITE)"
	rm -rf $(OBJ_PATH)
	rm -rf $(DEBUG_OBJ_PATH)
	@echo "$(GREEN)Done !✅ $(EOC)"

fclean: clean
	@echo "$(CYAN)♻  Cleaning executable ♻ $(WHITE)"
	rm -rf $(NAME)
	rm -rf $(DEBUG_NAME)
	rm -rf vert.spv
	rm -rf frag.spv
	@echo "$(GREEN)Done !✅ $(EOC)"

re: fclean all
re_debug: fclean debug

.PHONY: all debug clean fclean re re_debug
