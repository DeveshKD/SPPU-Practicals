add() {
    echo "Result: $(( $1 + $2 ))"
}

# Function to perform subtraction
subtract() {
    echo "Result: $(( $1 - $2 ))"
}

# Function to perform multiplication
multiply() {
    echo "Result: $(( $1 * $2 ))"
}

# Function to perform division
divide() {
    if [ $2 -eq 0 ]; then
        echo "Error: Division by zero is not allowed."
    else
        echo "Result: $(( $1 / $2 ))"
    fi
}

# Main menu loop
while true; do
    echo ""
    echo "Select operation:"
    echo "1) Addition"
    echo "2) Subtraction"
    echo "3) Multiplication"
    echo "4) Division"
    echo "5) Exit"
    read -p "Choose an option (1-5): " option

    case $option in
        1|2|3|4)
            read -p "Enter first number: " num1
            read -p "Enter second number: " num2
            ;;
    esac

    case $option in
        1) add $num1 $num2 ;;
        2) subtract $num1 $num2 ;;
        3) multiply $num1 $num2 ;;
        4) divide $num1 $num2 ;;
        5) echo "Exiting."; exit 0 ;;
        *) echo "Invalid option. Please try again." ;;
    esac
done
