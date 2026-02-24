def show_help():
    print("Here are some information about the program")


def execute_input(input_command):
    """ execute input """
    if input_command.lower() == "exit":
        return False
    elif input_command.lower() == "help":
        show_help()
    return True


def get_input():
    """ get input from command line """
    return input("Enter command: ")


def menu():
    print("You have the following options:")
    print("help")
    print("exit")


def start():
    """ Start information function """
    print("Welcome to my own CLI")


def main():
    """ Main function"""
    start()

    while True:
        menu()
     #   if not (execute_input(get_input())):
      #      break


if __name__ == "__main__":
    main()