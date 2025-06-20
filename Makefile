NAME = ft_ping

SRCS =	./ft_check_parameters.c ./ft_initialize_icmp_header.c	./ft_print_icmp_message.c \
		./ft_check_and_resolve_addr.c ./ft_get_destination_address.c ./ft_initialize_socket.c \
		./ft_print_ip_icmp_headers.c ./ft_check_for_error.c ./ft_get_host_ip_address.c \
		./ft_ping.c	./ft_split.c ./ft_check_for_help.c ./ft_handle_signal.c ./ft_utils.c \
		./ft_lstadd_back_bonus.c ft_lstclear_bonus.c ./ft_lstnew_bonus.c

OBJS =	./ft_check_parameters.o ./ft_initialize_icmp_header.o	./ft_print_icmp_message.o \
		./ft_check_and_resolve_addr.o ./ft_get_destination_address.o ./ft_initialize_socket.o \
		./ft_print_ip_icmp_headers.o ./ft_check_for_error.o ./ft_get_host_ip_address.o \
		./ft_ping.o	./ft_split.o ./ft_check_for_help.o ./ft_handle_signal.o ./ft_utils.o \
		./ft_lstadd_back_bonus.o /ft_lstclear_bonus.o ./ft_lstnew_bonus.o

OBJS = $(SRCS:.c=.o)

FLAGS = -Werror -Wextra -Wall

CC = gcc

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) $(FLAGS) -o $(NAME) -lm

%.o : %.c
	$(CC) -c $(FLAGS)  $< -o $@

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all